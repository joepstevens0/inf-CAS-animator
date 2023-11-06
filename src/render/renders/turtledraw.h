#ifndef TURTLEDRAW_H
#define TURTLEDRAW_H

#include <string>
#include <vector>
#include "texrender.h"

class TurtleDraw{
public:
    TurtleDraw();
    ~TurtleDraw();

    void add_rule(char left, std::string right);

    void clear_rules();

    void set_start_string(std::string start_string);
    void set_step_angle(float angle);
    void set_line_width(int w);
    void set_line_height(int h);
    void iterate(unsigned int iterations);
    void draw(TexRender* render, const int start_pos[2], int window_size[2]);

private:
    std::string draw_branch(TexRender* render,int start_pos[2], std::string path, float angle, int window_size[2]);

    std::string apply_rules(std::string start_string, unsigned int iterations);
    std::string apply_rule(char c);
    struct Rule{
        char left;
        std::string right;
    };
    std::vector<Rule> m_rules;
    std::string m_start_string = "";
    float m_step_angle = -45;
    int m_linesize[2] = {5,100};
    std::string m_path = "";
};

#endif