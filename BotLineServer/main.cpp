#include "stdafx.h"

namespace
{
	std::unique_ptr<BotLine>	gBotLine;
}


int main()
{
	gBotLine = std::make_unique<BotLine>();

	// ----- Timer Test -----
	try {
		gBotLine->Initialize();

		while (true)
		{
			gBotLine->Tick();
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << '\n';
		gBotLine.reset();
		return -1;
	}
	// ----------------------

	//g_networkManager = std::make_unique<NetworkManager>();
	//if (g_networkManager->Init(8000) == true) {
	//	while (false) {
	//		g_networkManager->ProcessIncomingPackets();
	//	}
	//}

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

	gBotLine.reset();

	return 0;
}