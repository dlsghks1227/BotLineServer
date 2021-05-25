#pragma once


class UDPSocket
{
public:
	UDPSocket() noexcept;
	~UDPSocket();

	UDPSocket(UDPSocket&&) = default;
	UDPSocket& operator = (UDPSocket&&) = default;

	UDPSocket(const UDPSocket&) = delete;
	UDPSocket& operator = (const UDPSocket&) = delete;

	int		Create();
	void	Clear();

	int		Bind(const SocketAddress& inBindAddress);
	int		SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress);
	int		ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress);
	int		SetNonBlockingMode(bool inShouldBeNonBlocking);

private:
	SOCKET				udpSocket;
	SocketAddress		socketAddress;
};