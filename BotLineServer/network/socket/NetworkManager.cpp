#include "../../framework.h"
#include "NetworkManager.h"

constexpr int PORT = 8000;
constexpr int BUFFER_SIZE = 2048;
constexpr int MAX_PACKETS_PER_FRAME_COUNT = 10;

void NetworkManager::Initialize(uint16_t inPort) noexcept(false)
{
    mSocket = std::make_shared<UDPSocket>();

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
            if (pair.second->GetObjectType() == ObjectType::JETBOT)
            {
                output.Write(MessageType::INFORMATION_REQUEST);
            }
            else
            {
                output.Write(MessageType::CONNECT_CHECK);
            }
            SendPacket(output, pair.first);
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

        output.Write(MessageType::INFORMATION_REQUEST);
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
    // Read object type
    ObjectType objectType;
    input.Read(objectType);
    std::stringstream log;

    bool wasNewObject = false;
    if (objectType == ObjectType::JETBOT)
    {
        const auto itr = mJetBotObjects.find(address);
        if (itr != mJetBotObjects.cend())
        {
            // 기존에 연결된 오브젝트 처리
            PacketProcessingFromJetbotObject(input, (*itr).second);
        }
        else
        {
            wasNewObject = true;
        }
    }
    else if (objectType == ObjectType::CONTROLLER)
    {
        const auto itr = mControllerObjects.find(address);
        if (itr != mControllerObjects.cend())
        {
            // 기존에 연결된 오브젝트 처리
            PacketProcessingFromControllerObject(input, (*itr).second);
        }
        else
        {
            wasNewObject = true;
        }
    }
    else
    {
        mLog->Add("Failed to add new object. Unknown Object Type\n");
        return;
    }

    if (wasNewObject)
    {
        // 새로 연결된 오브젝트 처리
        HandlePacketFromNewObject(objectType, input, address);
    }
}

void NetworkManager::PacketProcessingFromJetbotObject(InputMemoryBitStream& input, const JetbotObjectPtr& object) noexcept
{
    // Last received time update
    object->UpdateLastPacketTime();

    // Read command
    MessageType messageType;
    input.Read(messageType);

    if (messageType == MessageType::CONNECT)
    {
        // send connection success packet
        OutputMemoryBitStream   output;
        output.Write(messageType);
        output.Write(static_cast<uint32_t>(1234));

        SendPacket(output, object->GetSocketAddress());
    }
    else if (messageType == MessageType::INFORMATION_REQUEST)
    {
        float voltage = 0.0f;
        float cpuAverage = 0.0f;
        float memory = 0.0f;
        float disk = 0.0f;
        uint16_t left = 0U;
        uint16_t right = 0U;
        uint16_t speed = 0U;

        input.Read(voltage);
        input.Read(cpuAverage);
        input.Read(memory);
        input.Read(disk);

        input.Read(left);
        input.Read(right);
        input.Read(speed);

        object->SetVoltage(voltage);
        object->SetCpuAverage(cpuAverage);
        object->SetMemory(memory);
        object->SetDisk(disk);

        object->SetLastMessageType(messageType);
    }
    else if (messageType == MessageType::CONTROL)
    {
    }
    else if (messageType == MessageType::CONNECT_CHECK)
    {
    }
    else
    {
        mLog->Add("Packet processing failed. Unknown message type\n");
    }
}

void NetworkManager::PacketProcessingFromControllerObject(InputMemoryBitStream& input, const ControllerObjectPtr& object) noexcept
{
    // Last received time update
    object->UpdateLastPacketTime();

    // Read command
    MessageType messageType;
    input.Read(messageType);

    if (messageType == MessageType::CONNECT)
    {
        // send connection success packet
        OutputMemoryBitStream   output;
        output.Write(messageType);

        SendPacket(output, object->GetSocketAddress());
        mLog->Add("Connection\n");
    }
    else
    {
        mLog->Add("Packet processing failed. Unknown message type\n");
    }
}

void NetworkManager::HandlePacketFromNewObject(const ObjectType& type, InputMemoryBitStream& input, const SocketAddress& address) noexcept
{
    // Read command
    MessageType messageType;
    input.Read(messageType);

    if (messageType == MessageType::CONNECT)
    {
        // 연결 성공한 개체는 map에 저장

        if (type == ObjectType::JETBOT)
        {
            JetbotObjectPtr object = std::make_shared<JetbotObject>(address);
            mBotLineObjects[address] = object;
            mJetBotObjects[address] = object;
        }
        else if (type == ObjectType::CONTROLLER)
        {
            ControllerObjectPtr object = std::make_shared<ControllerObject>(address);
            mBotLineObjects[address] = object;
            mControllerObjects[address] = object;
        }

        // send connection success packet
        OutputMemoryBitStream   output;
        output.Write(messageType);

        SendPacket(output, address);

        std::stringstream ss{};
        ss << "Connected: " << address.ToString() << '\n';

        mLog->Add(ss.str());
    }
}

void NetworkManager::HandleObjectDisconnect(const BotLineObjectPtr& object) noexcept
{
    std::stringstream ss{};
    ss << "Disconnected: " << object->GetSocketAddress().ToString() << '\n';
    mLog->Add(ss.str());

    // send disconnection packet
    OutputMemoryBitStream   output;
    output.Write(MessageType::DISCONNECT);

    SendPacket(output, object->GetSocketAddress());

    if (object->GetObjectType() == ObjectType::JETBOT)
    {
        mJetBotObjects.erase(object->GetSocketAddress());
    }
    else if(object->GetObjectType() == ObjectType::CONTROLLER)
    {
        mControllerObjects.erase(object->GetSocketAddress());
    }
    mBotLineObjects.erase(object->GetSocketAddress());
}