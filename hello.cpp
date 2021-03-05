#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

#include "geo.h"

void drawGrid();
void drawCube(CubeClass cube);

const int size = 50;
const int num = 10;

const int MAX = size * num;
const int height = MAX + MAX/2;
const int width = MAX + MAX/2;
const int speed = size;
const int thickness = 4;

const int max_alpha_deg = 179;
const int min_alpha_deg = 1;
const int default_alpha_deg = 45;
int alpha_deg = default_alpha_deg;
double alpha = alpha_deg * (M_PI/180); //in radians

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

    Angle angle = {0, 0, 0};
    Point start = {0, 0, 0};
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
                {
                    cube.Translate({0,0,-1*speed});
                    if(cube.GetLocation().position.z < 0)
                    {
                        cube.SetPositionZ(0);
                    }
                }
                break;

                case ALLEGRO_KEY_UP:
                {
                    cube.Translate({0,0,1*speed});
                    if(cube.GetLocation().position.z > MAX-size)
                    {
                        cube.SetPositionZ(MAX-size);
                    }
                }
                break;

                case ALLEGRO_KEY_LEFT:
                {
                    cube.Translate({-1*speed,0,0});
                    if(cube.GetLocation().position.x < 0)
                    {
                        cube.SetPositionX(0);
                    }
                }
                break;

                case ALLEGRO_KEY_RIGHT:
                {
                    cube.Translate({1*speed,0,0});
                    if(cube.GetLocation().position.x > MAX-size)
                    {
                        cube.SetPositionX(MAX-size);
                    }
                }
                break;

                case ALLEGRO_KEY_PAD_2:
                {
                    cube.Translate({0,-1*speed,0});
                    if(cube.GetLocation().position.y < 0)
                    {
                        cube.SetPositionY(0);
                    }
                }
                break;

                case ALLEGRO_KEY_PAD_8:
                {
                    cube.Translate({0,1*speed,0});
                    if(cube.GetLocation().position.y > MAX-size)
                    {
                        cube.SetPositionY(MAX-size);
                    }
                }
                break;

                case ALLEGRO_KEY_PAD_4:
                {
                    alpha_deg = alpha_deg + 1;
                    if(alpha_deg > max_alpha_deg)
                    {
                        alpha_deg = max_alpha_deg;
                    }
                }
                break;

                case ALLEGRO_KEY_PAD_6:
                {
                    alpha_deg = alpha_deg - 1;
                    if(alpha_deg < min_alpha_deg)
                    {
                        alpha_deg = min_alpha_deg;
                    }
                }
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
            al_clear_to_color(al_map_rgb(255, 255, 255));

            alpha = alpha_deg * (M_PI/180);

            drawGrid();

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

void drawGrid()
{
    //AXES
    float axis_x = 0.5*((float)MAX)*cos(alpha);
    float axis_y = 0.5*((float)MAX)*sin(alpha);
    al_draw_line(0, height, MAX, height, al_map_rgb(0, 0, 0),thickness*2);
    al_draw_line(0, height, axis_x, height - axis_y, al_map_rgb(0, 0, 0),thickness*2);
    al_draw_line(0, height, 0, height - MAX, al_map_rgb(0, 0, 0),thickness*2);

    //Grid
    float loc = size;
    while(loc <= MAX)
    {
        al_draw_line(loc, height, axis_x + loc, height - axis_y, al_map_rgb(0, 0, 0),0);
        loc += size;
    }

    float locX = 0.5*(size)*cos(alpha);
    float locY = height - 0.5*(size)*sin(alpha);
    while(locY >= (height - axis_y - (0.01 * axis_y)))
    {
        al_draw_line(locX, locY, locX + MAX, locY, al_map_rgb(0, 0, 0),0);
        locX += 0.5*(size)*cos(alpha);
        locY -= 0.5*(size)*sin(alpha);
    }
}

void drawCube(CubeClass cube)
{
    Location loc = cube.GetLocation();
    Point *points3D = cube.GetVertices();
    Line *edges = cube.GetEdges();

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

    float points1[8] = { (float)points2D[0].x, (float)(height - points2D[0].y), (float)points2D[1].x, (float)(height - points2D[1].y), (float)points2D[3].x, (float)(height - points2D[3].y), (float)points2D[2].x, (float)(height - (float)points2D[2].y) };
    al_draw_filled_polygon(points1, 4, al_map_rgb(0, 200, 0));

    float points2[8] = { (float)points2D[1].x, (float)(height - points2D[1].y), (float)points2D[5].x, (float)(height - points2D[5].y), (float)points2D[7].x, (float)(height - points2D[7].y), (float)points2D[3].x, (float)(height - (float)points2D[3].y) };
    al_draw_filled_polygon(points2, 4, al_map_rgb(0, 150, 0));

    float points3[8] = { (float)points2D[2].x, (float)(height - points2D[2].y), (float)points2D[3].x, (float)(height - points2D[3].y), (float)points2D[7].x, (float)(height - points2D[7].y), (float)points2D[6].x, (float)(height - (float)points2D[6].y) };
    al_draw_filled_polygon(points3, 4, al_map_rgb(0, 100, 0));

    //plot edges between 2D points
    for (int i = 0; i < 12; ++i)
    {
        if(i != 2 && i != 8 && i != 9)
        {
            al_draw_line(points2D[edges[i].pointA].x, (height - points2D[edges[i].pointA].y), points2D[edges[i].pointB].x, (height - points2D[edges[i].pointB].y), al_map_rgb(0, 255, 0),thickness);
        }   
    }

    if(loc.position.y != 0)
    {
        //Initialize 2D points array
        Point pointsProjection2D[4];
        for (int i = 0; i < 4; ++i)
        {
            pointsProjection2D[i] = {0,height,0}; //We will just ignore the Z value when thinking in 2D, only x and y
        }

        //convert 3D points into 2D points, but 0 out y value
        int i = 0;
        pointsProjection2D[0].x = points3D[i].x + 0.5*(points3D[i].z)*cos(alpha);
        pointsProjection2D[0].y = 0.5*(points3D[i].z)*sin(alpha);

        i = 1;
        pointsProjection2D[1].x = points3D[i].x + 0.5*(points3D[i].z)*cos(alpha);
        pointsProjection2D[1].y = 0.5*(points3D[i].z)*sin(alpha);

        i = 4;
        pointsProjection2D[2].x = points3D[i].x + 0.5*(points3D[i].z)*cos(alpha);
        pointsProjection2D[2].y = 0.5*(points3D[i].z)*sin(alpha);

        i = 5;
        pointsProjection2D[3].x = points3D[i].x + 0.5*(points3D[i].z)*cos(alpha);
        pointsProjection2D[3].y = 0.5*(points3D[i].z)*sin(alpha);

        al_draw_line(pointsProjection2D[0].x, (height - pointsProjection2D[0].y), pointsProjection2D[1].x, (height - pointsProjection2D[1].y), al_map_rgb(0, 255, 0),thickness);
    
        al_draw_line(pointsProjection2D[0].x, (height - pointsProjection2D[0].y), pointsProjection2D[2].x, (height - pointsProjection2D[2].y), al_map_rgb(0, 255, 0),thickness);

        al_draw_line(pointsProjection2D[1].x, (height - pointsProjection2D[1].y), pointsProjection2D[3].x, (height - pointsProjection2D[3].y), al_map_rgb(0, 255, 0),thickness);

        al_draw_line(pointsProjection2D[2].x, (height - pointsProjection2D[2].y), pointsProjection2D[3].x, (height - pointsProjection2D[3].y), al_map_rgb(0, 255, 0),thickness);
    
        float points4[8] = { (float)pointsProjection2D[0].x, (float)(height - pointsProjection2D[0].y), (float)pointsProjection2D[1].x, (float)(height - pointsProjection2D[1].y), (float)pointsProjection2D[3].x, (float)(height - pointsProjection2D[3].y), (float)pointsProjection2D[2].x, (float)(height - (float)pointsProjection2D[2].y) };
        al_draw_filled_polygon(points4, 4, al_map_rgb(0, 100, 0));
    }
}