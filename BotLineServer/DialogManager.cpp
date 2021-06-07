#include "framework.h"
#include "DialogManager.h"


DialogManager::DialogManager() noexcept
{
    mLog = std::make_shared<ImguiWindow::Log>();
    mMainControl = std::make_shared<ImguiWindow::MainControl>();
    mObjectList = std::make_shared<ImguiWindow::ObjectList>();
}

void DialogManager::Initialize(const std::shared_ptr<UDPSocket>& socket) noexcept
{
    mLog->Initialize();
    mMainControl->Initialize(socket);
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

    mMainControl->Draw();

    mObjectList->DrawJetBotObjects();
    mObjectList->DrawControllerObjects();
    mObjectList->DrawXavierObjects();
}

void DialogManager::UpdateObjects(const JetbotObjects& jetbotObjects, const ControllerObjects& controllerObjects, const XavierObjects& xavierObjects) noexcept
{
    mMainControl->UpdateObjects(jetbotObjects);

    mObjectList->SetJetbotObject(jetbotObjects);
    mObjectList->SetControllerObjects(controllerObjects);
    mObjectList->SetXavierObjects(xavierObjects);
}