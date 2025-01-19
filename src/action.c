#include "hdr/camera.h"
#include "hdr/infex.h"
#include "hdr/state.h"
#include "hdr/world.h"

void action_quit(void)
{
    set_quit(true);
}

void action_mainmenu_background_initialise(void)
{
    world_initialise(64, 64);
    world_generate();
    camera_centre();
}
