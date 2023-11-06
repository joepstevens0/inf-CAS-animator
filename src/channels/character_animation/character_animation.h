#ifndef CHARACTER_ANIMATION_H
#define CHARACTER_ANIMATION_H

#include "../channel.h"
#include <vector>
#include "skeleton.h"

class CharacterAnimation : public Channel{
public:
    CharacterAnimation(std::string channel_name);
    ~CharacterAnimation();

    void draw(int frame, Render *render) override;
    void draw_ui(int frame, Render* render) override;
    const char *get_name();

    void load(FILE* in);
    void save(FILE* out);

    void onmousedown(int pos[2],Render* render) override;
private:

    std::string m_channel_name;

    Object* m_move_cube;
    glm::vec3 m_endpos = {15,15,0};
    glm::vec3 m_endrot = {0,0,0};

    glm::vec3 m_skel_pos = {0,0,0};
    float m_skel_scale = 1.f;
    float m_step = 0.01;

    Skeleton* m_skel;
    bool m_animate = false;
};

#endif