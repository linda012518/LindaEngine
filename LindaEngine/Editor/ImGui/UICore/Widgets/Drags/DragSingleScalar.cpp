#include "DragInt.h"
#include "DragFloat.h"
#include "DragDouble.h"

using namespace LindaEditor;

DragInt::DragInt(int p_min, int p_max, int p_value, float p_speed,  const std::string& p_label, const std::string& p_format)
	: DragSingleScalar<int>(ImGuiDataType_::ImGuiDataType_S32, p_min, p_max, p_value, p_speed, p_label, p_format)
{

}

DragFloat::DragFloat(float p_min, float p_max, float p_value, float p_speed, const std::string& p_label, const std::string& p_format)
	: DragSingleScalar<float>(ImGuiDataType_::ImGuiDataType_Float, p_min, p_max, p_value, p_speed, p_label, p_format)
{

}

DragDouble::DragDouble(double p_min, double p_max, double p_value, float p_speed, const std::string& p_label, const std::string& p_format)
	: DragSingleScalar<double>(ImGuiDataType_::ImGuiDataType_Double, p_min, p_max, p_value, p_speed, p_label, p_format)
{

}