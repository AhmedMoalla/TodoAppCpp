#pragma once

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Window.h"
#include <print>

namespace todo_widgets {
    static void menu_bar(const todo_imgui::Window& window) {
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

    static void debug_overlay(const todo_imgui::Window& window) {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                                        ImGuiWindowFlags_NoMove;
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos;
        ImVec2 work_size = viewport->WorkSize;
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

            const auto window_size = window.get_dimensions();
            ImGui::Text("Window Dimensions: (%d, %d)", window_size.width, window_size.height);
        }
        ImGui::End();
    }

    struct TaskCheckboxResult {
        bool checkbox_pressed;
        bool delete_pressed;
    };

    static TaskCheckboxResult task_checkbox(const int id, const std::string_view text, bool completed) {
        ImGui::BeginGroup();
        const auto str_id = "##" + std::to_string(id);

        const bool checkbox_pressed = ImGui::Checkbox(str_id.c_str(), &completed);

        const float availableWidth = ImGui::GetContentRegionAvail().x;
        const auto text_size = ImGui::CalcTextSize(text.data());
        auto window_padding = ImGui::GetStyle().WindowPadding;

        ImGui::SameLine(0, window_padding.x);
        ImGui::Text(text.data());
        bool delete_pressed = false;
        constexpr auto button_width = 15;
        constexpr auto checkbox_size = 19;
        ImGui::SameLine(0, availableWidth - text_size.x - checkbox_size - button_width - window_padding.x);
        delete_pressed = ImGui::Button(("x" + str_id).c_str());
        ImGui::EndGroup();

        if (ImGui::IsItemHovered()) {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 255, 50));
            draw_list->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 255, 20));
        }

        return {checkbox_pressed, delete_pressed};
    }

    static void set_popup_open(const std::string_view popup_id) {
        ImGui::OpenPopup(popup_id.data());
    }

    static bool popup(const std::string_view popup_id, const std::string_view& text) {
        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        bool ok_pressed = false;
        if (ImGui::BeginPopupModal(popup_id.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text(text.data());
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

    static bool delete_confirm_popup(const bool open, const todo::Task& task) {
        const std::string popup_id = std::format("Delete?##{}", std::to_string(task.id));
        if (open) {
            set_popup_open(popup_id);
        }

        constexpr std::string_view message = "Are you sure you want to delete task '{}' ?"
                "\nThis operation cannot be undone!";
        return popup(popup_id, std::format(message, task.title));
    }

    static bool creation_input(std::string* content) {
        bool create_pressed = false;
        ImGui::BeginGroup();

        constexpr std::string_view button_label = "Create";
        const float availableWidth = ImGui::GetContentRegionAvail().x;
        const ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
        const ImVec2 frame_padding = ImGui::GetStyle().FramePadding;
        const float button_width = ImGui::CalcTextSize(button_label.data()).x + (frame_padding.x * 2);
        const float input_width = availableWidth - button_width - window_padding.x;

        ImGui::PushItemWidth(input_width); // https://github.com/ocornut/imgui/issues/3714#issuecomment-758952794
        ImGui::InputTextWithHint("##Task Name", "<press Enter to create a task>", content);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button(button_label.data())) {
            create_pressed = true;
        }

        ImGui::EndGroup();
        return create_pressed;
    }
}
