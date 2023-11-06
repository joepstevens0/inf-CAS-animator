#include "step_ahead.h"
#include "imgui.h"

#include "render/camera.h"
#include <glm/gtx/spline.hpp>
#include <glm/gtc/quaternion.hpp>

StepAhead::StepAhead(std::string channel_name)
    : m_channel_name{channel_name}
{

    float s = 1;
    std::vector<Vertex> points = {
        Vertex{
            glm::vec3(0,0,0),
            glm::vec2(0,0)
        },
        Vertex{
            glm::vec3(s,0,0),
            glm::vec2(1,0)
        },
        Vertex{
            glm::vec3(0,s,0),
            glm::vec2(0,1)
        },
        Vertex{
            glm::vec3(s,s,0),
            glm::vec2(1,1)
        }
    };
    std::vector<Face> faces ={
        Face{
            0,1,3
        },
        Face{
            0,3,2
        }
    };
    m_object = Object::from_points(points, faces);
    m_freeform = new FreeForm(m_object);
}

StepAhead::~StepAhead(){
    delete m_object;
}

void StepAhead::draw_ui(int frame, Render *render)
{
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

            // object position
            ImGui::DragFloat3("Object position", &kf.pos[0], 0.01);
            ImGui::DragFloat3("Object rotation", &kf.rot[0], 1);
            ImGui::Separator();
            ImGui::BeginChild("control points");
            for (unsigned int i = 0; i < kf.control_points.size();++i){
                if (ImGui::DragFloat3(("C" + std::to_string(i) + "##" + std::to_string(i)).c_str(),&kf.control_points[i][0], 0.01f))
                    m_freeform->load_points(kf.control_points);
            }
            ImGui::EndChild();
            ImGui::Separator();
            if (ImGui::Button(("delete##" + std::to_string(m_selected)).c_str())){
                m_keyframes.erase(m_selected);
                m_selected = -1;
            }
        }
        ImGui::EndChild();
    }
}

void StepAhead::ui_frame_add_button(Render *render){
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
            kf.control_points = m_keyframes[m_selected].control_points;
        }
        else
        {
            // initialize keyframe values
            kf.pos = {0,0,0};
            kf.rot = {0,0,0};
            kf.control_points = m_freeform->save_points();
        }
        m_keyframes[frame_number] = kf;

        // set new frame as selected
        m_selected = frame_number;
    }
}

void StepAhead::draw(int frame, Render *render)
{
    // determine object position
    if (m_keyframes.size() > 0)
    {
        // find inbetween wich frames
        int first_frame = m_keyframes.begin()->first;
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
        float u = glm::clamp((float)(frame - first_frame)/(float)(second_frame - first_frame), 0.f, 1.f);
        if (first_frame == second_frame) u = 0;

        glm::vec3 cam_pos = linear(u, last_pos, next_pos);
        glm::fquat last_rot = glm::fquat{glm::radians(first_frame_kf.rot)};
        glm::fquat next_rot = glm::fquat{glm::radians(second_frame_kf.rot)};
        glm::fquat cam_rot = linear(u, last_rot, next_rot);
        glm::mat4 model = glm::mat4(cam_rot);
        model = glm::translate(model, cam_pos);
        m_object->set_model(model);

        std::vector<glm::vec3> linear_points;
        for (unsigned int i = 0; i < first_frame_kf.control_points.size();++i){
            linear_points.push_back(linear(u, first_frame_kf.control_points[i], second_frame_kf.control_points[i]));
        }
        m_freeform->load_points(linear_points);
    }
    else
    {
        m_object->set_pos({0, 0, 0});
        m_object->set_rot({0, 0});
    }
    render->draw_objects({m_object});
    render->draw_freeform(m_freeform);
}

const char *StepAhead::get_name()
{
    return m_channel_name.c_str();
}

glm::vec3 StepAhead::linear(float u, glm::vec3 p0, glm::vec3 p1)
{
    return p0 * (1 - u) + p1 * u;
}
glm::fquat StepAhead::linear(float u, glm::fquat p0, glm::fquat p1)
{
    return glm::mix(p0,p1, u);
}

void StepAhead::load(FILE *in)
{
    char buff[255];

    fscanf(in, "StepAheadStart: %19[^\n]\n", buff);
    m_channel_name = std::string(buff);
    KeyFrame kf;
    int frame;
    while (fgets(buff, 255, in) != NULL)
    {
        if (strcmp(buff, "StepAheadEnd\n") == 0)
            break;

        if (buff[0] == 'k'){
            sscanf(buff, "kf %d pos(%f %f %f) rot(%f %f %f)\n", 
                &frame, 
                &kf.pos.x, &kf.pos.y, &kf.pos.z, 
                &kf.rot.x, &kf.rot.y, &kf.rot.z
            );
        } else if (buff[0] == 'c'){
            fgets(buff, 255, in);
            while (buff[0] != '-'){
                glm::vec3 p;
                sscanf(buff, "(%f, %f, %f)\n", &p.x, &p.y, &p.z);
                kf.control_points.push_back(p);
                fgets(buff, 255, in);
            }
            m_keyframes[frame] = kf;
            kf.control_points.clear();
        }
    }
}
void StepAhead::save(FILE *out)
{
    fprintf(out, "StepAheadStart: %s\n", get_name());
    for (auto it = m_keyframes.begin(); it != m_keyframes.end(); ++it)
    {
        int frame = it->first;
        KeyFrame kf = it->second;
        fprintf(out, "kf %d pos(%f %f %f) rot(%f %f %f)\n", 
            frame, 
            kf.pos.x, kf.pos.y, kf.pos.z, 
            kf.rot.x, kf.rot.y, kf.rot.z
        );
        fprintf(out, "cp:\n");
        for (unsigned int i = 0; i < kf.control_points.size();++i){
            fprintf(out, "(%f, %f, %f)\n", kf.control_points[i].x, kf.control_points[i].y, kf.control_points[i].z);
        }
        fprintf(out, "-\n");
    }
    fprintf(out, "StepAheadEnd\n");
}

void StepAhead::onmousedown(int pos[2],Render* render){
    m_freeform->mousedown(pos,render);
}
void StepAhead::onmouseup(int pos[2],Render* render){
    m_freeform->mouseup(pos,render);
}