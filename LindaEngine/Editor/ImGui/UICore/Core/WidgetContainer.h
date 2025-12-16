#pragma once

#include <vector>

namespace LindaEditor
{
	class Widget;

	enum class MemoryMode
	{
		INTERNAL_MANAGMENT, EXTERNAL_MANAGMENT
	};

	class WidgetContainer
	{
	public:
		WidgetContainer() = default;

		virtual ~WidgetContainer();

		void RemoveWidget(Widget& p_widget);

		void RemoveAllWidgets();

		void ConsiderWidget(Widget& p_widget, bool p_manageMemory = true);

		void UnconsiderWidget(Widget& p_widget);

		void CollectGarbages();

		void DrawWidgets();

		void ReverseDrawOrder(bool reversed = true);

		template <typename T, typename ... Args>
		T& CreateWidget(Args&&... p_args)
		{
			_widgets.emplace_back(new T(p_args...), MemoryMode::INTERNAL_MANAGMENT);
			T& instance = *reinterpret_cast<T*>(_widgets.back().first);
			instance.SetParent(this);
			return instance;
		}

		std::vector<std::pair<Widget*, MemoryMode>>& GetWidgets();

	protected:
		std::vector<std::pair<Widget*, MemoryMode>> _widgets;
		bool _reversedDrawOrder = false;

	};
}
