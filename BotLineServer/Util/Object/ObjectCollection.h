#pragma once
#include "../../framework.h"

namespace Util
{
	class ObjectCollection final
	{
	public:
		ObjectCollection() noexcept;
		~ObjectCollection();

		ObjectCollection(ObjectCollection&&) = default;
		ObjectCollection& operator = (ObjectCollection&&) = default;

		ObjectCollection(const ObjectCollection&) = delete;
		ObjectCollection& operator = (const ObjectCollection&) = delete;

		void		OnUpdate(const Util::Timer& timer)			noexcept;
		void		OnLateUpdate(const Util::Timer& timer)		noexcept;
		void		OnRender(const Util::Timer& timer)			noexcept;

		void		Add(const std::shared_ptr<Object>& object)	noexcept;

		void		ProcessNewObjects()		noexcept;
		void		ProcessRemovals()		noexcept;

	private:
		std::vector<std::shared_ptr<Object>>	mObjects;
		std::vector<std::shared_ptr<Object>>	mNewObjects;
	};
}