#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define NOMINMAX
// Windows 헤더 파일
#include <windows.h>
#include <wrl.h>

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <WS2tcpip.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>
#include <cstdio>
#include <exception>
#include <utility>

#include <chrono>
#include <ctime>
#include <functional>

// string
#include <iostream>
#include <sstream>
#include <string>

// STL
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <algorithm>

#include <intrin.h>

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

#include <d3d11_1.h>
#include <dxgi1_6.h>

#include <DirectXMath.h>
#include <DirectXColors.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include "imgui/implot.h"

using	SystemTime = std::chrono::time_point<std::chrono::system_clock>;

#include "network/utility/NetworkException.h"
#include "network/utility/MemoryBitStream.h"
#include "Timer.h"

#include "network/socket/Packet.h"
#include "network/socket/SocketAddress.h"
#include "network/socket/UDPSocket.h"

#include "network/object/BotLineObject.h"
#include "network/object/JetbotObject.h"
#include "network/object/ControllerObject.h"


#include "Log.h"
#include "ObjectList.h"
#include "DialogManager.h"

#include "network/socket/NetworkManager.h"


// 콘솔창 출력
//#ifndef Assert
//#if defined( DEBUG ) || defined( _DEBUG )
//#include <dxgidebug.h>
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#define Assert(b) do {if (!(b)) { OutputDebugStringA("Assert: " #b "\n");}} while(0)
//#else
//#define Assert(b)
//#endif // DEBUG || _DEBUG
//#endif

// 모듈 기본 주소
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

namespace DX
{
	// https://github.com/Microsoft/DirectXTK/wiki/ThrowIfFailed
	// Helper class for COM exceptions

	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}