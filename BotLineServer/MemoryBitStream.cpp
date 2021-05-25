#include "stdafx.h"
#include "MemoryBitStream.h"

void OutputMemoryBitStream::WriteBits(uint8_t inData, uint32_t inBitCount)
{
	uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);

	if (nextBitHead > mBitCapacity) {
		ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
	}

	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x07;

	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);

	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (bitsFreeThisByte < inBitCount) {
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	mBitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, uint32_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);

	while (inBitCount > 8) {
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0) {
		WriteBits(*srcByte, inBitCount);
	}
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewBitCapacity)
{
	if (mBuffer == nullptr) {
		mBuffer = new char[inNewBitCapacity >> 3];
		ZeroMemory(mBuffer, inNewBitCapacity >> 3);
	}
	else {
		char* tempBuffer = new char[inNewBitCapacity >> 3];
		memset(tempBuffer, 0, inNewBitCapacity >> 3);
		memcpy_s(tempBuffer, inNewBitCapacity >> 3, mBuffer, inNewBitCapacity >> 3);
		delete[] mBuffer;
		mBuffer = tempBuffer;
	}

	mBitCapacity = inNewBitCapacity;
}


void InputMemoryBitStream::ReadBits(uint8_t& outData, uint32_t inBitCount)
{
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	outData = static_cast<uint8_t>(mBuffer[byteOffset]) >> bitOffset;

	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (bitsFreeThisByte < inBitCount) {
		outData |= static_cast<uint8_t>(mBuffer[byteOffset + 1]) << bitsFreeThisByte;
	}

	outData &= (~(0x00ff << inBitCount));
	mBitHead += inBitCount;
}

void InputMemoryBitStream::ReadBits(void* outData, uint32_t inBitCount)
{
	uint8_t* destByte = reinterpret_cast<uint8_t*>(outData);
	while (inBitCount > 8) {
		ReadBits(*destByte, 8);
		++destByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0) {
		ReadBits(*destByte, inBitCount);
	}
}