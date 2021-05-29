#include "stdafx.h"
#include "NetworkManager.h"

constexpr int PORT = 8000;
constexpr int BUFFER_SIZE = 2048;
constexpr int MAX_PACKETS_PER_FRAME_COUNT = 10;

void NetworkManager::Initialize(uint16_t inPort) noexcept(false)
{
    mSocket = std::make_unique<UDPSocket>();

    mSocket->Initialize();

    mSocket->Bind(SocketAddress(INADDR_ANY, inPort));
    mSocket->SetNonBlockingMode(true);
}

void NetworkManager::ProcessIncomingPackets(const Utility::Timer& timer) noexcept
{
    ReadIncomingPacketsIntoQueue();

    ProcessQueuedPackets();
}

void NetworkManager::SendForConnectCheck() noexcept
{
    if (mBotLineObject.empty() == false)
    {
        for (const auto& pair : mBotLineObject)
        {
            OutputMemoryBitStream output;
            output.Write(MessageType::CONNECT_CHECK);

            SendPacket(output, pair.first);

            std::cout << "send for connect check : " << pair.first.ToString() << '\n';
        }
    }
}

void NetworkManager::CheckForDisconnect() noexcept
{
    std::deque<BotLineObjectPtr>    disconnectedObjects;

    SystemTime  currentTime = std::chrono::system_clock::now();
    for (const auto& pair : mBotLineObject)
    {
        std::chrono::duration<double>   elapsedTime = currentTime - pair.second->GetLastPacketTime();
        if (elapsedTime.count() > sTimeout) {
            disconnectedObjects.push_back(pair.second);
        }
    }

    if (disconnectedObjects.empty() == false)
    {
        for (auto itr : disconnectedObjects)
        {
            HandleObjectDisconnect(itr);
        }
    }
}

void NetworkManager::SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress) noexcept
{
    int sentByteCount = mSocket->SendTo(inOutputStream.GetBufferPtr(), inOutputStream.GetByteLength(), inFromAddress);
}

void NetworkManager::ReadIncomingPacketsIntoQueue() noexcept
{
    char buffer[BUFFER_SIZE] = {};
    int bufferSize = sizeof(buffer);
    InputMemoryBitStream inputStream(buffer, bufferSize * 8);
    SocketAddress fromAddress;

    int readByteCount = mSocket->ReceiveFrom(buffer, bufferSize, fromAddress);
    if (readByteCount == 0)
    {
        return;
    }
    else if (readByteCount == -WSAECONNRESET)
    {
        const auto itr = mBotLineObject.find(fromAddress);
        if (itr != mBotLineObject.cend())
        {
            HandleObjectDisconnect(itr->second);
        }
        return;
    }
    else if (readByteCount > 0)
    {
        inputStream.ResetToCapacity(readByteCount);
        mPacketQueue.emplace(std::chrono::system_clock::now(), inputStream, fromAddress);
    }
}

void NetworkManager::ProcessQueuedPackets() noexcept
{
    while (mPacketQueue.empty() == false)
    {
        ReceivedPacket& nextPacket = mPacketQueue.front();

        // Process Packet...
        SystemTime              receivedTime    = nextPacket.GetReceivedTime();
        InputMemoryBitStream    inputStream     = nextPacket.GetPacketBuffer();

        PacketProcessing(nextPacket.GetPacketBuffer(), nextPacket.GetFromAddress());

        mPacketQueue.pop();
    }
}

void NetworkManager::PacketProcessing(InputMemoryBitStream& input, const SocketAddress& address) noexcept
{
    const auto itr = mBotLineObject.find(address);
    if (itr == mBotLineObject.cend())
    {
        // 새로 연결된 오브젝트 처리
        HandlePacketFromNewObject(input, address);
    }
    else
    {
        // 기존에 연결된 오브젝트 처리
        PacketProcessingFromObject(input, (*itr).second);
    }
}

void NetworkManager::PacketProcessingFromObject(InputMemoryBitStream& input, const BotLineObjectPtr& object) noexcept
{
    // Last received time update
    object->UpdateLastPacketTime();

    // Read command
    MessageType type;
    input.Read(type);

    if (type == MessageType::JETBOT_CONNECT || type == MessageType::CONTROL_CONNECT)
    {

    }
    else
    {

    }
}

void NetworkManager::HandlePacketFromNewObject(InputMemoryBitStream& input, const SocketAddress& address) noexcept
{
    // Read command
    MessageType type;
    input.Read(type);

    if (type == MessageType::JETBOT_CONNECT || type == MessageType::CONTROL_CONNECT)
    {
        // 연결 성공한 개체는 map에 저장
        BotLineObjectPtr object = std::make_shared<BotLineObject>(address);
        mBotLineObject[address] = object;

        // send connection success packet
        OutputMemoryBitStream   output;
        output.Write(MessageType::JETBOT_CONNECT);

        SendPacket(output, address);

        std::cout << "Connected : " << address.ToString() << '\n';
    }
}

void NetworkManager::HandleObjectDisconnect(const BotLineObjectPtr& object) noexcept
{
    mBotLineObject.erase(object->GetSocketAddress());
}