#pragma once

#include "ImGuiPanelEditor.h"

#include <string>
#include <vector>

namespace LindaEditor
{
	enum class FileType
	{
		Folder, Scene, Material, FBX, Prefab, Texture, RenderTexture, Shader, ShaderLibrary, Text, Font, Other
	};

	struct FileNode
	{
		std::string path;
		std::string name;
		FileType type = FileType::Other;
		std::vector<FileNode> children;
	};

	class ContentBrowserPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(ContentBrowserPanelEditor)
		ContentBrowserPanelEditor();
		void OnImGuiRender();

	private:
		void DrawContent(FileNode& fs);
		void DrawIcon(FileType type, float offsetX);
		void CollectFileFolder(FileNode& fs);
		void SortFileFolder(FileNode& fs);
		void ReloadResources();
		FileType CheckFileType(std::string fileName);

		void DrawBlankAreaDropTarget();
		void DragNodes(FileNode* fs);
		bool IsFileNodeSelected(FileNode* fs);
		void HandleNodeSelection(FileNode* node, bool isCtrlDown);
		void DeselectEntity(FileNode* node);
		void SelectSingle();

		FileNode* GetFolderNode(FileNode* root, std::string path);
		void AddNode(FileType type, std::string path);

	private:
		bool _resDirty = true;
		bool _doubleClicked = false;
		FileNode _fileSystem;
		FileNode* _selectedNode;
		FileNode* _hoveredNode;
		std::vector<FileNode*> _selectionNodes;
		ImVec2 _mouseDownPos;
	};
}

