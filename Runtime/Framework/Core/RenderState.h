#pragma once

namespace LindaEngine
{
	enum class DepthFunc
	{
		NEVER		= 0x0200,		//��Զ��ͨ����Ȳ���
		LESS		= 0x0201,		//��Ƭ�����ֵС�ڻ�������ֵʱͨ������
		EQUAL		= 0x0202,		//��Ƭ�����ֵ���ڻ����������ֵʱͨ������
		LEQUAL		= 0x0203,		//��Ƭ�����ֵС�ڵ��ڻ����������ֵʱͨ������
		GREATER		= 0x0204,		//��Ƭ�����ֵ���ڻ����������ֵʱͨ������
		NOTEQUAL	= 0x0205,		//��Ƭ�����ֵ�����ڻ����������ֵʱͨ������
		GEQUAL		= 0x0206,		//��Ƭ�����ֵ���ڵ��ڻ����������ֵʱͨ������
		ALWAYS		= 0x0207,		//��Զͨ����Ȳ���
	};

	enum class CullFaceFunc
	{
		None			= 0xFFFF,	//˫����ʾ
		BACK			= 0x0405,	//�޳�����
		FRONT			= 0x0404,	//�޳�����
		FRONT_AND_BACK	= 0x0408,	//�޳�����ͱ���
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
		CONSTANT_COLOR				= 0x8001,		// Factor = ConstantColor  ͨ��glBlendColor����
		ONE_MINUS_CONSTANT_COLOR	= 0x8002,		// Factor = 1-ConstantColor
		CONSTANT_ALPHA				= 0x8003,		// Factor = ConstantColor.alpha
		ONE_MINUS_CONSTANT_ALPHA	= 0x8004,		// Factor = 1ConstantColor.alpha
	};

	enum class BlendEquation
	{
		FUNC_ADD				= 0x8006, //Ĭ��ѡ�� ��� SourceColor + TargetColor
		FUNC_SUBTRACT			= 0x800A, // SourceColor - TargetColor
		FUNC_REVERSE_SUBTRACT	= 0x800B, // TargetColor - SourceColor
		MIN						= 0x8007, // min(SourceColor, TargetColor)
		MAX						= 0x8008, // max(SourceColor, TargetColor)
	};

	enum class StencilOp
	{
		KEEP		= 0x1E00,	//���ֵ�ǰ�����ģ��ֵ��Ĭ��
		ZERO		= 0,		//��ģ��ֵ����Ϊ0
		REPLACE		= 0x1E01,	//��ģ��ֵ����ΪglStencilFunc�������õ�refֵ
		INCR		= 0x1E02,	//���ģ��ֵС�����ֵ��ģ��ֵ��1
		INCR_WRAP	= 0x8507,	//��GL_INCRһ���������ģ��ֵ���������ֵ�����
		DECR		= 0x1E03,	//���ģ��ֵ������Сֵ��ģ��ֵ��1
		DECR_WRAP	= 0x8508,	//��GL_DECRһ���������ģ��ֵС��0��������Ϊ���ֵ
		INVERT		= 0x150A,	//��λ��ת��ǰ��ģ�建��ֵ
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
		bool depthTest = true; //�Ƿ�����Ȳ���
		bool depthWrite = true; //�Ƿ������д��
		DepthFunc depthFunc = DepthFunc::LESS; //��Ȳ��ԱȽ������		Ĭ�������ʹ�õ���Ⱥ�����GL_LESS
	};

	struct PolygonModeState
	{
		PolygonMode polygonMode = PolygonMode::FILL; //��Ⱦģʽ ��/��/��
		bool smoothLine = false;
		float lineWidth = 1.0f;
		bool smoothPoint = false;
		float pointSize = 1.0f;
	};

	struct StencilState
	{
		bool stencilTest = false; //�Ƿ���ģ�����
		int stencilWriteMask = 0xFF; //�Ƿ���ģ��д��
		StencilOp stencilFail = StencilOp::KEEP; //ģ�����ʧ��ʱ��ȡ����Ϊ
		StencilOp stencilZFail = StencilOp::KEEP; //ģ�����ͨ��������Ȳ���ʧ��ʱ��ȡ����Ϊ
		StencilOp stencilZPass = StencilOp::KEEP; //ģ����Ժ���Ȳ��Զ�ͨ��ʱ��ȡ����Ϊ
		StencilFunc stencilFunc = StencilFunc::ALWAYS; //ģ�����ʱ��ϵͳ�Ὣ��������ģ��ֵ��refֵ�Ƚ����������㣬�ٸ���funcָ�����߼����бȽ��ж�
		int stencilRef = 1; //ref��ģ�滺���ڲ��ԱȽ�֮ǰ�ֱ��mask���а�λ�����㣬Ȼ���ٱȽ�
		int stencilFuncMask = 0xFF;
	};

	struct BlendState
	{
		bool enableBlend = false; //�Ƿ���͸�����  //SourceFactor * SourceColor + targetFactor * TargetColor
		bool enableBlendSeparate = false;
		BlendFunc blendSrc = BlendFunc::SRC_ALPHA; //Դ����
		BlendFunc blendDis = BlendFunc::ONE_MINUS_SRC_ALPHA; //Ŀ������
		BlendEquation blendEqua = BlendEquation::FUNC_ADD; //��Ϸ���
		BlendFunc blendSrcAlpha = BlendFunc::SRC_ALPHA; //glBlendFuncSeparate �ֱ�����ʱ��Դalpha����
		BlendFunc blendDisAlpha = BlendFunc::ONE_MINUS_SRC_ALPHA; //glBlendFuncSeparate �ֱ�����ʱ��Ŀ��alpha����
		Color constantColor; //CONSTANT_COLOR  ͨ��glBlendColor����
	};

	struct RenderState
	{
		ColorMask colorMask; //��Ⱦ����

		DepthState depthState;

		CullFaceFunc cullFaceFunc = CullFaceFunc::BACK; //�޳���ʽ

		PolygonModeState polygonModeState;

		ShadeModel shadeModel = ShadeModel::SMOOTH; //��ֵ���ǵ�ɫ

		StencilState stencilState;

		BlendState blendState;
	};
}
