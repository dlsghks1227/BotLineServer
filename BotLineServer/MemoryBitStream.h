#pragma once
#include "stdafx.h"

// C++ 메모리 관리
// https://integraldx.github.io/HowToSNUCSE/%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%96%B8%EC%96%B4/C++/%EB%A9%94%EB%AA%A8%EB%A6%AC_%EA%B4%80%EB%A6%AC.html

inline uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision)
{
	return static_cast<uint32_t>((inNumber - inMin) / inPrecision);
}

inline float ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision)
{
	return static_cast<float>(inNumber) * inPrecision + inMin;
}

class OutputMemoryBitStream
{
public:
	OutputMemoryBitStream() noexcept :
		mBitHead(0),
		mBuffer(nullptr)
	{
		ReallocBuffer(2048 * 8);
	}

	virtual ~OutputMemoryBitStream() {
		delete[] mBuffer;
	}

	const	char*			GetBufferPtr()		const	noexcept	{ return mBuffer; }
			uint32_t		GetBitLength()		const	noexcept	{ return mBitHead; }
			uint32_t		GetByteLength()		const	noexcept	{ return (mBitHead + 7) >> 3; }

			void			WriteBits(uint8_t inData, uint32_t inBitCount)			noexcept;
			void			WriteBits(const void* inData, uint32_t inBitCount)		noexcept;
			void			WriteBytes(const void* inData, uint32_t inByteCount)	noexcept	{ WriteBits(inData, inByteCount << 3); }

			void			Write(bool inData)			noexcept	{ WriteBits(&inData, 1); }

	template<typename T>
	void	Write(const T& inData, uint32_t inBitCount = sizeof(T) * 8)	noexcept
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
			"Generic write only supports primitive data types");
		WriteBits(&inData, inBitCount);
	}


private:
	void		ReallocBuffer(uint32_t inNewBitCapacity)	noexcept;

	char*		mBuffer;
	uint32_t	mBitHead;
	uint32_t	mBitCapacity;
};

class InputMemoryBitStream
{
public:
	InputMemoryBitStream(char* inBuffer, uint32_t inBitCount) noexcept :
		mBuffer(inBuffer),
		mBitCapacity(inBitCount),
		mBitHead(0),
		mIsBufferOwner(false) {}

	InputMemoryBitStream(const InputMemoryBitStream& inOther) noexcept :
		mBitCapacity(inOther.mBitCapacity),
		mBitHead(inOther.mBitHead),
		mIsBufferOwner(true)
	{
		int byteCount = mBitCapacity / 8;
		mBuffer = new char[byteCount];

		// 복사
		memcpy_s(mBuffer, byteCount, inOther.mBuffer, byteCount);
	}

	virtual ~InputMemoryBitStream() {
		if (mIsBufferOwner == true) {
			delete[] mBuffer;
		}
	}

	const	char*		GetBufferPtr()			const	noexcept	{ return mBuffer; }
			uint32_t	GetRemainingBitCount()	const	noexcept	{ return mBitCapacity; }

			void		ReadBits(uint8_t& outData, uint32_t inBitCount)		noexcept;
			void		ReadBits(void* outData, uint32_t inBitCount)		noexcept;
			void		ReadBytes(void* outData, uint32_t inByteCount)		noexcept	{ ReadBits(outData, inByteCount); }

			void		Read(uint32_t& outData, uint32_t inBitCount = 32)	noexcept	{ ReadBits(&outData, inBitCount); }
			void		Read(int& outData, uint32_t inBitCount = 32)		noexcept	{ ReadBits(&outData, inBitCount); }
			void		Read(float& outData)								noexcept	{ ReadBits(&outData, 32); }

			void		Read(uint16_t& outData, uint32_t inBitCount = 16)	noexcept	{ ReadBits(&outData, inBitCount); }
			void		Read(int16_t& outData, uint32_t inBitCount = 16)	noexcept	{ ReadBits(&outData, inBitCount); }

			void		Read(uint8_t& outData, uint32_t inBitCount = 8)		noexcept	{ ReadBits(&outData, inBitCount); }

			void		Read(bool& outData)									noexcept	{ ReadBits(&outData, 1); }

			void		ResetToCapacity(uint32_t inByteCapacity)			noexcept	{ mBitCapacity = inByteCapacity << 3; mBitHead = 0; }

	template<typename T>
	void	Read(T& outData, uint32_t inBitCount = sizeof(T) * 8) {
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
			"Generic read only supports primitive data types");
		ReadBits(&outData, inBitCount);
	}

private:
	char*		mBuffer;
	uint32_t	mBitHead;
	uint32_t	mBitCapacity;
	bool		mIsBufferOwner;
};