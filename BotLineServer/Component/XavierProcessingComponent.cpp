#include "../framework.h"
#include "XavierProcessingComponent.h"

Component::XavierProcessingComponent::XavierProcessingComponent(Util::Object* owner) noexcept :
	BaseProcessingComponent(owner)
{
	mProcessingStorage.insert({ MessageType::INFO_JETBOT_POSITION,	std::bind(&XavierProcessingComponent::InfoJetbotPosition,	this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) });
	mProcessingStorage.insert({ MessageType::END_DATA,				std::bind(&XavierProcessingComponent::EndData,				this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) });

	mLeftArea	= CollisionArea{0.0f + sOffset, 0.5f,			0.7f};
	mRightArea	= CollisionArea{1.0f - sOffset, 0.5f,			0.7f};
	mTop		= CollisionArea{0.5f,			0.0f + sOffset, 0.7f};
	mBottom		= CollisionArea{0.5f,			1.0f - sOffset, 0.7f};

	mStationCollsion = { false, false, false, false };
}

Component::XavierProcessingComponent::~XavierProcessingComponent()
{
}

void Component::XavierProcessingComponent::OnCreate() noexcept
{
	BaseProcessingComponent::OnCreate();
}

void Component::XavierProcessingComponent::OnUpdate(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnUpdate(timer);
}

void Component::XavierProcessingComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnLateUpdate(timer);

	static double elapsedTime = 0.0;
	elapsedTime += timer.GetElapsedSeconds();

	if (elapsedTime > sUpdateCycle)
	{
		this->UpdateJetbotPosition();
		elapsedTime = 0.0;
	}
}

void Component::XavierProcessingComponent::OnRender(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnRender(timer);

	if (!ImGui::Begin("Rendering", 0, ImGuiWindowFlags_NoResize)) {
		ImGui::End();
		return;
	}
	ImGui::SetWindowSize(ImVec2(sWindowWidth + 20.0f, sWindowHeight + 40.0f));

	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImVec2(sWindowWidth, sWindowHeight);
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1,	IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1,		IM_COL32(255, 255, 255, 255));

	draw_list->AddCircle(
		ImVec2((canvas_p0.x + (mLeftArea.x * sWindowWidth)), (canvas_p0.y + (mLeftArea.y * sWindowHeight))), (mLeftArea.radius * sCircleSize),
		mStationCollsion[static_cast<int>(Position::LEFT)] ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 102, 255), 0, 3.0f);
	draw_list->AddCircle(ImVec2((canvas_p0.x + (mRightArea.x * sWindowWidth)), (canvas_p0.y + (mRightArea.y * sWindowHeight))), (mRightArea.radius * sCircleSize),
		mStationCollsion[static_cast<int>(Position::RIGHT)] ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 102, 255), 0, 3.0f);
	draw_list->AddCircle(ImVec2((canvas_p0.x + (mTop.x * sWindowWidth)), (canvas_p0.y + (mTop.y * sWindowHeight))), (mTop.radius * sCircleSize),
		mStationCollsion[static_cast<int>(Position::TOP)] ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 102, 255), 0, 3.0f);
	draw_list->AddCircle(ImVec2((canvas_p0.x + (mBottom.x * sWindowWidth)), (canvas_p0.y + (mBottom.y * sWindowHeight))), (mBottom.radius * sCircleSize),
		mStationCollsion[static_cast<int>(Position::BOTTOM)] ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 102, 255), 0, 3.0f);

	for (const auto& itr : mJetbotPositions)
	{
		draw_list->AddCircle(
			ImVec2(canvas_p0.x + (itr.mX), canvas_p0.y + (itr.mY)),
			sObjectCircleSize,
			IM_COL32(255, 255, 102, 255), 0, 3.0f);
	}

	const auto jetbotComponent = mObject->GetComponent<JetbotProcessingComponent>();
	for (const auto& itr : jetbotComponent->GetObjects())
	{
		ImU32 color = IM_COL32(255, 0, 0, 255);
		if (itr.second->GetJetbotState().mIsWorking == 1)
		{
			color = IM_COL32(0, 255, 102, 255);
		}
		switch (itr.second->GetJetbotState().mPosition)
		{
			case Position::LEFT:
			{
				draw_list->AddCircleFilled(
					ImVec2((canvas_p0.x + (mLeftArea.x * sWindowWidth)), (canvas_p0.y + (mLeftArea.y * sWindowHeight))), (mLeftArea.radius * sCircleSize), color, 0);
				break;
			}
			case Position::RIGHT:
			{
				draw_list->AddCircleFilled(
					ImVec2((canvas_p0.x + (mRightArea.x * sWindowWidth)), (canvas_p0.y + (mRightArea.y * sWindowHeight))), (mRightArea.radius * sCircleSize), color, 0);
				break;
			}
			case Position::TOP:
			{
				draw_list->AddCircleFilled(
					ImVec2((canvas_p0.x + (mTop.x * sWindowWidth)), (canvas_p0.y + (mTop.y * sWindowHeight))), (mTop.radius * sCircleSize), color, 0);
				break;
			}
			case Position::BOTTOM:
			{
				draw_list->AddCircleFilled(
					ImVec2((canvas_p0.x + (mBottom.x * sWindowWidth)), (canvas_p0.y + (mBottom.y * sWindowHeight))), (mBottom.radius * sCircleSize), color, 0);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	ImGui::End();
}

void Component::XavierProcessingComponent::InfoJetbotPosition(InputMemoryBitStream& input, const SocketAddress& fromAddress, const XavierObjectPtr& object) noexcept
{
	JetbotPosition	pos;

	input.Read(pos.mX);
	input.Read(pos.mY);

	bool isCollided = false;
	if (this->IsCollision(pos, mLeftArea))
	{
		mStationCollsion[static_cast<int>(Position::LEFT)] = true; isCollided = true;
	}
	if (this->IsCollision(pos, mRightArea))	
	{
		mStationCollsion[static_cast<int>(Position::RIGHT)] = true; isCollided = true;
	}
	if (this->IsCollision(pos, mTop))
	{
		mStationCollsion[static_cast<int>(Position::TOP)] = true; isCollided = true;
	}
	if (this->IsCollision(pos, mBottom))
	{
		mStationCollsion[static_cast<int>(Position::BOTTOM)] = true; isCollided = true;
	}

	if (isCollided == true)
	{
		const auto jetbotComponent = mObject->GetComponent<JetbotProcessingComponent>();
		for (const auto& itr : jetbotComponent->GetObjects())
		{
			if (itr.second->GetJetbotState().mIsWorking == 1)
			{
				int pos = static_cast<int>(itr.second->GetJetbotState().mPosition);
				if (pos < 0 || pos > 4) return;
				if (mStationCollsion[pos] == true)
				{
					jetbotComponent->JetbotArrived(itr.first);
					return;
				}
			}
		}
	}

	mTempPositions.push_back(pos);
}

void Component::XavierProcessingComponent::EndData(InputMemoryBitStream& input, const SocketAddress& fromAddress, const XavierObjectPtr& object) noexcept
{
	MessageType type = MessageType::DEFAULT;
	input.Read(type);

	switch (type)
	{
		case MessageType::INFO_JETBOT_POSITION:
		{
			mJetbotPositions = mTempPositions;
			mTempPositions.clear();
			break;
		}
		default:
		{
			break;
		}
	}
}

void Component::XavierProcessingComponent::UpdateJetbotPosition() noexcept
{
	mStationCollsion = { false, false, false, false };
	mJetbotPositions.clear();
	if (this->mObjects.empty() == false)
	{
		for (const auto& itr : this->mObjects)
		{
			OutputMemoryBitStream outputStream = OutputMemoryBitStream();
			outputStream.Write(MessageType::INFO_JETBOT_POSITION);
			mNetworkComponent->SendPacket(outputStream, itr.first);
		}
	}
}

bool Component::XavierProcessingComponent::IsCollision(const JetbotPosition& pos, const CollisionArea& area) noexcept
{
	float deltaX = (pos.mX - (area.x * sWindowWidth));
	float deltaY = (pos.mY - (area.y * sWindowHeight));
	float length = std::sqrtf((deltaX * deltaX) + (deltaY * deltaY));
	
	if (length > (sObjectCircleSize + (area.radius * sCircleSize)))
		return false;

	return true;
}
