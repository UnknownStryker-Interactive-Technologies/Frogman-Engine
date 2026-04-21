#ifndef _FE_RENDERER_D3D11_VIDEO_PLAYER_HPP_
#define _FE_RENDERER_D3D11_VIDEO_PLAYER_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <mfmediaengine.h>
#include <wrl/client.h>
#include <atomic>
#include <mutex>




BEGIN_NAMESPACE(FE)


// Windowed-rendering-mode wrapper around IMFMediaEngine. MF owns its own
// swap chain on the supplied HWND for the lifetime of this object; do NOT
// have any other D3D swap chain bound to the same HWND while the player is
// alive.
class video_player final
{
    class engine_notify;
    friend class engine_notify;

    Microsoft::WRL::ComPtr<IMFMediaEngine> m_engine;
    Microsoft::WRL::ComPtr<engine_notify> m_notify;
    Microsoft::WRL::ComPtr<IMFMediaEngineEx> m_engine_ex;
    HWND m_hwnd;

    std::mutex m_mtx;
    var::boolean m_is_COM_initialized;

public:
    video_player(const HWND target_window_p) noexcept;
    ~video_player() noexcept;

    // Blocks the calling thread until the clip finishes (ENDED) or fails.
    void play(FE::ASCII* file_path_p) noexcept;

private:
    FE::boolean __open(FE::ASCII* file_path_p) noexcept;
    FE::boolean __has_error() const noexcept;
    FE::boolean __is_ready() const noexcept;

public:
    video_player(const video_player&) = delete;
    video_player& operator=(const video_player&) = delete;
};


END_NAMESPACE
#endif