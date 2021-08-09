#pragma once
#include <exception>
#include <cstdio>
#include <sstream>


namespace Util
{
	class NetworkException : public std::exception
	{
	public:
		NetworkException(int errorCode) noexcept :
			mErrorCode(errorCode),
			mErrorMessage("") {};

		NetworkException(const std::string& message) noexcept :
			mErrorCode(-2),
			mErrorMessage(message) {};

		const char* what() const override
		{
			static char str[128] = {};

			if (mErrorCode == -2)
			{
				strcpy_s(str, mErrorMessage.c_str());
			}
			else
			{
				sprintf_s(str, "Failure with error code of %d", mErrorCode);
			}

			return str;
		}

	private:
		int			mErrorCode;
		std::string	mErrorMessage;
	};

	inline void Throw(int errorCode)
	{
		throw NetworkException(errorCode);
	}

	inline void Throw(const std::string& message)
	{
		throw NetworkException(message);
	}
}