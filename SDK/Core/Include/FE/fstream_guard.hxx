#ifndef _FE_CORE_FSTREAM_GUARD_HXX_
#define _FE_CORE_FSTREAM_GUARD_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/definitions.h>
#include <fstream>




BEGIN_NAMESPACE(FE)

template<class FStream>
class basic_fstream_guard final
{
private:
	FStream& m_fstream;

public:
	typedef FStream fstream_type;

	_FE_FORCE_INLINE_ basic_fstream_guard(FStream& file_io_p) noexcept : m_fstream(file_io_p) {}

	_FE_FORCE_INLINE_ ~basic_fstream_guard() noexcept
	{
		if (this->m_fstream.is_open() == true)
		{
			this->m_fstream.close();
		}
	}

	_FE_FORCE_INLINE_ FStream& get_stream() const noexcept { return this->m_fstream; }

	basic_fstream_guard(const basic_fstream_guard& other_p) noexcept = delete;
	basic_fstream_guard(basic_fstream_guard&& rvalue_p) noexcept = delete;
	basic_fstream_guard& operator=(const basic_fstream_guard& other_p) noexcept = delete;
	basic_fstream_guard& operator=(basic_fstream_guard&& rvalue_p) noexcept = delete;
};

using fstream_guard = FE::basic_fstream_guard<std::fstream>;
using wfstream_guard = FE::basic_fstream_guard<std::wfstream>;

using ofstream_guard = FE::basic_fstream_guard<std::ofstream>;
using wofstream_guard = FE::basic_fstream_guard<std::wofstream>;

using ifstream_guard = FE::basic_fstream_guard<std::ifstream>;
using wifstream_guard = FE::basic_fstream_guard<std::wifstream>;

END_NAMESPACE
#endif
