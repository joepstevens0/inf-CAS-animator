#ifndef VIRTUAL_CAMERA_H
#define VIRTUAL_CAMERA_H

#include "../channel.h"
#include <map>
#include<glm/gtc/quaternion.hpp>

class VirtualCamera : public Channel
{
public:
    VirtualCamera(std::string channel_name);

    void draw(int frame, Render *render) override;
    void draw_ui(int frame, Render* render) override;
    const char *get_name();

    void load(FILE* in);
    void save(FILE* out);

private:
    void ui_frame_add_button(Render *render);

    std::vector<glm::vec3> arc_to_points(float start, float end,glm::vec3 p0, glm::vec3 p1, glm::vec3 v0, glm::vec3 v1);

    struct KeyFrame
    {
        glm::vec3 pos;
        glm::vec3 rot;

        glm::vec3 control1 = {0,0,0};
        glm::vec3 control2 = {0,0,0};
        bool easeIn = false;
        bool easeOut = false;
    };

    static glm::vec3 linear(float u, glm::vec3 p0, glm::vec3 p1);
    static glm::fquat linear(float u, glm::fquat p0, glm::fquat p1);
    static glm::vec3 spline_hermite(float u, glm::vec3 p0, glm::vec3 p1, glm::vec3 v0, glm::vec3 v1);
    static float speed_control(float u, bool ease_in, bool ease_out);

    std::string m_channel_name;
    std::map<int, KeyFrame> m_keyframes;
    int m_selected = -1;

    bool m_enabled = false;
};

#endif