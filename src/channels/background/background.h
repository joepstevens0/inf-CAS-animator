#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../channel.h"
#include <vector>

class CubeMap;

#define MAX_BACKGROUND_PATH_SIZE 64

class Background : public Channel
{
public:
    Background(std::string channel_name);
    ~Background();

    void draw(int frame, Render *render) override;
    void draw_ui(int frame, Render* render) override;
    const char *get_name();

    void load(FILE* in);
    void save(FILE* out);

private:
    std::string m_channel_name;

    char m_path[MAX_BACKGROUND_PATH_SIZE] = "skybox/underwater/";

    CubeMap* m_skybox;
};

#endif