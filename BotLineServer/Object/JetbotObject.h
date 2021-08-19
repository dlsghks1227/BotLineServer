#pragma once
#include "../framework.h"


enum class Position : uint8_t
{
	LEFT = 0x00,
	RIGHT = 0x01,
	TOP = 0x02,
	BOTTOM = 0x03,

	DEFAULT = 0xFF,
};

struct JetbotState
{
	float		mVoltage;
	float		mCpuAverage;
	float		mMemory;
	float		mDisk;

	Position	mPosition;

	uint8_t		mIsWorking;
	uint8_t		mIsStop;
};


struct JetbotPosition
{
	float		mX;
	float		mY;
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