#include "MenuBarEditor.h"
#include "SceneManagerEditor.h"
#include "FileDialogs.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(MenuBarEditor, ImGuiPanelEditor)

void MenuBarEditor::OnImGuiRender()
{
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));       // 菜单栏背景
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));            // 文本颜色
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));         // 弹出菜单背景
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));          // 选中状态（子菜单打开时）

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.75f, 0.75f, 0.75f, 1.0f));   // 悬停背景
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));    // 激活背景
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));          // 边框颜色
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));       // 分隔线颜色

    
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene", "Ctrl+N"))
            {
                SceneManagerEditor::LoadScene();
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
            {
                SceneManagerEditor::LoadScene(FileDialogs::OpenFile("LindaEngine Scene (*.scene)\0*.scene\0"));
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
            {
                if (SceneManagerEditor::IsNewScene())
                {
                    SceneManagerEditor::SaveScene(FileDialogs::SaveFile("LindaEngine Scene (*.scene)\0*.scene\0"));
                }
                else
                {
                    SceneManagerEditor::SaveScene();
                }

            }
            if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
            {
                SceneManagerEditor::SaveScene(FileDialogs::SaveFile("LindaEngine Scene (*.scene)\0*.scene\0"));
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleColor(8);
}

