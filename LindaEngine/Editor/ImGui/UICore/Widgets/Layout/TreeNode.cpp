#include "TreeNode.h"

#include "imgui/imgui.h"

using namespace LindaEditor;

TreeNode::TreeNode(const std::string& p_name, bool p_arrowClickToOpen) :
	DataWidget(name),
	name(p_name),
	_arrowClickToOpen(p_arrowClickToOpen)
{
	_autoExecutePlugins = false;
}

void TreeNode::Open()
{
	_shouldOpen = true;
	_shouldClose = false;
}

void TreeNode::Close()
{
	_shouldClose = true;
	_shouldOpen = false;
}

bool TreeNode::IsOpened() const
{
	return _opened;
}

void TreeNode::_Draw_Impl()
{
	bool prevOpened = _opened;

	if (_shouldOpen)
	{
		ImGui::SetNextItemOpen(true);
		_shouldOpen = false;
	}
	else if (_shouldClose)
	{
		ImGui::SetNextItemOpen(false);
		_shouldClose = false;
	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
	if (_arrowClickToOpen)  flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	if (selected)			flags |= ImGuiTreeNodeFlags_Selected;
	if (leaf)				flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx((name + _widgetID).c_str(), flags);

	if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
	{
		ClickedEvent.Invoke();

		if (ImGui::IsMouseDoubleClicked(0))
		{
			DoubleClickedEvent.Invoke();
		}
	}

	if (opened)
	{
		if (!prevOpened)
			OpenedEvent.Invoke();

		_opened = true;

		ExecutePlugins(PluginExecutionContext::WIDGET); // Manually execute plugins to make plugins considering the TreeNode and no childs

		DrawWidgets();

		ImGui::TreePop();
	}
	else
	{
		if (prevOpened)
			ClosedEvent.Invoke();

		_opened = false;

		ExecutePlugins(PluginExecutionContext::WIDGET); // Manually execute plugins to make plugins considering the TreeNode and no childs
	}

}
