#pragma once

namespace ImguiWindow
{
    // utility structure for realtime plot
    struct ScrollingBuffer {
        int MaxSize;
        int Offset;
        ImVector<ImVec2> Data;
        ScrollingBuffer(int max_size = 2000) {
            MaxSize = max_size;
            Offset = 0;
            Data.reserve(MaxSize);
        }
        void AddPoint(float x, float y) {
            if (Data.size() < MaxSize)
                Data.push_back(ImVec2(x, y));
            else {
                Data[Offset] = ImVec2(x, y);
                Offset = (Offset + 1) % MaxSize;
            }
        }
        void Erase() {
            if (Data.size() > 0) {
                Data.shrink(0);
                Offset = 0;
            }
        }
    };

	class ObjectList
	{
	public:
		ObjectList() noexcept;
		~ObjectList() = default;

		void	Initialize(const std::shared_ptr<UDPSocket>& socket) noexcept;
		void	SetJetbotObject(const std::unordered_map<SocketAddress, std::shared_ptr<JetbotObject>>& objects)            noexcept;
        void    SetControllerObjects(const std::unordered_map<SocketAddress, std::shared_ptr<ControllerObject>>& objects)   noexcept;
        void    SetXavierObjects(const std::unordered_map<SocketAddress, std::shared_ptr<XavierObject>>& objects)       noexcept;
        void    DrawJetBotObjects()     noexcept;
        void    DrawControllerObjects() noexcept;
        void    DrawXavierObjects()     noexcept;

	private:
        std::shared_ptr<UDPSocket>                                              mSocket;

        std::unordered_map<SocketAddress, std::shared_ptr<JetbotObject>>	    mJetbotObject;
        std::unordered_map<SocketAddress, std::shared_ptr<ControllerObject>>	mControllerObject;
        std::unordered_map<SocketAddress, std::shared_ptr<XavierObject>>	    mXavierObject;
	};
}