#include "virtual_camera.h"
#include "imgui.h"

#include "render/camera.h"
#include <glm/gtx/spline.hpp>
#include <chrono>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/matrix_transform.hpp>

VirtualCamera::VirtualCamera(std::string channel_name)
    : m_channel_name{channel_name}
{
}

void VirtualCamera::draw_ui(int frame, Render *render)
{
    ImGui::Checkbox("enabled", &m_enabled);
    ImGui::Separator();

    // Left
    {
        ImGui::BeginChild("left pane", ImVec2(150, 0), true);

        // add frame button
        ui_frame_add_button(render);

        ImGui::Separator();

        for (auto it = m_keyframes.begin(); it != m_keyframes.end(); ++it)
        {
            int frame = it->first;
            if (ImGui::Selectable(("keyframe " +  std::to_string(frame) + "## " + std::to_string(frame)).c_str(), m_selected == frame))
                m_selected = frame;
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();

    // right
    if (m_selected >= 0)
    {
        ImGui::BeginChild("right pane", ImVec2(0, 0), true);
        {
            KeyFrame &kf = m_keyframes[m_selected];

            // camera position
            ImGui::DragFloat3("Camera position", &kf.pos[0], 0.01);
            ImGui::DragFloat3("Camera rotation", &kf.rot[0], 1);

            // camera animation
            if (m_selected > 0){
                ImGui::Checkbox("Ease in", &kf.easeIn);
                ImGui::Checkbox("Ease out", &kf.easeOut);
                ImGui::DragFloat3("Control 1", &kf.control1[0], 0.1);
                ImGui::DragFloat3("Control 2", &kf.control2[0], 0.1);
            }
            ImGui::Separator();
            if (ImGui::Button(("delete##" + std::to_string(m_selected)).c_str())){
                m_keyframes.erase(m_selected);
                m_selected = -1;
            }
        }
        ImGui::EndChild();
    }
}

void VirtualCamera::ui_frame_add_button(Render *render){
    static int frame_number = 0;
    if (ImGui::DragInt("frame number", &frame_number)){
        if (frame_number < 0) frame_number = 0;
    }
    // draw button if frame not in keyframes
    if (m_keyframes.count(frame_number) == 0 && ImGui::Button("Add frame"))
    {
        KeyFrame kf;
        if (m_selected >= 0)
        {
            // copy values of selected frame to new one
            kf.pos = m_keyframes[m_selected].pos;
            kf.rot = m_keyframes[m_selected].rot;
        }
        else
        {
            // initialize keyframe values
            kf.pos = {0,0,0};
            kf.rot = {0,0,0};
        }
        m_keyframes[frame_number] = kf;

        // set new frame as selected
        m_selected = frame_number;
    }
}
void VirtualCamera::draw(int frame, Render *render)
{
    if (m_enabled){
        // determine camera position
        if (m_keyframes.size() > 0)
        {
            // find inbetween wich frames
            int first_frame =  m_keyframes.begin()->first;
            int second_frame = (--m_keyframes.end())->first;
            for (auto it = m_keyframes.begin(); it != m_keyframes.end(); ++it){
                int f = it->first;
                if (f > first_frame && f <= frame) first_frame = f;
                if (f < second_frame && f >= frame) second_frame = f;
            }

            KeyFrame first_frame_kf = m_keyframes[first_frame];
            KeyFrame second_frame_kf = m_keyframes[second_frame];

            glm::vec3 last_pos = first_frame_kf.pos;
            glm::vec3 next_pos = second_frame_kf.pos;
            glm::vec3 c1 = second_frame_kf.control1;
            glm::vec3 c2 = second_frame_kf.control2;
            float u = glm::clamp((float)(frame - first_frame)/(float)(second_frame - first_frame), 0.f, 1.f);
            if (first_frame == second_frame) u = 0;

            // speed control
            u = speed_control(u, second_frame_kf.easeIn, second_frame_kf.easeOut);

            glm::vec3 cam_pos = spline_hermite(u, last_pos, next_pos, c1, c2);
            glm::fquat last_rot = glm::fquat{glm::radians(first_frame_kf.rot)};
            glm::fquat next_rot = glm::fquat{glm::radians(second_frame_kf.rot)};
            glm::fquat cam_rot = linear(u, last_rot, next_rot);
            glm::mat4 view = glm::mat4(cam_rot);
            view = glm::translate(view, cam_pos);
            render->cam()->set_view(view);
        }
        else
        {
            render->cam()->set_view(glm::mat4(1.0f));
        }
    } 
        std::vector<glm::vec3> points;
        for (auto it = m_keyframes.begin(); it != m_keyframes.end(); ++it){
            auto it_next = it;
            ++it_next;

            if (it_next == m_keyframes.end()) break;

            KeyFrame first_frame = it->second;
            KeyFrame second_frame = it_next->second;
            glm::vec3 last_pos = first_frame.pos;
            glm::vec3 next_pos = second_frame.pos;
            glm::vec3 c1 = second_frame.control1;
            glm::vec3 c2 = second_frame.control2;
            std::vector<glm::vec3> arc_points = arc_to_points(0, 1,last_pos, next_pos, c1, c2);
            points.insert(points.end(), arc_points.begin(), arc_points.end());
        }
        render->draw_points(points, {1,0,0,1}, 0.05);
    
}

std::vector<glm::vec3> VirtualCamera::arc_to_points(float start, float end,glm::vec3 p0, glm::vec3 p1, glm::vec3 v0, glm::vec3 v1){

    glm::vec3 start_p = -spline_hermite(start, p0, p1, v0, v1);
    glm::vec3 end_p = -spline_hermite(end, p0, p1, v0, v1);

    std::vector<glm::vec3> points = {start_p};
    if (glm::abs(glm::length(end_p - start_p)) > 0.1f){
        float middle = (end + start)/2.;
        std::vector<glm::vec3> sub_points = arc_to_points(start, middle, p0,p1, v0, v1);
        points.insert(points.end(), ++sub_points.begin(), sub_points.end());
        sub_points = arc_to_points(middle, end, p0,p1, v0, v1);
        points.insert(points.end(), ++sub_points.begin(), --sub_points.end());
    }

    points.push_back(end_p);
    return points;
}

const char *VirtualCamera::get_name()
{
    return m_channel_name.c_str();
}

glm::vec3 VirtualCamera::linear(float u, glm::vec3 p0, glm::vec3 p1)
{
    return p0 * (1 - u) + p1 * u;
}
glm::fquat VirtualCamera::linear(float u, glm::fquat p0, glm::fquat p1)
{
    return glm::mix(p0,p1, u);
}

glm::vec3 VirtualCamera::spline_hermite(float u, glm::vec3 p0, glm::vec3 p1, glm::vec3 v0, glm::vec3 v1)
{
    return glm::hermite(p0, v0, p1, v1, u);
}

float VirtualCamera::speed_control(float u, bool ease_in, bool ease_out)
{
    float eased = u == 0
    ? 0
    : u == 1
    ? 1
    : u < 0.5 ? pow(2, 20 * u - 10) / 2
    : (2 - pow(2, -20 * u + 10)) / 2;

    if (ease_in && u < 0.5) return eased;
    if (ease_out && u > 0.5) return eased;
    return u;
}

void VirtualCamera::load(FILE *in)
{
    char buff[255];

    fscanf(in, "VirtualCamStart: %19[^\n]\n", buff);
    m_channel_name = std::string(buff);
    while (fgets(buff, 255, in) != NULL)
    {
        if (strcmp(buff, "VirtualCamEnd\n") == 0)
            break;

        KeyFrame kf;
        int frame;
        int ease_in, ease_out;
        sscanf(buff, "kf %d pos(%f %f %f) rot(%f %f %f) c1(%f %f %f) c2(%f %f %f) ease_in(%d) ease_out(%d)\n", 
            &frame, 
            &kf.pos.x, &kf.pos.y, &kf.pos.z, 
            &kf.rot.x, &kf.rot.y, &kf.rot.z,
            &kf.control1.x, &kf.control1.y, &kf.control1.z,
            &kf.control2.x, &kf.control2.y, &kf.control2.z,
            &ease_in, &ease_out
        );
        kf.easeIn = ease_in;
        kf.easeOut = ease_out;
        m_keyframes[frame] = kf;
    }
}
void VirtualCamera::save(FILE *out)
{
    fprintf(out, "VirtualCamStart: %s\n", get_name());
    for (auto it = m_keyframes.begin(); it != m_keyframes.end(); ++it)
    {
        int frame = it->first;
        KeyFrame kf = it->second;
        fprintf(out, "kf %d pos(%f %f %f) rot(%f %f %f) c1(%f %f %f) c2(%f %f %f) ease_in(%d) ease_out(%d)\n", 
            frame, 
            kf.pos.x, kf.pos.y, kf.pos.z, 
            kf.rot.x, kf.rot.y, kf.rot.z,
            kf.control1.x, kf.control1.y, kf.control1.z,
            kf.control2.x, kf.control2.y, kf.control2.z,
            kf.easeIn, 
            kf.easeOut
        );
    }
    fprintf(out, "VirtualCamEnd\n");
}