#ifndef PLANTS_H
#define PLANTS_H

#include "../channel.h"
#include <vector>
#include <render/renders/turtledraw.h>

class Plants : public Channel
{
public:
    Plants(std::string channel_name);
    ~Plants();

    void draw(int frame, Render *render) override;
    void draw_ui(int frame, Render* render) override;
    const char *get_name();

    void load(FILE* in);
    void save(FILE* out);

private:
    std::string m_channel_name;

    TurtleDraw* m_turtle;

    struct Rule{
        char left;
        std::string right;
    };

    bool m_show = false;
    
    int m_pos[2] = {500,700};
    std::vector<Rule> m_rules;
    char m_start_string = 'F';
    int m_iterations = 5;
    float m_angle = 20;
    int m_line_width = 1;
    int m_line_height = 10;
};

#endif