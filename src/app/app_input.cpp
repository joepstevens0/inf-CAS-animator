#include "app.h"
#include "imgui.h"
#include <chrono>
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "channels/channel.h"

float scroll_value = 0;

void App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scroll_value = yoffset;
}

void App::handle_input()
{

    // camera controls
    if (m_mouse_cam){
        static float radius = 10.0f;
        static float rot[2] = {0,0};
        static bool first_click = true;
        static ImVec2 last_mouse_pos = {0,0};

        const float speed = 0.5;
        const float scroll_speed = 1;

        if (scroll_speed != 0)
            radius = glm::max(radius - scroll_value*scroll_speed, 0.01f);
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right)){
            ImVec2 pos = ImGui::GetMousePos();
            if (first_click){
                last_mouse_pos = pos;
                first_click = false;
            }

            rot[0] -= (pos.x - last_mouse_pos.x)*speed;
            rot[1] -= (pos.y - last_mouse_pos.y)*speed;
            rot[1] = glm::clamp(rot[1], -89.f, +89.f);
            last_mouse_pos = pos;
        } else {
            first_click = true;
        }

        glm::fquat q({glm::radians(rot[1]), glm::radians(rot[0]), 0});
        glm::mat4 m = glm::mat4_cast(q);

        glm::vec3 pos = m*glm::vec4(0,0,radius,1);
        glm::vec3 center = {0,0,0};
        glm::vec3 up = {0,1,0};
        glm::mat4 view = glm::lookAt(pos, center, up);
        m_render->cam()->set_view(view);
    }

    // left mouse events
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        ImVec2 t = ImGui::GetMousePos();
        int mouse_pos[2] = {(int)t.x, (int)t.y};
        for (unsigned int i = 0; i < m_channels.size();++i){
            m_channels[i]->ch->onclick(mouse_pos, m_render);
        }
    }
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        ImVec2 t = ImGui::GetMousePos();
        int mouse_pos[2] = {(int)t.x, (int)t.y};
        for (unsigned int i = 0; i < m_channels.size();++i){
            m_channels[i]->ch->onmousedown(mouse_pos, m_render);
        }
    }else
    {
        ImVec2 t = ImGui::GetMousePos();
        int mouse_pos[2] = {(int)t.x, (int)t.y};
        for (unsigned int i = 0; i < m_channels.size();++i){
            m_channels[i]->ch->onmouseup(mouse_pos, m_render);
        }
    }

    // // right mouse events
    // if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    // {
    //     ImVec2 mouse_pos = ImGui::GetMousePos();
    //     // printf("mouse move %d, %d\n", (int)mouse_pos.x, (int)mouse_pos.y);
    //     // m_cam->mousePress(mouse_pos.x, mouse_pos.y);
    //     // m_cam->mouseMove(mouse_pos.x, mouse_pos.y);
    // }
    // if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    // {
    //     // m_cam->mouseRelease();
    // }
    
    // // keyboard events
    // if (ImGui::IsKeyPressed(ImGuiKey_W))
    // {
    //     m_cam->keyPress(ImGuiKey_W);
    // }
    // if (ImGui::IsKeyPressed(ImGuiKey_S))
    //     m_cam->keyPress(ImGuiKey_S);
    // if (ImGui::IsKeyPressed(ImGuiKey_A))
    //     m_cam->keyPress(ImGuiKey_A);
    // if (ImGui::IsKeyPressed(ImGuiKey_D))
    //     m_cam->keyPress(ImGuiKey_D);
    // if (ImGui::IsKeyPressed(ImGuiKey_Space))
    //     m_cam->keyPress(ImGuiKey_Space);
    // if (ImGui::IsKeyPressed(ImGuiKey_LeftShift))
    //     m_cam->keyPress(ImGuiKey_LeftShift);

    // if (ImGui::IsKeyReleased(ImGuiKey_W))
    //     m_cam->keyRelease(ImGuiKey_W);
    // if (ImGui::IsKeyReleased(ImGuiKey_S))
    //     m_cam->keyRelease(ImGuiKey_S);
    // if (ImGui::IsKeyReleased(ImGuiKey_A))
    //     m_cam->keyRelease(ImGuiKey_A);
    // if (ImGui::IsKeyReleased(ImGuiKey_D))
    //     m_cam->keyRelease(ImGuiKey_D);
    // if (ImGui::IsKeyReleased(ImGuiKey_Space))
    //     m_cam->keyRelease(ImGuiKey_Space);
    // if (ImGui::IsKeyReleased(ImGuiKey_LeftShift))
    //     m_cam->keyRelease(ImGuiKey_LeftShift);

    // static auto last_render = std::chrono::system_clock::now();
    // auto current_time = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsed = current_time - last_render;
    // m_cam->update(elapsed.count());
    // last_render = current_time;

    scroll_value = 0;
}