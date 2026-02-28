#include "ContentBrowserPanelEditor.h"
#include "Path.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Entity.h"
#include "SceneManagerEditor.h"
#include "NodeEditor.h"
#include "Scene.h"

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

    DrawBlankAreaDropTarget();

    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
    {
        _hoveredNode = nullptr;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            _selectedNode = nullptr;
            _selectionNodes.clear();
        }
    }

	ImGui::End();
}

void ContentBrowserPanelEditor::DrawContent(FileNode& fs)
{
	std::vector<FileNode>& children = fs.children;
	bool hasChildren = children.size() > 0;

    bool isSelected = IsFileNodeSelected(&fs);
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

    
    float original_cursor_x = ImGui::GetCursorPosX();
    if (hasChildren == false)
        ImGui::SetCursorPosX(original_cursor_x);
    else
        ImGui::SetCursorPosX(original_cursor_x + 27);

	bool opened = ImGui::TreeNodeEx((void*)&fs, flags, fs.name.c_str());

    if (fs.type == FileType::Scene && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && _doubleClicked == false)
    {
        SceneManagerEditor::LoadScene(fs.path);
        _doubleClicked = true;
    }
    else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        _doubleClicked = false;
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        _mouseDownPos = ImGui::GetMousePos();
    }

    DragNodes(&fs);

	if (isSelected)
	{
		ImGui::PopStyleColor(2);
	}

    // 检查是否点击了展开三角区域
    bool clickedOnArrow = false;
    if (hasChildren && ImGui::IsItemHovered())
    {
        // 获取树节点的矩形区域
        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectMax = ImGui::GetItemRectMax();
        // 获取鼠标位置
        ImVec2 mousePos = ImGui::GetMousePos();

        // 计算展开三角的近似区域（在ImGui中，箭头区域通常在左侧）
        float arrowWidth = ImGui::GetFrameHeight();
        ImRect arrowRect = ImRect(rectMin.x, rectMin.y, rectMin.x + arrowWidth, rectMax.y);

        // 检查鼠标是否在箭头区域内
        if (arrowRect.Contains(mousePos))
        {
            clickedOnArrow = true;
        }
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        ImVec2 mouseDelta = ImVec2(
            ImGui::GetMousePos().x - _mouseDownPos.x,
            ImGui::GetMousePos().y - _mouseDownPos.y
        );

        float dragThreshold = 5.0f;
        bool isDragging = std::abs(mouseDelta.x) > dragThreshold || std::abs(mouseDelta.y) > dragThreshold;

        if (false == isDragging && false == clickedOnArrow)
        {
            HandleNodeSelection(&fs, ImGui::GetIO().KeyCtrl);
        }
    }

    if (ImGui::IsItemHovered() && _hoveredNode != &fs)
    {
        _hoveredNode = &fs;
    }

    DrawIcon(fs.type, original_cursor_x);

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

void ContentBrowserPanelEditor::DrawIcon(FileType type, float offsetX)
{
    uint64_t nativeColorID = 0;
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

    ImGui::SameLine();

    // 获取当前行的高度（通常使用文本行高）
    float line_height = ImGui::GetTextLineHeightWithSpacing(); // 包含行间距
    // 或者使用：float line_height = ImGui::GetFrameHeight(); // 获取按钮/输入框高度

    // 设置图像大小（比行高小一些）
    float image_size = 20;

    // 计算垂直偏移，使图像在行中间
    float image_offset_y = (line_height - image_size) * 0.5f;

    // 保存当前光标Y位置
    float original_cursor_y = ImGui::GetCursorPosY();

    // 调整Y位置使图像垂直居中
    ImGui::SetCursorPosY(original_cursor_y + image_offset_y);
    ImGui::SetCursorPosX(offsetX);

    ImGui::Image(reinterpret_cast<void*>(nativeColorID), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));

    //ImGui::SameLine();
    //ImGui::SetCursorPosY(original_cursor_y);
}

void ContentBrowserPanelEditor::CollectFileFolder(FileNode& fs)
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

        FileNode go;
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

void ContentBrowserPanelEditor::SortFileFolder(FileNode& fs)
{
    std::sort(fs.children.begin(), fs.children.end(), [](FileNode& a, FileNode& b) {
        return a.type < b.type; });

    for (auto& go : fs.children)
    {
        SortFileFolder(go);
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
    SortFileFolder(_fileSystem);
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
    else if (extension == "dae") return FileType::FBX;

    else if (extension == "scene") return FileType::Scene;

    else if (extension == "shader") return FileType::Shader;
    else if (extension == "glsl") return FileType::ShaderLibrary;

    else if (extension == "prefab") return FileType::Prefab;

    else if (extension == "rt") return FileType::RenderTexture;

    else if (extension == "ttf") return FileType::Font;

    else  return FileType::Other;

    return FileType::Folder;
}

void ContentBrowserPanelEditor::DrawBlankAreaDropTarget()
{
    if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->WorkRect, ImGui::GetID("##FullWindowDropTarget")))
    {
        ImGuiDragDropFlags target_flags = 0;
        target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILENODE_DRAG", target_flags))
        {
            // 单个实体拖拽
            FileNode* draggedNode = *(FileNode**)payload->Data;
            printf("BeginDragDropTarget Single \n");
        }
        else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILENODE_DRAG_MULTI", target_flags))
        {
            int count = payload->DataSize / sizeof(FileNode*);
            FileNode** draggedNodes = (FileNode**)payload->Data;
            printf("BeginDragDropTarget Multi \n");
        }
        else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG", target_flags))
        {
            Entity* draggedEntity = *(Entity**)payload->Data;
            std::string prefabPath = "Assets/" + draggedEntity->GetName() + ".prefab";
            SceneManagerEditor::GetCurrentNode()->scene->SerializePrefab(prefabPath, draggedEntity);
            AddNode(FileType::Prefab, prefabPath);
        }

        ImGui::EndDragDropTarget();
    }

}

void ContentBrowserPanelEditor::DragNodes(FileNode* fs)
{
    if (ImGui::BeginDragDropSource())
    {
        if (_selectionNodes.size() > 1 && IsFileNodeSelected(fs))
        {
            std::vector<FileNode*> draggingList(_selectionNodes.begin(), _selectionNodes.end());
            ImGui::SetDragDropPayload("FILENODE_DRAG_MULTI", draggingList.data(), draggingList.size() * sizeof(FileNode*));
            ImGui::Text("Dragging");
        }
        else
        {
            SelectSingle();
            ImGui::SetDragDropPayload("FILENODE_DRAG", &_selectedNode, sizeof(FileNode*));
            ImGui::Text("Dragging");
        }
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILENODE_DRAG"))
        {
            // 单个实体拖拽
            FileNode* draggedNode = *(FileNode**)payload->Data;
            printf("BeginDragDropTarget Single \n");
        }
        else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILENODE_DRAG_MULTI"))
        {
            int count = payload->DataSize / sizeof(FileNode*);
            FileNode** draggedNodes = (FileNode**)payload->Data;
            printf("BeginDragDropTarget Multi \n");
        }
        else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
        {
            Entity* draggedEntity = *(Entity**)payload->Data;
            std::string prefabPath;
            if (fs->type == FileType::Folder)
                prefabPath = fs->path + "/";
            else
                prefabPath = Path::GetFilePath(fs->path);
            prefabPath += draggedEntity->GetName();
            prefabPath += ".prefab";
            SceneManagerEditor::GetCurrentNode()->scene->SerializePrefab(prefabPath, draggedEntity);
            AddNode(FileType::Prefab, prefabPath);
        }

        ImGui::EndDragDropTarget();
    }

}

bool ContentBrowserPanelEditor::IsFileNodeSelected(FileNode* fs)
{
    return std::find(_selectionNodes.begin(), _selectionNodes.end(), fs) != _selectionNodes.end();
}

void ContentBrowserPanelEditor::HandleNodeSelection(FileNode* node, bool isCtrlDown)
{
    if (isCtrlDown)
    {
        if (IsFileNodeSelected(node))
        {
            DeselectEntity(node);
            if (_selectedNode == node && !_selectionNodes.empty())
            {
                _selectedNode = _selectionNodes.back();
            }
            else if (_selectedNode == node && _selectionNodes.empty())
            {
                _selectedNode = nullptr;
            }
        }
        else
        {
            _selectionNodes.push_back(node);
        }
    }
    else
    {
        SelectSingle();
    }

}

void ContentBrowserPanelEditor::DeselectEntity(FileNode* node)
{
    auto itr = std::find(_selectionNodes.begin(), _selectionNodes.end(), node);
    if (itr != _selectionNodes.end())
        _selectionNodes.erase(itr);
}

void ContentBrowserPanelEditor::SelectSingle()
{
    _selectionNodes.clear();
    _selectedNode = _hoveredNode;
    _selectionNodes.push_back(_selectedNode);
}

FileNode* ContentBrowserPanelEditor::GetFolderNode(FileNode* root, std::string path)
{
    if (root->type == FileType::Folder && Path::GetFilePath(path) == root->path + "/")
        return root;

    for (auto& go : root->children)
    {
        FileNode* temp = GetFolderNode(&go, path);
        if (temp != nullptr) return temp;
    }

    return nullptr;
}

void ContentBrowserPanelEditor::AddNode(FileType type, std::string path)
{
    FileNode* root = GetFolderNode(&_fileSystem, path);
    if (nullptr == root)
        return;

    FileNode node;
    node.type = type;
    node.path = path;
    node.name = Path::GetFileNameNoExtension(node.path);
    root->children.push_back(node);
}
