#pragma once

namespace LindaEngine
{
    class Camera;

    enum class RenderPassEvent
    {
        BeforeRendering = 0,
        BeforeRenderingShadows = 50,
        AfterRenderingShadows = 100,
        BeforeRenderingGbuffer = 210,
        AfterRenderingGbuffer = 220,
        BeforeRenderingDeferredLights = 230,
        AfterRenderingDeferredLights = 240,
        BeforeRenderingOpaques = 250,
        AfterRenderingOpaques = 300,
        BeforeRenderingSkybox = 350,
        AfterRenderingSkybox = 400,
        BeforeRenderingTransparents = 450,
        AfterRenderingTransparents = 500,
        BeforeRenderingPostProcessing = 550,
        AfterRenderingPostProcessing = 600,
        AfterRendering = 1000,
    };

	class ScriptablePass
	{
    public:
        virtual ~ScriptablePass() = default;
        virtual void Render(Camera* camera) = 0;

        RenderPassEvent& GetRenderPassEvent() { return _renderPassEvent; }

        friend bool operator < (const ScriptablePass& a, const ScriptablePass& b)
        {
            return a._renderPassEvent < b._renderPassEvent;
        }

        friend bool operator > (const ScriptablePass& a, const ScriptablePass& b)
        {
            return a._renderPassEvent > b._renderPassEvent;
        }

    protected:
        RenderPassEvent _renderPassEvent;
	};
}

