#pragma once
#include "../framework.h"


struct JetbotState
{
	float		mVoltage;
	float		mCpuAverage;
	float		mMemory;
	float		mDisk;

	uint32_t	mSpeed;
	uint32_t	mLeftWheel;
	uint32_t	mRightWheel;
};

class SocketAddress;
class JetbotObject final : public BotLineObject
{
public:
	JetbotObject(const SocketAddress& address) noexcept;
	virtual ~JetbotObject() = default;

	const	JetbotState&	GetJetbotState()							{ return mState; }
			void			SetJetbotState(const JetbotState& state)	{ mState = state; }

private:
	JetbotState		mState;
};