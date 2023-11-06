#include "background.h"
#include "imgui.h"

#include "render/cubemap.h"

Background::Background(std::string channel_name)
    : m_channel_name{channel_name}
{
    m_skybox = new CubeMap(m_path);
}
Background::~Background(){
    delete m_skybox;
}

void Background::draw_ui(int frame, Render *render)
{
    ImGui::InputText("path", m_path, MAX_BACKGROUND_PATH_SIZE);
    if (ImGui::Button("load")){
        delete m_skybox;
        m_skybox = new CubeMap(m_path);
    }
}

void Background::draw(int frame, Render *render)
{
    render->draw_skybox(m_skybox);
}

const char *Background::get_name()
{
    return m_channel_name.c_str();
}


void Background::load(FILE *in)
{
    char buff[255];

    fscanf(in, "BackgroundStart: %19[^\n]\n", buff);
    m_channel_name = std::string(buff);
    fscanf(in, "%19[^\n]\n", m_path);
    if (fgets(buff, 255, in) != NULL)
    {
        if (strcmp(buff, "BackgroundEnd\n") != 0){
            fprintf(stderr, "Failed to load Background channel\n");
            throw;
        }
    }

    delete m_skybox;
    m_skybox = new CubeMap(m_path);
}
void Background::save(FILE *out)
{
    fprintf(out, "BackgroundStart: %s\n", get_name());
    fprintf(out, "%s\n", m_path);
    fprintf(out, "BackgroundEnd\n");
}