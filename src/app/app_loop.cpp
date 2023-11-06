#include "app.h"

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "channels/channel.h"

const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void App::main_loop()
{
    glfwPollEvents();
    handle_input();

    // update window size for render
    int window_w;
    int window_h;
    glfwGetWindowSize(m_window, &window_w, &window_h);
    int window_size[2] = {window_w,  window_h};
    m_render->set_window_size(window_size);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    {

        ImGui::Begin("Editor");

        // channel load
        if (ImGui::Button("load")){
            printf("Loading channels...\n");
            load_channels();
            printf("Finished loading channels\n");
        }
        ImGui::SameLine();
        if (ImGui::Button("store")){
            printf("Saving channels...\n");
            store_channels();
            printf("Finished saving channels\n");
        }
        ImGui::SameLine();
        if (ImGui::Button("reset")){
            printf("Resetting channels...\n");
            clear_channels();
            printf("Finished resetting channels\n");
        }

        ImGui::Separator();

        // start animation button
        static bool started = false;
        if(ImGui::Button(started? "Stop" : "Start")){
            started = !started;
            m_current_frame = 0;
        }
        ImGui::SameLine();
        if (started)
            m_current_frame += 1;
        if (ImGui::DragInt("frame", &m_current_frame)){
            if (m_current_frame < 0)
             m_current_frame = 0;
        }

        ImGui::Separator();
        ImGui::Checkbox("mouse cam", &m_mouse_cam);
        ImGui::Separator();

        // channel add
        ui_channel_add();

        ImGui::Separator();

        // show channels
        ui_channels();
        ImGui::End();
    }
    {
        ImGui::Begin("Info");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
}

void App::ui_channel_add()
{
    static int selected = 0;
    static char channel_name[80] = {'_', '\0'};
    if (selected >= 0 && ImGui::Button("Add"))
    {
        ChannelType type = channel_name_to_type(channel_type_to_string[selected]);
        Channel* ch = create_channel(type, channel_name);
        add_channel(type, ch);
    }
    if (selected >= 0)
        ImGui::SameLine();
    const char* pop_up_text = ((selected < 0? "None" : channel_type_to_string[selected]) + "##" + std::to_string(selected)).c_str();
    if (ImGui::Button(pop_up_text))
    {
        ImGui::OpenPopup("channel_add_popup");
    }
    ImGui::InputText("channel name", (char *)channel_name, 80);
    if (ImGui::BeginPopup("channel_add_popup"))
    {
        ImGui::Text("Choose channel");
        ImGui::Separator();
        for (unsigned int i = 0; i < channel_type_to_string.size(); i++)
            if (ImGui::Selectable((channel_type_to_string[i] + "##" + std::to_string(i)).c_str()))
                selected = i;
        ImGui::EndPopup();
    }
}
void App::ui_channels()
{
    static int selected = -1;
    // Left
    {
        ImGui::BeginChild("left pane", ImVec2(150, 0), true);

        ImGui::Separator();

        for (int i = 0; i < (int)m_channels.size(); ++i)
        {
            std::string id_string = "##" + std::to_string(i);
            ChannelData* data = m_channels[i];

            const char* value = (channel_type_to_string[data->type] + ": " + data->ch->get_name() + id_string).c_str();
            if (ImGui::Selectable(value, selected == i))
                selected = i;

            // enable position swaps
            if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
            {
                int i_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                if (i_next >= 0 && i_next < (int)m_channels.size())
                {
                    m_channels[i] = m_channels[i_next];
                    m_channels[i_next] = data;
                    selected = i_next;
                    ImGui::ResetMouseDragDelta();
                }
            }

            // draw channel
            data->ch->draw(m_current_frame, m_render);
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();

    // right
    if (selected >= 0)
    {
        ImGui::BeginChild("right pane", ImVec2(0, 0), true);
        {
            ChannelData* data = m_channels[selected];

            data->ch->draw_ui(m_current_frame, m_render);
        }
        ImGui::EndChild();
    }
}

void App::ui_channel(Channel *ch, bool *p_open, float &progress)
{
    if (ImGui::Begin(ch->get_name(), p_open, ImGuiWindowFlags_MenuBar))
    {
        ch->draw_ui(progress, m_render);
    }
    ImGui::End();
}