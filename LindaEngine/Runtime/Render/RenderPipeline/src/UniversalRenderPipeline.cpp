#include "UniversalRenderPipeline.h"
#include "GraphicsContext.h"
#include "Settings.h"
#include "Graphic.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Texture.h"

#include <algorithm>

using namespace LindaEngine;

bool UniversalRenderPipeline::isFirstCamera = false;

int UniversalRenderPipeline::Initialize()
{
    RenderPipeline::Initialize();
    _urp.Initialize();

    return 0;
}

void UniversalRenderPipeline::Finalize()
{
    RenderPipeline::Finalize();
    _urp.Finalize();
}

void UniversalRenderPipeline::Render()
{
    //1 遍历相机
    //4 视锥剔除，排序
    //2 设置相机参数到shader
    //3 配置相机 HDR 后处理等等
    //5 设置清屏相关
    //6 渲染不透明物体
    //7 渲染天空
    //8 渲染透明物体

    const std::vector<Weak<Camera>> cameraList = CheckCameraList();

    isFirstCamera = true;
    for (auto& camera : cameraList)
    {
        Camera::currentRenderCamera = camera;

        SetupShaderParameters(camera);

        GraphicsConfig& config = GraphicsContext::graphicsConfig;
        int width = config.screenNewWidth;
        int height = config.screenNewHeight;
        if (nullptr != RenderTexture::finalRT)
        {
            width = RenderTexture::finalRT->width;
            height = RenderTexture::finalRT->height;
        }

        Ref<RenderTexture> rt = camera->GetRenderTarget();
        if (rt != nullptr)
        {
            width = rt->width;
            height = rt->height;
        }
        else
        {
            std::vector<FramebufferTextureSpecification> fts;

            FramebufferTextureSpecification color;
            color.colorFormat = camera->GetHDR() ? TextureFormat::RGBA32 : TextureFormat::RGBA8;
            fts.push_back(color);
            FramebufferTextureSpecification depth;
            depth.colorFormat = TextureFormat::Depth16;
            depth.filter = FilterMode::Point;
            depth.isRenderBuffer = true;
            fts.push_back(depth);

            rt = RenderTextureManager::Get(width, height, fts, camera->GetMSAA());
        }
        RenderTextureManager::SetRenderTarget(rt);
		glm::vec4& viewport = camera->GetViewport();
        Graphic::SetViewport((int)(viewport.x * width), (int)(viewport.y * height), (int)((viewport.z - viewport.x) * width), (int)((viewport.w - viewport.y) * height));
        glm::vec4& color = camera->GetClearColor();
        Graphic::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        switch (camera->GetClearType())
        {
        case CameraClearType::Skybox:
        case CameraClearType::SolidColor:
            Graphic::SetClearColor(color.r, color.g, color.b, color.a);
            Graphic::Clear(true, true, true);
            break;
        case CameraClearType::DepthOnly:
            Graphic::Clear(true, true, true);
            break;
        case CameraClearType::DontClear:
            Graphic::Clear(false, true, true);
            break;
        }

        _urp.Render(camera);
        isFirstCamera = false;
    }

    Camera::currentRenderCamera = nullptr;
}



