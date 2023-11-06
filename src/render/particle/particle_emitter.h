#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <glm/glm.hpp>
#include <vector>

class VertexBuffer;
class Program;
class Camera;

class ParticleEmitter{
public:
    ParticleEmitter();
    ~ParticleEmitter();

    void draw(Camera* cam, const int screen_size[2]);

    void set_color(float color[3]){
        m_color[0] = color[0];
        m_color[1] = color[1];
        m_color[2] = color[2];
    }
    void set_particle_size(float size){
        m_particle_size = size;
    }
    void set_particle_end_size(float size){
        m_particle_end_size = size;
    }
    void set_max_particles(int total){
        m_particles.resize(total, Particle{glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), -1.});
    }
    void set_spawn_time(float time){
        m_spawn_time = time;
    }
    void set_add_total(int total){
        m_add_total = total;
    }
    void set_time_to_live(float time){
        m_time_to_live = time;
    }
private:
    struct Particle{
        glm::vec3 pos;
        glm::vec3 velocity;
        float time_to_live;
    };
    VertexBuffer* m_vbo;
    Program* m_shader;

    std::vector<Particle> m_particles;
    float m_particle_size = 0.1;
    float m_color[4] = {0,0,1,0};
    float m_particle_end_size = 0;
    float m_spawn_time = 0.05;
    int m_add_total = 20;
    float m_time_to_live = 5;
};

#endif