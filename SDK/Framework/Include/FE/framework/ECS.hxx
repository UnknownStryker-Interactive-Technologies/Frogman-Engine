#ifndef _FE_FRAMEWORK_FRAMEWORK_ECS_HXX_
#define _FE_FRAMEWORK_FRAMEWORK_ECS_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/framework/framework.hxx>
#include <FE/list.hxx>
#include <FE/memory.hxx>
#include <FE/pool/memory_resource.hxx> // game memory pool

#include <FE/framework/smart_ptr.hxx>
#include <FE/framework/type_info.hxx> // FE::reflection::type_id<Archetype>()

// ECS data structures
#include <FE/farray.hxx>
#include <FE/hash.hxx>
#include <string>
#include <vector>

#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>

#include <boost/fiber/recursive_mutex.hpp> // fiber lock




CLASS_FORWARD_DECLARATION(FE, archetype_base);
CLASS_FORWARD_DECLARATION(FE, component_base);
CLASS_FORWARD_DECLARATION(FE::framework, ECS);
CLASS_FORWARD_DECLARATION(FE::framework, game_thread);
CLASS_FORWARD_DECLARATION(FE::framework, processors);




BEGIN_NAMESPACE(FE)


CLASS_FORWARD_DECLARATION(internal::ECS, gc_metadata_proxy_table);
CLASS_FORWARD_DECLARATION(internal::ECS, component_metadata);


template <class Archetype>
using entity = FE::smart_ptr<Archetype, FE::RefType::_Observer>; // pointers can be though of as integers or handles
using archetype = FE::smart_ptr<FE::archetype_base, FE::RefType::_Owner>;

template <class Component>
using component_view = FE::smart_ptr<Component, FE::RefType::_Observer>;


namespace framework
{
	using initializer = absl::flat_hash_map<std::pmr::string, std::pmr::string,
		FE::hash<std::pmr::string>,
		std::equal_to<std::pmr::string>,
		FE::polymorphic_allocator< std::pair<const std::pmr::string, std::pmr::string> >
	>;

	using initializer_list = absl::flat_hash_map<	std::pmr::string, initializer,
		FE::hash<std::pmr::string>,
		std::equal_to<std::pmr::string>,
		FE::polymorphic_allocator< std::pair<const std::pmr::string, initializer> >
	>;
}


class component_base
{
	friend class FE::archetype_base;
	friend class framework::ECS;
	friend class framework::game_thread;
	friend class framework::processors;
	friend class internal::ECS::gc_metadata_proxy_table;

	FE::smart_ptr<class internal::ECS::component_metadata, FE::RefType::_Owner> m_metadata;

public:
	component_base() noexcept = default;
	virtual ~component_base() noexcept = default;

	component_base(component_base&& other_p) noexcept;

	_FE_FORCE_INLINE_ component_base& operator=(component_base&& other_p) noexcept
	{
		m_metadata = std::move(other_p.m_metadata);
		return *this;
	}

	FE::ASCII* get_typename() const noexcept;
	FE::ASCII* get_memory_layout_version() const noexcept;
};

using component = FE::smart_ptr<FE::component_base, FE::RefType::_Owner>;
using system = void(*)(class FE::component_base*);


namespace internal::ECS
{
	class entities
	{
	public:
		_FE_MAYBE_UNUSED_ static constexpr FE::size max_entities = 509; // Total size of entities node in a FE::list is 4KiB, which is the system page size on x64 Windows.

	private:
		FE::entity<FE::archetype_base> m_entities[max_entities];
		var::size m_current_size = 0;

	public:
		entities() noexcept = default;
		~entities() noexcept = default;

		_FE_FORCE_INLINE_ FE::size add_entity(FE::entity<FE::archetype_base>&& comp_p) noexcept
		{
			FE_ASSERT(m_current_size < max_entities);

			m_entities[m_current_size] = std::move(comp_p);
			return m_current_size++;
		}

		_FE_FORCE_INLINE_ void remove_entity(FE::size index_p) noexcept
		{
			FE_ASSERT(index_p < m_current_size);

			m_entities[index_p].reset();
			last().swap(m_entities[index_p]);
		}

		_FE_FORCE_INLINE_ FE::entity<FE::archetype_base>& last() { return m_entities[m_current_size - 1]; }

		_FE_FORCE_INLINE_ var::size get_size() const { return m_current_size; }

		_FE_FORCE_INLINE_ FE::entity<FE::archetype_base>* begin() noexcept { return static_cast<FE::entity<FE::archetype_base>*>(m_entities); }
		_FE_FORCE_INLINE_ FE::entity<FE::archetype_base>* end() noexcept { return static_cast<FE::entity<FE::archetype_base>*>(m_entities) + m_current_size; }
	};


	class entity_metadata
	{
		friend class framework::game_thread;
		friend class framework::processors;
		friend class framework::ECS;

	public:
		FE::list<FE::internal::ECS::entities>::iterator _group;
		var::size _index;
		// FE::bitmask<Alloc> m_archetype; // bitmask of components that the entity has
		entity_metadata() noexcept;
		~entity_metadata() noexcept = default;
	};


	class components
	{
	public:
		_FE_MAYBE_UNUSED_ static constexpr FE::size max_components = 509; // Total size of components node in a FE::list is 4KiB, which is the system page size on x64 Windows.

	private:
		FE::component m_components[max_components];
		var::size m_current_size = 0;

	public:
		components() noexcept = default;
		~components() noexcept = default;

		_FE_FORCE_INLINE_ FE::size add_component(FE::component&& comp_p) noexcept
		{
			FE_ASSERT(m_current_size < max_components);

			m_components[m_current_size] = std::move(comp_p);
			return m_current_size++;
		}

		_FE_FORCE_INLINE_ void remove_component(FE::size index_p) noexcept
		{
			FE_ASSERT(index_p < m_current_size);

			m_components[index_p].reset();
			last().swap(m_components[index_p]);
		}

		_FE_FORCE_INLINE_ FE::component& last() { return m_components[m_current_size - 1]; }

		_FE_FORCE_INLINE_ var::size get_size() const { return m_current_size; }

		_FE_FORCE_INLINE_ FE::component* begin() noexcept { return static_cast<FE::component*>(m_components); }
		_FE_FORCE_INLINE_ FE::component* end() noexcept { return static_cast<FE::component*>(m_components) + m_current_size; }
	};


	enum struct CircularReferenceClass
	{
		_NotAGarbage = 0,
		_ProbGarbage = 1,
		_CertGarbage = 2
	};


	class gc_metadata // GC circular reference analyzer thread traverses this object.
	{
	public: // these lists' nodes are memory pooled; replace it with FE::forward_list later.
		using member_component_list_type = FE::list< FE::smart_ptr<class FE::component_base, FE::RefType::_Observer> >;
		using member_entity_list_type = FE::list< FE::smart_ptr<class FE::archetype_base, FE::RefType::_Observer> >;

		member_component_list_type _member_components;
		member_entity_list_type _member_entities;
		std::atomic<CircularReferenceClass> _circular_reference_class;

		gc_metadata() noexcept = default;
		~gc_metadata() noexcept = default;
	};


	class component_metadata
	{
		friend class gc_metadata_proxy_table;
		friend class framework::game_thread;
		friend class framework::processors;
		friend class framework::ECS;

		FE::smart_ptr<class gc_metadata, FE::RefType::_Owner> m_gc_metadata;

	public:
		FE::list<FE::internal::ECS::components>::iterator _group;
		var::size _index;
		FE::ASCII* _typename;
		var::size _typeid;
		FE::ASCII* _memory_layout_version; /* modify the string value when the memory layout of the component changes; this ensures correct auto serialization.
			the intial value is set to "default"
		*/
		component_metadata() noexcept;
		~component_metadata() noexcept = default;
	};


	class gc_metadata_proxy_table
	{
	public: // reserve and add watch at a batch in the component base constructor.
		template <typename T>
		_FE_FORCE_INLINE_ static void add_watch(FE::component_base* const host_p, T& property_p) noexcept
		{
			if constexpr (FE::is_observer_smart_ptr_v<T> == true)
			{
				if constexpr (std::is_base_of_v<FE::component_base, typename T::element_type> == true)
				{
					host_p->m_metadata->m_gc_metadata->_member_components.emplace_back( FE::upcast_observer<class FE::component_base>(property_p) );
				}
				else if constexpr (std::is_base_of_v<FE::archetype_base, typename T::element_type> == true)
				{
					host_p->m_metadata->m_gc_metadata->_member_entities.emplace_back( FE::upcast_observer<class FE::archetype_base>(property_p) );
				}
			}
		}
	};
}




class archetype_base
{
	friend class framework::ECS;
	friend class framework::game_thread;
	friend class framework::processors;
	using component_view_table = absl::flat_hash_map<	std::size_t, component_view<component_base>,
		FE::hash<std::size_t>,
		std::equal_to<std::size_t>,
		FE::polymorphic_allocator< std::pair< const std::size_t, component_view<component_base> > >
	>;

private:
	component_view_table m_component_view_table;
	std::pmr::string m_name;
	class framework::ECS* m_host;
	
public:
	archetype_base() noexcept = default;
	archetype_base(framework::ECS& host_p) noexcept;
	virtual ~archetype_base() noexcept;

	archetype_base(archetype_base&& other_p) noexcept;
	archetype_base& operator=(archetype_base&& other_p) noexcept;

	archetype_base(framework::ECS& host_p, FE::ASCII* const name_p, const FE::framework::initializer& other_p) noexcept;

	archetype_base(const archetype_base& other_p) noexcept;
	archetype_base& operator=(const archetype_base& other_p) noexcept;


	const std::pmr::string& get_name() const noexcept { return m_name; }


	template <class Component>
	component_view<Component> get_component() noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: T must be derived from FE::component_base.");
		typename component_view_table::iterator l_probe_result = m_component_view_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		if (l_probe_result != m_component_view_table.end())
		{
			return FE::downcast_observer<Component>(l_probe_result->second);
		}
		return component_view<Component>();
	}


	template <class Component, typename ...Arguments>
	FE::component_view<Component> add_component(Arguments&& ...arguments_p) noexcept;

	template <class Component>
	void destroy_component() noexcept;


	void attatch_component(const FE::component_view<component_base>& to_attatch_p) noexcept;

	template <class Component>
	FE::component_view<Component> detach_component() noexcept;


	FE::framework::initializer serialize_entity() noexcept;
	void deserialize_entity(const FE::framework::initializer& serialized_components_p) noexcept;

protected:
	FE::memory_resource* get_ecs_memory_resource() noexcept;
	_FE_FORCE_INLINE_ void switch_ecs_host(framework::ECS& new_host_p) noexcept { m_host = &new_host_p; }
};


END_NAMESPACE




BEGIN_NAMESPACE(FE::framework)


class ECS
{
	friend class ::FE::archetype_base;
	friend class game_thread;
	friend class processors;
	// using archetype_table = absl::flat_hash_map<FE::bitmask, FE::internal::ECS::entities, ...>
	using entity_table = absl::node_hash_map<std::pmr::string, FE::archetype, 
												FE::hash<std::pmr::string>,
												std::equal_to<std::pmr::string>,
												FE::polymorphic_allocator< std::pair<const std::pmr::string, FE::archetype> > // to do: delete!
	>;

	using component_table = absl::node_hash_map<std::size_t, // the robin hood hash map uses lighter hashing algorithm for integers, than objects.
												FE::pair<	FE::memory_resource,
															FE::list<FE::internal::ECS::components> 
														>
												>;
	
	FE::memory_resource m_memory_resource;
	FE::memory_resource m_archetype_pool;

	entity_table m_entity_table;
	component_table m_component_table;
	
	framework::initializer_list m_archetype_default_entities;
	std::pmr::string m_buffer;
	boost::fibers::recursive_mutex m_fiber_lock;
	FE::size m_max_entities;

public:
	ECS(FE::size max_entities_p, FE::size component_type_count_hint_p) noexcept;
	void initialize(framework::initializer_list&& initializer_list_p) noexcept;
	~ECS() noexcept = default;

	ECS(const ECS&) noexcept = delete;
	ECS& operator=(const ECS&) noexcept = delete;

	
	template <class Archetype, typename ...Arguments>
	FE::entity<Archetype> instanciate_entity(FE::ASCII* const entity_name_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);
		FE_ASSERT(m_entity_table.size() <= m_max_entities, "Assertion failed: cannot instantiate entities more than the value specified by the m_max_entities.");

		FE::archetype l_alloc_result = FE::make_owner<Archetype>( &m_archetype_pool, *this, std::forward<Arguments>(arguments_p)... );
		l_alloc_result->m_component_view_table = typename FE::archetype_base::component_view_table(&m_memory_resource);
		l_alloc_result->m_name = std::pmr::string( entity_name_p, &m_memory_resource);

		std::pair<typename entity_table::iterator, bool> l_result = m_entity_table.emplace( l_alloc_result->m_name, std::move(l_alloc_result) );
		
		if (l_result.second == true) // The emplace() was successful. 
		{
			return FE::downcast_owner_to_observer<Archetype>(l_result.first->second);
		}

		return entity<Archetype>();
	}

	template <class Archetype>
	FE::entity<FE::archetype_base> instanciate_entity_from_initializer(FE::ASCII* const entity_name_p, const FE::framework::initializer& serialized_entity_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		FE::entity<FE::archetype_base> l_entity = ECS::instanciate_entity<Archetype>(entity_name_p);
		if (l_entity.is_valid() == false)
		{
			return entity<FE::archetype_base>();
		}

		for (const auto& [component_identifier, serialized_component] : serialized_entity_p)
		{
			constexpr FE::ASCII* l_class = "class";
			constexpr FE::ASCII* l_struct = "struct";

			m_buffer = component_identifier;
			std::pmr::string::size_type l_pos = m_buffer.find(l_class);
			if (l_pos == std::pmr::string::npos)
			{
				l_pos = m_buffer.find(l_struct);
				FE_ASSERT(l_pos != std::pmr::string::npos, "Assertion failed: the component type name must start with 'class' or 'struct'.");
				l_pos += std::strlen(l_struct);
			}
			else
			{
				l_pos += std::strlen(l_class);
			}
			m_buffer.erase(0, l_pos);

			for (l_pos = m_buffer.find(' '); l_pos != std::pmr::string::npos; l_pos = m_buffer.find(' '))
			{
				m_buffer.erase(m_buffer.begin() + l_pos);
			}
			m_buffer.insert(0, "::");

			FE::task_base* l_component_adder = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(m_buffer);
			FE_ASSERT(l_component_adder != nullptr, "Assertion failed: the component adder function is not found. The component type may not be registered.");

			FE::component_view<FE::component_base> l_handle;
			FE::arguments<FE::entity<FE::archetype_base>> l_arguments;
			l_arguments._first = l_entity;
			(*l_component_adder)(this, &l_handle, &l_arguments);
		}
		deserialize_entity(serialized_entity_p, l_entity);
		return l_entity;
	}

	template <class Archetype>
	FE::entity<FE::archetype_base> instanciate_archetype_default_entity(FE::ASCII* const entity_name_p) noexcept
	{
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		framework::initializer* l_default_values = ECS::get_archetype_default_entity<Archetype>();
		if (l_default_values == nullptr)
		{
			return FE::entity<FE::archetype_base>{};
		}
		return ECS::instanciate_entity_from_initializer<Archetype>(entity_name_p, *l_default_values);
	}

	void destruct_entity(FE::entity<archetype_base> entt_p) noexcept;


	template <class Archetype>
	_FE_FORCE_INLINE_ void set_archetype_default_entity(FE::framework::initializer&& default_values_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		m_archetype_default_entities[ FE::framework::reflection::type_id<Archetype>().name() ] = std::move(default_values_p);
	}

	template <class Archetype>
	_FE_FORCE_INLINE_ FE::framework::initializer* const get_archetype_default_entity() noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename initializer_list::iterator l_probe_result = m_archetype_default_entities.find( FE::framework::reflection::type_id<Archetype>().name() );
		if (l_probe_result != m_archetype_default_entities.end())
		{
			return &(l_probe_result->second);
		}
		return nullptr;
	}


	template <class Archetype>
	FE::entity<Archetype> find_entity(FE::ASCII* const entity_name_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		m_buffer = entity_name_p;

		typename entity_table::iterator l_probe_result = m_entity_table.find(m_buffer);
		m_buffer.clear();
		if (l_probe_result != m_entity_table.end())
		{
			return FE::downcast_owner_to_observer<Archetype>(l_probe_result->second);
		}

		return entity<Archetype>();
	}


	template <class Component, typename ...Arguments>
	FE::component_view<Component> add_component(FE::entity<archetype_base> entt_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename component_table::iterator l_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		if (l_probe_result == m_component_table.end())
		{
			/*               ( m_memory_resource )
			*----------------------------------------------------------*
			*                    ( component pool )                    *
			*   *--------------------------------------------------*   *
			*   *                                                  *   *
			*   *                                                  *   *
			*   *                                                  *   *
			*   *                                                  *   *
			*   *                                                  *   *
			*   *--------------------------------------------------*   *
			*                                                          *
			*----------------------------------------------------------*
			*/
			typename component_table::mapped_type& l_list_and_allocator = m_component_table[FE::framework::reflection::type_id<Component>().hash_code()];
			l_list_and_allocator._second = std::move(FE::list<FE::internal::ECS::components>(&l_list_and_allocator._first));
			l_list_and_allocator._second.emplace_front(); // allocate a component pool.
			l_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		}


		FE::component l_alloc_result;
		for (typename component_table::mapped_type::second_type::iterator components = l_probe_result->second._second.begin(); components != l_probe_result->second._second.end(); ++components)
		{
			if (components->get_size() == components->max_components) // true if the component pool is full.
			{
				continue;
			}

			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._first), std::forward<Arguments>(arguments_p)...);
			l_alloc_result->m_metadata = FE::make_owner<FE::internal::ECS::component_metadata>( &m_memory_resource );
			l_alloc_result->m_metadata->_typename = FE::framework::reflection::type_id<Component>().name();
			l_alloc_result->m_metadata->m_gc_metadata = FE::make_owner<FE::internal::ECS::gc_metadata>(&m_memory_resource);
			l_alloc_result->m_metadata->m_gc_metadata->_member_components = typename FE::internal::ECS::gc_metadata::member_component_list_type(&m_memory_resource);
			l_alloc_result->m_metadata->m_gc_metadata->_member_entities = typename FE::internal::ECS::gc_metadata::member_entity_list_type(&m_memory_resource);

			FE::component_view<Component> l_view = FE::downcast_owner_to_observer<Component>(l_alloc_result);

			auto l_result = entt_p->m_component_view_table.emplace(FE::framework::reflection::type_id<Component>().hash_code(), l_view);
			if (l_result.second == false)
			{
				FE_LOG(FE::log::Severity::_Warning, "FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}

			FE::size l_idx = components->add_component(std::move(l_alloc_result));
			l_view->m_metadata->_group = components;
			l_view->m_metadata->_index = l_idx;
			l_view->m_metadata->_typeid = FE::framework::reflection::type_id<Component>().component_typeid();
			return l_view;
		}


		// could not find a component pool with free space, allocate a new component pool!
		if (l_alloc_result == nullptr) _FE_LIKELY_
		{
			// All components lists are full. Create a new one.
			l_probe_result->second._second.emplace_front();

			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._first), std::forward<Arguments>(arguments_p)...);
			l_alloc_result->m_metadata = FE::make_owner<FE::internal::ECS::component_metadata>( &m_memory_resource );
			l_alloc_result->m_metadata->_typename = FE::framework::reflection::type_id<Component>().name();
			l_alloc_result->m_metadata->m_gc_metadata = FE::make_owner<FE::internal::ECS::gc_metadata>(&m_memory_resource);
			l_alloc_result->m_metadata->m_gc_metadata->_member_components = typename FE::internal::ECS::gc_metadata::member_component_list_type(&m_memory_resource);
			l_alloc_result->m_metadata->m_gc_metadata->_member_entities = typename FE::internal::ECS::gc_metadata::member_entity_list_type(&m_memory_resource);

			FE::component_view<Component> l_view = FE::downcast_owner_to_observer<Component>(l_alloc_result);

			auto l_result = entt_p->m_component_view_table.emplace(FE::framework::reflection::type_id<Component>().hash_code(), l_view);
			if (l_result.second == false)
			{
				FE_LOG(FE::log::Severity::_Warning, "FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}

			FE::size l_idx = l_probe_result->second._second.front().add_component(std::move(l_alloc_result));
			l_view->m_metadata->_group = l_probe_result->second._second.begin();
			l_view->m_metadata->_index = l_idx;
			l_view->m_metadata->_typeid = FE::framework::reflection::type_id<Component>().component_typeid();
			return l_view;
		}

		return component_view<Component>();
	}

	template <class Component>
	_FE_FORCE_INLINE_ FE::component_view<FE::component_base> instanciate_component(FE::entity<archetype_base> entt_p) noexcept
	{
		return add_component<Component>(entt_p);
	}

	template <class Component>
	void remove_component(FE::entity<archetype_base> entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename archetype_base::component_view_table::iterator l_view_table_probe_result = entt_p->m_component_view_table.find(FE::framework::reflection::type_id<Component>().hash_code());

		if (l_view_table_probe_result == entt_p->m_component_view_table.end())
		{
			return; // The entity does not have this component.
		}

		typename component_table::iterator l_com_table_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		FE_ASSERT(l_com_table_probe_result != m_component_table.end(), "Assertion failed: the component table must have this component.");

		// Remove the component from the component table.
		l_view_table_probe_result->second->m_metadata->_group->remove_component(l_view_table_probe_result->second->m_metadata->_index);

		// Remove the component from the entity's component view table.
		entt_p->m_component_view_table.erase(l_view_table_probe_result);
	}


	void attatch_component(FE::entity<archetype_base> entt_p, const FE::component_view<component_base>& to_attatch_p) noexcept;

	template <class Component>
	FE::component_view<Component> detach_component(FE::entity<archetype_base> entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename FE::archetype_base::component_view_table::iterator l_probe_result = entt_p->m_component_view_table.find( FE::framework::reflection::type_id<Component>().hash_code() );
		FE_ASSERT(l_probe_result != entt_p->m_component_view_table.end(), "Assertion failed: the entity must have this component.");

		entt_p->m_component_view_table.erase(l_probe_result);
		return FE::downcast_observer<Component>(l_probe_result->second);
	}


	std::optional<FE::pair<FE::system, std::pmr::vector<std::size_t>>> find_system(FE::ASCII* const system_name_p) noexcept;


	FE::framework::initializer serialize_entity(FE::entity<archetype_base> entt_p) noexcept;
	void deserialize_entity(const FE::framework::initializer& serialized_components_p, FE::entity<archetype_base> out_entt_p) noexcept;
};


END_NAMESPACE




BEGIN_NAMESPACE(FE)


template <class Component, typename ...Arguments>
FE::component_view<Component> FE::archetype_base::add_component(Arguments&& ...arguments_p) noexcept
{
	FE_ASSERT(m_host != nullptr, "Assertion failed: the archetype's host ECS is null.");
	FE::entity<FE::archetype_base> l_self;
	FE::internal::smart_ptr::metadata<FE::archetype_base> l_forged_metadata{};
	l_forged_metadata._data = this;
	l_self.m_ptr.store(&l_forged_metadata, std::memory_order_relaxed);

	auto l_component_view = m_host->add_component<Component>(l_self, std::forward<Arguments>(arguments_p)...);
	l_self.m_ptr.store(nullptr, std::memory_order_relaxed);
	return l_component_view;
}

template <class Component>
void FE::archetype_base::destroy_component() noexcept
{
	FE_ASSERT(m_host != nullptr, "Assertion failed: the archetype's host ECS is null.");
	FE::entity<FE::archetype_base> l_self;
	FE::internal::smart_ptr::metadata<FE::archetype_base> l_forged_metadata{};
	l_forged_metadata._data = this;
	l_self.m_ptr.store(&l_forged_metadata, std::memory_order_relaxed);

	m_host->remove_component<Component>(l_self);
	l_self.m_ptr.store(nullptr, std::memory_order_relaxed);	
}


template <class Component>
FE::component_view<Component> FE::archetype_base::detach_component() noexcept
{
	FE_ASSERT(m_host != nullptr, "Assertion failed: the archetype's host ECS is null.");
	FE::entity<FE::archetype_base> l_self;
	FE::internal::smart_ptr::metadata<FE::archetype_base> l_forged_metadata{};
	l_forged_metadata._data = this;
	l_self.m_ptr.store(&l_forged_metadata, std::memory_order_relaxed);

	auto l_component_view = m_host->detach_component<Component>(l_self);
	l_self.m_ptr.store(nullptr, std::memory_order_relaxed);
	return l_component_view;
}


END_NAMESPACE
#endif