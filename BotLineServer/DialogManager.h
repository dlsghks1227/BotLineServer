#pragma once


class DialogManager
{
public:
	DialogManager() noexcept;
	~DialogManager() = default;

	using		JetbotObjects		= std::unordered_map<SocketAddress, std::shared_ptr<JetbotObject>>;
	using		ControllerObjects	= std::unordered_map<SocketAddress, std::shared_ptr<ControllerObject>>;

	const		std::shared_ptr<ImguiWindow::Log>&	GetLog()	const	noexcept	{ return mLog; }

	void		Initialize(const std::shared_ptr<UDPSocket>& socket)	noexcept;
	void		OnUpdate(const Utility::Timer& timer)					noexcept;
	void		OnLateUpdate(const Utility::Timer& timer)				noexcept;
	void		OnRender(const Utility::Timer& timer)					noexcept;

	void		UpdateObjects(const JetbotObjects& jetbotObjects,const ControllerObjects& controllerObjects) noexcept;

private:
	std::shared_ptr<ImguiWindow::Log>			mLog;
	std::shared_ptr<ImguiWindow::ObjectList>	mObjectList;
};