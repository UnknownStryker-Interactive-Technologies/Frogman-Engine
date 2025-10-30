// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/do_once.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/farray.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/algorithm/math.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/fstack.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/concurrent_vector.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/algorithm/utility.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/definitions.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/clock.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/fstream_guard.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/arena.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/fqueue.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/algorithm/string.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/scalable_pool.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/hash.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/list.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/log/format_string.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/log/log.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/iterator.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/log/logger.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/miscellaneous/private/macro_restrictions.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/block_pool.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/private/strlen.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/private/pool_common.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/random.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/miscellaneous/suppress_warnings.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pair.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/pool/memory_resource.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/type_traits.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/prerequisites.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/synchronized_access.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/private/memory_traits.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/function.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/types.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Core/Include/FE/memory.hxx>



void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::int_info*(::int_info*)> >("construct ::int_info", &::std::construct_at<::int_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::int_info*)> >("destruct ::int_info", &::std::destroy_at<::int_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::uint_info*(::uint_info*)> >("construct ::uint_info", &::std::construct_at<::uint_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::uint_info*)> >("destruct ::uint_info", &::std::destroy_at<::uint_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::real_info*(::real_info*)> >("construct ::real_info", &::std::construct_at<::real_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::real_info*)> >("destruct ::real_info", &::std::destroy_at<::real_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::clock*(::FE::clock*)> >("construct ::FE::clock", &::std::construct_at<::FE::clock>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::clock*)> >("destruct ::FE::clock", &::std::destroy_at<::FE::clock>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::range*(::range*)> >("construct ::range", &::std::construct_at<::range>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::range*)> >("destruct ::range", &::std::destroy_at<::range>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::hash_base*(::hash_base*)> >("construct ::hash_base", &::std::construct_at<::hash_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::hash_base*)> >("destruct ::hash_base", &::std::destroy_at<::hash_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::node*(::FE::node*)> >("construct ::FE::node", &::std::construct_at<::FE::node>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::node*)> >("destruct ::FE::node", &::std::destroy_at<::FE::node>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::list_iterator*(::FE::list_iterator*)> >("construct ::FE::list_iterator", &::std::construct_at<::FE::list_iterator>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::list_iterator*)> >("destruct ::FE::list_iterator", &::std::destroy_at<::FE::list_iterator>);

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
        { ::FE::ErrorCode::_FatalMemoryError_1XX_InvalidIteratorOps, "_FatalMemoryError_1XX_InvalidIteratorOps" },
        { ::FE::ErrorCode::_FatalMemoryError_1XX_AllocationFailure, "_FatalMemoryError_1XX_AllocationFailure" },
        { ::FE::ErrorCode::_FatalInputError_2XX_InvalidArgument, "_FatalInputError_2XX_InvalidArgument" },
        { ::FE::ErrorCode::_FatalInputError_2XX_Null, "_FatalInputError_2XX_Null" },
        { ::FE::ErrorCode::_FatalSerializationError_3XX_TypeMismatch, "_FatalSerializationError_3XX_TypeMismatch" },
        { ::FE::ErrorCode::_FatalSerializationError_3XX_TypeNotFound, "_FatalSerializationError_3XX_TypeNotFound" },
        { ::FE::ErrorCode::_FatalSerializationError_3XX_FileVersionMismatch, "_FatalSerializationError_3XX_FileVersionMismatch" },
        { ::FE::ErrorCode::_FatalDeserializationError_3XX_FileBufferEmpty, "_FatalDeserializationError_3XX_FileBufferEmpty" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_OpenProcessTokenFailure, "_FatalWinAPI_Error_4XX_OpenProcessTokenFailure" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_LookupPrivilegeValueFailure, "_FatalWinAPI_Error_4XX_LookupPrivilegeValueFailure" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_AdjustTokenPrivilegesFailure, "_FatalWinAPI_Error_4XX_AdjustTokenPrivilegesFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_4XX_VirtualAllocFailure, "_FatalMemoryError_4XX_VirtualAllocFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_4XX_VirtualLockFailure, "_FatalMemoryError_4XX_VirtualLockFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_4XX_VirtualUnlockFailure, "_FatalMemoryError_4XX_VirtualUnlockFailure" },
        { ::FE::ErrorCode::_FatalMemoryError_4XX_VirtualFreeFailure, "_FatalMemoryError_4XX_VirtualFreeFailure" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_GetTokenInformationFailure, "_FatalWinAPI_Error_4XX_GetTokenInformationFailure" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_LsaOpenPolicyFailure, "_FatalWinAPI_Error_4XX_LsaOpenPolicyFailure" },
        { ::FE::ErrorCode::_FatalWinAPI_Error_4XX_LsaAddAccountRights, "_FatalWinAPI_Error_4XX_LsaAddAccountRights" },
        { ::FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "_FatalRendererError_5XX_GLFW_InitializationFailure" },
        { ::FE::ErrorCode::_FatalRendererError_5XX_GLFW_WindowCreationFailure, "_FatalRendererError_5XX_GLFW_WindowCreationFailure" },
        { ::FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, "_FatalRendererError_5XX_RendererBackendDeviceCreationFailure" },
        { ::FE::ErrorCode::_FatalRendererError_5XX_RendererSwapChainCreationFailure, "_FatalRendererError_5XX_RendererSwapChainCreationFailure" },
        { ::FE::ErrorCode::_FatalRendererError_5XX_RendererRenderTargetViewCreationFailure, "_FatalRendererError_5XX_RendererRenderTargetViewCreationFailure" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::log::logger_base*(::FE::log::logger_base*)> >("construct ::FE::log::logger_base", &::std::construct_at<::FE::log::logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::log::logger_base*)> >("destruct ::FE::log::logger_base", &::std::destroy_at<::FE::log::logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::log::fatal_error_logger_base*(::FE::log::fatal_error_logger_base*)> >("construct ::FE::log::fatal_error_logger_base", &::std::construct_at<::FE::log::fatal_error_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::log::fatal_error_logger_base*)> >("destruct ::FE::log::fatal_error_logger_base", &::std::destroy_at<::FE::log::fatal_error_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::log::message_logger_base*(::FE::log::message_logger_base*)> >("construct ::FE::log::message_logger_base", &::std::construct_at<::FE::log::message_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::log::message_logger_base*)> >("destruct ::FE::log::message_logger_base", &::std::destroy_at<::FE::log::message_logger_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::pool::from_low_address*(::FE::internal::pool::from_low_address*)> >("construct ::FE::internal::pool::from_low_address", &::std::construct_at<::FE::internal::pool::from_low_address>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::pool::from_low_address*)> >("destruct ::FE::internal::pool::from_low_address", &::std::destroy_at<::FE::internal::pool::from_low_address>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::pool::less_than*(::FE::internal::pool::less_than*)> >("construct ::FE::internal::pool::less_than", &::std::construct_at<::FE::internal::pool::less_than>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::pool::less_than*)> >("destruct ::FE::internal::pool::less_than", &::std::destroy_at<::FE::internal::pool::less_than>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::pool::greater_than*(::FE::internal::pool::greater_than*)> >("construct ::FE::internal::pool::greater_than", &::std::construct_at<::FE::internal::pool::greater_than>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::pool::greater_than*)> >("destruct ::FE::internal::pool::greater_than", &::std::destroy_at<::FE::internal::pool::greater_than>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::pool::block_info*(::FE::internal::pool::block_info*)> >("construct ::FE::internal::pool::block_info", &::std::construct_at<::FE::internal::pool::block_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::pool::block_info*)> >("destruct ::FE::internal::pool::block_info", &::std::destroy_at<::FE::internal::pool::block_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::memory_resource*(::FE::memory_resource*)> >("construct ::FE::memory_resource", &::std::construct_at<::FE::memory_resource>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::memory_resource*)> >("destruct ::FE::memory_resource", &::std::destroy_at<::FE::memory_resource>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::void_t*(::FE::void_t*)> >("construct ::FE::void_t", &::std::construct_at<::FE::void_t>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::void_t*)> >("destruct ::FE::void_t", &::std::destroy_at<::FE::void_t>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::argument_base*(::FE::argument_base*)> >("construct ::FE::argument_base", &::std::construct_at<::FE::argument_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::argument_base*)> >("destruct ::FE::argument_base", &::std::destroy_at<::FE::argument_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::void_ptr*(::FE::void_ptr*)> >("construct ::FE::void_ptr", &::std::construct_at<::FE::void_ptr>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::void_ptr*)> >("destruct ::FE::void_ptr", &::std::destroy_at<::FE::void_ptr>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::cache_aligned_resource*(::cache_aligned_resource*)> >("construct ::cache_aligned_resource", &::std::construct_at<::cache_aligned_resource>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::cache_aligned_resource*)> >("destruct ::cache_aligned_resource", &::std::destroy_at<::cache_aligned_resource>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::align_8bytes*(::align_8bytes*)> >("construct ::align_8bytes", &::std::construct_at<::align_8bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::align_8bytes*)> >("destruct ::align_8bytes", &::std::destroy_at<::align_8bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::align_16bytes*(::align_16bytes*)> >("construct ::align_16bytes", &::std::construct_at<::align_16bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::align_16bytes*)> >("destruct ::align_16bytes", &::std::destroy_at<::align_16bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::align_32bytes*(::align_32bytes*)> >("construct ::align_32bytes", &::std::construct_at<::align_32bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::align_32bytes*)> >("destruct ::align_32bytes", &::std::destroy_at<::align_32bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::align_64bytes*(::align_64bytes*)> >("construct ::align_64bytes", &::std::construct_at<::align_64bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::align_64bytes*)> >("destruct ::align_64bytes", &::std::destroy_at<::align_64bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::align_128bytes*(::align_128bytes*)> >("construct ::align_128bytes", &::std::construct_at<::align_128bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::align_128bytes*)> >("destruct ::align_128bytes", &::std::destroy_at<::align_128bytes>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::page_alignment*(::page_alignment*)> >("construct ::page_alignment", &::std::construct_at<::page_alignment>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::page_alignment*)> >("destruct ::page_alignment", &::std::destroy_at<::page_alignment>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::CPU_L1_cache_line*(::CPU_L1_cache_line*)> >("construct ::CPU_L1_cache_line", &::std::construct_at<::CPU_L1_cache_line>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::CPU_L1_cache_line*)> >("destruct ::CPU_L1_cache_line", &::std::destroy_at<::CPU_L1_cache_line>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::SIMD_auto_alignment*(::SIMD_auto_alignment*)> >("construct ::SIMD_auto_alignment", &::std::construct_at<::SIMD_auto_alignment>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::SIMD_auto_alignment*)> >("destruct ::SIMD_auto_alignment", &::std::destroy_at<::SIMD_auto_alignment>);
}
