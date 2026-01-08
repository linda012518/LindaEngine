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

	struct FileSystem
	{
		std::string path;
		std::string name;
		FileType type = FileType::Other;
		std::vector<FileSystem> children;
	};

	class ContentBrowserPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(ContentBrowserPanelEditor)
		ContentBrowserPanelEditor();
		void OnImGuiRender();

	private:
		void DrawContent(FileSystem& fs);
		void DrawIcon(FileType type);
		void CollectFileFolder(FileSystem& fs);
		void ReloadResources();
		FileType CheckFileType(std::string fileName);

	private:
		bool _resDirty = true;
		FileSystem _fileSystem;
	};
}

