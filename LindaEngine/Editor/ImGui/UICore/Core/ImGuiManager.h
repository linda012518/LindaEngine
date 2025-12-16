#pragma once

#include "AutoPtr.h"
#include "UIStyle.h"
#include "ImGuiPanelEditor.h"

#include "imgui/imgui.h"

#include <unordered_map>
#include <string>

namespace LindaEditor
{
	class ImGuiManager
	{
	public:
		int Initialize();
		void Finalize();

		void OnImGuiRender();

		void AddPanel();

		void ApplyStyle(Style style);
		bool LoadFont(const std::string& p_path, float p_fontSize);
		bool UnloadFont(const std::string& p_path);
		bool UseFont(const std::string& p_path);
		void UseDefaultFont();
		void EnableEditorLayoutSave(bool p_value);
		bool IsEditorLayoutSaveEnabled() const;
		void SetEditorLayoutSaveFilename(const std::string& p_filename);
		void SetEditorLayoutAutosaveFrequency(float p_frequency);
		float GetEditorLayoutAutosaveFrequency(float p_frequeny);
		void EnableDocking(bool p_value);
		void ResetLayout(const std::string& p_config) const;
		bool IsDockingEnabled() const;

	private:
		std::unordered_map<std::string, ImFont*> m_fonts;
		std::string _layoutSaveFilename = "Temporary/imgui.ini";
		bool _dockingState;
		std::vector<Ref<ImGuiPanelEditor>> _panels;
	};
}
