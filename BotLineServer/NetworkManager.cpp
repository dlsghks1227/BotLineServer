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

void NetworkManager::CheckForDisconnect() noexcept
{
    std::deque<BotLineObjectPtr>    disconnectedObjects;

    SystemTime  currentTime = std::chrono::system_clock::now();
    for (const auto& pair : mBotLineObjects)
    {
        std::chrono::duration<double>   elapsedTime = currentTime - pair.second->GetLastPacketTime();
        if (elapsedTime.count() > sTimeout) {
            disconnectedObjects.push_back(pair.second);
        }
    }

    for (auto itr : disconnectedObjects)
    {
        HandleObjectDisconnect(itr);
    }
}

void NetworkManager::VerifyConnection() noexcept
{
    if (mBotLineObjects.empty() == false)
    {
        for (const auto& pair : mBotLineObjects)
        {
            OutputMemoryBitStream output;
            output.Write(MessageType::CONNECT_CHECK);

            SendPacket(output, pair.first);

            std::cout << "send for connect check : " << pair.first.ToString() << '\n';
        }
    }
}

void NetworkManager::SendJetbotInfomation() noexcept
{
    //    1 bytes       |       ? bytes         |
    //  message type    |   jetbot information  |

    for (const auto& pair : mJetBotObjects)
    {
        OutputMemoryBitStream output;

        output.Write(MessageType::CONTROLLER_JETOBT_INFORMATION_REQUEST);
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
        const auto itr = mBotLineObjects.find(fromAddress);
        if (itr != mBotLineObjects.cend())
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
    const auto itr = mBotLineObjects.find(address);
    if (itr == mBotLineObjects.cend())
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

    if (type == MessageType::JETBOT_CONNECT || type == MessageType::CONTROLLER_CONNECT)
    {

    }
    else if (type == MessageType::JETBOT_INFORMATION_REQUEST)
    {

    }
    else if (type == MessageType::CONTROLLER_JETOBT_INFORMATION_REQUEST)
    {

    }
    else
    {
        std::cout << "Packet processing failed. Unknown message type" << '\n';
    }
}

void NetworkManager::HandlePacketFromNewObject(InputMemoryBitStream& input, const SocketAddress& address) noexcept
{
    // Read command
    MessageType type;
    input.Read(type);

    if (type == MessageType::JETBOT_CONNECT || type == MessageType::CONTROLLER_CONNECT)
    {
        // 연결 성공한 개체는 map에 저장

        if (type == MessageType::JETBOT_CONNECT)
        {
            JetbotObjectPtr     object = std::make_shared<JetbotObject>(address);
            mBotLineObjects[address] = object;
            mJetBotObjects[address] = object;
        }
        else if (type == MessageType::CONTROLLER_CONNECT)
        {
            ControllerObjectPtr object = std::make_shared<ControllerObject>(address);
            mBotLineObjects[address] = object;
            mControllerObjects[address] = object;
        }
        else
        {
            std::cout << "Failed to add new object. Unknown Object Type" << '\n';
            return;
        }

        // send connection success packet
        OutputMemoryBitStream   output;
        output.Write(type);

        SendPacket(output, address);

        std::cout << "Connected : " << address.ToString() << '\n';
    }
}

void NetworkManager::HandleObjectDisconnect(const BotLineObjectPtr& object) noexcept
{
    mBotLineObjects.erase(object->GetSocketAddress());

    if (object->GetObjectType() == ObjectType::JETBOT)
    {
        mJetBotObjects.erase(object->GetSocketAddress());
    }
    else if(object->GetObjectType() == ObjectType::CONTROLLER)
    {
        mControllerObjects.erase(object->GetSocketAddress());
    }
}