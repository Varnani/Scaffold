#include <imgui.h>

#include <InputInfoLayer.hpp>

void InputInfoLayer::OnRenderUI(float deltaTime)
{
    ImGui::Begin("Input Info");

    ImGui::TextUnformatted("Mouse position:");
    ImGui::TextUnformatted("Last mouse key:");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextUnformatted("Last Key Events:");

    static int selectionIndex = 0;

    if (ImGui::BeginListBox("key log box", ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y)))
    {
        for (size_t i = 0; i < m_keyLog.size(); i++)
        {
            std::string& log = m_keyLog[i];

            const bool isSelected = (selectionIndex == i);

            if (ImGui::Selectable(log.c_str(), isSelected)) selectionIndex = i;
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::End();
}
