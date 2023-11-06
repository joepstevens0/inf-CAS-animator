#ifndef STEP_AHEAD_H
#define STEP_AHEAD_H

#include "../channel.h"
#include <map>

class Object;

class StepAhead : public Channel
{
public:
    StepAhead(std::string channel_name);
    ~StepAhead();

    void draw(int frame, Render *render) override;
    void draw_ui(int frame, Render* render) override;
    const char *get_name();

    void load(FILE* in);
    void save(FILE* out);

    void onmousedown(int pos[2],Render* render) override;
    void onmouseup(int pos[2],Render* render) override;

private:
    void ui_frame_add_button(Render *render);
    struct KeyFrame
    {
        glm::vec3 pos;
        glm::vec3 rot;
        std::vector<glm::vec3> control_points;
    };

    static glm::vec3 linear(float u, glm::vec3 p0, glm::vec3 p1);
    static glm::fquat linear(float u, glm::fquat p0, glm::fquat p1);

    std::string m_channel_name;
    std::map<int, KeyFrame> m_keyframes;
    int m_selected = -1;

    Object* m_object;
    FreeForm* m_freeform;
};

#endif