#include "stdafx.h"

namespace
{
	std::unique_ptr<NetworkManager>		g_networkManager;
}

enum class COMMAND : uint8_t {
	JETBOT_CONNECT,
	JETBOT_DISCONNECT,

	CONTROL_CONNECT,
	CONTROL_DISCONNECT,
};

#pragma pack(push, 1)
struct Packet {
	COMMAND		mCommand;
};
#pragma pack(pop)

int main()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int result = WSAStartup(wVersionRequested, &wsaData);
	if (result != 0) {
		std::cout << "WSAStartup() failed with error : " << result << '\n';
		return -1;
	}

	g_networkManager = std::make_unique<NetworkManager>();
	if (g_networkManager->Init(8000) == true) {
		while (true) {
			g_networkManager->ProcessIncomingPackets();
		}
	}

	//while (true) {
	//	SocketAddress clientAddress{};
	//	char buffer[BUFFER_SIZE] = { 0 };
	//	int byteSize = g_udpSocket->ReceiveFrom(buffer, BUFFER_SIZE, clientAddress);
	//	if (byteSize > 0) {
	//		{
	//		}
	//		std::cout << '[' << clientAddress.ToString().c_str() << "] " << buffer << '\n';
	//	}
	//}

	//g_networkManager.reset();
	WSACleanup();

	return 0;
}