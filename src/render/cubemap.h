#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>

class CubeMap{
public:
    CubeMap(std::string path);
    ~CubeMap();

    void bind(int slot);
private:
    unsigned int m_texture;
};

#endif