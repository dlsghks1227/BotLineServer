#pragma once
#include "../framework.h"


struct XavierState
{
	float		mCpuAverage;
	float		mMemory;
	float		mDisk;
};

class SocketAddress;
class XavierObject final : public BotLineObject
{
public:
	XavierObject(const SocketAddress& address) noexcept;
	virtual ~XavierObject() = default;

	const		XavierState&	GetXavierState()							{ return mState; }
				void			SetXavierState(const XavierState& state)	{ mState = state; }

private:
	XavierState		mState;
};

