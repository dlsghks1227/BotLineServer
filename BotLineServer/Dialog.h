#pragma once
#include "framework.h"

#include "Component.h"

namespace Dialog
{
	class Dialog
	{
	public:
		Dialog() noexcept
		{

		}
		virtual ~Dialog()
		{
			for (auto& itr : mComponents)
			{
				itr.reset();
			}
		};

		template<typename T>
		std::shared_ptr<T>	AddComponent()
		{
			static_assert(std::is_base_of<Component, T>::value,
				"T must derive from Component");

			for (const auto& exisitingComponent : mComponents)
			{
				if (std::dynamic_pointer_cast<T>(exisitingComponent))
				{
					return std::dynamic_pointer_cast<T>(exisitingComponent);
				}
			}

			std::shared_ptr<T>	newComponent = std::make_shared<T>(this);
			mComponents.push_back(newComponent);

			return newComponent;
		}

		template<typename T>
		std::shared_ptr<T>	GetComponent()
		{
			static_assert(std::is_base_of<Component, T>::value,
				"T must derive from Component");

			for (const auto& exisitingComponent : mComponents)
			{
				if (std::dynamic_pointer_cast<T>(exisitingComponent))
				{
					return std::dynamic_pointer_cast<T>(exisitingComponent);
				}
			}

			return nullptr;
		}

		const	bool&		IsQueuedForRemoval()	const	{ return mIsQueueForRemoval; }
				void		QueueForRemoval()				{ mIsQueueForRemoval = true; }

		void				Initialize()
		{
			for (const auto& itr : mComponents)
			{
				itr->Initialize();
			}
		}

		void				OnUpdate(const Util::Timer& timer)
		{
			for (const auto& itr : mComponents)
			{
				itr->OnUpdate(timer);
			}
		}
		void				OnLateUpdate(const Util::Timer& timer)
		{
			for (const auto& itr : mComponents)
			{
				itr->OnLateUpdate(timer);
			}
		}
		void				OnRender(const Util::Timer& timer)
		{
			for (const auto& itr : mComponents)
			{
				itr->OnRender(timer);
			}
		}

	protected:
		std::vector<std::shared_ptr<Component>>	mComponents;
		bool									mIsQueueForRemoval;
	};
}