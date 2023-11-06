#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>

#include "render/render.h"

class Channel{
public:
    virtual ~Channel() {}
    virtual void draw(int frame, Render* render) = 0;
    virtual void draw_ui(int frame, Render* render) = 0;
    virtual const char* get_name() = 0;

    virtual void load(FILE* in) = 0;
    virtual void save(FILE* out) = 0;

    virtual void onclick(int pos[2],Render* render){}
    virtual void onmousedown(int pos[2],Render* render){}
    virtual void onmouseup(int pos[2],Render* render){}
};

#endif