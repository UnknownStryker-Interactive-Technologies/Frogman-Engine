/*
Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "header_tool.hpp"
#include "error_code.hpp"
#include "parser.hpp"
#include "symbol_counter.hpp"
#include "tokenizer.hpp"




namespace FHT::parser
{
	_FE_NODISCARD_ header_file_root build_reflextree(const directory_t& file_path_p, const std::pmr::list<token>& token_list_p)
	{
		FHT::symbol_counter::symbol_count l_total_nums = FHT::symbol_counter::try_count_all_symbols(token_list_p.begin(), token_list_p.cend());
		{
			static std::mutex l_s_log_lock;
			std::lock_guard<std::mutex> l_guard(l_s_log_lock);
			std::wcout << L"Frogman Engine Header Tool: In the file located at '" << file_path_p.c_str() << "'\n";
			std::cout << "Frogman Engine Header Tool: the total number of namespaces is " << l_total_nums._namespaces << '\n';
			std::cout << "Frogman Engine Header Tool: the total number of classes is " << l_total_nums._classes << '\n';
			std::cout << "Frogman Engine Header Tool: the total number of structs is " << l_total_nums._structs << "\n";
			std::cout << "Frogman Engine Header Tool: the total number of enum structs is " << l_total_nums._enum_structs << "\n";
			std::cout << "Frogman Engine Header Tool: the total number of the Frogman Engine ECS system methods is " << l_total_nums._systems << "\n\n";
		}

		header_file_root l_root = 
		{ 
			._path_to_the_header_file = file_path_p,
			._namespaces{	framework::get_framework().get_memory_resource()},
			._classes{		framework::get_framework().get_memory_resource()},
			._structs{		framework::get_framework().get_memory_resource()},
			._enum_structs{	framework::get_framework().get_memory_resource()},
			._system_fptrs{	framework::get_framework().get_memory_resource()}
		};

		l_root._namespaces.reserve(l_total_nums._namespaces);
		l_root._classes.reserve(l_total_nums._classes);
		l_root._structs.reserve(l_total_nums._structs);
		l_root._enum_structs.reserve(l_total_nums._enum_structs);
		l_root._system_fptrs.reserve(l_total_nums._systems);


		context_stack_t l_context_stack{ 1, FHT::Context::_Global, framework::get_framework().get_memory_resource() };

		for (auto iterator = token_list_p.begin(); iterator != token_list_p.end(); ++iterator)
		{
			switch (iterator->_vocabulary)
			{
			case Vocabulary::_FrogmanEngineSystemMacro:
				l_root._system_fptrs.emplace_back(build_ecs_system_node(u8"::", iterator, token_list_p.end()));
				break;


			case Vocabulary::_BeginNamespace:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Namespace:
				switch (std::next(iterator)->_vocabulary) // skip unnamed namespace scopes, e.g., "namespace { ... }"
				{
				case Vocabulary::_LeftCurlyBracket:
					{
						var::int64 l_scope_level = 0;
						const auto l_scope_begin = std::next(iterator);
						auto l_scope_end = l_scope_begin;
						do
						{
							switch (l_scope_end->_vocabulary)
							{
							case Vocabulary::_LeftCurlyBracket:
								++l_scope_level;
								break;

							case Vocabulary::_RightCurlyBracket:
								--l_scope_level;
								break;

							default:
								break;
							}
							++l_scope_end;
						} while (l_scope_level > 0);
						iterator = l_scope_end;
					}
					continue;

					
				case Vocabulary::_AssignmentOperator:
					while (iterator->_vocabulary != Vocabulary::_Semicolon)
					{
						++iterator;
					}
					continue;


				default:
					break;
				}
				l_root._namespaces.emplace_back(build_namespace_node_recursive(u8"::", iterator, token_list_p.end(), l_context_stack));
				break;

			case Vocabulary::_FrogmanEngineClassReflectionMacro:
				_FE_FALLTHROUGH_;
			case Vocabulary::_ClassIdentifier:
				l_root._classes.emplace_back(build_class_node(u8"::", iterator, token_list_p.end()));
				break;

			case Vocabulary::_FrogmanEngineStructReflectionMacro:
				_FE_FALLTHROUGH_;
			case Vocabulary::_StructIdentifier:
				l_root._structs.emplace_back(build_struct_node(u8"::", iterator, token_list_p.end()));
				break;

			case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
				_FE_FALLTHROUGH_;
			case Vocabulary::_EnumStructIdentifier:
				l_root._enum_structs.emplace_back(build_enum_struct_node(u8"::", iterator, token_list_p.end()));
				break;


			default:
				break;
			}
		}

		return l_root;
	}

	_FE_NODISCARD_ namespace_node build_namespace_node_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p, context_stack_t& context_stack_p)
	{
		namespace_node l_node = 
		{
				._target_namespace_name{ framework::get_framework().get_memory_resource() },
				._nested_namespaces{ framework::get_framework().get_memory_resource() },
				._classes{ framework::get_framework().get_memory_resource() },
				._structs{ framework::get_framework().get_memory_resource() },
				._enum_structs{ framework::get_framework().get_memory_resource() },
				._system_fptrs{ framework::get_framework().get_memory_resource() }
		};

		FHT::symbol_counter::symbol_count l_total_nums = FHT::symbol_counter::try_count_the_current_scope_level_symbols(out_token_iterator_p, end_p);
		l_node._nested_namespaces.reserve(l_total_nums._namespaces);
		l_node._classes.reserve(l_total_nums._classes);
		l_node._structs.reserve(l_total_nums._structs);
		l_node._enum_structs.reserve(l_total_nums._enum_structs);
		l_node._system_fptrs.reserve(l_total_nums._systems);


		switch (out_token_iterator_p->_vocabulary)
		{
		case Vocabulary::_BeginNamespace:
			while (out_token_iterator_p->_vocabulary != Vocabulary::_NamespaceIdentifier)
			{
				++out_token_iterator_p;
			}

			l_node._target_namespace_name = parent_namespace_p;

			while (out_token_iterator_p->_vocabulary != Vocabulary::_RightParen)
			{
				l_node._target_namespace_name += out_token_iterator_p->_code;
				++out_token_iterator_p;
			}
			l_node._target_namespace_name += u8"::";
			break;


		case Vocabulary::_Namespace:
			while (out_token_iterator_p->_vocabulary != Vocabulary::_NamespaceIdentifier)
			{
				++out_token_iterator_p;
			}

			l_node._target_namespace_name = parent_namespace_p;

			while (out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket)
			{
				l_node._target_namespace_name += out_token_iterator_p->_code;
				++out_token_iterator_p;
			}
			l_node._target_namespace_name += u8"::";
			break;


		_FE_NODEFAULT_;
		}


		while (out_token_iterator_p != end_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_RightCurlyBracket:
				_FE_FALLTHROUGH_;
			case Vocabulary::_EndNamespace:
				return l_node;


			case Vocabulary::_FrogmanEngineSystemMacro:
				if (context_stack_p.back() == Context::_ClassIdentifier || context_stack_p.back() == Context::_StructIdentifier)
				{
					break; // skip the system macro if it is inside a class or struct.
				}
				l_node._system_fptrs.emplace_back(build_ecs_system_node(l_node._target_namespace_name, out_token_iterator_p, end_p));
				break;


			case Vocabulary::_BeginNamespace:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Namespace:
				switch (std::next(out_token_iterator_p)->_vocabulary) // skip unnamed namespace scopes, e.g., "namespace { ... }"
				{
				case Vocabulary::_LeftCurlyBracket:
					{
						var::int64 l_scope_level = 0;
						const auto l_scope_begin = std::next(out_token_iterator_p);
						auto l_scope_end = l_scope_begin;
						do
						{
							switch (l_scope_end->_vocabulary)
							{
							case Vocabulary::_LeftCurlyBracket:
								++l_scope_level;
								break;

							case Vocabulary::_RightCurlyBracket:
								--l_scope_level;
								break;

							default:
								break;
							}
							++l_scope_end;
						} while (l_scope_level > 0);
						out_token_iterator_p = l_scope_end;
					}
					continue;


				case Vocabulary::_AssignmentOperator:
					while (out_token_iterator_p->_vocabulary != Vocabulary::_Semicolon)
					{
						++out_token_iterator_p;
					}
					continue;


				default:
					break;
				}
				l_node._nested_namespaces.emplace_back(build_namespace_node_recursive(l_node._target_namespace_name, out_token_iterator_p, end_p, context_stack_p));
				break;

			case Vocabulary::_FrogmanEngineClassReflectionMacro:
				_FE_FALLTHROUGH_;
			case Vocabulary::_ClassIdentifier:
				l_node._classes.emplace_back(build_class_node(l_node._target_namespace_name, out_token_iterator_p, end_p));
				break;

			case Vocabulary::_FrogmanEngineStructReflectionMacro:
				_FE_FALLTHROUGH_;
			case Vocabulary::_StructIdentifier:
				l_node._structs.emplace_back(build_struct_node(l_node._target_namespace_name, out_token_iterator_p, end_p));
				break;

			case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
				_FE_FALLTHROUGH_;
			case Vocabulary::_EnumStructIdentifier:
				l_node._enum_structs.emplace_back(build_enum_struct_node(l_node._target_namespace_name, out_token_iterator_p, end_p));
				break;


			default:
				++out_token_iterator_p;
				break;
			}
		}

		return l_node;
	}




	enum struct AccessModifierScope : FE::uint8
	{
		_Private = 0,
		_Protected = 1,
		_Public = 2
	};

	_FE_NODISCARD_ class_node build_class_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_FrogmanEngineClassReflectionMacro || out_token_iterator_p->_vocabulary == Vocabulary::_ClassIdentifier);
		class_node l_node = 
		{
			._this_class_name{ parent_namespace_p, framework::get_framework().get_memory_resource() }
		};

		if (out_token_iterator_p->_vocabulary == Vocabulary::_FrogmanEngineClassReflectionMacro)
		{
			l_node._has_marker = true;
			while (out_token_iterator_p->_vocabulary != Vocabulary::_ClassIdentifier)
			{
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "Reached the end of code before reaching to FE_CLASS() marker");
				++out_token_iterator_p;
			}
		}
		file_buffer_t l_default_constructor = { out_token_iterator_p->_code, framework::get_framework().get_memory_resource() };


		++out_token_iterator_p;
		if (out_token_iterator_p->_vocabulary == Vocabulary::_Semicolon)
		{
			l_node._is_forward_decl = true;
			return l_node;
		}


		while (out_token_iterator_p->_vocabulary != Vocabulary::_AnyDecl)
		{
			++out_token_iterator_p;
		}


		file_buffer_t l_scope = { out_token_iterator_p->_code, framework::get_framework().get_memory_resource() };
		std::pmr::list<token> l_tokens = FHT::tokenizer::tokenize_any_decl(l_scope);

		FHT::tokenizer::purge_comments(l_tokens);// removes /**/ and // comments.
		FHT::tokenizer::purge_preprocessor(l_tokens);// removes the # preprocessor directives and its contents.
		FHT::tokenizer::purge_string_literals_and_backslashes(l_tokens); // removes the \, characters, and strings.
		FHT::tokenizer::purge_template(l_tokens); // removes the template declarations.
		std::erase_if(l_tokens, [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; });

		for (auto& token : l_tokens)
		{
			if (token._vocabulary == Vocabulary::_LeftCurlyBracket)
			{
				break;
			}
			l_default_constructor += token._code;
		}
		l_node._this_class_name += l_default_constructor;
		file_buffer_t l_constructor_variant = { l_default_constructor, framework::get_framework().get_memory_resource() };

		l_default_constructor += u8"()";
		l_constructor_variant += u8"(";


		l_tokens = FHT::tokenizer::tokenize_header(l_scope, L"");

		FHT::tokenizer::purge_comments(l_tokens);// removes /**/ and // comments.
		FHT::tokenizer::purge_preprocessor(l_tokens);// removes the # preprocessor directives and its contents.
		FHT::tokenizer::purge_string_literals_and_backslashes(l_tokens); // removes the \, characters, and strings.
		FHT::tokenizer::purge_template(l_tokens); // removes the template declarations.
		std::erase_if(l_tokens, [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; });


		AccessModifierScope l_current_access_modifier_scope = AccessModifierScope::_Private;
		var::boolean l_is_current_func_virtual = false;
		var::boolean l_is_probably_destructor = false;
		for (const auto& token : l_tokens)
		{
			switch (token._vocabulary)
			{
			case Vocabulary::_Private:
				l_current_access_modifier_scope = AccessModifierScope::_Private;
				break;

			case Vocabulary::_Protected:
				l_current_access_modifier_scope = AccessModifierScope::_Protected;
				break;

			case Vocabulary::_Public:
				l_current_access_modifier_scope = AccessModifierScope::_Public;
				break;


			case Vocabulary::_Virtual:
				l_is_current_func_virtual = true;
				break;

			case Vocabulary::_BitwiseNot:
				l_is_probably_destructor = true;
				break;


			case Vocabulary::_AnyDecl:
				_FE_FALLTHROUGH_;
			case Vocabulary::_ClassStructEnumMethodForwardDeclaration:
			{
				l_scope = token._code; // copy a function scope
				auto l_func_tokens = FHT::tokenizer::tokenize_any_decl(l_scope);
				FHT::tokenizer::purge_comments(l_func_tokens);// removes /**/ and // comments.
				FHT::tokenizer::purge_preprocessor(l_func_tokens);// removes the # preprocessor directives and its contents.
				FHT::tokenizer::purge_string_literals_and_backslashes(l_func_tokens); // removes the \, characters, and strings.
				FHT::tokenizer::purge_template(l_func_tokens); // removes the template declarations.
				std::erase_if(l_func_tokens, [](const auto& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; });


				if (l_is_current_func_virtual)
				{
					var::boolean l_is_probably_pure_virtual = false;
					for (auto func_token = l_func_tokens.rbegin(); func_token != l_func_tokens.rend(); ++func_token)
					{
						switch (func_token->_vocabulary)
						{
						case Vocabulary::_AssignmentOperator:
							if (l_is_probably_pure_virtual /*&& l_prev_token == Vocabulary::_Virtual*/)
							{
								l_node._has_pure_virtual = true;
								goto EscapeLoopA;
							}
							break;

						default:
							if (func_token->_code == u8"0")
							{
								l_is_probably_pure_virtual = true;
								break;
							}
							l_is_probably_pure_virtual = false;
							break;
						}
					}
				EscapeLoopA:
					break;
				}


				identifier l_function{ framework::get_framework().get_memory_resource() };
				for (const auto& func_token : l_func_tokens)
				{
					if (func_token._vocabulary == Vocabulary::_Semicolon || func_token._vocabulary == Vocabulary::_LeftCurlyBracket)
					{
						break;
					}
					l_function += func_token._code;
					l_function += u8' ';
				}

				if (l_is_probably_destructor && FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), l_default_constructor.c_str()))
				{
					if (FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), u8"=delete"))
					{
						l_node._is_destructor_deleted_or_not_public = true;
					}
					else
					{
						l_node._is_destructor_deleted_or_not_public = l_current_access_modifier_scope != AccessModifierScope::_Public;
					}
					break;
				}

				if (FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), l_default_constructor.c_str()))
				{
					if (FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), u8"=delete"))
					{
						l_node._has_explicit_default_public_constructor = false;
					}
					else
					{
						l_node._has_explicit_default_public_constructor = l_current_access_modifier_scope == AccessModifierScope::_Public;
					}
					break;
				}

				if (FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), l_constructor_variant.c_str()))
				{
					l_node._has_constructor_variants = true;
				}
				break;
			}


			default:
				l_is_current_func_virtual = false;
				l_is_probably_destructor = false;
				break;
			}
		}
		return l_node;
	}

	_FE_NODISCARD_ struct_node build_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_FrogmanEngineStructReflectionMacro || out_token_iterator_p->_vocabulary == Vocabulary::_StructIdentifier);
		struct_node l_node =
		{
			._identifier{ parent_namespace_p, framework::get_framework().get_memory_resource() }
		};
		if (out_token_iterator_p->_vocabulary == Vocabulary::_FrogmanEngineStructReflectionMacro)
		{
			l_node._has_marker = true;
			while (out_token_iterator_p->_vocabulary != Vocabulary::_StructIdentifier)
			{
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "Reached the end of code before reaching to FE_STRUCT() marker");
				++out_token_iterator_p;
			}
		}
		file_buffer_t l_default_constructor = { out_token_iterator_p->_code, framework::get_framework().get_memory_resource() };


		++out_token_iterator_p;
		if (out_token_iterator_p->_vocabulary == Vocabulary::_Semicolon)
		{
			l_node._is_forward_decl = true;
			return l_node;
		}


		while (out_token_iterator_p->_vocabulary != Vocabulary::_AnyDecl)
		{
			++out_token_iterator_p;
		}


		file_buffer_t l_scope = { out_token_iterator_p->_code, framework::get_framework().get_memory_resource() };
		std::pmr::list<token> l_tokens = FHT::tokenizer::tokenize_any_decl(l_scope);

		FHT::tokenizer::purge_comments(l_tokens);// removes /**/ and // comments.
		FHT::tokenizer::purge_preprocessor(l_tokens);// removes the # preprocessor directives and its contents.
		FHT::tokenizer::purge_string_literals_and_backslashes(l_tokens); // removes the \, characters, and strings.
		FHT::tokenizer::purge_template(l_tokens); // removes the template declarations.
		std::erase_if(l_tokens, [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; });

		for (auto& token : l_tokens)
		{
			if (token._vocabulary == Vocabulary::_LeftCurlyBracket)
			{
				break;
			}
			l_default_constructor += token._code;
		}
		l_node._identifier += l_default_constructor;
		file_buffer_t l_constructor_variant = { l_default_constructor, framework::get_framework().get_memory_resource() };

		l_default_constructor += u8"()";
		l_constructor_variant += u8"(";


		l_tokens = FHT::tokenizer::tokenize_header(l_scope, L"");

		FHT::tokenizer::purge_comments(l_tokens);// removes /**/ and // comments.
		FHT::tokenizer::purge_preprocessor(l_tokens);// removes the # preprocessor directives and its contents.
		FHT::tokenizer::purge_string_literals_and_backslashes(l_tokens); // removes the \, characters, and strings.
		FHT::tokenizer::purge_template(l_tokens); // removes the template declarations.
		std::erase_if(l_tokens, [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; });


		AccessModifierScope l_current_access_modifier_scope = AccessModifierScope::_Public;
		var::boolean l_is_current_func_virtual = false;
		var::boolean l_is_probably_destructor = false;
		for (const auto& token : l_tokens)
		{
			switch (token._vocabulary)
			{
			case Vocabulary::_Private:
				l_current_access_modifier_scope = AccessModifierScope::_Private;
				break;

			case Vocabulary::_Protected:
				l_current_access_modifier_scope = AccessModifierScope::_Protected;
				break;

			case Vocabulary::_Public:
				l_current_access_modifier_scope = AccessModifierScope::_Public;
				break;


			case Vocabulary::_BitwiseNot:
				l_is_probably_destructor = true;
				break;


			case Vocabulary::_AnyDecl:
				_FE_FALLTHROUGH_;
			case Vocabulary::_ClassStructEnumMethodForwardDeclaration:
			{
				l_scope = token._code; // copy a function scope
				auto l_func_tokens = FHT::tokenizer::tokenize_any_decl(l_scope);
				FHT::tokenizer::purge_comments(l_func_tokens);// removes /**/ and // comments.
				FHT::tokenizer::purge_preprocessor(l_func_tokens);// removes the # preprocessor directives and its contents.
				FHT::tokenizer::purge_string_literals_and_backslashes(l_func_tokens); // removes the \, characters, and strings.
				FHT::tokenizer::purge_template(l_func_tokens); // removes the template declarations.
				std::erase_if(l_func_tokens, [](const auto& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; });


				THROW_CPP_SYNTAX_ERROR(l_is_current_func_virtual, "the virtual function declaration is not allowed inside a struct in Frogman C++.");


				identifier l_function{ framework::get_framework().get_memory_resource() };
				for (const auto& func_token : l_func_tokens)
				{
					if (func_token._vocabulary == Vocabulary::_Semicolon || func_token._vocabulary == Vocabulary::_LeftCurlyBracket)
					{
						break;
					}
					l_function += func_token._code;
					l_function += u8' ';
				}

				if (l_is_probably_destructor && FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), l_default_constructor.c_str()))
				{
					if (FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), u8"=delete"))
					{
						l_node._is_destructor_deleted_or_not_public = true;
					}
					else
					{
						l_node._is_destructor_deleted_or_not_public = l_current_access_modifier_scope != AccessModifierScope::_Public;
					}
					break;
				}

				if (FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), l_default_constructor.c_str()))
				{
					if (FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), u8"=delete"))
					{
						l_node._has_explicit_default_public_constructor = false;
					}
					else
					{
						l_node._has_explicit_default_public_constructor = l_current_access_modifier_scope == AccessModifierScope::_Public;
					}
					break;
				}

				if (FE::algorithm::string::space_insensitive_contains(l_function.c_str(), l_function.length(), l_constructor_variant.c_str()))
				{
					l_node._has_constructor_variants = true;
				}
				break;
			}


			default:
				l_is_probably_destructor = false;
				break;
			}
		}
		return l_node;
	}

	_FE_NODISCARD_ enum_struct_node build_enum_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_FrogmanEngineEnumStructReflectionMacro ||out_token_iterator_p->_vocabulary == Vocabulary::_EnumStructIdentifier, "Assertion Failed: Enum struct identifier is missing.");
		enum_struct_node l_node =
		{
			._target_enum_struct_name{ parent_namespace_p , framework::get_framework().get_memory_resource() },
			._enum_struct_fields{ framework::get_framework().get_memory_resource() }
		};
		if (out_token_iterator_p->_vocabulary == Vocabulary::_FrogmanEngineEnumStructReflectionMacro)
		{
			l_node._has_marker = true;
			while (out_token_iterator_p->_vocabulary != Vocabulary::_EnumStructIdentifier)
			{
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "Reached the end of code before reaching to FE_ENUM_STRUCT() marker");
				++out_token_iterator_p;
			}
		}
		l_node._target_enum_struct_name += out_token_iterator_p->_code;


		while (out_token_iterator_p != end_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_RightCurlyBracket:
				++out_token_iterator_p;
				l_node._is_forward_decl = false;
				return l_node;

			case Vocabulary::_EnumStructField:
				l_node._enum_struct_fields.emplace_back(out_token_iterator_p->_code);
				break;

			case Vocabulary::_Semicolon:
				return l_node;

			default:
				break;
			}
			++out_token_iterator_p;
		}

		return l_node;
	}

	_FE_NODISCARD_ system_node build_ecs_system_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		system_node l_node;
		while (out_token_iterator_p != end_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_FrogmanEngineSystemMacro:
				_FE_FALLTHROUGH_;
			case Vocabulary::_LeftParen:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Comma:
				_FE_FALLTHROUGH_;
			case Vocabulary::_NamespaceConcatenator:
				++out_token_iterator_p;
				break;

			case Vocabulary::_RightParen:
				++out_token_iterator_p;
				goto ExitLoop;

			case Vocabulary::_FrogmanEngineSystemArgSysCallPhase:
				l_node._syscall_phase = out_token_iterator_p->_code;
				++out_token_iterator_p;
				break;

			case Vocabulary::_FrogmanEngineSystemArgWorldTagEnum:
				l_node._world_tag_enum = out_token_iterator_p->_code;
				++out_token_iterator_p;
				break;

			default:
				THROW_CPP_SYNTAX_ERROR(true, "Frogman Engine C++ Reflection Syntax Error: the FE_SYSTEM() macro argument order is incorrect, or is ill-formed.");
				break;
			}
		}
	ExitLoop:
		while (out_token_iterator_p->_vocabulary != Vocabulary::_AnyDecl
			&& out_token_iterator_p->_vocabulary != Vocabulary::_ClassStructEnumMethodForwardDeclaration)
		{
			++out_token_iterator_p;
		}

		THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_code.starts_with(u8"void") == false, "Frogman Engine C++ Reflection Syntax Error: this function cannot be qualified as the Frogman Engine ECS system method; the function signature requirement is: void(::FE::world&).");
		l_node._sysname = out_token_iterator_p->_code;

		constexpr auto l_void_keyword = u8"void";
		auto l_void_pos = l_node._sysname.find(l_void_keyword);
		FE_ASSERT(l_void_pos != identifier::npos);
		l_node._sysname.erase(0, l_void_pos + FE::algorithm::string::length(l_void_keyword));

		THROW_CPP_SYNTAX_ERROR(FE::algorithm::string::space_insensitive_contains(l_node._sysname.c_str(), l_node._sysname.length(), u8"world&") == false, "Frogman Engine C++ Reflection Syntax Error: this function cannot be qualified as the Frogman Engine ECS system method; the function signature requirement is: void(::FE::world&).");

		var::uint64 l_space_length = 0;
		for (auto i = 0; i < l_node._sysname.length(); ++i)
		{
			if (l_node._sysname[i] <= ' ')
			{
				++l_space_length;
				continue;
			}
			break;
		}
		l_node._sysname.erase(0, l_space_length);

		auto l_end_of_fn_name_pos = l_node._sysname.find('(');
		THROW_CPP_SYNTAX_ERROR(l_end_of_fn_name_pos == identifier::npos, "Frogman Engine C++ Reflection Syntax Error: this function cannot be qualified as the Frogman Engine ECS system method; the function signature requirement is: void(::FE::world&).");

		l_node._sysname.erase(l_end_of_fn_name_pos, l_node._sysname.length() - l_end_of_fn_name_pos);
		l_node._sysname.insert(0, parent_namespace_p);
		return l_node;
	}
}