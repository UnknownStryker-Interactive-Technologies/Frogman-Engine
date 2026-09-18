// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\EnumStructInitializerTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\AttributeMacroPlacementTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\MultilineCommentInBodyTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\PreprocessorAndCommentsPurgeTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\AliasTemplatePurgeTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\CommentMarkerInBlockCommentTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\StringPurgeTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\PureVirtualDetectionTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\LiteralTokenizationTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\NamespaceTest.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\SystemParseTest.hpp>




template <typename T, typename... Args>
T* construct(T* location_p, Args&&... args_p)
{
    new(location_p) T( ::std::forward<Args>(args_p)... );
    return location_p;
}


template <typename T>
void destruct(T* ptr_p)
{
    if (ptr_p == nullptr) return;
    ptr_p->~T();
}




void load_reflection_data()
{

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::init_kind_i1 >("::init_kind_i1",
    {
        { ::init_kind_i1::_A, "_A" },
        { ::init_kind_i1::2, "2" },
        { ::init_kind_i1::_B, "_B" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::init_kind_i2 >("::init_kind_i2",
    {
        { ::init_kind_i2::_A, "_A" },
        { ::init_kind_i2::var, "var" },
        { ::init_kind_i2::int64, "int64" },
        { ::init_kind_i2::_B, "_B" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::init_kind_i3 >("::init_kind_i3",
    {
        { ::init_kind_i3::_A, "_A" },
        { ::init_kind_i3::var, "var" },
        { ::init_kind_i3::int64, "int64" },
        { ::init_kind_i3::_B, "_B" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::init_kind_i4 >("::init_kind_i4",
    {
        { ::init_kind_i4::_A, "_A" },
        { ::init_kind_i4::var, "var" },
        { ::init_kind_i4::int64, "int64" },
        { ::init_kind_i4::_B, "_B" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::attr_ok1*(::attr_ok1*)> >("construct ::attr_ok1", &::construct<::attr_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::attr_ok1*)> >("destruct ::attr_ok1", &::destruct<::attr_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::attr_p1*(::attr_p1*)> >("construct ::attr_p1", &::construct<::attr_p1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::attr_p1*)> >("destruct ::attr_p1", &::destruct<::attr_p1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::attr_p2*(::attr_p2*)> >("construct ::attr_p2", &::construct<::attr_p2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::attr_p2*)> >("destruct ::attr_p2", &::destruct<::attr_p2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::attr_ok3*(::attr_ok3*)> >("construct ::attr_ok3", &::construct<::attr_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::attr_ok3*)> >("destruct ::attr_ok3", &::destruct<::attr_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::attr_ok4*(::attr_ok4*)> >("construct ::attr_ok4", &::construct<::attr_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::attr_ok4*)> >("destruct ::attr_ok4", &::destruct<::attr_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::attr_p4*(::attr_p4*)> >("construct ::attr_p4", &::construct<::attr_p4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::attr_p4*)> >("destruct ::attr_p4", &::destruct<::attr_p4>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::attr_kind_ok2 >("::attr_kind_ok2",
    {
        { ::attr_kind_ok2::_A, "_A" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::attr_kind_p3 >("::attr_kind_p3",
    {
        { ::attr_kind_p3::_A, "_A" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_ok1*(::body_comment_ok1*)> >("construct ::body_comment_ok1", &::construct<::body_comment_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_ok1*)> >("destruct ::body_comment_ok1", &::destruct<::body_comment_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_b2*(::body_comment_b2*)> >("construct ::body_comment_b2", &::construct<::body_comment_b2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_b2*)> >("destruct ::body_comment_b2", &::destruct<::body_comment_b2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_sentinel_b4*(::body_comment_sentinel_b4*)> >("construct ::body_comment_sentinel_b4", &::construct<::body_comment_sentinel_b4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_sentinel_b4*)> >("destruct ::body_comment_sentinel_b4", &::destruct<::body_comment_sentinel_b4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_b5*(::body_comment_b5*)> >("construct ::body_comment_b5", &::construct<::body_comment_b5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_b5*)> >("destruct ::body_comment_b5", &::destruct<::body_comment_b5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_sentinel_b5*(::body_comment_sentinel_b5*)> >("construct ::body_comment_sentinel_b5", &::construct<::body_comment_sentinel_b5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_sentinel_b5*)> >("destruct ::body_comment_sentinel_b5", &::destruct<::body_comment_sentinel_b5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_ns_ok5::body_comment_ok5*(::body_comment_ns_ok5::body_comment_ok5*)> >("construct ::body_comment_ns_ok5::body_comment_ok5", &::construct<::body_comment_ns_ok5::body_comment_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_ns_ok5::body_comment_ok5*)> >("destruct ::body_comment_ns_ok5::body_comment_ok5", &::destruct<::body_comment_ns_ok5::body_comment_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_ok2*(::body_comment_ok2*)> >("construct ::body_comment_ok2", &::construct<::body_comment_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_ok2*)> >("destruct ::body_comment_ok2", &::destruct<::body_comment_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_ok3*(::body_comment_ok3*)> >("construct ::body_comment_ok3", &::construct<::body_comment_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_ok3*)> >("destruct ::body_comment_ok3", &::destruct<::body_comment_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_b1*(::body_comment_b1*)> >("construct ::body_comment_b1", &::construct<::body_comment_b1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_b1*)> >("destruct ::body_comment_b1", &::destruct<::body_comment_b1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_b3*(::body_comment_b3*)> >("construct ::body_comment_b3", &::construct<::body_comment_b3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_b3*)> >("destruct ::body_comment_b3", &::destruct<::body_comment_b3>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::body_comment_kind_ok4 >("::body_comment_kind_ok4",
    {
        { ::body_comment_kind_ok4::_A, "_A" },
        { ::body_comment_kind_ok4::_B, "_B" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_WorldTick, &::alias_system_a6);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_WorldTick, &::alias_system_a7);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok1*(::alias_after_ok1*)> >("construct ::alias_after_ok1", &::construct<::alias_after_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok1*)> >("destruct ::alias_after_ok1", &::destruct<::alias_after_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a1*(::alias_sentinel_a1*)> >("construct ::alias_sentinel_a1", &::construct<::alias_sentinel_a1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a1*)> >("destruct ::alias_sentinel_a1", &::destruct<::alias_sentinel_a1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok2*(::alias_after_ok2*)> >("construct ::alias_after_ok2", &::construct<::alias_after_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok2*)> >("destruct ::alias_after_ok2", &::destruct<::alias_after_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok3*(::alias_after_ok3*)> >("construct ::alias_after_ok3", &::construct<::alias_after_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok3*)> >("destruct ::alias_after_ok3", &::destruct<::alias_after_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_a1*(::alias_after_a1*)> >("construct ::alias_after_a1", &::construct<::alias_after_a1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_a1*)> >("destruct ::alias_after_a1", &::destruct<::alias_after_a1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok4*(::alias_after_ok4*)> >("construct ::alias_after_ok4", &::construct<::alias_after_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok4*)> >("destruct ::alias_after_ok4", &::destruct<::alias_after_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok5*(::alias_after_ok5*)> >("construct ::alias_after_ok5", &::construct<::alias_after_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok5*)> >("destruct ::alias_after_ok5", &::destruct<::alias_after_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a3*(::alias_sentinel_a3*)> >("construct ::alias_sentinel_a3", &::construct<::alias_sentinel_a3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a3*)> >("destruct ::alias_sentinel_a3", &::destruct<::alias_sentinel_a3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a4*(::alias_sentinel_a4*)> >("construct ::alias_sentinel_a4", &::construct<::alias_sentinel_a4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a4*)> >("destruct ::alias_sentinel_a4", &::destruct<::alias_sentinel_a4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a6*(::alias_sentinel_a6*)> >("construct ::alias_sentinel_a6", &::construct<::alias_sentinel_a6>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a6*)> >("destruct ::alias_sentinel_a6", &::destruct<::alias_sentinel_a6>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_ns_a4::alias_inner_a4*(::alias_ns_a4::alias_inner_a4*)> >("construct ::alias_ns_a4::alias_inner_a4", &::construct<::alias_ns_a4::alias_inner_a4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_ns_a4::alias_inner_a4*)> >("destruct ::alias_ns_a4::alias_inner_a4", &::destruct<::alias_ns_a4::alias_inner_a4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_ns_a5::alias_after_a5*(::alias_ns_a5::alias_after_a5*)> >("construct ::alias_ns_a5::alias_after_a5", &::construct<::alias_ns_a5::alias_after_a5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_ns_a5::alias_after_a5*)> >("destruct ::alias_ns_a5::alias_after_a5", &::destruct<::alias_ns_a5::alias_after_a5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_ns_a5::alias_sentinel_a5*(::alias_ns_a5::alias_sentinel_a5*)> >("construct ::alias_ns_a5::alias_sentinel_a5", &::construct<::alias_ns_a5::alias_sentinel_a5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_ns_a5::alias_sentinel_a5*)> >("destruct ::alias_ns_a5::alias_sentinel_a5", &::destruct<::alias_ns_a5::alias_sentinel_a5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_a2*(::alias_after_a2*)> >("construct ::alias_after_a2", &::construct<::alias_after_a2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_a2*)> >("destruct ::alias_after_a2", &::destruct<::alias_after_a2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a2*(::alias_sentinel_a2*)> >("construct ::alias_sentinel_a2", &::construct<::alias_sentinel_a2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a2*)> >("destruct ::alias_sentinel_a2", &::destruct<::alias_sentinel_a2>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::alias_after_kind_a3 >("::alias_after_kind_a3",
    {
        { ::alias_after_kind_a3::_A, "_A" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_WorldTick, &::marker_ns_m6::marker_system_m6);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ok1*(::marker_ok1*)> >("construct ::marker_ok1", &::construct<::marker_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ok1*)> >("destruct ::marker_ok1", &::destruct<::marker_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_lost_g1b*(::marker_lost_g1b*)> >("construct ::marker_lost_g1b", &::construct<::marker_lost_g1b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_lost_g1b*)> >("destruct ::marker_lost_g1b", &::destruct<::marker_lost_g1b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ok3*(::marker_ok3*)> >("construct ::marker_ok3", &::construct<::marker_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ok3*)> >("destruct ::marker_ok3", &::destruct<::marker_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ns_ok6::marker_ok6*(::marker_ns_ok6::marker_ok6*)> >("construct ::marker_ns_ok6::marker_ok6", &::construct<::marker_ns_ok6::marker_ok6>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ns_ok6::marker_ok6*)> >("destruct ::marker_ns_ok6::marker_ok6", &::destruct<::marker_ns_ok6::marker_ok6>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_before_g1*(::marker_before_g1*)> >("construct ::marker_before_g1", &::construct<::marker_before_g1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_before_g1*)> >("destruct ::marker_before_g1", &::destruct<::marker_before_g1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ok4*(::marker_ok4*)> >("construct ::marker_ok4", &::construct<::marker_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ok4*)> >("destruct ::marker_ok4", &::destruct<::marker_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ok7*(::marker_ok7*)> >("construct ::marker_ok7", &::construct<::marker_ok7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ok7*)> >("destruct ::marker_ok7", &::destruct<::marker_ok7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_lost_g1a*(::marker_lost_g1a*)> >("construct ::marker_lost_g1a", &::construct<::marker_lost_g1a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_lost_g1a*)> >("destruct ::marker_lost_g1a", &::destruct<::marker_lost_g1a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ok8*(::marker_ok8*)> >("construct ::marker_ok8", &::construct<::marker_ok8>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ok8*)> >("destruct ::marker_ok8", &::destruct<::marker_ok8>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ns_m5::marker_m5*(::marker_ns_m5::marker_m5*)> >("construct ::marker_ns_m5::marker_m5", &::construct<::marker_ns_m5::marker_m5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ns_m5::marker_m5*)> >("destruct ::marker_ns_m5::marker_m5", &::destruct<::marker_ns_m5::marker_m5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ns_m7::marker_m7*(::marker_ns_m7::marker_m7*)> >("construct ::marker_ns_m7::marker_m7", &::construct<::marker_ns_m7::marker_m7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ns_m7::marker_m7*)> >("destruct ::marker_ns_m7::marker_m7", &::destruct<::marker_ns_m7::marker_m7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::marker_ok2*(::marker_ok2*)> >("construct ::marker_ok2", &::construct<::marker_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::marker_ok2*)> >("destruct ::marker_ok2", &::destruct<::marker_ok2>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::marker_kind_ok5 >("::marker_kind_ok5",
    {
        { ::marker_kind_ok5::_A, "_A" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::marker_kind_m1 >("::marker_kind_m1",
    {
        { ::marker_kind_m1::_A, "_A" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::marker_kind_m2 >("::marker_kind_m2",
    {
        { ::marker_kind_m2::_A, "_A" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::marker_ns_m4::marker_kind_m4 >("::marker_ns_m4::marker_kind_m4",
    {
        { ::marker_ns_m4::marker_kind_m4::_A, "_A" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::marker_kind_m3 >("::marker_kind_m3",
    {
        { ::marker_kind_m3::_A, "_A" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_virtual_ok4*(::body_virtual_ok4*)> >("construct ::body_virtual_ok4", &::construct<::body_virtual_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_virtual_ok4*)> >("destruct ::body_virtual_ok4", &::destruct<::body_virtual_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_virtual_ok5*(::body_virtual_ok5*)> >("construct ::body_virtual_ok5", &::construct<::body_virtual_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_virtual_ok5*)> >("destruct ::body_virtual_ok5", &::destruct<::body_virtual_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::derived_ok6*(::derived_ok6*)> >("construct ::derived_ok6", &::construct<::derived_ok6>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::derived_ok6*)> >("destruct ::derived_ok6", &::destruct<::derived_ok6>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_member_c10*(::lit_member_c10*)> >("construct ::lit_member_c10", &::construct<::lit_member_c10>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_member_c10*)> >("destruct ::lit_member_c10", &::destruct<::lit_member_c10>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l1d*(::lit_sentinel_l1d*)> >("construct ::lit_sentinel_l1d", &::construct<::lit_sentinel_l1d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l1d*)> >("destruct ::lit_sentinel_l1d", &::destruct<::lit_sentinel_l1d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_member_c11*(::lit_member_c11*)> >("construct ::lit_member_c11", &::construct<::lit_member_c11>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_member_c11*)> >("destruct ::lit_member_c11", &::destruct<::lit_member_c11>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l4a*(::lit_sentinel_l4a*)> >("construct ::lit_sentinel_l4a", &::construct<::lit_sentinel_l4a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l4a*)> >("destruct ::lit_sentinel_l4a", &::destruct<::lit_sentinel_l4a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_c*(::lit_sentinel_c*)> >("construct ::lit_sentinel_c", &::construct<::lit_sentinel_c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_c*)> >("destruct ::lit_sentinel_c", &::destruct<::lit_sentinel_c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l1a*(::lit_sentinel_l1a*)> >("construct ::lit_sentinel_l1a", &::construct<::lit_sentinel_l1a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l1a*)> >("destruct ::lit_sentinel_l1a", &::destruct<::lit_sentinel_l1a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l1b*(::lit_sentinel_l1b*)> >("construct ::lit_sentinel_l1b", &::construct<::lit_sentinel_l1b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l1b*)> >("destruct ::lit_sentinel_l1b", &::destruct<::lit_sentinel_l1b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_hidden_l2*(::lit_hidden_l2*)> >("construct ::lit_hidden_l2", &::construct<::lit_hidden_l2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_hidden_l2*)> >("destruct ::lit_hidden_l2", &::destruct<::lit_hidden_l2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l3a*(::lit_sentinel_l3a*)> >("construct ::lit_sentinel_l3a", &::construct<::lit_sentinel_l3a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l3a*)> >("destruct ::lit_sentinel_l3a", &::destruct<::lit_sentinel_l3a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l1c*(::lit_sentinel_l1c*)> >("construct ::lit_sentinel_l1c", &::construct<::lit_sentinel_l1c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l1c*)> >("destruct ::lit_sentinel_l1c", &::destruct<::lit_sentinel_l1c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l2*(::lit_sentinel_l2*)> >("construct ::lit_sentinel_l2", &::construct<::lit_sentinel_l2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l2*)> >("destruct ::lit_sentinel_l2", &::destruct<::lit_sentinel_l2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l3b*(::lit_sentinel_l3b*)> >("construct ::lit_sentinel_l3b", &::construct<::lit_sentinel_l3b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l3b*)> >("destruct ::lit_sentinel_l3b", &::destruct<::lit_sentinel_l3b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l3c*(::lit_sentinel_l3c*)> >("construct ::lit_sentinel_l3c", &::construct<::lit_sentinel_l3c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l3c*)> >("destruct ::lit_sentinel_l3c", &::destruct<::lit_sentinel_l3c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l4b*(::lit_sentinel_l4b*)> >("construct ::lit_sentinel_l4b", &::construct<::lit_sentinel_l4b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l4b*)> >("destruct ::lit_sentinel_l4b", &::destruct<::lit_sentinel_l4b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l4c*(::lit_sentinel_l4c*)> >("construct ::lit_sentinel_l4c", &::construct<::lit_sentinel_l4c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l4c*)> >("destruct ::lit_sentinel_l4c", &::destruct<::lit_sentinel_l4c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l4d*(::lit_sentinel_l4d*)> >("construct ::lit_sentinel_l4d", &::construct<::lit_sentinel_l4d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l4d*)> >("destruct ::lit_sentinel_l4d", &::destruct<::lit_sentinel_l4d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_hidden_l5a*(::lit_hidden_l5a*)> >("construct ::lit_hidden_l5a", &::construct<::lit_hidden_l5a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_hidden_l5a*)> >("destruct ::lit_hidden_l5a", &::destruct<::lit_hidden_l5a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l6b*(::lit_sentinel_l6b*)> >("construct ::lit_sentinel_l6b", &::construct<::lit_sentinel_l6b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l6b*)> >("destruct ::lit_sentinel_l6b", &::destruct<::lit_sentinel_l6b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l5a*(::lit_sentinel_l5a*)> >("construct ::lit_sentinel_l5a", &::construct<::lit_sentinel_l5a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l5a*)> >("destruct ::lit_sentinel_l5a", &::destruct<::lit_sentinel_l5a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_hidden_l5b*(::lit_hidden_l5b*)> >("construct ::lit_hidden_l5b", &::construct<::lit_hidden_l5b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_hidden_l5b*)> >("destruct ::lit_hidden_l5b", &::destruct<::lit_hidden_l5b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l6a*(::lit_sentinel_l6a*)> >("construct ::lit_sentinel_l6a", &::construct<::lit_sentinel_l6a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l6a*)> >("destruct ::lit_sentinel_l6a", &::destruct<::lit_sentinel_l6a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l5b*(::lit_sentinel_l5b*)> >("construct ::lit_sentinel_l5b", &::construct<::lit_sentinel_l5b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l5b*)> >("destruct ::lit_sentinel_l5b", &::destruct<::lit_sentinel_l5b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l6c*(::lit_sentinel_l6c*)> >("construct ::lit_sentinel_l6c", &::construct<::lit_sentinel_l6c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l6c*)> >("destruct ::lit_sentinel_l6c", &::destruct<::lit_sentinel_l6c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_member_l7a*(::lit_member_l7a*)> >("construct ::lit_member_l7a", &::construct<::lit_member_l7a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_member_l7a*)> >("destruct ::lit_member_l7a", &::destruct<::lit_member_l7a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_member_l7b*(::lit_member_l7b*)> >("construct ::lit_member_l7b", &::construct<::lit_member_l7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_member_l7b*)> >("destruct ::lit_member_l7b", &::destruct<::lit_member_l7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_ns_l7::lit_member_l7c*(::lit_ns_l7::lit_member_l7c*)> >("construct ::lit_ns_l7::lit_member_l7c", &::construct<::lit_ns_l7::lit_member_l7c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_ns_l7::lit_member_l7c*)> >("destruct ::lit_ns_l7::lit_member_l7c", &::destruct<::lit_ns_l7::lit_member_l7c>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::kindZ1 >("::kindZ1",
    {
        { ::kindZ1::_A, "_A" },
        { ::kindZ1::_B, "_B" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::lit_hidden_kind_l2 >("::lit_hidden_kind_l2",
    {
        { ::lit_hidden_kind_l2::_A, "_A" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Nested::GlobalRenderableComponent*(::FE::Test::Nested::GlobalRenderableComponent*)> >("construct ::FE::Test::Nested::GlobalRenderableComponent", &::construct<::FE::Test::Nested::GlobalRenderableComponent>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Nested::GlobalRenderableComponent*)> >("destruct ::FE::Test::Nested::GlobalRenderableComponent", &::destruct<::FE::Test::Nested::GlobalRenderableComponent>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Nested::EnemyEntity*(::FE::Test::Nested::EnemyEntity*)> >("construct ::FE::Test::Nested::EnemyEntity", &::construct<::FE::Test::Nested::EnemyEntity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Nested::EnemyEntity*)> >("destruct ::FE::Test::Nested::EnemyEntity", &::destruct<::FE::Test::Nested::EnemyEntity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Nested::PlayerEntity*(::FE::Test::Nested::PlayerEntity*)> >("construct ::FE::Test::Nested::PlayerEntity", &::construct<::FE::Test::Nested::PlayerEntity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Nested::PlayerEntity*)> >("destruct ::FE::Test::Nested::PlayerEntity", &::destruct<::FE::Test::Nested::PlayerEntity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Nested::RenderSystem*(::FE::Test::Nested::RenderSystem*)> >("construct ::FE::Test::Nested::RenderSystem", &::construct<::FE::Test::Nested::RenderSystem>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Nested::RenderSystem*)> >("destruct ::FE::Test::Nested::RenderSystem", &::destruct<::FE::Test::Nested::RenderSystem>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::Color >("::FE::Test::Color",
    {
        { ::FE::Test::Color::Red, "Red" },
        { ::FE::Test::Color::Green, "Green" },
        { ::FE::Test::Color::Blue, "Blue" },
        { ::FE::Test::Color::Yellow, "Yellow" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::ByteStatus >("::FE::Test::ByteStatus",
    {
        { ::FE::Test::ByteStatus::Idle, "Idle" },
        { ::FE::Test::ByteStatus::Running, "Running" },
        { ::FE::Test::ByteStatus::Paused, "Paused" },
        { ::FE::Test::ByteStatus::Stopped, "Stopped" },
        { ::FE::Test::ByteStatus::Error, "Error" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::EmptyEnum >("::FE::Test::EmptyEnum",
    {
        { ::FE::Test::EmptyEnum::_, "_" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::LongCode >("::FE::Test::LongCode",
    {
        { ::FE::Test::LongCode::MinValue, "MinValue" },
        { ::FE::Test::LongCode::Zero, "Zero" },
        { ::FE::Test::LongCode::MaxValue, "MaxValue" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::SingleValue >("::FE::Test::SingleValue",
    {
        { ::FE::Test::SingleValue::Only, "Only" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_EngineInitialization, &::Game::boot_memory_arenas);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostWorldTick, &::Game::evict_far_cells);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDefaultEntityInitialization, &::Game::spawn_persistent_actors);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_GameInstanceInitialization, &::Game::load_player_profile);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldBegin, &::Game::seed_encounter_rng);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldInitialization, &::Game::build_navmesh);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_GameInstanceBegin, &::Game::play_intro_cinematic);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreGameInstanceTick, &::Game::poll_input_devices);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldTick, &::Game::tick_weather);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_WorldBegin, &::Game::fade_in_title_ui);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_GameInstanceTick, &::Game::advance_save_timer);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostGameInstanceTick, &::Game::flush_input_edge_flags);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreWorldTick, &::Game::stream_in_nearby_cells);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldTick, &::Game::tick_arena_hazards);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_EngineDeinitialization, &::Game::teardown_memory_arenas);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(EditorWorldTag::Viewport, FE::_WorldTick, &::Game::tick_editor_gizmos);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::refresh_transform_cache);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::rebuild_spatial_hash);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_PostPhysics, &::Game::write_back_transforms);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::SystemCallPhase::_EntityTick, &::Game::run_behaviour_trees);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostEntityTick, &::Game::apply_deferred_despawns);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreRenderQueueCommit, &::Game::cull_and_sort_draw_calls);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_PrePhysics, &::Game::sync_kinematic_bodies);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_StartPhysics, &::Game::begin_broadphase);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, g_physics_phase, &::Game::integrate_rigid_bodies);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_EndPhysics, &::Game::resolve_contact_events);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostUpdateWork, &::Game::collect_profiling_counters);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostRenderQueueCommit, &::Game::submit_debug_lines);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldEnd, &::Game::tally_battle_results);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_GameInstanceEnd, &::Game::write_session_log);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDefaultEntityDeinitialization, &::Game::destroy_persistent_actors);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDeinitialization, &::Game::release_navmesh);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_GameInstanceDeinitialization, &::Game::save_player_profile);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Game::CombatDirector*(::Game::CombatDirector*)> >("construct ::Game::CombatDirector", &::construct<::Game::CombatDirector>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Game::CombatDirector*)> >("destruct ::Game::CombatDirector", &::destruct<::Game::CombatDirector>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Game::LoadingScreenDriver*(::Game::LoadingScreenDriver*)> >("construct ::Game::LoadingScreenDriver", &::construct<::Game::LoadingScreenDriver>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Game::LoadingScreenDriver*)> >("destruct ::Game::LoadingScreenDriver", &::destruct<::Game::LoadingScreenDriver>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Game::EditorInspector*(::Game::EditorInspector*)> >("construct ::Game::EditorInspector", &::construct<::Game::EditorInspector>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Game::EditorInspector*)> >("destruct ::Game::EditorInspector", &::destruct<::Game::EditorInspector>);
}
