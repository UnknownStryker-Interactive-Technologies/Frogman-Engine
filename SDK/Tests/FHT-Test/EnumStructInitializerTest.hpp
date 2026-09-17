#ifndef _FE_HEADER_TOOL_ENUM_STRUCT_INITIALIZER_TEST_HPP_
#define _FE_HEADER_TOOL_ENUM_STRUCT_INITIALIZER_TEST_HPP_

// FHT Does not support such expressions below!
enum struct init_kind_i1
{
	_A = pick_enum_initializer(1, 2),
	_B
};
enum struct init_kind_i2
{
	_A = std::is_same_v<var::int32, var::int64>,
	_B
};
enum struct init_kind_i3
{
	_A = static_cast<var::int32>(sizeof(std::pair<var::int32, var::int64>)),
	_B
};
enum struct init_kind_i4
{
	_A = sizeof(std::pair<var::int32, var::int64>),
	_B
};


#endif // _FE_HEADER_TOOL_ENUM_STRUCT_INITIALIZER_TEST_HPP_
