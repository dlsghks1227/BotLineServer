#pragma once
#include "../../framework.h"

namespace Util
{
	class Object
	{
	public:
		Object() noexcept :
			mIsEnable(true),
			mIsQueuedForRemoval(false)
		{
		}

		virtual ~Object()
		{
			for (auto& itr : mComponents)
			{
				itr.reset();
			}
		}

		template<typename T>
		std::shared_ptr<T>	AddComponent()	noexcept
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			for (const auto& exisitingComponent : mComponents)
			{
				// 스마트 포인터 형변환
				// https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
				if (std::dynamic_pointer_cast<T>(exisitingComponent))
				{
					return std::dynamic_pointer_cast<T>(exisitingComponent);
				}
			}

			std::shared_ptr<T> newComponent = std::make_shared<T>(this);
			mComponents.push_back(newComponent);

			return newComponent;
		}

		template<typename T>
		std::shared_ptr<T> GetComponent()	noexcept
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			for (const auto& exisitingComponent : mComponents)
			{
				if (std::dynamic_pointer_cast<T>(exisitingComponent))
				{
					return std::dynamic_pointer_cast<T>(exisitingComponent);
				}
			}

			return nullptr;
		}

		void	OnCreate()							noexcept
		{
			for (const auto& itr : mComponents)
			{
				itr->OnCreate();
			}
		}

		void	OnUpdate(const Util::Timer& timer)		noexcept
		{
			for (const auto& itr : mComponents)
			{
				itr->OnUpdate(timer);
			}
		}

		void	OnLateUpdate(const Util::Timer& timer)	noexcept
		{
			for (const auto& itr : mComponents)
			{
				itr->OnLateUpdate(timer);
			}
		}

		void	OnRender(const Util::Timer& timer)		noexcept
		{
			for (const auto& itr : mComponents)
			{
				itr->OnRender(timer);
			}
		}

		void	SetEnable(bool isEnable)	noexcept	{ mIsEnable = isEnable; }
		bool	GetEnable()					noexcept	{ return mIsEnable; }

		void	QeuedForRemoval()			noexcept	{ mIsQueuedForRemoval = true; }
		bool	IsQueuedForRemoval()		noexcept	{ return mIsQueuedForRemoval; }


	protected:
		std::vector<std::shared_ptr<Component>>		mComponents;
		
		bool		mIsEnable;
		bool		mIsQueuedForRemoval;
	};
}