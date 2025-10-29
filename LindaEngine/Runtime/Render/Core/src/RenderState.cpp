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

std::unordered_map<std::string, BlendEquation> RenderStateEnumMap::blendEquationMap = {
	{ "FUNC_ADD", BlendEquation::FUNC_ADD },
	{ "FUNC_SUBTRACT", BlendEquation::FUNC_SUBTRACT },
	{ "FUNC_REVERSE_SUBTRACT", BlendEquation::FUNC_REVERSE_SUBTRACT },
	{ "MIN", BlendEquation::MIN },
	{ "MAX", BlendEquation::MAX }
};

std::unordered_map<std::string, StencilOp> RenderStateEnumMap::stencilOpMap = {
	{ "KEEP", StencilOp::KEEP },
	{ "ZERO", StencilOp::ZERO },
	{ "REPLACE", StencilOp::REPLACE },
	{ "INCR", StencilOp::INCR },
	{ "DECR", StencilOp::DECR },
	{ "DECR_WRAP", StencilOp::DECR_WRAP },
	{ "INVERT", StencilOp::INVERT },
	{ "INCR_WRAP", StencilOp::INCR_WRAP }
};

std::unordered_map<std::string, StencilFunc> RenderStateEnumMap::stencilFuncMap = {
	{ "NEVER", StencilFunc::NEVER },
	{ "LESS", StencilFunc::LESS },
	{ "LEQUAL", StencilFunc::LEQUAL },
	{ "GREATER", StencilFunc::GREATER },
	{ "GEQUAL", StencilFunc::GEQUAL },
	{ "EQUAL", StencilFunc::EQUAL },
	{ "NOTEQUAL", StencilFunc::NOTEQUAL },
	{ "ALWAYS", StencilFunc::ALWAYS }
};
