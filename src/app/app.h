#ifndef APP_H
#define APP_H

#include <vector>
#include <string>

class GLFWwindow;
class Channel;
class Render;
class Camera;
class Object;

#include <glad/glad.h>

class App
{
public:
    App();
    ~App();

    void run();

private:
    enum ChannelType
    {
        virtual_cam = 0,
        background = 1,
        step_ahead = 2,
        character_animation = 3,
        special_effect = 4,
        plants = 5
    };

    struct ChannelData{
        Channel* ch;
        ChannelType type;
        bool open = false;
        float progress = 0.f;
    };

    void main_loop();
    void create_window();
    void handle_input();
    
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void glfw_error_callback(int error, const char *description);

    void load_channels();
    void store_channels();

    Channel* create_channel(ChannelType type, std::string channel_name);
    void add_channel(ChannelType type, Channel* channel);
    void remove_channel(int index);
    void clear_channels();

    void ui_channel_add();
    void ui_channels();
    void ui_channel(Channel *ch, bool *p_open, float &progress);

    GLFWwindow *m_window;
    std::vector<ChannelData*> m_channels;
    int m_current_frame = 0;
    Render *m_render{nullptr};
    Camera *m_cam;

    bool m_mouse_cam = true;

    const std::vector<std::string> channel_type_to_string = {"virtual_camera", "background", "step_ahead", "character_animation", "special_effect", "plants"};
    ChannelType channel_name_to_type(std::string name);
};

#endif