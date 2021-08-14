#pragma once
#include "../framework.h"

namespace Component
{

	class DataComponent : public Util::Component
	{
	public:
		DataComponent(Util::Object* owner)	noexcept;
		virtual ~DataComponent();

		void		OnCreate()								noexcept	override;
		void		OnUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void		OnRender(const Util::Timer& timer)		noexcept	override;

		void		AddJetbotObject(const SocketAddress& fromAddress)	noexcept;
		void		AddXavierObject(const SocketAddress& fromAddress)	noexcept;

		void		Remove(const BotLineObjectPtr& object)				noexcept;

		const	std::unordered_map<SocketAddress, BotLineObjectPtr>&	GetBotLineObjects()	const	noexcept	{ return mBotLineObjects; }
		const	std::unordered_map<SocketAddress, JetbotObjectPtr>&		GetJetbotObjects()	const	noexcept	{ return mJetBotObjects; }
		const	std::unordered_map<SocketAddress, XavierObjectPtr>&		GetXavierObjects()	const	noexcept	{ return mXavierObjects; }

	private:
		std::unordered_map<SocketAddress, BotLineObjectPtr>		mBotLineObjects;
		std::unordered_map<SocketAddress, JetbotObjectPtr>		mJetBotObjects;
		std::unordered_map<SocketAddress, XavierObjectPtr>		mXavierObjects;

	};
};