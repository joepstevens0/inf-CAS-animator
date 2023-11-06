#include "vertexbuffer.h"

void VertexBuffer::draw()
{
    glBindVertexArray(vao);
    glPointSize(10.0);
    glDrawArrays(mode, // mode
                 0,            // starting index in the enabled arrays
                 indices       // number of indices to be rendered
    );
}
void VertexBuffer::draw_instanced(int instances){
    glBindVertexArray(vao);
    glPointSize(10.0);
    glDrawArraysInstanced(mode, 0, indices, instances);
}
VertexBuffer *VertexBuffer::from_rect()
{
    std::vector<float> vertices = {
        -1., -1., 0., 0., // bottom left
        -1., 1., 0., 1.,  // top left
        1., 1., 1., 1.,   // top  right

        -1., -1., 0., 0., // bottom left
        1., 1., 1., 1.,   // top  right
        1., -1., 1., 0.   // bottom right
    };

    GLuint vbo = create_vbo(vertices);
    GLuint vao = create_vao(vbo, {2, 2});

    return new VertexBuffer{vbo, vao, 6};
}

VertexBuffer *VertexBuffer::from_faces(std::vector<Vertex> vertices, std::vector<Face> faces)
{
    std::vector<float> verts;
    int total = 0;

    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        push_face(verts, vertices, faces[i]);
        total += 3;
    }

    return VertexBuffer::from_3d_vertices(verts, total);
}

VertexBuffer *VertexBuffer::from_points(std::vector<glm::vec3> points)
{
    std::vector<float> vertices;
    for (unsigned int i = 0; i < points.size();++i){
        vertices.push_back(points[i][0]);
        vertices.push_back(points[i][1]);
        vertices.push_back(points[i][2]);
    }
    GLuint vbo = create_vbo(vertices);
    GLuint vao = create_vao(vbo, {3});
    VertexBuffer * buffer = new VertexBuffer(vbo, vao, points.size());
    buffer->mode = GL_POINTS;
    return buffer;
}

void VertexBuffer::push_face(std::vector<float> &verts, const std::vector<Vertex> &vertices, Face f)
{
    push_vertex(verts, vertices[f.index_0]);
    push_vertex(verts, vertices[f.index_1]);
    push_vertex(verts, vertices[f.index_2]);
}
void VertexBuffer::push_vertex(std::vector<float> &verts, Vertex v)
{
    verts.push_back(v.pos.x);
    verts.push_back(v.pos.y);
    verts.push_back(v.pos.z);
    verts.push_back(v.texCoord.x);
    verts.push_back(v.texCoord.y);
}

VertexBuffer *VertexBuffer::from_cube()
{
    std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    return from_3d_vertices(vertices, 36);
}

VertexBuffer *VertexBuffer::from_3d_vertices(std::vector<float> vertices, int total)
{
    GLuint vbo = create_vbo(vertices);
    GLuint vao = create_vao(vbo, {3, 2});

    return new VertexBuffer{vbo, vao, total};
}

VertexBuffer::~VertexBuffer()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

VertexBuffer::VertexBuffer(GLuint vbo, GLuint vao, int indices) : vbo{vbo}, vao{vao}, indices{indices} {}

GLuint VertexBuffer::create_vao(GLuint vbo, std::vector<int> attributes)
{
    GLuint vao = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLint stride = 0;
    for (unsigned int i = 0; i < attributes.size(); ++i)
    {
        stride += 4 * attributes[i];
    }
    size_t offset = 0;
    for (unsigned int i = 0; i < attributes.size(); ++i)
    {
        glVertexAttribPointer(
            i,                   // index of the generic vertex attribute ("layout (location = 0)")
            attributes[i],       // the number of components per generic vertex attribute
            GL_FLOAT,            // data type
            GL_FALSE,            // normalized (int-to-float conversion)
            stride,              // stride (byte offset between consecutive attributes)
            (const void *)offset // offset of the first component
        );
        glEnableVertexAttribArray(i);

        offset += attributes[i] * sizeof(float);
    }

    // glVertexAttribPointer(
    //     0, // index of the generic vertex attribute ("layout (location = 0)")
    //     2, // the number of components per generic vertex attribute
    //     GL_FLOAT, // data type
    //     GL_FALSE, // normalized (int-to-float conversion)
    //     4 * sizeof(
    //             GLint), // stride (byte offset between consecutive attributes)
    //     nullptr         // offset of the first component
    // );
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(
    //     1, // index of the generic vertex attribute ("layout (location = 0)")
    //     2, // the number of components per generic vertex attribute
    //     GL_FLOAT, // data type
    //     GL_FALSE, // normalized (int-to-float conversion)
    //     4 * sizeof(
    //             GLint), // stride (byte offset between consecutive attributes)
    //     (const void *)(2 * sizeof(float)) // offset of the first component
    // );
    // glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return vao;
}

GLuint VertexBuffer::create_vbo(std::vector<float> vertices)
{
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);

    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,                 // target
                 vertices.size() * sizeof(float), // size of data in bytes
                 &vertices[0],                    // pointer to data
                 GL_STATIC_DRAW                   // usage
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

void VertexBuffer::update_vbo(void* data, int size){
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,                 // target
                 size, // size of data in bytes
                 data,                    // pointer to data
                 GL_STATIC_DRAW                   // usage
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::update_faces(std::vector<Vertex> vertices, std::vector<Face> faces){
    std::vector<float> verts;
    int total = 0;

    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        push_face(verts, vertices, faces[i]);
        total += 3;
    }
    update_vbo(&verts[0], vertices.size() * sizeof(float));
    total = verts.size();
}