#include <FE/blacklist_evaluator.hxx>
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include <FE/engine.hpp>
#include <FE/renderer.hxx>

#include <list>

#include <absl/container/flat_hash_map.h>

#include <FE/algorithm/utility.hxx>




BEGIN_NAMESPACE(FE::internal)


using shader_macro_map = absl::flat_hash_map<std::string_view, renderer::shader_define*,
    typename absl::lts_20260107::container_internal::FlatHashMapPolicy<std::string_view, renderer::shader_define*>::DefaultHash,
    typename absl::lts_20260107::container_internal::FlatHashMapPolicy<std::string_view, renderer::shader_define*>::DefaultEq,
    std::pmr::polymorphic_allocator<std::pair<const std::string_view, renderer::shader_define*>>>;


void __regenerate_macro_combination(shader_macro_map& in_out_shader_macro_map_p, typename shader_macro_map::iterator& in_out_current_macro_p) noexcept
{
    ++(in_out_current_macro_p->second->_current_value);
    FE_ASSERT(in_out_current_macro_p->second->_current_value != min_value<FE::int64>, "Assertion failed: value overflowed");

    while (in_out_current_macro_p->second->_current_value > in_out_current_macro_p->second->_value_range._second)
    {
        in_out_current_macro_p->second->_current_value = in_out_current_macro_p->second->_value_range._first;
        ++in_out_current_macro_p;

        if (in_out_current_macro_p == in_out_shader_macro_map_p.end())
        {
            return;
        }

        ++(in_out_current_macro_p->second->_current_value);
    }
    
 //   for (auto& [key, value] : in_out_shader_macro_map_p)
 //   {
 //       std::cout << value->_current_value;
 //   }
	//std::cout << std::endl;

    in_out_current_macro_p = in_out_shader_macro_map_p.begin();
}


enum struct Token : FE::uint8
{
	_Not = 0,                           // !    Priority 0

	_Lhs_IsLessThan_Rhs = 1,            // <    Priority 1
	_Lhs_IsLessThanOrEqualTo_Rhs = 2,   // <=   Priority 1
    _Lhs_IsGreaterThan_Rhs = 3,         // >    Priority 1
    _Lhs_IsGreaterThanOrEqualTo_Rhs = 4,// >=   Priority 1

    _Lhs_IsEqualTo_Rhs = 5,             // ==   Priority 2
    _Lhs_IsNotEqualTo_Rhs = 6,          // !=   Priority 2

	_And = 7,                           // &&   Priority 3

	_Or = 8,                            // ||   Priority 4

	_Identifier = 9,                    // identifier

    _IntegerLiteral = 10,

    _LeftParenthesis = 11, // Priority 0
    _RightParenthesis = 12, // Priority 0
    _Space = 13
};

struct token
{
	Token _token;
	std::pmr::string _identifier; // only valid when _token is _Identifier or _IntegerLiteral
};

Token __tokenize_in_detail(FE::ASCII* iterator_p) noexcept
{
    switch (::FE::algorithm::string::hash_bytes<char>(iterator_p, 2))
    {
    STRING_CASE("<=") :
        return Token::_Lhs_IsLessThanOrEqualTo_Rhs;

    STRING_CASE(">=") :
        return Token::_Lhs_IsGreaterThanOrEqualTo_Rhs;

    STRING_CASE("==") :
        return Token::_Lhs_IsEqualTo_Rhs;

    STRING_CASE("!=") :
        return Token::_Lhs_IsNotEqualTo_Rhs;


    STRING_CASE("&&") :
        return Token::_And;

    STRING_CASE("||") :
        return Token::_Or;


    default:
        break;
    }


    switch (*iterator_p)
    {
    case '!':
        return Token::_Not;

    case '<':
        return Token::_Lhs_IsLessThan_Rhs;

    case '>':
        return Token::_Lhs_IsGreaterThan_Rhs;


    case '(':
        return Token::_LeftParenthesis;

    case ')':
        return Token::_RightParenthesis;


	case '0':
		_FE_FALLTHROUGH_;
	case '1':
		_FE_FALLTHROUGH_;
	case '2':
        _FE_FALLTHROUGH_;
	case '3':
        _FE_FALLTHROUGH_;
	case '4':
        _FE_FALLTHROUGH_;
	case '5':
        _FE_FALLTHROUGH_;
	case '6':
        _FE_FALLTHROUGH_;
	case '7':
        _FE_FALLTHROUGH_;
	case '8':
        _FE_FALLTHROUGH_;
	case '9':
		return Token::_IntegerLiteral;


    case ' ':
        return Token::_Space;


    default:
        return Token::_Identifier;
    }
}

void __tokenize(std::pmr::list<token>& out_token_stream_p, const std::string_view& expression_p) noexcept
{
    for (FE::ASCII* iterator = expression_p.data(); *iterator != '\0';)
    {
        if (*iterator == ' ')
        {
			++iterator; // skip spaces
            continue;
		}

        token l_token = 
        {
            ._token = __tokenize_in_detail(iterator), 
            ._identifier = std::pmr::string(FE::engine::get_engine().get_large_memory_resource())
        };

        switch (l_token._token)
        {
        case Token::_Not: // !
			++iterator;
            break;

        case Token::_Lhs_IsLessThan_Rhs: // <
			++iterator;
            break;

        case Token::_Lhs_IsLessThanOrEqualTo_Rhs: // <=
			iterator += 2;
            break;

        case Token::_Lhs_IsGreaterThan_Rhs: // >
            ++iterator;
            break;

        case Token::_Lhs_IsGreaterThanOrEqualTo_Rhs: // >=
            iterator += 2;
            break;

        case Token::_Lhs_IsEqualTo_Rhs: // ==
            iterator += 2;
            break;

        case Token::_Lhs_IsNotEqualTo_Rhs: // !=
            iterator += 2;
            break;
  
        case Token::_And: // &&
            iterator += 2;
            break;

        case Token::_Or: // ||
            iterator += 2;
            break;


        case Token::_LeftParenthesis: // (
            ++iterator;
            break;

        case Token::_RightParenthesis: // )
            ++iterator;
            break;

        case Token::_Space: // ' '
            ++iterator;
            break;


        default:
            break;
        }

        if ((l_token._token != Token::_Identifier) &&
            (l_token._token != Token::_IntegerLiteral))
        {
            out_token_stream_p.push_back(l_token);
            continue;
        }

        var::size l_identifier_length = 0;
        while ((l_token._token == Token::_Identifier) || (l_token._token == Token::_IntegerLiteral))
        {
            Token l_tmp = __tokenize_in_detail(iterator);
            if ((l_tmp != Token::_Identifier) &&
                (l_tmp != Token::_IntegerLiteral)) _FE_UNLIKELY_
            {
                break;
            }
            ++iterator;
            ++l_identifier_length;
            l_token._token = l_tmp;
        } 
        l_token._identifier.assign(iterator - l_identifier_length, l_identifier_length);

        out_token_stream_p.push_back(l_token);
    }
}

void __get_lhs_and_rhs( std::pmr::list<token>::iterator operator_p, const shader_macro_map& shader_macro_map_p,
                        var::int64& out_lhs_p, var::int64& out_rhs_p) noexcept
{
    FE_EXIT_IF((var::int8)operator_p->_token >= (var::int8)Token::_Identifier, FE::ErrorCode::_ShaderPermutationBlacklistEvaluationSyntaxError_MissingOperator, "Frogman Engine Shader Permutation Blacklist Evaluation Syntax Error: it is not a valid operator");

    auto l_lhs_it = std::prev(operator_p);
    switch (l_lhs_it->_token)
    {
    case Token::_IntegerLiteral:
    {
        auto l_result = FE::algorithm::utility::string_to_int(l_lhs_it->_identifier.data());
        out_lhs_p = l_result._value;
    }
    break;

    case Token::_Identifier:
    {
        auto l_result = shader_macro_map_p.find(l_lhs_it->_identifier);
        FE_EXIT_IF(l_result == shader_macro_map_p.end(), FE::ErrorCode::_ShaderPermutationBlacklistEvaluationSyntaxError_MacroUndefined, "Frogman Engine Shader Permutation Blacklist Evaluation Syntax Error: macro undefined");
        out_lhs_p = (l_result->second->_current_value);
    }
    break;

    _FE_NODEFAULT_;
    }


    auto l_rhs_it = std::next(operator_p);
    switch (l_rhs_it->_token)
    {
    case Token::_IntegerLiteral:
    {
        auto l_result = FE::algorithm::utility::string_to_int(l_rhs_it->_identifier.data());
        out_rhs_p = l_result._value;
    }
    break;

    case Token::_Identifier:
    {
        auto l_result = shader_macro_map_p.find(l_rhs_it->_identifier);
        FE_EXIT_IF(l_result == shader_macro_map_p.end(), FE::ErrorCode::_ShaderPermutationBlacklistEvaluationSyntaxError_MacroUndefined, "Frogman Engine Shader Permutation Blacklist Evaluation Syntax Error: macro undefined");
        out_rhs_p = (l_result->second->_current_value);
    }
    break;

    _FE_NODEFAULT_;
    }
}

void __evaluate_token_stream(const shader_macro_map& shader_macro_map_p, std::pmr::list<token>& token_stream_p) noexcept
{
    // Calculate !
    for (auto token = token_stream_p.begin(); token != token_stream_p.end(); ++token)
    {
        if (token->_token == Token::_Not)
        {
			auto l_operator = token;
            ++token;
            FE_EXIT_IF(token == token_stream_p.end(), FE::ErrorCode::_ShaderPermutationBlacklistEvaluationSyntaxError_MissingOperand, "Frogman Engine Shader Permutation Blacklist Evaluation Syntax Error: missing operand");
            
			var::int64 l_value = 0;
            switch (token->_token)
            {
            case Token::_IntegerLiteral:
                l_value = ~FE::algorithm::utility::string_to_int(token->_identifier.data())._value;
                break;

            case Token::_Identifier:
                {
                    auto l_result = shader_macro_map_p.find(token->_identifier);
                    FE_EXIT_IF(l_result == shader_macro_map_p.end(), FE::ErrorCode::_ShaderPermutationBlacklistEvaluationSyntaxError_MacroUndefined, "Frogman Engine Shader Permutation Blacklist Evaluation Syntax Error: macro undefined");
                    
                    l_value = ~(l_result->second->_current_value);
					token->_token = Token::_IntegerLiteral;
                }
				break;

            _FE_NODEFAULT_;
            }
            token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_value));
            FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_value);
			token_stream_p.erase(l_operator);
        }
    }


	// Calculate <, <=, >, and >=
    for (auto token = token_stream_p.begin(); token != token_stream_p.end(); ++token)
    {
        switch (token->_token)
        {
        case Token::_Lhs_IsLessThan_Rhs:
            {
                var::int64 l_lhs = 0;
			    var::int64 l_rhs = 0;
			    __get_lhs_and_rhs(token, shader_macro_map_p, l_lhs, l_rhs);

                var::int64 l_result = l_lhs < l_rhs;
                token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_result));
                FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_result);
                token->_token = Token::_IntegerLiteral;

                token_stream_p.erase(std::prev(token));
				token_stream_p.erase(std::next(token));
            }
            break;

        case Token::_Lhs_IsLessThanOrEqualTo_Rhs:
        {
            var::int64 l_lhs = 0;
            var::int64 l_rhs = 0;
            __get_lhs_and_rhs(token, shader_macro_map_p, l_lhs, l_rhs);

            var::int64 l_result = l_lhs <= l_rhs;
            token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_result));
            FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_result);
            token->_token = Token::_IntegerLiteral;

            token_stream_p.erase(std::prev(token));
            token_stream_p.erase(std::next(token));
        }
        break;

        case Token::_Lhs_IsGreaterThan_Rhs:
        {
            var::int64 l_lhs = 0;
            var::int64 l_rhs = 0;
            __get_lhs_and_rhs(token, shader_macro_map_p, l_lhs, l_rhs);

            var::int64 l_result = l_lhs > l_rhs;
            token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_result));
            FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_result);
            token->_token = Token::_IntegerLiteral;

            token_stream_p.erase(std::prev(token));
            token_stream_p.erase(std::next(token));
        }

        case Token::_Lhs_IsGreaterThanOrEqualTo_Rhs:
        {
            var::int64 l_lhs = 0;
            var::int64 l_rhs = 0;
            __get_lhs_and_rhs(token, shader_macro_map_p, l_lhs, l_rhs);

            var::int64 l_result = l_lhs >= l_rhs;
            token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_result));
            FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_result);
            token->_token = Token::_IntegerLiteral;

            token_stream_p.erase(std::prev(token));
            token_stream_p.erase(std::next(token));
        }
        break;

        default:
            break;
        }
    }


	// == and !=
    for (auto token = token_stream_p.begin(); token != token_stream_p.end(); ++token)
    {
        switch (token->_token)
        {
        case Token::_Lhs_IsEqualTo_Rhs:
        {
            var::int64 l_lhs = 0;
            var::int64 l_rhs = 0;
            __get_lhs_and_rhs(token, shader_macro_map_p, l_lhs, l_rhs);

            var::int64 l_result = l_lhs == l_rhs;
            token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_result));
            FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_result);
            token->_token = Token::_IntegerLiteral;

            token_stream_p.erase(std::prev(token));
            token_stream_p.erase(std::next(token));
        }
        break;

        case Token::_Lhs_IsNotEqualTo_Rhs:
        {
            var::int64 l_lhs = 0;
            var::int64 l_rhs = 0;
            __get_lhs_and_rhs(token, shader_macro_map_p, l_lhs, l_rhs);

            var::int64 l_result = l_lhs != l_rhs;
            token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_result));
            FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_result);
            token->_token = Token::_IntegerLiteral;

            token_stream_p.erase(std::prev(token));
            token_stream_p.erase(std::next(token));
        }
        break;

        default:
            break;
        }
    }


    // &&
    for (auto token = token_stream_p.begin(); token != token_stream_p.end(); ++token)
    {
        if (token->_token == Token::_And)
        {
            var::int64 l_lhs = 0;
            var::int64 l_rhs = 0;
            __get_lhs_and_rhs(token, shader_macro_map_p, l_lhs, l_rhs);

            var::int64 l_result = l_lhs & l_rhs;
            token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_result));
            FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_result);
            token->_token = Token::_IntegerLiteral;

            token_stream_p.erase(std::prev(token));
            token_stream_p.erase(std::next(token));
        }
    }


    // ||
    for (auto token = token_stream_p.begin(); token != token_stream_p.end(); ++token)
    {
        if (token->_token == Token::_Or)
        {
            var::int64 l_lhs = 0;
            var::int64 l_rhs = 0;
            __get_lhs_and_rhs(token, shader_macro_map_p, l_lhs, l_rhs);

            var::int64 l_result = l_lhs | l_rhs;
            token->_identifier.resize(FE::algorithm::utility::count_int_digit_length(l_result));
            FE::algorithm::utility::int_to_string(token->_identifier.data(), token->_identifier.length(), l_result);
            token->_token = Token::_IntegerLiteral;

            token_stream_p.erase(std::prev(token));
            token_stream_p.erase(std::next(token));
        }
    }
}

void __evaluate_blacklist(const shader_macro_map& shader_macro_map_p, const std::string_view& expression_p, var::boolean& is_blacklisted_p) noexcept
{
    std::pmr::list<token> l_token_stream(FE::engine::get_engine().get_large_memory_resource());
	__tokenize(l_token_stream, expression_p);

    std::pmr::vector<Token> l_syntax_checker(FE::engine::get_engine().get_large_memory_resource());
    l_syntax_checker.reserve(l_token_stream.size());
    for (auto token = l_token_stream.begin(); token != l_token_stream.end(); ++token)
    {
        switch (token->_token)
        {
        case Token::_LeftParenthesis:
			l_syntax_checker.push_back(Token::_LeftParenthesis);
            break; 

        case Token::_RightParenthesis:
            FE_EXIT_IF(l_syntax_checker.size() == 0, FE::ErrorCode::_ShaderPermutationBlacklistEvaluationSyntaxError_MissingParenthesis, "Frogman Engine Shader Permutation Blacklist Evaluation Syntax Error: missing parenthesis");
            l_syntax_checker.pop_back();
            break;
        }
    }
	FE_EXIT_IF(l_syntax_checker.size() != 0, FE::ErrorCode::_ShaderPermutationBlacklistEvaluationSyntaxError_MissingParenthesis, "Frogman Engine Shader Permutation Blacklist Evaluation Syntax Error: missing parenthesis");


    std::pmr::list<token> l_intermediate_buffer(FE::engine::get_engine().get_large_memory_resource());
    for (auto token = l_token_stream.begin(); token != l_token_stream.end(); ++token)
    {
        switch (token->_token)
        {
        case Token::_RightParenthesis:
            {
                auto l_iterator = std::prev(token);
                for (; l_iterator->_token != Token::_LeftParenthesis; --l_iterator)
                {
                    l_intermediate_buffer.push_front(*l_iterator);
                }
                l_token_stream.erase(l_iterator, token);

                __evaluate_token_stream(shader_macro_map_p, l_intermediate_buffer);
                *token = std::move(l_intermediate_buffer.front());
                l_intermediate_buffer.clear();
                token = l_token_stream.begin();
                break;
            }
            break;
        }
    }

    __evaluate_token_stream(shader_macro_map_p, l_token_stream);
    FE::algorithm::utility::int_info l_result = FE::algorithm::utility::string_to_int(l_token_stream.front()._identifier.data());
    if (l_result._value != 0)
    {
        is_blacklisted_p = true;
    }
}

void __filter_shader_macro_combinations(renderer::shader& in_out_shader_p) noexcept
{
    if (in_out_shader_p._permutation_blacklist.size() == 0) _FE_UNLIKELY_
    {
        return;
    }

    shader_macro_map l_valid_defines(FE::engine::get_engine().get_large_memory_resource()); // probe failure leads to expression evaluation failure.

    for (renderer::shader_define& define : in_out_shader_p._defines)
    {
		l_valid_defines[define._identifier] = &define; // register them all
    }

    // evaluate the blacklist and remove any permutations that match any of the blacklisted combinations.
	std::pmr::string l_blacklist_expr(FE::engine::get_engine().get_large_memory_resource());
    var::size l_to_reserve = 0;
    for (std::pmr::string& blacklist : in_out_shader_p._permutation_blacklist)
    {
		l_to_reserve += blacklist.length();
        l_to_reserve += FE::algorithm::string::length("() || ");
    }
	l_blacklist_expr.reserve(l_to_reserve);

    l_blacklist_expr += '(';
    l_blacklist_expr += in_out_shader_p._permutation_blacklist.front();
    l_blacklist_expr += ")";

    for (auto blacklist = std::next( in_out_shader_p._permutation_blacklist.begin() ); blacklist != in_out_shader_p._permutation_blacklist.end(); ++blacklist)
    {
        l_blacklist_expr += " || ";
		l_blacklist_expr += '(';
        l_blacklist_expr += *blacklist;
		l_blacklist_expr += ")";
    }

    for (typename shader_macro_map::iterator it = l_valid_defines.begin(); it != l_valid_defines.end();)
    {
        var::boolean l_is_blacklisted = false;
        __evaluate_blacklist(l_valid_defines, l_blacklist_expr, l_is_blacklisted);

        if (l_is_blacklisted == true)
        {
            __regenerate_macro_combination(l_valid_defines, it);
            continue;
        }

        in_out_shader_p._macro_combinations.emplace_back(std::pmr::vector<renderer::shader::macro>{FE::engine::get_engine().get_large_memory_resource()});
        in_out_shader_p._macro_combinations.back().reserve(l_valid_defines.size());

        for (const auto& [identifier, define] : l_valid_defines)
        {
            FE::uint64 l_capacity = FE::algorithm::utility::count_int_digit_length(define->_current_value) + 1;
            renderer::shader::macro l_macro =
            {
                .Name = identifier.data(),
                .Definition = std::pmr::polymorphic_allocator< char >(in_out_shader_p._macro_combinations.back().get_allocator()).allocate(l_capacity)
            };
            FE::algorithm::utility::int_to_string< char >((char*)l_macro.Definition, l_capacity, define->_current_value);
            in_out_shader_p._macro_combinations.back().push_back(l_macro);
        }
		in_out_shader_p._macro_combinations.back().emplace_back(nullptr, nullptr); // null-terminator

        __regenerate_macro_combination(l_valid_defines, it);
    }
}


END_NAMESPACE