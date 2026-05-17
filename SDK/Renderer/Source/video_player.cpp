/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/video_player.hpp>
#include <mfapi.h>
#include <mfmediaengine.h>
#include <combaseapi.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "ole32.lib")




// ---------------------------------------------------------------------------
// IMFMediaEngineNotify callback — receives events from MF's internal threads
// ---------------------------------------------------------------------------
class FE::video_player::engine_notify final : public IMFMediaEngineNotify
{
    std::atomic_bool   m_can_play;
    std::atomic_bool   m_has_ended;
    std::atomic_bool   m_has_error;
    std::atomic<ULONG> m_ref;

public:
    engine_notify()
        :   m_can_play(false), 
            m_has_ended(false), 
            m_has_error(false), 
            m_ref(1) 
    {}

    ULONG STDMETHODCALLTYPE AddRef()  override { return ++m_ref; }
    ULONG STDMETHODCALLTYPE Release() override
    {
        ULONG l_c = --m_ref;
        if (l_c == 0) 
        {
            delete this; 
        }
        return l_c;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid_p, void** ppv_p) override
    {
        if (ppv_p == nullptr) 
        {
            return E_POINTER; 
        }

        if ((riid_p == __uuidof(IUnknown)) || (riid_p == __uuidof(IMFMediaEngineNotify)))
        {
            *ppv_p = static_cast<IMFMediaEngineNotify*>(this);
            AddRef();
            return S_OK;
        }
        *ppv_p = nullptr;
        return E_NOINTERFACE;
    }

    HRESULT STDMETHODCALLTYPE EventNotify(DWORD event_p, DWORD_PTR, DWORD) override
    {
        switch (event_p)
        {
        case MF_MEDIA_ENGINE_EVENT_CANPLAY:
            m_can_play.store(true, std::memory_order_release);
            break;
        case MF_MEDIA_ENGINE_EVENT_ENDED:
            m_has_ended.store(true, std::memory_order_release);
            break;
        case MF_MEDIA_ENGINE_EVENT_ERROR:
            m_has_error.store(true, std::memory_order_release);
            break;
        }
        return S_OK;
    }

    _FE_FORCE_INLINE_ bool CanPlay()  const noexcept { return m_can_play.load(std::memory_order_acquire); }
    _FE_FORCE_INLINE_ bool HasEnded() const noexcept { return m_has_ended.load(std::memory_order_acquire); }
    _FE_FORCE_INLINE_ bool HasError() const noexcept { return m_has_error.load(std::memory_order_acquire); }

    void ResetAll() noexcept
    {
        m_can_play.store(false, std::memory_order_release);
        m_has_ended.store(false, std::memory_order_release);
        m_has_error.store(false, std::memory_order_release);
    }
};




FE::video_player::video_player(const HWND target_window_p) noexcept
    :   m_engine(), 
        m_notify(), 
        m_engine_ex(),
        m_hwnd(target_window_p),
        m_mtx(), 
        m_is_COM_initialized(false)
{
    FE_ASSERT(target_window_p != nullptr);
    m_is_COM_initialized = SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)); // Initialize COM on this thread.
	FE_ASSERT(m_is_COM_initialized == true);

    _FE_MAYBE_UNUSED_ bool l_was_successful = (MFStartup(MF_VERSION) == S_OK); // Initialize Media Foundation.
    FE_ASSERT(l_was_successful == true);

    m_notify.Attach(new engine_notify()); // make com_ptr<engine_notify>()

    // Windowed rendering mode: MF creates and owns its own swap chain on
    // the supplied HWND. No DXGI manager, no shared texture, no manual
    // TransferVideoFrame — MF renders straight to the window.
    Microsoft::WRL::ComPtr<IMFAttributes> l_attributes;
    l_was_successful = (MFCreateAttributes(&l_attributes, 2) == S_OK); // create two slots.
    FE_ASSERT(l_was_successful == true);

    l_was_successful = (l_attributes->SetUINT64(MF_MEDIA_ENGINE_PLAYBACK_HWND, (UINT64)target_window_p) == S_OK);
    FE_ASSERT(l_was_successful == true);

    l_was_successful = (l_attributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, m_notify.Get()) == S_OK); // Map MF_MEDIA_ENGINE_CALLBACK with m_notify.Get()
    FE_ASSERT(l_was_successful == true);

    Microsoft::WRL::ComPtr<IMFMediaEngineClassFactory> l_factory; // Create the factory for instance creation from DLLs.
    l_was_successful = (CoCreateInstance(CLSID_MFMediaEngineClassFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&l_factory)) == S_OK);
    FE_ASSERT(l_was_successful == true);

	l_was_successful = (l_factory->CreateInstance(0, l_attributes.Get(), &m_engine) == S_OK); // Create the Media Engine instance.
    FE_ASSERT(l_was_successful == true);

	l_was_successful = SUCCEEDED(m_engine.As(&m_engine_ex)); // Convert to IMFMediaEngineEx.
    FE_ASSERT(l_was_successful == true);
}

FE::video_player::~video_player() noexcept
{
    // Tears down MF's internal swap chain on the target HWND so the caller can re-attach its own D3D swap chain afterwards.
	FE_ASSERT(m_engine != nullptr);
    m_engine->Shutdown();
    m_engine.Reset();
    m_notify.Reset();

	MFShutdown(); // literally, shuts it down.

	CoUninitialize(); // Uninitialize COM on this thread.
}


FE::boolean FE::video_player::__has_error() const noexcept
{
    return m_notify->HasError();
}

FE::boolean FE::video_player::__is_ready() const noexcept
{
    return (__has_error() == false) && m_notify->CanPlay();
}


FE::boolean FE::video_player::__open(FE::ASCII* file_path_p) noexcept
{
    if (file_path_p == nullptr) 
    {
        return false;
    }

    std::lock_guard<std::mutex> l_guard(m_mtx);

    m_notify->ResetAll();

    var::wchar l_wide[_ALLOWED_DIRECTORY_LENGTH_] = L"\0";
    _FE_MAYBE_UNUSED_ FE::int32 l_length = MultiByteToWideChar(CP_UTF8, NULL, file_path_p, (int)strlen(file_path_p)+1, l_wide, _ALLOWED_DIRECTORY_LENGTH_);
    FE_ASSERT(l_length > 0);
  
    BSTR l_url = SysAllocString(l_wide);
    if (l_url == nullptr) 
    {
        return false; 
    }

    HRESULT l_hr = m_engine->SetSource(l_url);
    SysFreeString(l_url);
    if (FAILED(l_hr)) 
    {
        return false; 
    }
    return SUCCEEDED(m_engine->Play());
}


void FE::video_player::play(FE::ASCII* file_path_p) noexcept
{
    FE_EXIT_IF(__open(file_path_p) == false,
        FE::ErrorCode::_FatalWinAPI_MF_Error_4XX_VideoFileOpenFailure,
        "Frogman Engine Video Player Error: Failed to open video file.");

    while ((__is_ready() == false) && (__has_error() == false))
    {
        _mm_pause();
    }
    
    if (__has_error() == true) 
    {
        return; 
    }

    for (;;)
    {
        if (m_notify->HasError() == true)
        {
            break; 
        }
        if (m_notify->HasEnded() == true) 
        {
            break; 
        }
        if (m_engine->IsEnded() == TRUE) 
        {
            break; 
        }

        m_engine_ex->UpdateVideoStream(nullptr, nullptr, nullptr);
        _mm_pause();
    }
}