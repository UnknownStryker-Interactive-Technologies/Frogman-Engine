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
#include "preprocessor.hpp"
#include "tokenizer.hpp"




namespace FHT::preprocessor
{
	void preprocess_macros(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, const program_options::defined_macros& macro_table_p) noexcept
	{
		while (*code_iterator_p <= ' ') // skip all spaces
		{
			++code_iterator_p;
		}

		var::uint16 l_macro_identifier_length = 0;
		for (auto it = code_iterator_p; FHT::tokenizer::is_a_valid_letter_for_identifiers(*it); ++it)
		{
			++l_macro_identifier_length;
		}
		out_token_p._code.assign(code_iterator_p, l_macro_identifier_length);


		STRING_SWITCH(out_token_p._code.c_str())
		{
		STRING_CASE(u8"FE_CLASS_HAS_A_BASE"):
			_FE_FALLTHROUGH_;

		STRING_CASE(u8"FE_PROPERTY"):
			_FE_FALLTHROUGH_;

		STRING_CASE(u8"FE_STATIC_METHOD"):
			_FE_FALLTHROUGH_;

		STRING_CASE(u8"FE_METHOD"):
			_FE_FALLTHROUGH_;


		STRING_CASE(u8"FE_CLASS"):
			_FE_FALLTHROUGH_;

		STRING_CASE(u8"FE_STRUCT"):
			_FE_FALLTHROUGH_;

		STRING_CASE(u8"FE_ENUM_STRUCT"):
			_FE_FALLTHROUGH_;

		STRING_CASE(u8"FE_SYSTEM"):
			_FE_FALLTHROUGH_;

		STRING_CASE(u8"FE_ENABLE_SERIALIZATION") :
			_FE_FALLTHROUGH_;

		STRING_CASE(u8"FHT_GENERATED"):
			_FE_FALLTHROUGH_;


		STRING_CASE(u8"END_NAMESPACE") :
			out_token_p._code.clear();
			return; // Frogman Engine markers cannot be overriden
		}


		auto l_macro_define = macro_table_p.find((FE::ASCII*)out_token_p._code.c_str());
		if (l_macro_define == macro_table_p.end())
		{
			out_token_p._code.clear();
			return; // macro not found.
		}

		// macro found! Substitute!
		out_token_p._code.assign( (FE::UTF8*)l_macro_define->second.c_str() );
		out_token_p._vocabulary = Vocabulary::_Macro;
		out_token_p._macro_identifier_length = l_macro_identifier_length;
	}
}