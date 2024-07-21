// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/framework/reflection/property.hpp>




BEGIN_NAMESPACE(FE::framework::reflection)

typename property:: property_meta_data_map_type*    property::s_property_map = nullptr;
thread_local typename property::class_layer_stack   property::tl_s_class_layer;

typename property::lock_type                                 property::s_lock;
thread_local typename property::file_handler                 property::tl_s_fstream; 
thread_local typename property::input_buffer_type            property::tl_s_input_buffer;
thread_local typename property::input_buffer_iterator_type   property::tl_s_position;

END_NAMESPACE