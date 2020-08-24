#include "imgui_extensions.h"
#include "imgui_internal.h"




/* Provide GUI slider with values according to interval steps */
bool ImGui::SliderIntWithSteps(const char* label, int* v, int v_min, int v_max, int v_step, const char* display_format)
{
    if (!display_format)
        display_format = "%d";

    int tmp_val = *v;
    bool value_changed = ImGui::SliderInt(label, &tmp_val, v_min, v_max, display_format);

    // Round the actual slider value to the cloasest bound interval
    if (v_step > 1)
        tmp_val -= (tmp_val - v_min) % v_step;
    *v = tmp_val;

    return value_changed;
}
bool ImGui::SeekSlider(const char* label, int* v, const char* display_format)
{
    return SliderInt(label, v, 0, 100, display_format);
}

void ImGui::SetContentRegionWidth(float x)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->ContentRegionRect.Max.x = x;
    window->Size.x = x;
}

bool ImGui::ButtonWithEnable(const char* label, bool enable, const ImVec2& size_arg) {
    if (!enable) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
    }
    auto res = ImGui::Button(label, size_arg);
    if (!enable) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    return res;
}

void ImGui::PushDisabled(bool disabled) {
    if (!disabled) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
    }
 }

void ImGui::PopDisabled(bool disabled) {
    if (!disabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
}