#ifndef SPECIAL_EFFECT_H
#define SPECIAL_EFFECT_H

#include "../channel.h"
#include <vector>

class ParticleEmitter;

class SpecialEffect : public Channel{
public:
    SpecialEffect(std::string channel_name);
    ~SpecialEffect();

    void draw(int frame, Render *render) override;
    void draw_ui(int frame, Render* render) override;
    const char *get_name();

    void load(FILE* in);
    void save(FILE* out);
private:
    std::string m_channel_name;

    ParticleEmitter* m_emitter;
    Object* m_ground;

    float m_color[3] = {0,0,1};
    float m_particle_size = 0.1;
    float m_particle_min_size = 0;
    float m_spawn_time = 0.05;
    int m_max_particles = 10000;
    float m_time_to_live = 5;
    int m_add_total = 20;
};


#endif