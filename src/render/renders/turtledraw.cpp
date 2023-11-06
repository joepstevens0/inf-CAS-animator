#include "turtledraw.h"
#include <cmath>
#include <stdio.h>
#include <glm/gtc/quaternion.hpp>

const float COLOR[4] = {1.,0,0,1.};


TurtleDraw::TurtleDraw(){

}

TurtleDraw::~TurtleDraw(){

}

void TurtleDraw::add_rule(char left, std::string right){
    m_rules.push_back(Rule{left,right});
}
void TurtleDraw::clear_rules(){
    m_rules.clear();
}
void TurtleDraw::set_start_string(std::string start_string){
    m_start_string = start_string;
}
void TurtleDraw::set_step_angle(float angle){
    m_step_angle = -angle;
}
void TurtleDraw::set_line_width(int w){
    m_linesize[0] = w;
}
void TurtleDraw::set_line_height(int h){
    m_linesize[1] = h;
}
void TurtleDraw::iterate(unsigned int iterations){
    m_path = apply_rules(m_start_string, iterations);
}

void TurtleDraw::draw(TexRender* render, const int pos[2], int window_size[2]){
    int x = pos[0];
    int y = pos[1];
    float current_angle = 0;

    // reverse path string
    std::string path = std::string(m_path.rbegin(),m_path.rend());

    while (path.size() > 0){
        char c = path[path.size() - 1];
        path.pop_back();
        if (c == 'F'){
            int p[2] = {x,y};
            render->draw_rect(p, m_linesize, COLOR, current_angle, window_size);

            // move turtle
            float rad = glm::radians(current_angle);
            x = x + sin(rad)*m_linesize[1];
            y = y - cos(rad)*m_linesize[1];
        }else if (c == '+'){
            current_angle += m_step_angle;
        }else if (c == '-'){
            current_angle -= m_step_angle;
        }else if (c == '['){
            int p[2] = {x,y};
            path = draw_branch(render, p, path, current_angle, window_size);
        }
    }
}
std::string TurtleDraw::draw_branch(TexRender* render,int pos[2], std::string path, float current_angle, int window_size[2]){
    while (path.size() > 0){
        char c = path[path.size() - 1];
        path.pop_back();
        if (c == 'F'){
            render->draw_rect(pos, m_linesize, COLOR, current_angle, window_size);

            float rad = glm::radians(current_angle);
            pos[0] = pos[0] + sin(rad)*m_linesize[1];
            pos[1] = pos[1] - cos(rad)*m_linesize[1];

        }else if (c == '+'){
            current_angle += m_step_angle;
        }else if (c == '-'){
            current_angle -= m_step_angle;
        }else if (c == '['){
            int p[2] = {pos[0], pos[1]};
            path = draw_branch(render, p, path, current_angle, window_size);
        }else if (c == ']'){
            return path;
        }
    }
    return path;
}

std::string TurtleDraw::apply_rules(std::string str, unsigned int iterations){
    for (unsigned int it = 0; it < iterations;++it){
        std::string new_string = "";
        for (unsigned int char_index = 0; char_index < str.length();++char_index){
            char c = str[char_index];
            new_string += apply_rule(c);
        }
        str = new_string;
    }
    return str;
}

std::string TurtleDraw::apply_rule(char c){
    for (unsigned int i = 0; i < m_rules.size(); ++i){
        if (m_rules[i].left == c){
            return m_rules[i].right;
        }
    }
    return std::string(1,c);
}