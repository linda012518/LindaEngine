#pragma once

#include "Widget.h"
#include "DataDispatcher.h"

namespace LindaEditor
{
	template <typename T>
	class DataWidget : public Widget
	{
	public:
		DataWidget(T& p_dataHolder) : m_data(p_dataHolder) {};

		virtual void Draw();

		void NotifyChange();

	private:
		T& m_data;
	};

	template<typename T>
	inline void DataWidget<T>::Draw()
	{
		if (enabled)
		{
			TRY_GATHER(T, m_data);
			Widget::Draw();
			TRY_PROVIDE(T, m_data);
		}
	}

	template<typename T>
	inline void DataWidget<T>::NotifyChange()
	{
		TRY_NOTIFY_CHANGE(T);
	}
}
