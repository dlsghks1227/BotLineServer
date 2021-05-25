#include "stdafx.h"
#include "UDPSocket.h"

UDPSocket::UDPSocket() noexcept :
	udpSocket()
{
}

UDPSocket::~UDPSocket()
{
	this->Clear();
}

int UDPSocket::Create()
{
	udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpSocket == INVALID_SOCKET) {
		std::cout << "socket() failed with error : " << WSAGetLastError() << '\n';
		return WSAGetLastError();
	}

	return NO_ERROR;
}

int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
	int result = bind(this->udpSocket, &inBindAddress.GetSockAddr(), inBindAddress.GetSockAddrSize());
	if (result == SOCKET_ERROR) {
		std::cout << "bind() failed with error : " << WSAGetLastError() << '\n';
		return WSAGetLastError();
	}
	return NO_ERROR;
}

int UDPSocket::SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress)
{
	int byteSize = sendto(this->udpSocket, static_cast<const char*>(inToSend), inLength, 0, &inToAddress.GetSockAddr(), inToAddress.GetSockAddrSize());

	if (byteSize >= 0) {
		return byteSize;
	} 
	else {
		std::cout << "sendto() failed with error : " << WSAGetLastError() << '\n';
		return WSAGetLastError();
	}
}

int UDPSocket::ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress)
{
	socklen_t fromLength = outFromAddress.GetSockAddrSize();
	int byteSize = recvfrom(this->udpSocket, static_cast<char*>(inToReceive), inMaxLength, 0, &outFromAddress.GetSockAddr(), &fromLength);

	if (byteSize >= 0) {
		return byteSize;
	}
	else {
		int error = WSAGetLastError();

		if (error == WSAEWOULDBLOCK) {
			return 0;
		}
		else if (error == WSAECONNRESET) {
			std::cout << "Connection reset from " << outFromAddress.ToString().c_str() << '\n';
			return -WSAECONNRESET;
		}
		else {
			std::cout << "recvfrom() failed with error : " << WSAGetLastError() << '\n';
			return -error;
		}
	}
}

int UDPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking)
{
	u_long arg = inShouldBeNonBlocking ? 1 : 0;
	int result = ioctlsocket(udpSocket, FIONBIO, &arg);

	if (result == SOCKET_ERROR) {
		std::cout << "ioctlsocket() failed with error : " << WSAGetLastError() << '\n';
		return WSAGetLastError();
	}
	return NO_ERROR;
}

void UDPSocket::Clear()
{
	closesocket(udpSocket);
}