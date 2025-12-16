#include "RadioButtonLinker.h"

#include "imgui/imgui.h"

using namespace LindaEditor;

RadioButtonLinker::RadioButtonLinker() : DataWidget<int>(_selected)
{
}

void RadioButtonLinker::Link(RadioButton& p_radioButton)
{
	p_radioButton._radioID = _availableRadioID++;
	auto listenerID = p_radioButton.ClickedEvent += std::bind(&RadioButtonLinker::OnRadioButtonClicked, this, std::placeholders::_1);

	if (p_radioButton.IsSelected() && _selected == -1)
		_selected = p_radioButton._radioID;

	_radioButtons.emplace_back(listenerID, std::ref(p_radioButton));
}

void RadioButtonLinker::Unlink(RadioButton& p_radioButton)
{
	auto it = std::find_if(_radioButtons.begin(), _radioButtons.end(), [&p_radioButton](std::pair<LindaEngine::ListenerID, std::reference_wrapper<RadioButton>>& p_pair)
	{
		return &p_pair.second.get() == &p_radioButton;
	});

	if (it != _radioButtons.end())
	{
		it->second.get().ClickedEvent.RemoveListener(it->first);
		_radioButtons.erase(it);
	}
}

int RadioButtonLinker::GetSelected() const
{
	return _selected;
}

void RadioButtonLinker::_Draw_Impl()
{
}

void RadioButtonLinker::OnRadioButtonClicked(int p_radioID)
{
	if (p_radioID != _selected)
	{
		_selected = p_radioID;
		ValueChangedEvent.Invoke(_selected);
		this->NotifyChange();

		for (const auto& [listener, radioButton] : _radioButtons)
			radioButton.get()._selected = radioButton.get()._radioID == _selected;
	}
}
