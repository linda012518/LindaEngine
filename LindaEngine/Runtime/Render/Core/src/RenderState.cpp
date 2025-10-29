#include "RenderState.h"

using namespace LindaEngine;

std::unordered_map<std::string, DepthFunc> RenderStateEnumMap::depthFuncMap = {
	{ "NEVER", DepthFunc::NEVER },
	{ "LESS", DepthFunc::LESS },
	{ "EQUAL", DepthFunc::EQUAL },
	{ "LEQUAL", DepthFunc::LEQUAL },
	{ "GREATER", DepthFunc::GREATER },
	{ "NOTEQUAL", DepthFunc::NOTEQUAL },
	{ "GEQUAL", DepthFunc::GEQUAL },
	{ "ALWAYS", DepthFunc::ALWAYS }
};

std::unordered_map<std::string, CullFaceFunc> RenderStateEnumMap::cullFaceFuncMap = {
	{ "None", CullFaceFunc::None },
	{ "FRONT", CullFaceFunc::FRONT },
	{ "FRONT_AND_BACK", CullFaceFunc::FRONT_AND_BACK },
	{ "BACK", CullFaceFunc::BACK }
};

std::unordered_map<std::string, PolygonMode> RenderStateEnumMap::polygonModeMap = {
	{ "POINT", PolygonMode::POINT },
	{ "LINE", PolygonMode::LINE },
	{ "FILL", PolygonMode::FILL }
};

std::unordered_map<std::string, BlendFunc> RenderStateEnumMap::blendFuncMap = {
	{ "ZERO", BlendFunc::ZERO },
	{ "ONE", BlendFunc::ONE },
	{ "SRC_COLOR", BlendFunc::SRC_COLOR },
	{ "ONE_MINUS_SRC_COLOR", BlendFunc::ONE_MINUS_SRC_COLOR },
	{ "DST_COLOR", BlendFunc::DST_COLOR },
	{ "ONE_MINUS_DST_COLOR", BlendFunc::ONE_MINUS_DST_COLOR },
	{ "SRC_ALPHA", BlendFunc::SRC_ALPHA },
	{ "ONE_MINUS_SRC_ALPHA", BlendFunc::ONE_MINUS_SRC_ALPHA },
	{ "DST_ALPHA", BlendFunc::DST_ALPHA },
	{ "ONE_MINUS_DST_ALPHA", BlendFunc::ONE_MINUS_DST_ALPHA },
	{ "CONSTANT_COLOR", BlendFunc::CONSTANT_COLOR },
	{ "ONE_MINUS_CONSTANT_COLOR", BlendFunc::ONE_MINUS_CONSTANT_COLOR },
	{ "CONSTANT_ALPHA", BlendFunc::CONSTANT_ALPHA },
	{ "ONE_MINUS_CONSTANT_ALPHA", BlendFunc::ONE_MINUS_CONSTANT_ALPHA }
};