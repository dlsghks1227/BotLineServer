#include "framework.h"
#include "DialogManager.h"


DialogManager::DialogManager() noexcept
{
    mLog = std::make_shared<ImguiWindow::Log>();
    mObjectList = std::make_shared<ImguiWindow::ObjectList>();
}

void DialogManager::Initialize(const std::shared_ptr<UDPSocket>& socket) noexcept
{
    mLog->Initialize();
    mObjectList->Initialize(socket);
}

void DialogManager::OnUpdate(const Utility::Timer& timer) noexcept
{
}

void DialogManager::OnLateUpdate(const Utility::Timer& timer) noexcept
{
}

void DialogManager::OnRender(const Utility::Timer& timer) noexcept
{
    mLog->Draw("Network log");

    mObjectList->DrawJetBotObjects();
    mObjectList->DrawControllerObjects();
}

void DialogManager::UpdateObjects(const JetbotObjects& jetbotObjects, const ControllerObjects& controllerObjects) noexcept
{
    mObjectList->SetJetbotObject(jetbotObjects);
    mObjectList->SetControllerObjects(controllerObjects);
}