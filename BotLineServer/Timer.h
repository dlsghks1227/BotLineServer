#pragma once
#include "framework.h"

#include <cmath>
#include <cstdint>
#include <exception>

namespace Utility
{
	class Timer
	{
	public:
		Timer() noexcept(false) :
			mElapsedTicks(0),
			mLeftOverTicks(0),
			mFrameCount(0),
			mFramesPerSecond(0),
			mFramesThisSecond(0),
			mQpcSecondCounter(0),
			mIsFixedTimeStep(false),
			mTargetElapsedTicks(sTickPerSecond / 144)
		{
			// ���� ī������ ��, �ʴ� ������ ��ȯ
			if (!QueryPerformanceFrequency(&mQpcFrequency))
			{
				Utility::Throw("failed with QueryPerformanceFrequency()");
			}

			// ���� ī������ ���� ��, CPU�� Tick ��ȯ
			if (!QueryPerformanceCounter(&mQpcLastTime))
			{
				Utility::Throw("failed with QueryPerformanceCounter()");
			}

			// �ִ� ��Ÿ ���� 1/10 �ʷ� �ʱ�ȭ
			mQpcMaxDelta = static_cast<uint64_t>(mQpcFrequency.QuadPart / 10);
		}

		// Integer�� 10,000,000 Tick/second �� ��Ÿ��
		static constexpr	uint64_t	sTickPerSecond = 100000000;

		static constexpr	double		TicksToSeconds(uint64_t ticks)	noexcept	{ return static_cast<double>(ticks) / sTickPerSecond; }
		static constexpr	uint64_t	SecondsToTicks(double seconds)	noexcept	{ return static_cast<uint64_t>(seconds) * sTickPerSecond; }


							uint64_t	GetElapsedTicks()		const	noexcept	{ return mElapsedTicks; }
							double		GetElapsedSeconds()		const	noexcept	{ return TicksToSeconds(mElapsedTicks); }

							uint32_t	GetFrameCount()			const	noexcept	{ return mFrameCount; }

							uint32_t	GetFramesPerSecond()	const	noexcept	{ return mFramesPerSecond; }


		void	SetFixedTimeStep(bool isFixedTimeStep)			noexcept	{ mIsFixedTimeStep = isFixedTimeStep; }

		void	SetTargetElapsedTicks(uint64_t targetElapsed)	noexcept	{ mTargetElapsedTicks = targetElapsed; }
		void	SetTargetElapsedSeconds(double targetElapsed)	noexcept	{ mTargetElapsedTicks = SecondsToTicks(targetElapsed); }

		template<typename TUpdate>
		void	Tick(const TUpdate& update)
		{
			// ���� �ð�
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				Utility::Throw("failed with QueryPerformanceCounter()");
			}

			// �帥 �ð� ����
			uint64_t timeDelta = static_cast<uint64_t>(currentTime.QuadPart - mQpcLastTime.QuadPart);

			// ������ �ð� �缳��
			mQpcLastTime = currentTime;
			mQpcSecondCounter += timeDelta;

			if (timeDelta > mQpcMaxDelta)
			{
				timeDelta = mQpcMaxDelta;
			}

			// �ʴ� ƽ ���
			timeDelta *= sTickPerSecond;
			timeDelta /= static_cast<uint64_t>(mQpcFrequency.QuadPart);

			uint32_t lastFrameCount = mFrameCount;

			if (mIsFixedTimeStep)
			{
				// Fixed timestep update logic

				// If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
				// the clock to exactly match the target value. This prevents tiny and irrelevant errors
				// from accumulating over time. Without this clamping, a game that requested a 60 fps
				// fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
				// accumulate enough tiny errors that it would drop a frame. It is better to just round
				// small deviations down to zero to leave things running smoothly.

				if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - mTargetElapsedTicks))) < sTickPerSecond / 4000)
				{
					timeDelta = mTargetElapsedTicks;
				}

				mLeftOverTicks += timeDelta;

				while (mLeftOverTicks >= mTargetElapsedTicks)
				{
					mElapsedTicks	= mTargetElapsedTicks;
					mLeftOverTicks	-= mTargetElapsedTicks;
					mFrameCount		+= 1;

					update();
				}

			}
			else
			{
				mElapsedTicks	= timeDelta;
				mLeftOverTicks	= 0;
				mFrameCount		+= 1;

				update();
			}

			// ��Ż ƽ �ִ� �� ����

			// ���� �������� ����
			if (mFrameCount != lastFrameCount)
			{
				mFramesThisSecond += 1;
			}

			if (mQpcSecondCounter >= static_cast<uint64_t>(mQpcFrequency.QuadPart))
			{
				mFramesPerSecond	= mFramesThisSecond;
				mFramesThisSecond	= 0;
				mQpcSecondCounter	%= static_cast<uint64_t>(mQpcFrequency.QuadPart);
			}
		}

	private:
		LARGE_INTEGER	mQpcFrequency;
		LARGE_INTEGER	mQpcLastTime;
		uint64_t		mQpcMaxDelta;

		uint64_t		mElapsedTicks;
		uint64_t		mLeftOverTicks;

		uint32_t		mFrameCount;
		uint32_t		mFramesPerSecond;
		uint32_t		mFramesThisSecond;
		uint64_t		mQpcSecondCounter;

		// ������ ����
		bool			mIsFixedTimeStep;
		uint64_t		mTargetElapsedTicks;
	};
}