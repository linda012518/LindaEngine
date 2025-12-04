#pragma once

#include "AutoPtr.h"
#include "ImGuiPanelEditor.h"
#include "Texture.h"

#include <string>
#include <vector>

namespace LindaEditor
{
	class EditViewPanelEditor;
	class PlayViewPanelEditor;

	class ImGuiContextEditor
	{
	public:
		int Initialize();
		void Finalize();

		void Begin();
		void End();

		void OnImGuiRender();

		void AddPanel();
		Ref<LindaEngine::RenderTexture> GetEditRenderTexture();
		Ref<LindaEngine::RenderTexture> GetPlayRenderTexture();

	private:
		std::vector<Ref<ImGuiPanelEditor>> _panels;
		Ref<EditViewPanelEditor> _editPanel;
		Ref<PlayViewPanelEditor> _playPanel;
	};
}
