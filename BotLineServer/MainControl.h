#pragma once

namespace ImguiWindow
{
	class MainControl
	{
	public:
		MainControl() noexcept;
		~MainControl() = default;

		void	Initialize(const std::shared_ptr<UDPSocket>& socket) noexcept;

		void	UpdateObjects(const std::unordered_map<SocketAddress, std::shared_ptr<JetbotObject>>& objects) noexcept;
		void	Draw() noexcept;

	private:
		std::shared_ptr<UDPSocket>                                              mSocket;

		std::unordered_map<SocketAddress, std::shared_ptr<JetbotObject>>	    mJetbotObjects;
	};
}