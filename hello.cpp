#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

#include "geo.h"

void drawCube(CubeClass cube);

int width = 1000;
int height = 600;
int size = height/16;
int speed = size;
int thickness = 2;

const double alpha = 45 * (M_PI/180); //in radians

int main()
{
    al_init();
    al_init_font_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(width, height);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;

    std::cout << "width = " << width << "\n";
    std::cout << "height = " << height << "\n";
    std::cout << "size = " << size << "\n";

    Angle angle = {0, 0, 0};
    Point origin = {0, 0, 0};
    Point start = {size*4, 0, 0};
    Location location = {start, angle};
    
    CubeClass cube(size, location);

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
        }
        else if(event.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            switch(event.keyboard.keycode)
            {
                case ALLEGRO_KEY_DOWN:
                    cube.Translate({0,0,-1*speed});
                break;

                case ALLEGRO_KEY_UP:
                    cube.Translate({0,0,1*speed});
                break;

                case ALLEGRO_KEY_LEFT:
                    cube.Translate({-1*speed,0,0});
                break;

                case ALLEGRO_KEY_RIGHT:
                    cube.Translate({1*speed,0,0});
                break;
            }
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

            //AXES
            al_draw_line(0, height, width, height, al_map_rgb(255, 0, 0),thickness*2);
            al_draw_line(0, height, height, 0, al_map_rgb(255, 0, 0),thickness*2);
            al_draw_line(0, height, 0, 0, al_map_rgb(255, 0, 0),thickness*2);

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
    Point *points3D = cube.GetVertices();
    Line *edges = cube.GetEdges();

    int roll = location.orientation.roll;
    int pitch = location.orientation.pitch;
    int yaw = location.orientation.yaw;

    //Initialize 2D points array
    Point points2D[8];
    for (int i = 0; i < 8; ++i)
    {
        points2D[i] = {0,height,0}; //We will just ignore the Z value when thinking in 2D, only x and y
    }

    //convert 3D points into 2D points
    for (int i = 0; i < 8; ++i)
    {
        points2D[i].x = points3D[i].x + 0.5*(points3D[i].z)*cos(alpha);
        points2D[i].y = points3D[i].y + 0.5*(points3D[i].z)*sin(alpha);
    }

    //plot edges between 2D points
    for (int i = 0; i < 12; ++i)
    {
        //if(i != 2 && i != 8 && i != 9)
        //{
            al_draw_line(points2D[edges[i].pointA].x, (height - points2D[edges[i].pointA].y), points2D[edges[i].pointB].x, (height - points2D[edges[i].pointB].y), al_map_rgb(0, 255, 0),thickness);
        //}   
    }
}