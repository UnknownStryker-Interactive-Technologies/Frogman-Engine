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

    FE::algorithm::string::to_lowercase(l_buffer.data());

    return 0;
}