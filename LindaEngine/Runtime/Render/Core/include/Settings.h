#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

namespace LindaEngine
{
	enum class SortingCriteria
	{
        None = 0,						//不排序
        SortingLayer = 1,				//按层排序
        RenderQueue = 2,				//按材质的队列排序
        FarToNear = 4,					//按从远到近排序
        QuantizedFrontToBack = 8,		//按从近到远排序
        OptimizeStateChanges = 0x10,	//按材质排序，减少渲染状态切换
        CanvasOrder = 0x20,				//按UI画布排序
        RendererPriority = 0x40,		//按Renderer优先级排序
        CommonOpaque = 0x3B,			//不透明对象的典型排序
        CommonTransparent = 0x17		//透明对象的典型排序
	};

	struct SortingSettings
	{
		glm::vec3 cameraPosition;
		glm::vec3 cameraForward; //点乘算观察方向的投影距离
        SortingCriteria criteria;
	};

	struct DrawingSettings
	{
		SortingSettings sortSettings;
		std::vector<std::string> lightModes;
	};

	struct FilteringSettings
	{
		int layerMask;
		int minQueue;
		int maxQueue;
	};
}

