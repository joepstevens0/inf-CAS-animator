#include "special_effect.h"
#include "imgui.h"
#include <render/particle/particle_emitter.h>

SpecialEffect::SpecialEffect(std::string channel_name)
    : m_channel_name{channel_name}
{
    m_emitter = new ParticleEmitter();
    m_ground = Object::from_cube();
    m_ground->scale({40,0.1,10});
    m_ground->move({0,0,0});
}
SpecialEffect::~SpecialEffect(){
    delete m_emitter;
}

void SpecialEffect::draw_ui(int frame, Render *render)
{
    // if (ImGui::InputFloat3("color", m_color)){
    //     m_emitter->set_color(m_color);
    // }
    if (ImGui::DragFloat("particle size", &m_particle_size, 0.001, 0, 2.)){
        m_emitter->set_particle_size(m_particle_size);
    }
    if (ImGui::DragFloat("min size", &m_particle_min_size, 0.001, 0, 2.)){
        m_emitter->set_particle_end_size(m_particle_min_size);
    }
    if (ImGui::DragFloat("Spawn time", &m_spawn_time, 0.001, 0)){
        m_emitter->set_spawn_time(m_spawn_time);
    }
    if (ImGui::InputInt("Max particles", &m_max_particles)){
        if (m_max_particles > 1000) m_max_particles = 1000;
        if (m_max_particles < 0) m_max_particles = 0;
        m_emitter->set_max_particles(m_max_particles);
    }
    if (ImGui::InputInt("Spawn per time", &m_add_total)){
        if (m_add_total > 1000) m_add_total = 1000;
        if (m_add_total < 0) m_add_total = 0;
        m_emitter->set_add_total(m_add_total);
    }
    if (ImGui::InputFloat("Time to live", &m_time_to_live, 0.5, 0)){
        m_emitter->set_time_to_live(m_time_to_live);
    }
}

void SpecialEffect::draw(int frame, Render *render)
{
    render->draw_objects({m_ground});
    render->draw_particle(m_emitter);
}

const char* SpecialEffect::get_name()
{
    return m_channel_name.c_str();
}


void SpecialEffect::load(FILE *in)
{
    char buff[255];

    fscanf(in, "SpecialEffectStart: %19[^\n]\n", buff);
    m_channel_name = std::string(buff);

    fscanf(in, "color(%f %f %f) size(%f) min_size(%f) spawn_time(%f) max_part(%d) ttl(%f) add_per_spawn(%d)\n", 
        &m_color[0], &m_color[1], &m_color[2],
        &m_particle_size,
        &m_particle_min_size,
        &m_spawn_time,
        &m_max_particles,
        &m_time_to_live,
        &m_add_total
     );

    if (fgets(buff, 255, in) != NULL)
    {
        if (strcmp(buff, "SpecialEffectEnd\n") != 0){
            fprintf(stderr, "Failed to load SpecialEffect channel\n");
            throw;
        }
    }

    // update emitter
    m_emitter->set_color(m_color);
    m_emitter->set_particle_size(m_particle_size);
    m_emitter->set_particle_end_size(m_particle_min_size);
    m_emitter->set_spawn_time(m_spawn_time);
    m_emitter->set_max_particles(m_max_particles);
    m_emitter->set_add_total(m_add_total);
    m_emitter->set_time_to_live(m_time_to_live);
}
void SpecialEffect::save(FILE *out)
{
    fprintf(out, "SpecialEffectStart: %s\n", get_name());
    fprintf(out, "color(%f %f %f) size(%f) min_size(%f) spawn_time(%f) max_part(%d) ttl(%f) add_per_spawn(%d)\n", 
        m_color[0], m_color[1], m_color[2],
        m_particle_size,
        m_particle_min_size,
        m_spawn_time,
        m_max_particles,
        m_time_to_live,
        m_add_total
     );
    fprintf(out, "SpecialEffectEnd\n");
}