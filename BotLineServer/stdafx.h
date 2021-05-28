// 미리 컴파일된 헤더
#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <WS2tcpip.h>

#include <vector>
#include <list>
#include <chrono>
#include <thread>
#include <ctime>
#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <queue>
#include <algorithm>


// 쓰레드 세이프
// https://docs.microsoft.com/ko-kr/cpp/parallel/concrt/reference/concurrency-namespace?view=msvc-160
// https://docs.microsoft.com/ko-kr/cpp/parallel/concrt/reference/concurrent-queue-class?view=msvc-160
#include <concurrent_queue.h>
#include <concurrent_vector.h>

#include "NetworkException.h"
#include "Timer.h"


#include "SocketAddress.h"
#include "MemoryBitStream.h"
#include "UDPSocket.h"

#include "NetworkManager.h"

#include "BotLine.h"