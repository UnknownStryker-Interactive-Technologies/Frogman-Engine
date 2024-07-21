// std
#include <string>
#include <cstring>

// test target
#include <FE/core/algorithm/string.hxx>


extern "C" int LLVMFuzzerTestOneInput(const char* data_p, _MAYBE_UNUSED_ long long size_p)
{
    std::string l_buffer;
    l_buffer.assign(data_p, size_p);
    l_buffer.back() = '\0';

    _DISCARD_ auto l_result = FE::algorithm::string::count_chars(l_buffer.c_str(), *l_buffer.c_str());

    return 0;
}