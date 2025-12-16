#include "InputInt.h"
#include "InputFloat.h"
#include "InputDouble.h"

using namespace LindaEditor;

InputInt::InputInt(int p_defaultValue, int p_step, int p_fastStep, const std::string& p_label, const std::string& p_format, bool p_selectAllOnClick)
	: InputSingleScalar<int>(ImGuiDataType_::ImGuiDataType_S32, p_defaultValue, p_step, p_fastStep, p_label, p_format, p_selectAllOnClick)
{
}

InputFloat::InputFloat
(
	float p_defaultValue,
	float p_step,
	float p_fastStep,
	const std::string& p_label,
	const std::string& p_format,
	bool p_selectAllOnClick
)
	: InputSingleScalar<float>(ImGuiDataType_::ImGuiDataType_Float, p_defaultValue, p_step, p_fastStep, p_label, p_format, p_selectAllOnClick)
{

}

InputDouble::InputDouble
(
	double p_defaultValue,
	double p_step,
	double p_fastStep,
	const std::string& p_label,
	const std::string& p_format,
	bool p_selectAllOnClick
)
	: InputSingleScalar<double>(ImGuiDataType_::ImGuiDataType_Double, p_defaultValue, p_step, p_fastStep, p_label, p_format, p_selectAllOnClick)
{

}