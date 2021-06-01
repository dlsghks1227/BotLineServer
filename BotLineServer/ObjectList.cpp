#include "framework.h"
#include "ObjectList.h"

ImguiWindow::ObjectList::ObjectList() noexcept
{
}

void ImguiWindow::ObjectList::Initialize() noexcept
{
}

void ImguiWindow::ObjectList::SetJetbotObject(const std::unordered_map<SocketAddress, std::shared_ptr<JetbotObject>>& objects) noexcept
{
    mJetbotObject = objects;
}

void ImguiWindow::ObjectList::SetControllerObjects(const std::unordered_map<SocketAddress, std::shared_ptr<ControllerObject>>& objects) noexcept
{
    mControllerObject = objects;
}

void ImguiWindow::ObjectList::DrawJetBotObjects() noexcept
{
    static std::string      selected = {};
    static JetbotObject*    objectSelected = nullptr;
    static ScrollingBuffer  sVoltage, sCpuAverage, sMemory, sDisk;
    static float            time = 0.0;

    if (!ImGui::Begin("JetBot Objects"))
    {
        ImGui::End();
        return;
    }

    {
        ImGui::BeginChild("JetBot Objects", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.30f, -1), true);
        for (const auto& pair : mJetbotObject)
        {
            std::stringstream ss = {};
            ss << pair.first.ToString();
            if (ImGui::Selectable(ss.str().c_str(), selected == pair.first.ToString()))
            {
                selected = pair.first.ToString();
                objectSelected = pair.second.get();
            }
        }

        if (mJetbotObject.empty())
        {
            sVoltage.Erase();
            sCpuAverage.Erase();
            sMemory.Erase();
            sDisk.Erase();
            time = 0.0f;
            objectSelected = nullptr;
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();

    {
        ImGui::BeginChild("JetBot Detail", ImVec2(0, -1), true);
        if (objectSelected)
        {
            ImGui::Text(objectSelected->GetSocketAddress().ToString().c_str());
            ImGui::Separator();
            ImGui::Text("Voltage: %.2f", objectSelected->GetVoltage());
            ImGui::Text("CPU:     %.2f", objectSelected->GetCpuAverage());
            ImGui::Text("Memory:  %.2f", objectSelected->GetMemory());
            ImGui::Text("Disk:    %.2f", objectSelected->GetDisk());

            ImGui::Text("Last Message Type: %d", static_cast<int>(objectSelected->GetLastMessageType()));

            ImGui::Separator();

            time += ImGui::GetIO().DeltaTime;

            sVoltage.AddPoint(time, (objectSelected->GetVoltage() - 5.0f) * 0.25f);
            sCpuAverage.AddPoint(time, objectSelected->GetCpuAverage());
            sMemory.AddPoint(time, objectSelected->GetMemory() * 0.02f);
            sDisk.AddPoint(time, objectSelected->GetDisk() * 0.02f);

            const static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_Lock;
            ImPlot::SetNextPlotLimitsX(static_cast<double>(time) - 10.0, static_cast<double>(time), ImGuiCond_Always);
            ImPlot::SetNextPlotLimitsY(0, 2);
            if (ImPlot::BeginPlot("##State", nullptr, nullptr, ImVec2(-1, 150), 0, flags, flags)) {
                ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                ImPlot::PlotLine("Voltage", &sVoltage.Data[0].x, &sVoltage.Data[0].y, sVoltage.Data.size(), sVoltage.Offset, 2 * sizeof(float));
                ImPlot::PlotLine("CPU", &sCpuAverage.Data[0].x, &sCpuAverage.Data[0].y, sCpuAverage.Data.size(), sVoltage.Offset, 2 * sizeof(float));
                ImPlot::PlotLine("Memory", &sMemory.Data[0].x, &sMemory.Data[0].y, sMemory.Data.size(), sMemory.Offset, 2 * sizeof(float));
                ImPlot::PlotLine("Disk", &sDisk.Data[0].x, &sDisk.Data[0].y, sDisk.Data.size(), sDisk.Offset, 2 * sizeof(float));
                ImPlot::EndPlot();
            }
        }
        ImGui::EndChild();
    }

    ImGui::End();
}

void ImguiWindow::ObjectList::DrawControllerObjects() noexcept
{
    static std::string          selected = {};
    static ControllerObject*    objectSelected = nullptr;

    if (!ImGui::Begin("Controller Objects"))
    {
        ImGui::End();
        return;
    }

    // Left
    {
        ImGui::BeginChild("Controller Objects", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), true);
        for (const auto& pair : mControllerObject)
        {
            std::stringstream ss = {};
            ss << pair.first.ToString();
            if (ImGui::Selectable(ss.str().c_str(), selected == pair.first.ToString()))
            {
                selected = pair.first.ToString();
                objectSelected = pair.second.get();
            }
        }

        if (mControllerObject.empty())
        {
            objectSelected = nullptr;
        }

        ImGui::EndChild();
    }
    ImGui::SameLine();

    // Right
    {
        ImGui::BeginChild("Controller Detail", ImVec2(0, -1), true);
        if (objectSelected)
        {

            ImGui::Text(objectSelected->GetSocketAddress().ToString().c_str());
            ImGui::Separator();

            /* ImGui::Separator();
                if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                {
                    if (ImGui::BeginTabItem("Description"))
                    {
                        ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Details"))
                    {
                        ImGui::Text("ID: 0123456789");
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }*/
        }
        ImGui::EndChild();
    }

    ImGui::End();
}