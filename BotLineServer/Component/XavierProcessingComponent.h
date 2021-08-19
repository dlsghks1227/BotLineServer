#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent;
	class XavierProcessingComponent : public BaseProcessingComponent<XavierObject>
	{
	public:
		XavierProcessingComponent(Util::Object* owner)			noexcept;
		virtual ~XavierProcessingComponent();

		static	constexpr	double	sUpdateCycle		= 0.5f;
		static	constexpr	float	sWindowWidth		= 1280.0f;
		static	constexpr	float	sWindowHeight		= 720.0f;
		static	constexpr	float	sCircleSize			= 100.0f;
		static	constexpr	float	sObjectCircleSize	= 40.0f;
		static	constexpr	float	sOffset				= 0.2f;

		void		OnCreate()									noexcept	override;
		void		OnUpdate(const Util::Timer& timer)			noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnRender(const Util::Timer& timer)			noexcept	override;

		const	std::list<JetbotPosition>& GetJetbotPosition()	noexcept	{ return mJetbotPositions; };

	private:
		struct StationCollision
		{
			bool	left;
			bool	right;
			bool	top;
			bool	bottom;
		};

		struct CollisionArea
		{
			float	x;
			float	y;
			float	radius;
		};

		void		InfoJetbotPosition(InputMemoryBitStream& input, const SocketAddress& fromAddress, const XavierObjectPtr& object)	noexcept;
		void		EndData(InputMemoryBitStream& input, const SocketAddress& fromAddress, const XavierObjectPtr& object)				noexcept;

		void		UpdateJetbotPosition() noexcept;
		bool		IsCollision(const JetbotPosition& pos, const CollisionArea& area) noexcept;

		CollisionArea		mLeftArea;
		CollisionArea		mRightArea;
		CollisionArea		mTop;
		CollisionArea		mBottom;

		std::vector<bool>				mStationCollsion;

		std::list<JetbotPosition>		mTempPositions;
		std::list<JetbotPosition>		mJetbotPositions;
	};
};