#pragma once

#include <stdint.h>
#include <iostream>

#include "GraphicsDriverAPI.h"
#include "Platform.h"

namespace LindaEngine 
{
	struct GraphicsConfig
	{
		/// Inline all-elements constructor.
		/// \param[in] r the red color depth in bits
		/// \param[in] g the green color depth in bits
		/// \param[in] b the blue color depth in bits
		/// \param[in] a the alpha color depth in bits
		/// \param[in] d the depth buffer depth in bits
		/// \param[in] s the stencil buffer depth in bits
		/// \param[in] msaa the msaa sample count
		/// \param[in] width the screen width in pixel
		/// \param[in] height the screen height in pixel
		GraphicsConfig(uint32_t r = 8, uint32_t g = 8, uint32_t b = 8, uint32_t a = 8,
			uint32_t d = 24, uint32_t s = 8, uint32_t msaa = 0,
			uint32_t width = 1920, uint32_t height = 1080, 
			const char* app_name = "LindaEngine", bool fullScreen_ = false,
			GraphicsDriverAPI api = GraphicsDriverAPI::None, Platform p = Platform::Windows) :
			redBits(r), greenBits(g), blueBits(b), alphaBits(a),
			depthBits(d), stencilBits(s), msaaSamples(msaa),
			screenWidth(width), screenHeight(height), screenNewWidth(width), screenNewHeight(height), 
			appName(app_name), fullScreen(fullScreen_), graphicsAPI(api), platformOS(p)
		{}

		uint32_t redBits; ///< red color channel depth in bits
		uint32_t greenBits; ///< green color channel depth in bits
		uint32_t blueBits; ///< blue color channel depth in bits
		uint32_t alphaBits; ///< alpha color channel depth in bits
		uint32_t depthBits; ///< depth buffer depth in bits
		uint32_t stencilBits; ///< stencil buffer depth in bits
		uint32_t msaaSamples; ///< MSAA samples
		uint32_t screenWidth;
		uint32_t screenHeight;
		const char* appName;
		uint32_t screenNewWidth; //win会重新匹配窗口
		uint32_t screenNewHeight; //win会重新匹配窗口
		GraphicsDriverAPI graphicsAPI;
		Platform platformOS;
		bool fullScreen;

		friend std::ostream& operator<<(std::ostream& out, const GraphicsConfig& conf)
		{
			out << "App Name:" << conf.appName << std::endl;
			out << "GfxConfiguration:" <<
				" R:" << conf.redBits <<
				" G:" << conf.greenBits <<
				" B:" << conf.blueBits <<
				" A:" << conf.alphaBits <<
				" D:" << conf.depthBits <<
				" S:" << conf.stencilBits <<
				" M:" << conf.msaaSamples <<
				" W:" << conf.screenWidth <<
				" H:" << conf.screenHeight <<
				std::endl;
			return out;
		}
	};

}

