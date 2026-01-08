#include "ContentBrowserPanelEditor.h"
#include "Path.h"
#include "TextureManager.h"
#include "Texture.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <filesystem>
#include <iostream>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(ContentBrowserPanelEditor, ImGuiPanelEditor)

ContentBrowserPanelEditor::ContentBrowserPanelEditor()
{
    ReloadResources();
}

void ContentBrowserPanelEditor::OnImGuiRender()
{
	ImGui::Begin("Content Browser");

	for (auto& go : _fileSystem.children)
	{
		DrawContent(go);
	}

	ImGui::End();
}

void ContentBrowserPanelEditor::DrawContent(FileSystem& fs)
{
	std::vector<FileSystem>& children = fs.children;
	bool hasChildren = children.size() > 0;

	bool isSelected = false;
	ImGuiTreeNodeFlags flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (hasChildren)
	{
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	}
	else
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (isSelected)
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.20f, 0.50f, 0.80f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.20f, 0.50f, 0.80f, 1.00f));
	}

    DrawIcon(fs.type);

	bool opened = ImGui::TreeNodeEx((void*)&fs, flags, fs.name.c_str());

	if (isSelected)
	{
		ImGui::PopStyleColor(2);
	}

	if (opened)
	{
		if (hasChildren)
		{
			for (auto& child : children)
			{
                DrawContent(child);
			}
		}
		ImGui::TreePop();
	}

}

void ContentBrowserPanelEditor::DrawIcon(FileType type)
{
    unsigned int nativeColorID = 0;
    switch (type)
    {
    case FileType::Folder: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Folder.png")->nativeColorID; break;
    case FileType::Scene: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Scene.png")->nativeColorID; break;
    case FileType::Material: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Material.png")->nativeColorID; break;
    case FileType::FBX: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/FBX.png")->nativeColorID; break;
    case FileType::Font: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Font.png")->nativeColorID; break;
    case FileType::Prefab: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Prefab.png")->nativeColorID; break;
    case FileType::Texture: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Texture.png")->nativeColorID; break;
    case FileType::RenderTexture: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/RenderTexture.png")->nativeColorID; break;
    case FileType::Shader: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Shader.png")->nativeColorID; break;
    case FileType::ShaderLibrary: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/ShaderLibrary.png")->nativeColorID; break;
    case FileType::Text: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Text.png")->nativeColorID; break;
    case FileType::Other: nativeColorID = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Other.png")->nativeColorID; break;
    }

    // 获取当前行的高度（通常使用文本行高）
    float line_height = ImGui::GetTextLineHeightWithSpacing(); // 包含行间距
    // 或者使用：float line_height = ImGui::GetFrameHeight(); // 获取按钮/输入框高度

    // 设置图像大小（比行高小一些）
    float image_size = 20; // 70% 的行高

    // 计算垂直偏移，使图像在行中间
    float image_offset_y = (line_height - image_size) * 0.5f;

    // 保存当前光标Y位置
    float original_cursor_y = ImGui::GetCursorPosY();

    // 调整Y位置使图像垂直居中
    ImGui::SetCursorPosY(original_cursor_y + image_offset_y);

    ImGui::Image(reinterpret_cast<void*>(nativeColorID), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::SameLine();
    ImGui::SetCursorPosY(original_cursor_y);
}

void ContentBrowserPanelEditor::CollectFileFolder(FileSystem& fs)
{
    for (const auto& entry : std::filesystem::directory_iterator(fs.path)) {

        char toReplace = '\\';
        char replacement = '/';
        std::string str = entry.path().string();
        size_t pos = str.find(toReplace);
        while (pos != std::string::npos) {
            str.replace(pos, 1, 1, replacement);
            pos = str.find(toReplace, pos + 1);
        }

        FileSystem go;
        go.path = str;
        go.name = Path::GetFileNameNoExtension(go.path);
        
        if (entry.is_directory()) {
            go.type = FileType::Folder;
            CollectFileFolder(go);
        }
        else {
            go.type = CheckFileType(go.path);
        }
        fs.children.push_back(go);
    }
}

void ContentBrowserPanelEditor::ReloadResources()
{
    if (_resDirty == false)
        return;

    _resDirty = false;

    _fileSystem.path = "Assets";
    _fileSystem.name = Path::GetFileNameNoExtension(_fileSystem.path);
    _fileSystem.type = FileType::Folder;
    CollectFileFolder(_fileSystem);
    int a = 0;
}

FileType ContentBrowserPanelEditor::CheckFileType(std::string fileName)
{
    std::string extension = Path::GetFileExtensionName(fileName);

    if (extension == "png") return FileType::Texture;
    else if (extension == "jpg") return FileType::Texture;
    else if (extension == "hdr") return FileType::Texture;

    else if (extension == "txt") return FileType::Text;
    else if (extension == "json") return FileType::Text;

    else if (extension == "mat") return FileType::Material;

    else if (extension == "FBX") return FileType::FBX;
    else if (extension == "fbx") return FileType::FBX;

    else if (extension == "scene") return FileType::Scene;

    else if (extension == "shader") return FileType::Shader;
    else if (extension == "glsl") return FileType::ShaderLibrary;

    else if (extension == "prefab") return FileType::Prefab;

    else if (extension == "rt") return FileType::RenderTexture;

    else if (extension == "ttf") return FileType::Font;

    else  return FileType::Other;

    return FileType::Folder;
}
