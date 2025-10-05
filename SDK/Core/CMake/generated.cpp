// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.h> 
#include <FE/framework/framework.hpp> 
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/log/format_string.h>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/miscellaneous/define_max_min.h>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/miscellaneous/undefine_max_min.h>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/private/pool_common.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/miscellaneous/suppress_warnings.h>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/log/logger.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/memory_resource.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/miscellaneous/private/macro_restrictions.h>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/algorithm/math.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/log/log.h>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/block_pool.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/algorithm/utility.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/scalable_pool.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/algorithm/string.hxx>



void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::pool::block_info*(::FE::internal::pool::block_info*)> >("construct ::FE::internal::pool::block_info", &::std::construct_at<::FE::internal::pool::block_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::pool::block_info*)> >("destruct ::FE::internal::pool::block_info", &::std::destroy_at<::FE::internal::pool::block_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::log::fatal_error_logger_base*(::FE::log::fatal_error_logger_base*)> >("construct ::FE::log::fatal_error_logger_base", &::std::construct_at<::FE::log::fatal_error_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::log::fatal_error_logger_base*)> >("destruct ::FE::log::fatal_error_logger_base", &::std::destroy_at<::FE::log::fatal_error_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::log::logger_base*(::FE::log::logger_base*)> >("construct ::FE::log::logger_base", &::std::construct_at<::FE::log::logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::log::logger_base*)> >("destruct ::FE::log::logger_base", &::std::destroy_at<::FE::log::logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::log::fatal_error_logger_base*(::FE::log::fatal_error_logger_base*)> >("construct ::FE::log::fatal_error_logger_base", &::std::construct_at<::FE::log::fatal_error_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::log::fatal_error_logger_base*)> >("destruct ::FE::log::fatal_error_logger_base", &::std::destroy_at<::FE::log::fatal_error_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::log::message_logger_base*(::FE::log::message_logger_base*)> >("construct ::FE::log::message_logger_base", &::std::construct_at<::FE::log::message_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::log::message_logger_base*)> >("destruct ::FE::log::message_logger_base", &::std::destroy_at<::FE::log::message_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::memory_resource*(::FE::memory_resource*)> >("construct ::FE::memory_resource", &::std::construct_at<::FE::memory_resource>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::memory_resource*)> >("destruct ::FE::memory_resource", &::std::destroy_at<::FE::memory_resource>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::ErrorCode >("::FE::ErrorCode",
    {
        { ::FE::ErrorCode::_None, "_None" },
        { ::FE::ErrorCode::_FatalHardwareResourceError_CPU_HasNotEnoughThreads, "_FatalHardwareResourceError_CPU_HasNotEnoughThreads" },
        { ::FE::ErrorCode::_FatalError_DynamicCastFailure_TypeMismatch, "_FatalError_DynamicCastFailure_TypeMismatch" },
        { ::FE::ErrorCode::_FatalLoggerError_IncorrectStringFormatterSyntex, "_FatalLoggerError_IncorrectStringFormatterSyntex" },
        { ::FE::ErrorCode::_FatalSwitchCaseError_ReachedNoDefault, "_FatalSwitchCaseError_ReachedNoDefault" },
        { ::FE::ErrorCode::_FatalError_TableInsertionFailure, "_FatalError_TableInsertionFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment, "_FatalMemoryError_1XX_IncorrectAddressAlignment" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_NullPtr, "_FatalMemoryError_1XX_NullPtr" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_AccessViolation, "_FatalMemoryError_1XX_AccessViolation" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_HeapCorruption, "_FatalMemoryError_1XX_HeapCorruption" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_DoubleFree, "_FatalMemoryError_1XX_DoubleFree" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow, "_FatalMemoryError_1XX_BufferOverflow" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize, "_FatalMemoryError_1XX_InvalidSize" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_InvalidIterator, "_FatalMemoryError_1XX_InvalidIterator" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "_FatalMemoryError_1XX_FalseDeallocation" },
        { ::FE::ErrorCode::_FatalInputError_2XX_InvalidArgument, "_FatalInputError_2XX_InvalidArgument" },
        { ::FE::ErrorCode::_FatalInputError_2XX_Null, "_FatalInputError_2XX_Null" },
        { ::FE::ErrorCode::_FatalSerializationError_3XX_TypeMismatch, "_FatalSerializationError_3XX_TypeMismatch" },
        { ::FE::ErrorCode::_FatalSerializationError_3XX_TypeNotFound, "_FatalSerializationError_3XX_TypeNotFound" },
        { ::FE::ErrorCode::_FatalSerializationError_3XX_FileVersionMismatch, "_FatalSerializationError_3XX_FileVersionMismatch" },
        { ::FE::ErrorCode::_FatalDeserializationError_3XX_FileBufferEmpty, "_FatalDeserializationError_3XX_FileBufferEmpty" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_OpenProcessTokenFailure, "_FatalWinAPI_Error_4XX_OpenProcessTokenFailure" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_LookupPrivilegeValueFailure, "_FatalWinAPI_Error_4XX_LookupPrivilegeValueFailure" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_AdjustTokenPrivilegesFailure, "_FatalWinAPI_Error_4XX_AdjustTokenPrivilegesFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_VirtualAllocFailure, "_FatalMemoryError_1XX_VirtualAllocFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_VirtualLockFailure, "_FatalMemoryError_1XX_VirtualLockFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_VirtualUnlockFailure, "_FatalMemoryError_1XX_VirtualUnlockFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_VirtualFreeFailure, "_FatalMemoryError_1XX_VirtualFreeFailure" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::uint_info*(::uint_info*)> >("construct ::uint_info", &::std::construct_at<::uint_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::uint_info*)> >("destruct ::uint_info", &::std::destroy_at<::uint_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::real_info*(::real_info*)> >("construct ::real_info", &::std::construct_at<::real_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::real_info*)> >("destruct ::real_info", &::std::destroy_at<::real_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::pool::from_low_address*(::FE::internal::pool::from_low_address*)> >("construct ::FE::internal::pool::from_low_address", &::std::construct_at<::FE::internal::pool::from_low_address>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::pool::from_low_address*)> >("destruct ::FE::internal::pool::from_low_address", &::std::destroy_at<::FE::internal::pool::from_low_address>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::pool::less_than*(::FE::internal::pool::less_than*)> >("construct ::FE::internal::pool::less_than", &::std::construct_at<::FE::internal::pool::less_than>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::pool::less_than*)> >("destruct ::FE::internal::pool::less_than", &::std::destroy_at<::FE::internal::pool::less_than>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::pool::greater_than*(::FE::internal::pool::greater_than*)> >("construct ::FE::internal::pool::greater_than", &::std::construct_at<::FE::internal::pool::greater_than>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::pool::greater_than*)> >("destruct ::FE::internal::pool::greater_than", &::std::destroy_at<::FE::internal::pool::greater_than>);
}
