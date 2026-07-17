#ifndef _FE_RENDERER_CONSTANT_BUFFER_HXX_
#define _FE_RENDERER_CONSTANT_BUFFER_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <d3d11.h>
#include <wrl/client.h>




BEGIN_NAMESPACE(FE)


struct constant_buffer
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> _buffer;
	D3D11_BUFFER_DESC _desc;
};


END_NAMESPACE
#endif