#pragma once

#include "Component.h"
#include "Frustum.h"
#include "Texture.h"
#include "PostProcessEffectRenderer.h"

#include "glm/glm.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace LindaEngine
{
	enum class CameraType
	{
		None, PerspectiveCamera, OrthoCamera, CubeCamera, DirectionalCamera, SpotCamera, PointCamera
	};

	enum class CameraClearType
	{
		Skybox, SolidColor, DepthOnly, DontClear
	};

	class Camera : public Component
	{
	public:
		Camera(Entity& entity, bool enable = true);
		virtual ~Camera();

		virtual void MakeViewMatrix();
		virtual void MakeProjectionMatrix() = 0;
		virtual void MakeViewProjectionMatrix();

		void Tick();
		void TransformDirty();

		const glm::mat4& GetViewMatrix() { return _viewMatrix; }
		const glm::mat4& GetProjectMatrix() { return _projectMatrix; }
		const glm::mat4& GetViewProjectMatrix() { return _viewProjectMatrix; }
		const glm::mat4& GetViewInverseMatrix() { return _viewInverseMatrix; }
		const glm::mat4& GetProjectInverseMatrix() { return _projectInverseMatrix; }
		const glm::mat4& GetViewProjectInverseMatrix() { return _viewProjectInverseMatrix; }

		float GetNear() { return _zNear; }
		float GetFar() { return _zFar; }

		CameraType& GetCameraType() { return _cameraType; }
		CameraClearType& GetClearType() { return _clearType; }
		int GetDepth() { return _depth; }
		Frustum& GetFrustum() { return _frustum; }
		int GetMSAA() { return _msaa; }
		int GetLayerMask() { return _layerMask; }
		bool GetHDR() { return _hdrEnable; }
		Ref<RenderTexture> GetRenderTarget() { return _renderTexture; }
		std::vector<Ref<PostProcessEffectRenderer>>& GetPostStack() { return _postStack; }
		bool HasPostProcess() { return _postStack.size() > 0; }
		glm::vec4& GetClearColor() { return _clearColor; }

		void AddPostProcess(Ref<PostProcessEffectRenderer> postProcess);
		Ref<PostProcessEffectRenderer> AddPostProcess(std::string postProcess);
		void SetCameraType(CameraType type) { _cameraType = type; }
		void SetClearType(CameraClearType type) { _clearType = type; }
		void SetDepth(int depth) { _depth = depth; }
		void SetNearFar(float near, float far, float dontCare = -1.0f);
		void SetMSAA(int msaa) { _msaa = msaa; }
		void SetLayerMask(int layer) { _layerMask = layer; }
		void SetHDR(bool hdr) { _hdrEnable = hdr; }
		void SetRenderTarget(Ref<RenderTexture> target);
		void SetClearColor(glm::vec4& color) { _clearColor = color; }

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		glm::vec3 ScreenToWorldPosition(glm::vec3& screenPos);

	protected:
		static CameraClearType GetClearTypeByString(std::string str);

	public:
		static Camera* currentRenderCamera;

	protected:
		glm::mat4 _viewMatrix;
		glm::mat4 _projectMatrix;
		glm::mat4 _viewProjectMatrix;

		glm::mat4 _viewInverseMatrix;
		glm::mat4 _projectInverseMatrix;
		glm::mat4 _viewProjectInverseMatrix;

		float _zNear;
		float _zFar;

		bool _viewDirty;
		bool _projectDirty;
		bool _renderTextureDirty;

		CameraType _cameraType;
		glm::vec4 _clearColor;
		CameraClearType _clearType;
		int _depth;
		Ref<RenderTexture> _renderTexture;
		bool _hdrEnable;
		int _msaa;
		int _layerMask;
		Frustum _frustum;
		std::vector<Ref<PostProcessEffectRenderer>> _postStack;
	};

	class PerspectiveCamera : public Camera
	{
		friend class CubeCamera;

	public:
		DECLARE_DYNAMIC_CREATE()
		PerspectiveCamera(Entity& entity, bool enable = true);
		virtual ~PerspectiveCamera();

		float GetAspectRatio() { return _aspectRatio; }
		float GetFOV() { return _fov; }

		virtual void MakeProjectionMatrix();

		void SetProjectionData(float fov, float ratio, float near, float far, float dontCare = -1.0f);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		void OnEvent(IEventHandler* sender, int eventCode, Event& eventData);

		void OnImguiRender();

	private:
		float _fov;
		float _aspectRatio;
	};

	class OrthoCamera : public Camera
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		OrthoCamera(Entity& entity, bool enable = true);
		virtual ~OrthoCamera();

		virtual void MakeProjectionMatrix();

		void SetProjectionData(float left, float right, float top, float bottom, float near, float far, float dontCare = -1.0f);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		void OnImguiRender();

	private:
		float _left;
		float _right;
		float _bottom;
		float _top;
	};

	class CubeCamera : public Camera
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		CubeCamera(Entity& entity, bool enable = true);
		virtual ~CubeCamera();

		virtual void MakeViewMatrix();
		virtual void MakeProjectionMatrix();
		virtual void MakeViewProjectionMatrix();

		glm::mat4& GetVPMatrix(int index);
		void SetProjectionData(float near, float far, float dontCare = -1.0f);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		glm::mat4 projectionMatrix;
		glm::mat4 cameraMatrixs[6];
	};

}
