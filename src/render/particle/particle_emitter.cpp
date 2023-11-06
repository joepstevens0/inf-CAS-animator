#include "particle_emitter.h"
#include <render/vertexbuffer.h>
#include <render/program.h>

#include <random>
#include <chrono>

ParticleEmitter::ParticleEmitter()
{
    m_vbo = VertexBuffer::from_rect();
    m_shader = Program::from_path("shader/particle.vert", "shader/particle.frag");

    m_particles.resize(10000, Particle{glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), -1.});
}

ParticleEmitter::~ParticleEmitter()
{
    delete m_shader;
    delete m_vbo;
}

void ParticleEmitter::draw(Camera *cam, const int screen_size[2])
{
    m_shader->set_used();
    m_shader->bind_cam(cam, screen_size);
    for (unsigned int i = 0; i < m_particles.size(); ++i)
    {
        Particle p = m_particles[i];
        float t = p.time_to_live/m_time_to_live;
        float size = m_particle_size - (m_particle_size - m_particle_end_size)*(1-t);
        float pos[4] = {p.pos[0], p.pos[1], p.pos[2], size};
        m_shader->bind_vecf4("uParticlePositions[" + std::to_string(i) + "]", pos);
        float color[4] = {0,0,1,1};
        m_shader->bind_vecf4("uColor", color);
    }
    m_vbo->draw_instanced(m_particles.size());


    static auto last_update = std::chrono::system_clock::now();
    static auto last_add = std::chrono::system_clock::now();
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = current_time - last_update;
    double elapsed_update = diff.count();
    diff = current_time - last_add;
    double elapsed_add = diff.count();

    if (elapsed_add >= m_spawn_time){
        last_add = current_time;

        // add new particles
        int total_adding = m_add_total;
        for (unsigned int i = 0; i < m_particles.size(); ++i)
        {
            Particle &p = m_particles[i];
            if (p.time_to_live < 0){
                float xpos = ((float)std::rand()/(float)RAND_MAX)*2 - 1;
                float zpos = ((float)std::rand()/(float)RAND_MAX)*2 - 1;
                p.pos = glm::vec3(xpos, 10., zpos);
                float yvel = 0;
                float xvel = 0.4;
                p.velocity = glm::vec3(xvel, yvel, 0);
                p.time_to_live = m_time_to_live;

                total_adding -= 1;
                if (total_adding <= 0) break;
            }
        }
    }

    last_update = current_time;

    // update particles for next draw
    for (unsigned int i = 0; i < m_particles.size(); ++i)
    {
        Particle &p = m_particles[i];
        float gravity = 0.01;
        p.velocity.y -= gravity;

        p.pos += p.velocity * (float)(elapsed_update*10.f);

        // collision detection and response
        if (p.pos.y < -0){
            p.pos.y = 0;
            glm::vec3 N = {0,1,0};
            p.velocity = p.velocity - (1.f + 0.5f)*(p.velocity*N)*N;
        }

        if (p.time_to_live > 0)
        {
            p.time_to_live -= elapsed_update;
        } else {
            p = Particle{glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), -1.};
        }
    }
}