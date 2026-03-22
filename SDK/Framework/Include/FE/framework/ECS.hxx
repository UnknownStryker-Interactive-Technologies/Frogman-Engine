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

#include <FE/bitmask.hxx>
#include <FE/concurrent_vector.hxx> // needs a bit of rework and design shifts to remove an internal lock.
#include <FE/farray.hxx>
#include <FE/hash.hxx>
#include <FE/list.hxx>
#include <FE/memory.hxx>
#include <FE/pool/memory_resource.hxx> 

#include <FE/framework/smart_ptr.hxx>
#include <FE/framework/type_info.hxx> // FE::reflection::type_id<Archetype>()

#include <concurrent_vector.h>
#include <vector>

#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>

//#include <boost/fiber/recursive_mutex.hpp> // fiber lock; replace it with the in-house fiber mutex when it is ready.




CLASS_FORWARD_DECLARATION(FE, archetype_base);
CLASS_FORWARD_DECLARATION(FE, component_base);
CLASS_FORWARD_DECLARATION(FE::framework, ECS);
CLASS_FORWARD_DECLARATION(FE::framework, game_processor);




BEGIN_NAMESPACE(FE)


CLASS_FORWARD_DECLARATION(internal::ECS, gc_metadata_base);
STRUCT_FORWARD_DECLARATION(internal::ECS, component_metadata);


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
	friend class framework::ECS;
	friend class ::FE::framework::game_processor;
	friend class internal::ECS::gc_metadata_base;

	FE::smart_ptr<struct internal::ECS::component_metadata, FE::RefType::_Owner> m_metadata;

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
};

using component = FE::smart_ptr<FE::component_base, FE::RefType::_Owner>;
using system = void(*)(class FE::component_base*);

namespace internal
{
	enum struct GarbageClass
	{
		_NotAGarbage = 0,
		_ProbGarbage = 1,
		_CertGarbage = 2
	};
}

namespace internal::ECS
{
	class entities
	{
	public:
		_FE_MAYBE_UNUSED_ static constexpr FE::size max_entities = 524285; // Total size of entities node in a FE::list is 4 MiB.

	private:
		FE::archetype m_entities[max_entities];
		std::atomic_size_t m_current_size = 0;

	public:
		entities() noexcept = default;
		~entities() noexcept = default;

		_FE_FORCE_INLINE_ FE::size add_entity(FE::archetype&& comp_p) noexcept
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

		_FE_FORCE_INLINE_ FE::archetype& last() { return m_entities[m_current_size - 1]; }

		_FE_FORCE_INLINE_ var::size get_size() const { return m_current_size; }

		_FE_FORCE_INLINE_ FE::archetype* begin() noexcept { return static_cast<FE::archetype*>(m_entities); }
		_FE_FORCE_INLINE_ FE::archetype* end() noexcept { return static_cast<FE::archetype*>(m_entities) + m_current_size; }
	};


	class components
	{
	public:
		_FE_MAYBE_UNUSED_ static constexpr FE::size max_components = 1021; // Total size of components node in a FE::list is 8 KiB.

	private:
		FE::component m_components[max_components];
		std::atomic_size_t m_current_size = 0;

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

#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to alignment specifier
	// to avoid false sharing issues
	class alignas(FE::CPU_L1_cache_line::size) gc_metadata
	{
	public: // these lists' nodes are memory pooled; replace it with FE::forward_list later.
		using member_component_list_type = FE::list< class FE::component_base* >;

		member_component_list_type _member_components;
		std::atomic<::FE::internal::GarbageClass> _garbage_class;

		gc_metadata() noexcept;
		~gc_metadata() noexcept = default;
	};
#pragma warning(pop)

	struct component_metadata
	{
		friend class framework::ECS;
		friend class gc_metadata_base;

		FE::smart_ptr<class gc_metadata, FE::RefType::_Owner> _gc_metadata; // the GC metadata is read by a separate thread; I chose to store the GC metadata in a separate place to avoid false sharing issues.

		FE::list<FE::internal::ECS::components>::iterator _group;
		var::size _index;
		FE::ASCII* _typename;
		var::size _type_hash;
	};


	class gc_metadata_base
	{
	public: // reserve and add watch at a batch in the component base constructor.
		template <typename T>
		_FE_FORCE_INLINE_ static void add_watch(FE::component_base* const host_p, T& property_p) noexcept
		{
			if constexpr (FE::is_observer_smart_ptr_v<T> == true)
			{
				if constexpr (std::is_base_of_v<FE::component_base, typename T::element_type> == true)
				{
					host_p->m_metadata->_gc_metadata->_member_components.emplace_back( property_p.operator->() );
				}
			}
		}
	};
}




class archetype_base
{
	friend class framework::ECS;
	friend class ::FE::framework::game_processor;

	using component_view_table = absl::flat_hash_map<var::size, ::FE::component_view<component_base>,
		FE::hash<var::size>,
		std::equal_to<var::size>,
		FE::polymorphic_allocator< std::pair< FE::size, ::FE::component_view<component_base> > >
	>;

	//using component_view_table = FE::concurrent_vector<	FE::component_view<component_base>, 
	//													FE::cache_aligned_allocator<FE::component_view<component_base>>
	//	// sadly, there is no such boost::fiber::shared_mutex. I need to remove the internal lock in the concurrent_vector to avoid undermining fiber concurrency.
	//	// I am using the FE::concurrent_vector as of writing, becuase it is thread-safe to call reserve() concurrently.
	//>;

private:
	component_view_table m_component_view_table;
	class framework::ECS* m_host;
	FE::ASCII* m_memory_layout_version;
	FE::list<FE::internal::ECS::entities>::iterator m_group;
	var::size m_index;

public:
	archetype_base(framework::ECS& host_p) noexcept;
	virtual ~archetype_base() noexcept;


	archetype_base(framework::ECS& host_p, const FE::framework::initializer& other_p) noexcept;
	archetype_base& operator=(const FE::framework::initializer& other_p) noexcept;


	template <class Component>
	_FE_FORCE_INLINE_ component_view<Component> get_component() noexcept
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
	_FE_FORCE_INLINE_ void set_memory_layout_version(FE::ASCII* const version_p) noexcept { m_memory_layout_version = version_p; }
	FE::memory_resource* get_ecs_memory_resource() noexcept;
	_FE_FORCE_INLINE_ void switch_ecs_host(framework::ECS& new_host_p) noexcept { m_host = &new_host_p; }
};

CLASS_FORWARD_DECLARATION(internal::ECS, component_table_getter);
CLASS_FORWARD_DECLARATION(internal::ECS, gc_root_getter);

END_NAMESPACE




BEGIN_NAMESPACE(FE::framework)


class ECS
{
	friend class ::FE::archetype_base; // for archetype_base::get_ecs_memory_resource();
	friend class FE::framework::game_processor;

public:
	using archetype_table = FE::list< FE::internal::ECS::entities, FE::page_aligned_allocator<FE::internal::ECS::entities> >;

	using component_table = absl::node_hash_map<std::size_t, // the robin hood hash map uses lighter hashing algorithm for integers, than objects.
												FE::pair<	FE::memory_resource,
															FE::list<FE::internal::ECS::components>
														>
												>;
	struct gc_root
	{
		concurrency::concurrent_vector<typename archetype_table::iterator,
										FE::cache_aligned_allocator<typename archetype_table::iterator>> _entity_roots;
		
		concurrency::concurrent_vector<typename component_table::mapped_type::second_type::iterator,
										FE::cache_aligned_allocator<typename component_table::mapped_type::second_type::iterator>> _component_roots;
	};

private:
	gc_root m_gc_root;

	FE::memory_resource m_memory_resource;
	FE::memory_resource m_archetype_pool;

	archetype_table m_entity_list;
	component_table m_component_table;
	
	framework::initializer_list m_archetype_default_entities;
	std::pmr::string m_buffer;
	//boost::fibers::recursive_mutex m_fiber_lock;

public:
	ECS(FE::size component_type_count_hint_p) noexcept;
	void initialize(framework::initializer_list&& initializer_list_p) noexcept;
	~ECS() noexcept = default;

	ECS(const ECS&) noexcept = delete;
	ECS& operator=(const ECS&) noexcept = delete;

	
	template <class Archetype, typename ...Arguments>
	FE::entity<Archetype> instanciate_entity(Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		//std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		FE::archetype l_alloc_result = FE::make_owner<Archetype>( &m_archetype_pool, *this, std::forward<Arguments>(arguments_p)... );
		l_alloc_result->m_component_view_table = typename FE::archetype_base::component_view_table(&m_memory_resource);

		for (typename archetype_table::iterator entity_group = m_entity_list.begin(); entity_group != m_entity_list.end(); ++entity_group)
		{
			if (entity_group->get_size() == entity_group->max_entities)
			{
				continue;
			}

			FE::entity<Archetype> l_view = FE::downcast_owner_to_observer<Archetype>(l_alloc_result);
			FE::size l_idx = entity_group->add_entity( std::move(l_alloc_result) );
			l_view->m_group = entity_group;
			l_view->m_index = l_idx;
			return l_view;
		}

		if (l_alloc_result != nullptr) // is std::move()-ed? l_alloc_result must be nullptr if std::move()-ed.
		{ // could not find a group with free space; allocate a new group.
			m_entity_list.emplace_front();
			m_gc_root._entity_roots.push_back(m_entity_list.begin()); // add to the GC tracking list.

			FE::entity<Archetype> l_view = FE::downcast_owner_to_observer<Archetype>(l_alloc_result);
			FE::size l_idx = m_entity_list.begin()->add_entity(std::move(l_alloc_result));
			l_view->m_group = m_entity_list.begin();
			l_view->m_index = l_idx;
			return l_view;
		}

		return entity<Archetype>();
	}

	template <class Archetype>
	FE::entity<FE::archetype_base> instanciate_entity_from_initializer(const FE::framework::initializer& serialized_entity_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		//std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		FE::entity<FE::archetype_base> l_entity = ECS::instanciate_entity<Archetype>();
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
			(*l_component_adder)(this, &l_handle, &l_arguments); // Perform reflection magic to add the component.
		}
		deserialize_entity(serialized_entity_p, l_entity);
		return l_entity;
	}

	template <class Archetype>
	FE::entity<FE::archetype_base> instanciate_archetype_default_entity() noexcept
	{
		//std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		framework::initializer* l_default_values = ECS::get_archetype_default_entity<Archetype>();
		if (l_default_values == nullptr)
		{
			return FE::entity<FE::archetype_base>{};
		}
		return ECS::instanciate_entity_from_initializer<Archetype>(*l_default_values);
	}


	template <class Archetype>
	_FE_FORCE_INLINE_ void set_archetype_default_entity(FE::framework::initializer&& default_values_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		//std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		m_archetype_default_entities[ FE::framework::reflection::type_id<Archetype>().name() ] = std::move(default_values_p);
	}

	template <class Archetype>
	_FE_FORCE_INLINE_ FE::framework::initializer* const get_archetype_default_entity() noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		//std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename initializer_list::iterator l_probe_result = m_archetype_default_entities.find( FE::framework::reflection::type_id<Archetype>().name() );
		if (l_probe_result != m_archetype_default_entities.end())
		{
			return &(l_probe_result->second);
		}
		return nullptr;
	}


	template <class Component, typename ...Arguments>
	FE::component_view<Component> add_component(FE::entity<archetype_base> entt_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
		//std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename component_table::iterator l_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		if (l_probe_result == m_component_table.end())
		{
			typename component_table::mapped_type& l_list_and_allocator = m_component_table[FE::framework::reflection::type_id<Component>().hash_code()];
			l_list_and_allocator._second = std::move(FE::list<FE::internal::ECS::components>(&l_list_and_allocator._first));
			l_list_and_allocator._second.emplace_front(); // allocate a component pool.

			m_gc_root._component_roots.push_back(l_list_and_allocator._second.begin()); // add to the GC tracking list.

			l_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
			FE_ASSERT(l_probe_result != m_component_table.end(), "Assertion failed: failed to emplace a new list of components in the component table.");
		}


		FE::component l_alloc_result;
		for (typename component_table::mapped_type::second_type::iterator component_group = l_probe_result->second._second.begin(); component_group != l_probe_result->second._second.end(); ++component_group)
		{
			if (component_group->get_size() == component_group->max_components) // true if the component pool is full.
			{
				continue;
			}

			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._first), std::forward<Arguments>(arguments_p)...);
			l_alloc_result->m_metadata = FE::make_owner<FE::internal::ECS::component_metadata>( &m_memory_resource );
			l_alloc_result->m_metadata->_type_hash = FE::framework::reflection::type_id<Component>().hash_code();
			l_alloc_result->m_metadata->_typename = FE::framework::reflection::type_id<Component>().name();
			l_alloc_result->m_metadata->_gc_metadata = FE::make_owner<FE::internal::ECS::gc_metadata>(&m_memory_resource);
			l_alloc_result->m_metadata->_gc_metadata->_member_components = typename FE::internal::ECS::gc_metadata::member_component_list_type(&m_memory_resource);

			FE::component_view<Component> l_view = FE::downcast_owner_to_observer<Component>(l_alloc_result);

			auto l_result = entt_p->m_component_view_table.emplace(FE::framework::reflection::type_id<Component>().hash_code(), l_view);
			if (l_result.second == false)
			{
				FE_LOG(FE::log::Severity::_Warning, "FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}

			FE::size l_idx = component_group->add_component(std::move(l_alloc_result));
			l_view->m_metadata->_group = component_group;
			l_view->m_metadata->_index = l_idx;
			return l_view;
		}


		// could not find a component pool with free space, allocate a new component pool!
		if (l_alloc_result == nullptr) _FE_LIKELY_
		{
			// All components lists are full. Create a new one.
			l_probe_result->second._second.emplace_front();
			m_gc_root._component_roots.push_back(l_probe_result->second._second.begin()); // add to the GC tracking list.

			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._first), std::forward<Arguments>(arguments_p)...);
			l_alloc_result->m_metadata = FE::make_owner<FE::internal::ECS::component_metadata>( &m_memory_resource );
			l_alloc_result->m_metadata->_type_hash = FE::framework::reflection::type_id<Component>().hash_code();
			l_alloc_result->m_metadata->_typename = FE::framework::reflection::type_id<Component>().name();
			l_alloc_result->m_metadata->_gc_metadata = FE::make_owner<FE::internal::ECS::gc_metadata>(&m_memory_resource);
			l_alloc_result->m_metadata->_gc_metadata->_member_components = typename FE::internal::ECS::gc_metadata::member_component_list_type(&m_memory_resource);

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
			return l_view;
		}

		return component_view<Component>();
	}

	template <class Component>
	_FE_FORCE_INLINE_ FE::component_view<FE::component_base> instanciate_component(FE::entity<archetype_base> entt_p) noexcept
	{
		return this->add_component<Component>(entt_p);
	}

	template <class Component>
	_FE_FORCE_INLINE_ void remove_component(FE::entity<archetype_base> entt_p) noexcept
	{
		this->detach_component<Component>(entt_p);
	}


	void attatch_component(FE::entity<archetype_base> entt_p, const ::FE::component_view<component_base>& to_attatch_p) noexcept;

	template <class Component>
	FE::component_view<Component> detach_component(FE::entity<archetype_base> entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
		//std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename FE::archetype_base::component_view_table::iterator l_probe_result = entt_p->m_component_view_table.find( FE::framework::reflection::type_id<Component>().hash_code() );
		FE_ASSERT(l_probe_result != entt_p->m_component_view_table.end(), "Assertion failed: the entity must have this component.");
	
		entt_p->m_component_view_table.erase(l_probe_result);
		return FE::downcast_observer<Component>(l_probe_result->second);
	}


	FE::framework::initializer serialize_entity(FE::entity<archetype_base> entt_p, FE::ASCII* const entity_memory_layout_version) noexcept;
	void deserialize_entity(const FE::framework::initializer& serialized_components_p, FE::entity<archetype_base> out_entt_p, FE::ASCII* const entity_memory_layout_version) noexcept;
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