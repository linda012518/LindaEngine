#pragma once

#include "Pluginable.h"
#include "WidgetContainer.h"

#include <string>

namespace LindaEditor
{
	class Panel : public Pluginable, public WidgetContainer
	{
	public:
		Panel();
		void Draw();
		const std::string& GetPanelID() const;

	protected:
		virtual void _Draw_Impl() = 0;

	public:
		bool enabled = true;

	protected:
		std::string _panelID;

	private:
		static uint64_t __PANEL_ID_INCREMENT;
	};

}

