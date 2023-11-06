#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glad/glad.h>
#include <gl_header.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
  glm::vec3 pos;
  glm::vec2 texCoord;
};
struct Face
{
  int index_0;
  int index_1;
  int index_2;
};

class VertexBuffer
{
public:
  void draw();
  void draw_instanced(int instances);
  static VertexBuffer* from_rect();
  static VertexBuffer* from_cube();
  static VertexBuffer* from_3d_vertices(std::vector<float> vertices, int total);
  static VertexBuffer* from_faces(std::vector<Vertex> vertices, std::vector<Face> faces);
  static VertexBuffer* from_points(std::vector<glm::vec3> points);

  ~VertexBuffer();

  VertexBuffer(VertexBuffer &&other)
  {
    vbo = other.vbo;
    vao = other.vao;
    indices = other.indices;
    other.vao = 0;
    other.vbo = 0;
    other.indices = 0;
  };

  void update_faces(std::vector<Vertex> vertices, std::vector<Face> faces);
  void update_vbo(void* data, int size);

private:
  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer(GLuint vbo, GLuint vao, int indices);

  static void push_face(std::vector<float> &verts, const std::vector<Vertex> &vertices, Face f);
  static void push_vertex(std::vector<float> &verts, Vertex v);

  static GLuint create_vao(GLuint vbo, std::vector<int> attributes);
  static GLuint create_vbo(std::vector<float> vertices);

  GLuint vbo;
  GLuint vao;

  int indices;
  int mode = GL_TRIANGLES;
};

#endif