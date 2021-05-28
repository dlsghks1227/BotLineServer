#pragma once


class UDPSocket
{
public:
	UDPSocket() noexcept;
	~UDPSocket();

	UDPSocket(UDPSocket&&) = default;
	UDPSocket& operator = (UDPSocket&&) = default;

	// 복사 연산자 delete
	UDPSocket(const UDPSocket&) = delete;
	UDPSocket& operator = (const UDPSocket&) = delete;

	void	Initialize()	noexcept(false);
	void	OnDestory()		noexcept;

	void	Bind(const SocketAddress& inBindAddress)			noexcept(false);
	void	SetNonBlockingMode(bool inShouldBeNonBlocking)		noexcept(false);

	int		SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress)	noexcept;
	int		ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress)	noexcept;

private:
	SOCKET				mSocket;
	SocketAddress		mSocketAddress;
};