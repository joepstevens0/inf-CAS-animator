#include "app.h"

#include <stdio.h>
#include "channels/channel.h"

void App::load_channels()
{
    // delete all channels
    clear_channels();


    FILE *in = fopen("channels.txt", "r");
    if (in != NULL)
    {
        while (!feof(in))
        {
            char type_name[80];
            fscanf(in, "\%19[^\n]\n", type_name);
            printf("Loading channel %s\n", type_name);
            ChannelType type = channel_name_to_type(std::string(type_name));
            Channel *ch = create_channel(type, "temp_name");
            ch->load(in);
            add_channel(type, ch);
            fgetc(in);
        }
    }
    fclose(in);
}
void App::store_channels()
{
    FILE *out = fopen("channels.txt", "w");

    if (out == NULL)
    {
        fprintf(stderr, "Error saving file\n");
        throw;
    }
    for (size_t i = 0; i < m_channels.size(); ++i)
    {
        Channel *ch = m_channels[i]->ch;
        ChannelType type = m_channels[i]->type;

        std::string typestring = channel_type_to_string[type];
        fprintf(out, "%s\n", typestring.c_str());
        ch->save(out);
        if (i + 1 < m_channels.size())
            fprintf(out, "\n");
    }
    fclose(out);
}