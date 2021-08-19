#include "../framework.h"
#include "JetbotProcessingComponent.h"

Component::JetbotProcessingComponent::JetbotProcessingComponent(Util::Object* owner) noexcept :
	BaseProcessingComponent<JetbotObject>(owner)
{
	mProcessingStorage.insert({ MessageType::INFO_CURRENT_STATE,	std::bind(&JetbotProcessingComponent::InfoCurrentState,	this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
}

Component::JetbotProcessingComponent::~JetbotProcessingComponent()
{
}

void Component::JetbotProcessingComponent::OnCreate() noexcept
{
	BaseProcessingComponent::OnCreate();
}

void Component::JetbotProcessingComponent::OnUpdate(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnUpdate(timer);
}

void Component::JetbotProcessingComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnLateUpdate(timer);

	static double elapsedTime = 0.0;
	elapsedTime += timer.GetElapsedSeconds();

	if (elapsedTime > sUpdateCycle)
	{
		this->UpdateCurrentState();
		elapsedTime = 0.0;
	}
}

void Component::JetbotProcessingComponent::OnRender(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnRender(timer);

	if (!ImGui::Begin("Jetbot")) {
		ImGui::End();
		return;
	}

	for (const auto& itr : mObjects)
	{
		if (itr.second->GetJetbotState().mIsWorking == false)
		{
			for (int i = 0; i < 4; i++)
			{
				if (static_cast<int>(itr.second->GetJetbotState().mPosition) != i)
				{
					std::stringstream ss{};
					ss << i;
					if (ImGui::Button(ss.str().c_str(), ImVec2(20.0f, 20.0f)) == true)
					{
						this->JetbotChangePosition(itr.first, static_cast<Position>(i));
					}
				}
			}
		}
	}

	ImGui::End();
}

void Component::JetbotProcessingComponent::JetbotChangePosition(const SocketAddress& address, const Position& pos) noexcept
{
	const auto itr = mObjects.find(address);
	if (itr != mObjects.cend())
	{
		OutputMemoryBitStream outputStream = OutputMemoryBitStream();
		outputStream.Write(MessageType::JETBOT_CONTROL);
		outputStream.Write(pos);
		mNetworkComponent->SendPacket(outputStream, address);
	}
}

void Component::JetbotProcessingComponent::JetbotArrived(const SocketAddress& address) noexcept
{
	const auto itr = mObjects.find(address);
	if (itr != mObjects.cend())
	{
		OutputMemoryBitStream outputStream = OutputMemoryBitStream();
		outputStream.Write(MessageType::JETBOT_ARRIVED);
		mNetworkComponent->SendPacket(outputStream, address);
	}
}

void Component::JetbotProcessingComponent::InfoCurrentState(InputMemoryBitStream& input, const SocketAddress& fromAddress, const JetbotObjectPtr& object) noexcept
{
	JetbotState state = JetbotState();

	input.Read(state.mVoltage);
	input.Read(state.mCpuAverage);
	input.Read(state.mMemory);
	input.Read(state.mDisk);

	input.Read(state.mPosition);

	for (const auto& itr : mObjects)
	{
		if (itr.first != fromAddress || itr.second->GetJetbotState().mPosition != Position::DEFAULT)
		{
			if (itr.second->GetJetbotState().mPosition == static_cast<Position>(state.mPosition))
			{
				OutputMemoryBitStream outputStream = OutputMemoryBitStream();
				outputStream.Write(MessageType::JETBOT_POSITION_ERROR);
				mNetworkComponent->SendPacket(outputStream, fromAddress);

				return;
			}
		}
	}

	input.Read(state.mIsWorking);
	input.Read(state.mIsStop);

	object->SetJetbotState(state);
}

void Component::JetbotProcessingComponent::UpdateCurrentState() noexcept
{
	if (this->mObjects.empty() == false)
	{
		for (const auto& itr : this->mObjects)
		{
			OutputMemoryBitStream outputStream = OutputMemoryBitStream();
			outputStream.Write(MessageType::INFO_CURRENT_STATE);
			mNetworkComponent->SendPacket(outputStream, itr.first);
		}
	}
}
