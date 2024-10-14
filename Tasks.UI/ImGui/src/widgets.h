#pragma once

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Window.h"
#include <print>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace tasks_widgets {
    [[maybe_unused]] static void menu_bar(const tasks_ui::Window& window) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4")) {
                    window.close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    [[maybe_unused]] static void debug_overlay(const tasks_ui::Window& window) {
        constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                                  ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                                  ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                                                  ImGuiWindowFlags_NoMove;
        constexpr float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        const ImVec2 work_pos = viewport->WorkPos;
        const ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = work_pos.x + work_size.x - PAD;
        window_pos.y = work_pos.y + PAD;
        window_pos_pivot.x = 1.0f;
        window_pos_pivot.y = 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Example: Simple overlay", reinterpret_cast<bool *>(true), window_flags)) {
            ImGui::Text("Debug Overlay");
            ImGui::Separator();

            const auto [width, height] = window.get_dimensions();
            ImGui::Text("Window Dimensions: (%d, %d)", width, height);
        }
        ImGui::End();
    }

    static bool transparent_button(const char* text, const float width, const float height = 0) {
        // Set the button style to have no background, border, or hover effect
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Transparent background
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0)); // No hover effect
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));  // No active effect
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0); // No border
        // Add other alignment if needed
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0,0.5f)); // Left Align

        ImGui::Button(text, ImVec2(width, height));

        // Restore original style settings
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(2);
        return ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    }

    struct TaskCheckboxResult {
        bool checkbox_pressed = false;
        bool x_pressed = false;
        bool save_pressed = false;
        bool edit_mode_entered = false; // True for one frame after edit mode is enabled
    };

    static TaskCheckboxResult task_checkbox(const int id, const std::string_view text, bool completed, bool& edit_mode, std::string* edit_text) {
        TaskCheckboxResult result;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.f, 0.f));

        ImGui::BeginGroup();
        {
            ImGui::PopStyleVar();

            const auto str_id = "##" + std::to_string(id);
            result.checkbox_pressed = ImGui::Checkbox((str_id + "Checkbox").c_str(), &completed);

            const float available_width = ImGui::GetContentRegionAvail().x;
            const auto frame_padding = ImGui::GetStyle().FramePadding;
            const auto item_spacing = ImGui::GetStyle().ItemSpacing;
            const auto checkbox_width = ImGui::GetItemRectSize().x;
            constexpr auto x_button_text = "x";
            const auto x_button_width = ImGui::CalcTextSize(x_button_text).x + (frame_padding.x * 2);

            if (!edit_mode) {
                ImGui::SameLine(0, 0);
                const auto text_width = available_width - checkbox_width - x_button_width;
                edit_mode = transparent_button((text.data() + str_id).c_str(), text_width);
                if (edit_mode) {
                    result.edit_mode_entered = true;
                }
                ImGui::SameLine(0, 0);
                result.x_pressed = ImGui::Button((x_button_text + str_id).c_str());
            } else {
                ImGui::SameLine();
                constexpr auto save_button_text = "save";
                const auto save_button_width = ImGui::CalcTextSize(save_button_text).x + (frame_padding.x * 2);
                const auto input_width = available_width - checkbox_width - (item_spacing.x * 3) - save_button_width - x_button_width;
                ImGui::PushItemWidth(input_width);
                ImGui::InputTextWithHint((str_id + text.data()).c_str(), "Task title", edit_text);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                result.save_pressed = ImGui::Button((save_button_text + str_id).c_str()); ImGui::SameLine();
                result.x_pressed = ImGui::Button((x_button_text + str_id).c_str());
            }

            ImGui::EndGroup();
        }

        if (ImGui::IsItemHovered()) {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 255, 50));
            draw_list->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 255, 20));
        }

        return result;
    }

    static void set_popup_open(const std::string_view popup_id) {
        ImGui::OpenPopup(popup_id.data());
    }

    static bool popup(const std::string_view popup_id, const std::string_view& text) {
        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        bool ok_pressed = false;
        if (ImGui::BeginPopupModal(popup_id.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("%s", text.data());
            ImGui::Separator();

            static bool dont_ask_me_next_time = false;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time); // Todo: Implement
            ImGui::PopStyleVar();

            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
                ok_pressed = true;
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        return ok_pressed;
    }

    static bool delete_confirm_popup(const bool open, const tasks::Task& task) {
        const std::string popup_id = std::format("Delete?##{}", std::to_string(task.id));
        if (open) {
            set_popup_open(popup_id);
        }

        constexpr std::string_view message = "Are you sure you want to delete task '{}' ?"
                "\nThis operation cannot be undone!";
        return popup(popup_id, std::format(message, task.title));
    }

    static bool creation_input(std::string* content) {
        ImGui::BeginGroup();

        constexpr std::string_view button_label = "Create";
        const float availableWidth = ImGui::GetContentRegionAvail().x;
        const ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
        const ImVec2 frame_padding = ImGui::GetStyle().FramePadding;
        const float button_width = ImGui::CalcTextSize(button_label.data()).x + (frame_padding.x * 2);
        const float input_width = availableWidth - button_width - window_padding.x;

        // When you click on create it should focus the input
        // When input is focused pressing enter should trigger the button
        ImGui::PushItemWidth(input_width); // https://github.com/ocornut/imgui/issues/3714#issuecomment-758952794
        ImGui::InputTextWithHint("##Task Name", "<press Enter to create a task>", content);
        ImGui::SetNextItemShortcut(ImGuiKey_Enter, ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_Repeat);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        const bool create_pressed = ImGui::Button(button_label.data());

        ImGui::EndGroup();
        return create_pressed;
    }
}
