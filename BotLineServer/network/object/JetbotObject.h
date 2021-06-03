#pragma once
#include "../../framework.h"

class SocketAddress;
class JetbotObject final : public BotLineObject
{
public:
	JetbotObject(const SocketAddress& address) noexcept;
	~JetbotObject() = default;

	const	float&			GetVoltage()		const					noexcept { return mVoltage; }
			void			SetVoltage(const float& voltage)			noexcept { mVoltage = voltage; }

	const	float&			GetCpuAverage()		const					noexcept { return mCpuAverage; }
			void			SetCpuAverage(const float& cpuAverage)		noexcept { mCpuAverage = cpuAverage; }

	const	float&			GetMemory()			const					noexcept { return mMemory; }
			void			SetMemory(const float& memory)				noexcept { mMemory = memory; }

	const	float&			GetDisk()			const					noexcept { return mDisk; }
			void			SetDisk(const float& disk)					noexcept { mDisk = disk; }

	const	uint32_t&		GetSpeed()			const					noexcept { return mSpeed; }
			void			SetSpeed(const uint32_t& speed)				noexcept { mSpeed = speed; }

	const	uint32_t&		GetLeftWheel()		const					noexcept { return mLeftWheel; }
			void			SetLeftWheel(const uint32_t& wheel)			noexcept { mLeftWheel = wheel; }

	const	uint32_t&		GetRightWheel()		const					noexcept { return mRightWheel; }
			void			SetRightWheel(const uint32_t& wheel)		noexcept { mRightWheel = wheel; }

	const	MessageType&	GetLastMessageType() const					noexcept { return mLastMessageType; }
			void			SetLastMessageType(const MessageType& type)	noexcept { mLastMessageType = type; }

private:
	float		mVoltage;
	float		mCpuAverage;
	float		mMemory;
	float		mDisk;

	uint32_t	mSpeed;
	uint32_t	mLeftWheel;
	uint32_t	mRightWheel;

	MessageType	mLastMessageType;
};