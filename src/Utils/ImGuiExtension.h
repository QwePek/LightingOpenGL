#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace ImGui
{
	bool Drag_3(const char* label, ImGuiDataType_ type, void* data_1, void* data_2, void* data_3, float v_speed, const void* min, const void* max, const char* format)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		bool value_changed = false;
		BeginGroup();
		PushID(label);
		PushMultiItemsWidths(3, CalcItemWidth());

		PushID(1);
		value_changed |= DragScalar("", type, data_1, v_speed, min, max, format);
		PopID();
		PopItemWidth();

		PushID(2);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		value_changed |= DragScalar("", type, data_2, v_speed, min, max, format);
		PopID();
		PopItemWidth();

		PushID(3);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		value_changed |= DragScalar("", type, data_3, v_speed, min, max, format);
		PopID();
		PopItemWidth();

		PopID();

		const char* label_end = FindRenderedTextEnd(label);
		if (label != label_end)
		{
			SameLine(0, g.Style.ItemInnerSpacing.x);
			TextEx(label, label_end);
		}
		EndGroup();
		return value_changed;
	}

	bool Drag_Float3(const char* data_1_str, const char* data_2_str, const char* data_3_str, float* data_1, float* data_2, float* data_3, 
		float data_1_min, float data_1_max, float data_2_min, float data_2_max, float data_3_min, float data_3_max, float v_speed)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		bool value_changed = false;
		BeginGroup();
		PushID(data_1_str);
		PushMultiItemsWidths(3, CalcItemWidth());

		const char* usedDatPtr_str = data_1_str;
		for (int i = 0; i < 3; i++)
		{
			PushID(i);
			if (i != 0)
				SameLine(0, g.Style.ItemInnerSpacing.x);

			switch (i)
			{
			case 0:
				value_changed |= DragFloat(data_1_str, data_1, v_speed, data_1_min, data_1_max);
				usedDatPtr_str = data_1_str;
				break;
			case 1:
				value_changed |= DragFloat(data_2_str, data_2, v_speed, data_2_min, data_2_max);
				usedDatPtr_str = data_2_str;
				break;
			case 2:
				value_changed |= DragFloat(data_3_str, data_3, v_speed, data_3_min, data_3_max);
				usedDatPtr_str = data_3_str;
				break;
			}

			PopID();
			PopItemWidth();
		}
		PopID();
		EndGroup();
		return value_changed;
	}
}