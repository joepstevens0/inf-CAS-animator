#include "plants.h"
#include "imgui.h"


Plants::Plants(std::string channel_name)
    : m_channel_name{channel_name}
{
    m_turtle = new TurtleDraw();
}
Plants::~Plants(){
    delete m_turtle;
}

void Plants::draw_ui(int frame, Render *render)
{
    ImGui::Checkbox("show plant", &m_show);
    if (ImGui::DragInt2("pos", m_pos)){
        
    }
    char start_buff[2] = {m_start_string, '\0'};
    if (ImGui::InputText("start symbol", start_buff, 2)){
        m_start_string = start_buff[0];
    }
    if (ImGui::DragInt("iterations", &m_iterations, 0.01)){
        if (m_iterations < 0) m_iterations = 0;
    }
    if (ImGui::DragInt("line width", &m_line_width, 0.01)){
        if (m_line_width < 0) m_line_width = 0;
    }
    if (ImGui::DragInt("line height", &m_line_height, 0.01)){
        if (m_line_height < 0) m_line_height = 0;
    }
    if (ImGui::DragFloat("Angle", &m_angle, 0.1)){
    }
    ImGui::BeginChild("rules pane", ImVec2(0, 0), true);
        {
            for (unsigned int i = 0; i < m_rules.size();++i){
                std::string str_i = std::to_string(i);

                // rule left side
                char left_buff[2] = {m_rules[i].left, '\0'};
                ImGui::PushItemWidth(20);
                if (ImGui::InputText(("##left" + str_i).c_str(), left_buff, 2)){
                    m_rules[i].left = left_buff[0];
                }
                ImGui::PopItemWidth();

                ImGui::SameLine();
                ImGui::Text("=");
                ImGui::SameLine();

                // rule right side
                char buffer[256];
                strcpy(buffer, m_rules[i].right.c_str());
                if (ImGui::InputText(("##right" + str_i).c_str(), buffer, 256)){
                    m_rules[i].right = buffer;
                }
                ImGui::Separator();
            }

            if (ImGui::Button("Add rule")){
                m_rules.push_back({'\0', ""});
            }
        }
    ImGui::EndChild();
}

void Plants::draw(int frame, Render *render)
{
    if (!m_show) return;

    m_turtle->clear_rules();
    for (unsigned int i = 0; i < m_rules.size();++i){
        m_turtle->add_rule(m_rules[i].left, m_rules[i].right);
    }
    m_turtle->set_start_string(std::string("") + m_start_string);
    m_turtle->set_step_angle(m_angle);
    m_turtle->iterate(m_iterations);
    m_turtle->set_line_width(m_line_width);
    m_turtle->set_line_height(m_line_height);
    render->draw_turtle(m_turtle, m_pos);
}

const char *Plants::get_name()
{
    return m_channel_name.c_str();
}


void Plants::load(FILE *in)
{
    char buff[255];

    fscanf(in, "PlantsStart: %19[^\n]\n", buff);
    m_channel_name = std::string(buff);
    fscanf(in, "start_char(%c) it(%d) angle(%f) width(%d) height(%d) pos(%d,%d)\n"
    , &m_start_string, &m_iterations, &m_angle, &m_line_width, &m_line_height, &m_pos[0], &m_pos[1]);
    while (fgets(buff, 255, in) != NULL)
    {
        if (strcmp(buff, "PlantsEnd\n") == 0)
            break;
        char left;
        char buffer[256];
        sscanf(buff, "rule: %c = %s\n", &left, buffer);
        m_rules.push_back({left, buffer});
    }
}
void Plants::save(FILE *out)
{
    fprintf(out, "PlantsStart: %s\n", get_name());
    fprintf(out, "start_char(%c) it(%d) angle(%f) width(%d) height(%d) pos(%d,%d)\n"
    , m_start_string, m_iterations, m_angle, m_line_width, m_line_height, m_pos[0], m_pos[1]);
    for (auto it = m_rules.begin(); it != m_rules.end(); ++it)
    {
        fprintf(out, "rule: %c = %s\n", it->left, it->right.c_str());
    }
    fprintf(out, "PlantsEnd\n");
}