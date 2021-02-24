#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

#include "geo.h"

void drawCube(CubeClass cube);

int width = 0;
int height = 0;
int size = 0;
int thickness = 0;

int main()
{
    al_init();
    al_init_font_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;

    width = al_get_display_width(disp);
    std::cout << "width = " << width << "\n";
    height = al_get_display_height(disp);
    std::cout << "height = " << height << "\n";

    Angle angle = {0, 0, 0};
    Point origin = {width/2, 0, height/2};
    Location location = {origin, angle};

    size = (std::min(width, height))/4;
    thickness = 3;
    std::cout << "size = " << size << "\n";
    CubeClass cube(size, location);

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            redraw = true;
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            //HELLO TEXT
            //al_draw_text(font, al_map_rgb(0, 0, 255), width/2, height/2, 0, "Hello!");

            //CUBE
            drawCube(cube);

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

void drawCube(CubeClass cube)
{
    Location location = cube.GetLocation();
    Line *edges = cube.GetEdges();

    int roll = location.orientation.roll;
    int pitch = location.orientation.pitch;
    int yaw = location.orientation.yaw;

    //convert 3D edges into 2D drawing
    for (int i = 0; i < 12; ++i)
    {
        if ((edges.pointA.y == 0) && (edges.pointB.y == 0))
        {
            al_draw_line(edges[i].pointA.x, (height - edges[i].pointA.z), edges[i].pointB.x, (height - edges[i].pointB.z), al_map_rgb(255, 0, 0),thickness);
        }

        else
        {
            
        }
        
    }
}