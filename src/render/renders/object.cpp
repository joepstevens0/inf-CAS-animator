#include "object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(VertexBuffer *vbo)
    : vbo{vbo}
{
    update_model();
}

Object::~Object()
{
    delete vbo;
}

Object *Object::from_cube()
{
    return new Object{VertexBuffer::from_cube()};
}

Object *Object::from_points(std::vector<Vertex> vertices, std::vector<Face> faces)
{
    Object *ob = new Object{VertexBuffer::from_faces(vertices, faces)};

    ob->vertices = vertices;
    ob->faces = faces;

    return ob;
}

void Object::update_vertex(Vertex v, int index)
{
    vertices[index] = v;
    // vbo->update_faces(vertices, faces);
    delete vbo;
    vbo = VertexBuffer::from_faces(vertices, faces);
}
Vertex Object::get_vert(int index) const
{
    return vertices[index];
}

void Object::move(glm::vec3 v)
{
    pos += v;
    update_model();
}
void Object::set_pos(glm::vec3 v)
{
    pos = v;
    update_model();
}
void Object::set_rot(glm::vec2 v)
{
    rot = v;
    update_model();
}
void Object::scale(glm::vec3 v)
{
    size *= v;
    update_model();
}
void Object::rotate_x(float angle)
{
    rot[0] += angle;
    update_model();
}
void Object::rotate_y(float angle)
{
    rot[1] += angle;
    update_model();
}

void Object::update_model()
{
    if (!model_set)
    {
        model = glm::mat4(1.0f); // identity matrix
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(rot[0]), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot[1]), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, size);
    }
}
void Object::draw(Program *shader, glm::mat4 offset)
{
    glm::mat4 m = offset * model;

    shader->bind_mat4("uModel", &m[0][0]);

    vbo->draw();
}