#pragma once
#include "../framework.h"

namespace Util
{
	class Object;
	class Component
	{
	public:
		Component(Object* owner) noexcept : mObject(owner) {}
		virtual ~Component() = default;

		virtual void		OnCreate()								noexcept	{};
		virtual void		OnUpdate(const Util::Timer& timer)		noexcept	{};
		virtual void		OnLateUpdate(const Util::Timer& timer)	noexcept	{};
		virtual void		OnRender(const Util::Timer& timer)		noexcept	{};

	protected:
		Object*		mObject;
	};
}