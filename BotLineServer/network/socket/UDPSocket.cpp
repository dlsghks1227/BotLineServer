#include "../../framework.h"
#include "UDPSocket.h"

UDPSocket::UDPSocket() noexcept :
	mSocket()
{
}

UDPSocket::~UDPSocket()
{
	this->OnDestory();
}

void UDPSocket::Initialize() noexcept(false)
{
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET) {
		Utility::Throw(WSAGetLastError());
	}
}

void UDPSocket::OnDestory() noexcept
{
	closesocket(mSocket);
}

void UDPSocket::Bind(const SocketAddress& inBindAddress) noexcept(false)
{
	int result = bind(this->mSocket, &inBindAddress.GetSockAddr(), static_cast<int>(inBindAddress.GetSockAddrSize()));
	if (result == SOCKET_ERROR) {
		Utility::Throw(WSAGetLastError());
	}
}

void UDPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking) noexcept(false)
{
	u_long arg = inShouldBeNonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);

	if (result == SOCKET_ERROR) {
		Utility::Throw(WSAGetLastError());
	}
}

int UDPSocket::SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress) noexcept
{
	int byteSize = sendto(this->mSocket, static_cast<const char*>(inToSend), inLength, 0, &inToAddress.GetSockAddr(), static_cast<int>(inToAddress.GetSockAddrSize()));

	if (byteSize >= 0) {
		return byteSize;
	} 
	else {
		std::cout << "sendto() failed with error : " << WSAGetLastError() << '\n';
		return WSAGetLastError();
	}
}

int UDPSocket::ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress) noexcept
{
	socklen_t fromLength = static_cast<socklen_t>(outFromAddress.GetSockAddrSize());
	int byteSize = recvfrom(this->mSocket, static_cast<char*>(inToReceive), inMaxLength, 0, &outFromAddress.GetSockAddr(), &fromLength);

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