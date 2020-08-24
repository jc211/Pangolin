#pragma once
#include "imgui.h"

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)
#define ImGui_ScopePushFont(f) ImGui::ScopePushFont CONCAT(scope_push_font, __LINE__) (f)
#define ImGui_ScopePushStyleColor(idx, col) ImGui::ScopePushStyleColor CONCAT(scope_push_style_color, __LINE__) (idx, col)
#define ImGui_ScopePushStyleVar(idx, val) ImGui::ScopePushStyleVar CONCAT(scope_push_style_var, __LINE__) (idx, val)

namespace ImGui {
	IMGUI_API void          SetContentRegionWidth(float y);
	class ScopePushFont
	{
	public:
		ScopePushFont(ImFont* font) { PushFont(font); }
		~ScopePushFont() { PopFont(); }
	};

	class ScopePushStyleColor
	{
	public:
		ScopePushStyleColor(ImGuiCol idx, const ImVec4& col) { PushStyleColor(idx, col); }
		~ScopePushStyleColor() { PopStyleColor(); }
	};
	;
	class ScopePushStyleVar
	{
	public:
		ScopePushStyleVar(ImGuiStyleVar idx, float val) { PushStyleVar(idx, val); }
		ScopePushStyleVar(ImGuiStyleVar idx, const ImVec2& val) { PushStyleVar(idx, val); }
		~ScopePushStyleVar() { PopStyleVar(); }
	};

	/* Implement slider with increments other than 1. The implementation is a variation on
	https://github.com/ocornut/imgui/issues/1183 */
	IMGUI_API bool          SliderIntWithSteps(const char* label, int* v, int v_min, int v_max, int v_step = 1, const char* display_format = "%.3f");

	bool SeekSlider(const char* label, int* v, const char* display_format);

	IMGUI_API bool ButtonWithEnable(const char* label, bool enable = true, const ImVec2& size = ImVec2(0, 0));

	IMGUI_API void PushDisabled(bool disabled = true);
	IMGUI_API void PopDisabled(bool disabled = true);
}

