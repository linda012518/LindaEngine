#include "SliderInt.h"
#include "SliderFloat.h"
#include "SliderDouble.h"

LindaEditor::SliderInt::SliderInt(int p_min, int p_max, int p_value, SliderOrientation p_orientation, const std::string& p_label, const std::string& p_format)
	: SliderSingleScalar<int>(ImGuiDataType_::ImGuiDataType_S32, p_min, p_max, p_value, p_orientation, p_label, p_format)
{
}

LindaEditor::SliderFloat::SliderFloat(float p_min, float p_max, float p_value, SliderOrientation p_orientation, const std::string& p_label, const std::string& p_format)
	: SliderSingleScalar<float>(ImGuiDataType_::ImGuiDataType_Float, p_min, p_max, p_value, p_orientation, p_label, p_format)
{
}

LindaEditor::SliderDouble::SliderDouble(double p_min, double p_max, double p_value, SliderOrientation p_orientation, const std::string& p_label, const std::string& p_format)
	: SliderSingleScalar<double>(ImGuiDataType_::ImGuiDataType_Double, p_min, p_max, p_value, p_orientation, p_label, p_format)
{
}
