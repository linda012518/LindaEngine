#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <string>

namespace LindaEngine
{
	struct AnimationPosePosition
	{
		glm::vec3 data;
		float timeStamp;
	};

	struct AnimationPoseRotation
	{
		glm::quat data;
		float timeStamp;
	};

	struct AnimationPoseScale
	{
		glm::vec3 data;
		float timeStamp;
	};

	struct BoneTrack
	{
		std::vector<AnimationPosePosition> tracksPosition;
		std::vector<AnimationPoseRotation> tracksRotation;
		std::vector<AnimationPoseScale> tracksScale;
		std::string name;

		glm::vec3 TickPosition(float time);
		glm::quat TickRotation(float time);
		glm::vec3 TickScale(float time);

	private:
		int GetTrackPositionIndex(float time);
		int GetTrackRotationIndex(float time);
		int GetTrackScaleIndex(float time);
		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	};

	struct AnimationClip
	{
		float duration;
		float ticksPerSecond;
		std::vector<BoneTrack> tracks;
		std::string name;
	};
}
