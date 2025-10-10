#pragma once

namespace LindaEngine
{
	enum class DepthFunc
	{
		NEVER		= 0x0200,		//永远不通过深度测试
		LESS		= 0x0201,		//在片段深度值小于缓冲的深度值时通过测试
		EQUAL		= 0x0202,		//在片段深度值等于缓冲区的深度值时通过测试
		LEQUAL		= 0x0203,		//在片段深度值小于等于缓冲区的深度值时通过测试
		GREATER		= 0x0204,		//在片段深度值大于缓冲区的深度值时通过测试
		NOTEQUAL	= 0x0205,		//在片段深度值不等于缓冲区的深度值时通过测试
		GEQUAL		= 0x0206,		//在片段深度值大于等于缓冲区的深度值时通过测试
		ALWAYS		= 0x0207,		//永远通过深度测试
	};

	enum class CullFaceFunc
	{
		None			= 0xFFFF,	//双面显示
		BACK			= 0x0405,	//剔除背面
		FRONT			= 0x0404,	//剔除正面
		FRONT_AND_BACK	= 0x0408,	//剔除正面和背面
	};

	enum class PolygonMode
	{
		POINT = 0x1B00, LINE = 0x1B01, FILL = 0x1B02
	};

	enum class ShadeModel
	{
		SMOOTH = 0x1D01, FLAT = 0x1D00 //glShadeModel
	};

	enum class BlendFunc
	{
		ZERO						= 0,			// Factor = 0
		ONE							= 1,			// Factor = 1
		SRC_COLOR					= 0x0300,		// Factor = SourceColor
		ONE_MINUS_SRC_COLOR			= 0x0301,		// Factor = 1-SourceColor
		DST_COLOR					= 0x0306,		// Factor = TargetColor
		ONE_MINUS_DST_COLOR			= 0x0307,		// Factor = 1-TargetColor
		SRC_ALPHA					= 0x0302,		// Factor = SourceColor.alpha
		ONE_MINUS_SRC_ALPHA			= 0x0303,		// Factor = 1-SourceColor.alpha
		DST_ALPHA					= 0x0304,		// Factor = TargetColor.alpha
		ONE_MINUS_DST_ALPHA			= 0x0305,		// Factor = 1-TargetColor.alpha
		CONSTANT_COLOR				= 0x8001,		// Factor = ConstantColor  通过glBlendColor设置
		ONE_MINUS_CONSTANT_COLOR	= 0x8002,		// Factor = 1-ConstantColor
		CONSTANT_ALPHA				= 0x8003,		// Factor = ConstantColor.alpha
		ONE_MINUS_CONSTANT_ALPHA	= 0x8004,		// Factor = 1ConstantColor.alpha
	};

	enum class BlendEquation
	{
		FUNC_ADD				= 0x8006, //默认选项 相加 SourceColor + TargetColor
		FUNC_SUBTRACT			= 0x800A, // SourceColor - TargetColor
		FUNC_REVERSE_SUBTRACT	= 0x800B, // TargetColor - SourceColor
		MIN						= 0x8007, // min(SourceColor, TargetColor)
		MAX						= 0x8008, // max(SourceColor, TargetColor)
	};

	enum class StencilOp
	{
		KEEP		= 0x1E00,	//保持当前储存的模板值，默认
		ZERO		= 0,		//将模板值设置为0
		REPLACE		= 0x1E01,	//将模板值设置为glStencilFunc函数设置的ref值
		INCR		= 0x1E02,	//如果模板值小于最大值则将模板值加1
		INCR_WRAP	= 0x8507,	//与GL_INCR一样，但如果模板值超过了最大值则归零
		DECR		= 0x1E03,	//如果模板值大于最小值则将模板值减1
		DECR_WRAP	= 0x8508,	//与GL_DECR一样，但如果模板值小于0则将其设置为最大值
		INVERT		= 0x150A,	//按位翻转当前的模板缓冲值
	};

	enum class StencilFunc
	{
		NEVER		= 0x0200,
		LESS		= 0x0201,
		LEQUAL		= 0x0203,
		GREATER		= 0x0204,
		GEQUAL		= 0x0206,
		EQUAL		= 0x0202,
		NOTEQUAL	= 0x0205,
		ALWAYS		= 0x0207,
	};

	struct Color
	{
		float r; float g; float b; float a;

		friend bool operator == (const Color& a, const Color& b)
		{
			return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
		}

		friend bool operator != (const Color& a, const Color& b)
		{
			return a.r != b.r && a.g != b.g && a.b != b.b && a.a != b.a;
		}
	};

	struct ColorMask
	{
		bool R = true; bool G = true; bool B = true; bool A = true;

		friend bool operator == (const ColorMask& a, const ColorMask& b)
		{
			return a.R == b.R && a.G == b.G && a.B == b.B && a.A == b.A;
		}

		friend bool operator != (const ColorMask& a, const ColorMask& b)
		{
			return a.R != b.R && a.G != b.G && a.B != b.B && a.A != b.A;
		}
	};

	struct DepthState
	{
		bool depthTest = true; //是否开启深度测试
		bool depthWrite = true; //是否开启深度写入
		DepthFunc depthFunc = DepthFunc::LESS; //深度测试比较运算符		默认情况下使用的深度函数是GL_LESS
	};

	struct PolygonModeState
	{
		PolygonMode polygonMode = PolygonMode::FILL; //渲染模式 点/线/面
		bool smoothLine = false;
		float lineWidth = 1.0f;
		bool smoothPoint = false;
		float pointSize = 1.0f;
	};

	struct StencilState
	{
		bool stencilTest = false; //是否开启模版测试
		int stencilWriteMask = 0xFF; //是否开启模版写入
		StencilOp stencilFail = StencilOp::KEEP; //模板测试失败时采取的行为
		StencilOp stencilZFail = StencilOp::KEEP; //模板测试通过，但深度测试失败时采取的行为
		StencilOp stencilZPass = StencilOp::KEEP; //模板测试和深度测试都通过时采取的行为
		StencilFunc stencilFunc = StencilFunc::ALWAYS; //模板测试时，系统会将缓冲区的模板值与ref值先进行掩码运算，再根据func指定的逻辑进行比较判断
		int stencilRef = 1; //ref和模版缓冲在测试比较之前分别和mask进行按位与运算，然后再比较
		int stencilFuncMask = 0xFF;
	};

	struct BlendState
	{
		bool enableBlend = false; //是否开启透明混合  //SourceFactor * SourceColor + targetFactor * TargetColor
		bool enableBlendSeparate = false;
		BlendFunc blendSrc = BlendFunc::SRC_ALPHA; //源因子
		BlendFunc blendDis = BlendFunc::ONE_MINUS_SRC_ALPHA; //目标因子
		BlendEquation blendEqua = BlendEquation::FUNC_ADD; //混合方程
		BlendFunc blendSrcAlpha = BlendFunc::SRC_ALPHA; //glBlendFuncSeparate 分别设置时的源alpha因子
		BlendFunc blendDisAlpha = BlendFunc::ONE_MINUS_SRC_ALPHA; //glBlendFuncSeparate 分别设置时的目标alpha因子
		Color constantColor; //CONSTANT_COLOR  通过glBlendColor设置
	};

	struct RenderState
	{
		ColorMask colorMask; //渲染遮罩

		DepthState depthState;

		CullFaceFunc cullFaceFunc = CullFaceFunc::BACK; //剔除方式

		PolygonModeState polygonModeState;

		ShadeModel shadeModel = ShadeModel::SMOOTH; //插值还是单色

		StencilState stencilState;

		BlendState blendState;
	};
}
