#ifndef OBJECT_H
#define OBJECT_H

#include "../vertexbuffer.h"
#include "../program.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Object
{
public:
    ~Object();
    static Object *from_cube();
    static Object *from_points(std::vector<Vertex> vertices, std::vector<Face> faces);

    void draw(Program* shader, glm::mat4 offset = glm::mat4(1.f));

    void move(glm::vec3 v);
    void set_pos(glm::vec3 v);
    void set_rot(glm::vec2 v);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void scale(glm::vec3 v);

    int getId() const { return m_id; }

    glm::vec3& getPos() { return pos; }
    glm::vec2& getRot() { return rot;}

    void set_model(glm::mat4 m)
    {
        model_set = true;
        model = m;
        pos = glm::vec3(m[0][3],m[1][3], m[2][3]);
    }
    glm::mat4 get_model() const{ return model;}

    void update_vertex(Vertex v, int index);
    Vertex get_vert(int index) const;
    int total_verts() const{ return vertices.size();}

    void set_id(int id){m_id = id;}

private:
    Object(VertexBuffer* vbo);

    void update_model();

    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    int m_id = 0;
    glm::vec3 pos = {0, 0, 0};
    glm::vec2 rot = {0,0};
    glm::vec3 size = {1, 1, 1};

    bool model_set = false;

    glm::mat4 model;

    VertexBuffer* vbo;
};

#endif