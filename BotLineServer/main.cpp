#include "stdafx.h"

namespace
{
	std::unique_ptr<NetworkManager>		g_networkManager;
}


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

	// ----- Timer Test -----
	try {
		Utility::Timer timer = Utility::Timer();
		timer.SetFixedTimeStep(true);

		float a[10] = { 0 };
		while(true) {
			timer.Tick([timer, &a]() 
				{
					for (int i = 0; i < 10; i++) {
						a[i] += static_cast<float>(timer.GetElapsedSeconds());
					}

					for (int i = 0; i < 10; i++) {
						std::cout << std::round(a[i]) << '\t';
					}
					std::cout << '\n';
				});
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << '\n';
	}
	// ----------------------

	g_networkManager = std::make_unique<NetworkManager>();
	if (g_networkManager->Init(8000) == true) {
		while (false) {
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