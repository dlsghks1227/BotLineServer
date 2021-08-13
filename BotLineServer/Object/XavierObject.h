#pragma once
#include "../framework.h"

class SocketAddress;
class XavierObject final : public BotLineObject
{
public:
	XavierObject(const SocketAddress& address) noexcept;
	virtual ~XavierObject() = default;

	const	float&			GetCpuAverage()		const					noexcept { return mCpuAverage; }
			void			SetCpuAverage(const float& cpuAverage)		noexcept { mCpuAverage = cpuAverage; }

	const	float&			GetMemory()			const					noexcept { return mMemory; }
			void			SetMemory(const float& memory)				noexcept { mMemory = memory; }

	const	float&			GetDisk()			const					noexcept { return mDisk; }
			void			SetDisk(const float& disk)					noexcept { mDisk = disk; }

private:
	float		mCpuAverage;
	float		mMemory;
	float		mDisk;
};

