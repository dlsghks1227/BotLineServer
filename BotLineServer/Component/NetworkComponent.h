#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent : public Util::Component
	{
	public:
		NetworkComponent(Util::Object* owner)		noexcept;
		virtual ~NetworkComponent();

		void	Initialize(const std::shared_ptr<Util::Object>& ui) noexcept;
	private:
		std::shared_ptr<Util::Object>		mUI;
	};
};