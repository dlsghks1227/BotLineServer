#include "ObjectCollection.h"

Util::ObjectCollection::ObjectCollection() noexcept
{
}

Util::ObjectCollection::~ObjectCollection()
{
	for (auto& itr : mObjects)
	{
		itr.reset();
	}

	for (auto& itr : mNewObjects)
	{
		itr.reset();
	}
}

void Util::ObjectCollection::OnUpdate(const Util::Timer& timer) noexcept
{
	this->ProcessRemovals();
	this->ProcessNewObjects();

	for (const auto& itr : mObjects)
	{
		if (itr->GetEnable() == true)
		{
			itr->OnUpdate(timer);
		}
	}
}

void Util::ObjectCollection::OnLateUpdate(const Util::Timer& timer) noexcept
{
	for (const auto& itr : mObjects)
	{
		if (itr->GetEnable() == true)
		{
			itr->OnLateUpdate(timer);
		}
	}
}

void Util::ObjectCollection::OnRender(const Util::Timer& timer) noexcept
{
	for (const auto& itr : mObjects)
	{
		if (itr->GetEnable() == true)
		{
			itr->OnRender(timer);
		}
	}
}

void Util::ObjectCollection::Add(const std::shared_ptr<Object>& object) noexcept
{
	mNewObjects.push_back(object);
}

void Util::ObjectCollection::ProcessNewObjects() noexcept
{
	if (mNewObjects.empty() == false)
	{
		for (const auto& itr : mNewObjects)
		{
			itr->OnCreate();
		}
		mObjects.insert(mObjects.end(), mNewObjects.begin(), mNewObjects.end());
		mNewObjects.clear();
	}
}

void Util::ObjectCollection::ProcessRemovals() noexcept
{
	auto itr = mObjects.begin();
	while (itr != mObjects.end())
	{
		auto obj = **itr;

		if (obj.IsQueuedForRemoval() == true)
		{
			itr = mObjects.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


// std::map(set) insert 정확하게 사용하기
// https://yonmy.com/archives/9