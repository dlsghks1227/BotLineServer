#pragma once
#include "../../framework.h"

class SocketAddress;
class JetbotObject final : public BotLineObject
{
public:
	JetbotObject(const SocketAddress& address) noexcept;
	~JetbotObject() = default;

	const	float&			GetVoltage()	const						noexcept { return mVoltage; }
			void			SetVoltage(const float& voltage)			noexcept { mVoltage = voltage; }

	const	float&			GetCpuAverage()	const						noexcept { return mCpuAverage; }
			void			SetCpuAverage(const float& cpuAverage)		noexcept { mCpuAverage = cpuAverage; }

	const	float&			GetMemory()		const						noexcept { return mMemory; }
			void			SetMemory(const float& memory)				noexcept { mMemory = memory; }

	const	float&			GetDisk()		const						noexcept { return mDisk; }
			void			SetDisk(const float& disk)					noexcept { mDisk = disk; }

	const	MessageType&	GetLastMessageType() const					noexcept { return mLastMessageType; }
			void			SetLastMessageType(const MessageType& type)	noexcept { mLastMessageType = type; }

private:
	float		mVoltage;
	float		mCpuAverage;
	float		mMemory;
	float		mDisk;
	MessageType	mLastMessageType;
};