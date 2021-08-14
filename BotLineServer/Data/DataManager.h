#pragma once
#include "../framework.h"

namespace Data
{
	class DataManager
	{
	public:
		DataManager() noexcept = default;
		~DataManager() = default;

		DataManager(DataManager&&) = default;
		DataManager& operator = (DataManager&&) = default;

		DataManager(const DataManager&) = delete;
		DataManager& operator = (const DataManager&) = delete;

		void		Add(const ObjectType& type, const SocketAddress& fromAddress)	noexcept;
		void		Remove(const BotLineObjectPtr& object)							noexcept;

		void		AddJetbotObject(const SocketAddress& fromAddress)	noexcept;
		void		AddXavierObject(const SocketAddress& fromAddress)	noexcept;

		const	std::unordered_map<SocketAddress, BotLineObjectPtr>& GetBotLineObjects()	const	noexcept { return mBotLineObjects; }
		const	std::unordered_map<SocketAddress, JetbotObjectPtr>& GetJetbotObjects()		const	noexcept { return mJetBotObjects; }
		const	std::unordered_map<SocketAddress, XavierObjectPtr>& GetXavierObjects()		const	noexcept { return mXavierObjects; }

	private:
		std::unordered_map<SocketAddress, BotLineObjectPtr>		mBotLineObjects;
		std::unordered_map<SocketAddress, JetbotObjectPtr>		mJetBotObjects;
		std::unordered_map<SocketAddress, XavierObjectPtr>		mXavierObjects;
	};
};