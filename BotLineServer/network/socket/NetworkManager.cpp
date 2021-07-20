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
            if (pair.second->GetObjectType() == ObjectType::JETBOT || pair.second->GetObjectType() == ObjectType::XAVIER)
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
    else if (objectType == ObjectType::XAVIER)
    {
        const auto itr = mXavierObjects.find(address);
        if (itr != mXavierObjects.cend())
        {
            // 기존에 연결된 오브젝트 처리
            PacketProcessingFromXavierObjec(input, (*itr).second);
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
        output.Write(object->GetSocketAddress().GetHash());

        SendPacket(output, object->GetSocketAddress());

        std::stringstream ss{};
        ss << "Connected: " << object->GetSocketAddress().ToString() << " - " << object->GetSocketAddress().GetHash() << '\n';

        mLog->Add(ss.str());
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

        JetbotMoveState moveState = JetbotMoveState::GO;

        input.Read(voltage);
        input.Read(cpuAverage);
        input.Read(memory);
        input.Read(disk);

        input.Read(left);
        input.Read(right);
        input.Read(speed);

        input.Read(moveState);

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
        output.Write(object->GetSocketAddress().GetHash());

        SendPacket(output, object->GetSocketAddress());
        mLog->Add("Connection\n");
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

void NetworkManager::PacketProcessingFromXavierObjec(InputMemoryBitStream& input, const XavierObjectPtr& object) noexcept
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
        output.Write(object->GetSocketAddress().GetHash());

        SendPacket(output, object->GetSocketAddress());

        std::stringstream ss{};
        ss << "Connected: " << object->GetSocketAddress().ToString() << " - " << object->GetSocketAddress().GetHash() << '\n';

        mLog->Add(ss.str());
    }
    else if (messageType == MessageType::INFORMATION_REQUEST)
    {
        float voltage = 0.0f;
        float cpuAverage = 0.0f;
        float memory = 0.0f;
        float disk = 0.0f;

        input.Read(voltage);
        input.Read(cpuAverage);
        input.Read(memory);
        input.Read(disk);

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
    else if (messageType == MessageType::ALL_STOP)
    {
        uint16_t isAllStop = 0;

        input.Read(isAllStop);


        std::stringstream ss = {};

        ss << static_cast<int>(isAllStop) << '\n';

        mLog->Add(ss.str());

        for (const auto& pair : mJetBotObjects)
        {
            OutputMemoryBitStream output;
            output.Write(MessageType::ALL_STOP);
            output.Write(isAllStop);

            SendPacket(output, pair.first);
        }
        if (isAllStop == 1) {
            mLog->Add("All Stop!\n");
        }
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
        std::stringstream ss{};
        if (type == ObjectType::JETBOT)
        {
            JetbotObjectPtr object = std::make_shared<JetbotObject>(address);
            mBotLineObjects[address] = object;
            mJetBotObjects[address] = object;
            ss << "Jetbot";
        }
        else if (type == ObjectType::CONTROLLER)
        {
            ControllerObjectPtr object = std::make_shared<ControllerObject>(address);
            mBotLineObjects[address] = object;
            mControllerObjects[address] = object;
            ss << "Controller";
        }
        else if (type == ObjectType::XAVIER)
        {
            XavierObjectPtr object = std::make_shared<XavierObject>(address);
            mBotLineObjects[address] = object;
            mXavierObjects[address] = object;
            ss << "Xavier";
        }

        // send connection success packet
        OutputMemoryBitStream   output;

        output.Write(messageType);
        output.Write(address.GetHash());

        SendPacket(output, address);

        ss << " Connected: " << address.ToString() << "-" << address.GetHash() << '\n';

        mLog->Add(ss.str());
    }
}

void NetworkManager::HandleObjectDisconnect(const BotLineObjectPtr& object) noexcept
{
    std::stringstream ss{};

    ss << " Disconnected: " << object->GetSocketAddress().ToString() << '\n';

    // send disconnection packet
    OutputMemoryBitStream   output;
    output.Write(MessageType::DISCONNECT);

    SendPacket(output, object->GetSocketAddress());

    if (object->GetObjectType() == ObjectType::JETBOT)
    {
        mJetBotObjects.erase(object->GetSocketAddress());
        ss.str("Jebot" + ss.str());
    }
    else if(object->GetObjectType() == ObjectType::CONTROLLER)
    {
        mControllerObjects.erase(object->GetSocketAddress());
        ss.str("Controller" + ss.str());
    }
    else if (object->GetObjectType() == ObjectType::XAVIER)
    {
        mXavierObjects.erase(object->GetSocketAddress());
        ss.str("Xavier" + ss.str());
    }
    mBotLineObjects.erase(object->GetSocketAddress());

    mLog->Add(ss.str());
}