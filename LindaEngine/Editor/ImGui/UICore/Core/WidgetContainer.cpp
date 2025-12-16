#include "WidgetContainer.h"
#include "Widget.h"

#include <algorithm>
#include <span>
#include <ranges>

using namespace LindaEditor;

WidgetContainer::~WidgetContainer()
{
	RemoveAllWidgets();
}

void WidgetContainer::RemoveWidget(Widget& p_widget)
{
	auto found = std::find_if(_widgets.begin(), _widgets.end(), [&p_widget](std::pair<Widget*, MemoryMode>& p_pair)
		{
			return p_pair.first == &p_widget;
		});

	if (found != _widgets.end())
	{
		if (found->second == MemoryMode::INTERNAL_MANAGMENT)
			delete found->first;

		_widgets.erase(found);
	}
}

void WidgetContainer::RemoveAllWidgets()
{
	std::for_each(_widgets.begin(), _widgets.end(), [](auto& pair)
	{
		if (pair.second == MemoryMode::INTERNAL_MANAGMENT)
			delete pair.first;
	});

	_widgets.clear();
}

void WidgetContainer::ConsiderWidget(Widget& p_widget, bool p_manageMemory)
{
	_widgets.emplace_back(std::make_pair(&p_widget, p_manageMemory ? MemoryMode::INTERNAL_MANAGMENT : MemoryMode::EXTERNAL_MANAGMENT));
	p_widget.SetParent(this);
}

void WidgetContainer::UnconsiderWidget(Widget& p_widget)
{
	auto found = std::find_if(_widgets.begin(), _widgets.end(), [&p_widget](std::pair<Widget*, MemoryMode>& p_pair)
	{
		return p_pair.first == &p_widget;
	});

	if (found != _widgets.end())
	{
		p_widget.SetParent(nullptr);
		_widgets.erase(found);
	}
}

void WidgetContainer::CollectGarbages()
{
	_widgets.erase(std::remove_if(_widgets.begin(), _widgets.end(), [](std::pair<Widget*, MemoryMode>& p_item)
	{
		bool toDestroy = p_item.first && p_item.first->IsDestroyed();

		if (toDestroy && p_item.second == MemoryMode::INTERNAL_MANAGMENT)
			delete p_item.first;

		return toDestroy;
	}), _widgets.end());
}

void WidgetContainer::DrawWidgets()
{
	CollectGarbages();

	using WidgetType = decltype(_widgets)::value_type::first_type;

	// We copy the widgets to draw in a separate vector to avoid issues when a widget is
	// added/destroyed during the draw process, which would invalidate the iterator.
	// This is especially useful to allow plugins to add and remove widgets during the draw process.
	std::vector<WidgetType> widgetsToDraw;
	widgetsToDraw.reserve(_widgets.size());
	std::ranges::copy(_widgets | std::views::keys, std::back_inserter(widgetsToDraw));

	if (_reversedDrawOrder) [[unlikely]]
	{
		for (WidgetType widget : widgetsToDraw | std::views::reverse)
		{
			widget->Draw();
		}
	}
	else
	{
		for (WidgetType widget : widgetsToDraw)
		{
			widget->Draw();
		}
	}
}

void WidgetContainer::ReverseDrawOrder(bool reversed)
{
	_reversedDrawOrder = reversed;
}

std::vector<std::pair<Widget*, MemoryMode>>& WidgetContainer::GetWidgets()
{
	return _widgets;
}
