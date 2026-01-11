#include "AnimationClip.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace LindaEngine;

glm::vec3 AnimationClip::Tick(float time)
{
	for (auto& go : tracks)
	{
		go.TickPosition(time);
		go.TickRotation(time);
		go.TickScale(time);
	}
	return glm::vec3(1.0f);
}

glm::vec3 BoneTrack::TickPosition(float time)
{
	if (0 == tracksPosition.size())
		return glm::vec3(0.0f);
	if (1 == tracksPosition.size())
		return glm::vec3(tracksPosition[0].data);

	int p0Index = GetTrackPositionIndex(time);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(tracksPosition[p0Index].timeStamp, tracksPosition[p1Index].timeStamp, time);
	return glm::mix(tracksPosition[p0Index].data, tracksPosition[p1Index].data, scaleFactor);
}

glm::quat BoneTrack::TickRotation(float time)
{
	if (0 == tracksRotation.size())
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	if (1 == tracksRotation.size())
		return glm::normalize(tracksRotation[0].data);

	int p0Index = GetTrackRotationIndex(time);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(tracksRotation[p0Index].timeStamp, tracksRotation[p1Index].timeStamp, time);
	glm::quat finalRotation = glm::slerp(tracksRotation[p0Index].data, tracksRotation[p1Index].data, scaleFactor);
	return glm::normalize(finalRotation);
}

glm::vec3 BoneTrack::TickScale(float time)
{
	if (0 == tracksScale.size())
		return glm::vec3(1.0f);
	if (1 == tracksScale.size())
		return tracksScale[0].data;

	int p0Index = GetTrackScaleIndex(time);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(tracksScale[p0Index].timeStamp, tracksScale[p1Index].timeStamp, time);
	return glm::mix(tracksScale[p0Index].data, tracksScale[p1Index].data, scaleFactor);
}

int BoneTrack::GetTrackPositionIndex(float time)
{
	size_t len = tracksPosition.size();
	for (int index = 0; index < len - 1; ++index)
	{
		if (time < tracksPosition[index + 1].timeStamp)
			return index;
	}
	return 0;
}

int BoneTrack::GetTrackRotationIndex(float time)
{
	size_t len = tracksRotation.size();
	for (int index = 0; index < len - 1; ++index)
	{
		if (time < tracksRotation[index + 1].timeStamp)
			return index;
	}
	return 0;
}

int BoneTrack::GetTrackScaleIndex(float time)
{
	size_t len = tracksScale.size();
	for (int index = 0; index < len - 1; ++index)
	{
		if (time < tracksScale[index + 1].timeStamp)
			return index;
	}
	return 0;
}

float BoneTrack::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

