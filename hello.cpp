#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

#include "geo.h"

#define BLACK           al_map_rgb(0, 0, 0)
#define WHITE           al_map_rgb(255, 255, 255)
#define LIGHT_GREY      al_map_rgb(200, 200, 200)
#define MEDIUM_GREY     al_map_rgb(150, 150, 150)
#define DARK_GREY       al_map_rgb(100, 100, 100)
#define LIME_GREEN      al_map_rgb(0, 255, 0)
#define LIGHT_GREEN     al_map_rgb(0, 200, 0)
#define MEDIUM_GREEN    al_map_rgb(0, 150, 0)
#define DARK_GREEN      al_map_rgb(0, 100, 0)
#define SKY_BLUE        al_map_rgb(135, 206, 235)
#define STEEL_BLUE      al_map_rgb(70, 130, 180)

void drawGrid();
void drawCubes();
void drawPlacedCube(CubeClass cube);
void drawPlayerCube();
void drawMenu();

const int size = 60;
const int num = 10;

const int MAX = size * num;
const int height = MAX + MAX/2;
const int width = MAX + MAX/2;
const int menu = width/4;
const int speed = size;
const int thickness = 4;

const int max_alpha_deg = 90;
const int min_alpha_deg = 1;
const int default_alpha_deg = 45;
int alpha_deg = default_alpha_deg;
double alpha = alpha_deg * (M_PI/180); //in radians

//          X    Y    Z
char spaces[num][num][num];
// 0 = nothing
// 1 = player cube
// 2 = placed cube

char playerX = 0;
char playerY = 0;
char playerZ = 0;

Angle angle = {0, 0, 0};
Point start = {0, 0, 0};
Location location = {start, angle};
CubeClass cube(size, location);

int main()
{
    al_init();
    al_init_font_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(width + menu, height);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;

    for(int i = 0; i < num; i++)
    {
        for(int j = 0; j < num; j++)
        {
            for(int k = 0; k < num; k++)
            {
                spaces[i][j][k] = 0;
            }
        }
    }

    spaces[0][0][0] = 1;    //Player cube at 0,0,0

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
                case ALLEGRO_KEY_SPACE:
                {
                    for(int i = 0; i < num; i++)
                    {
                        for(int j = 0; j < num; j++)
                        {
                            for(int k = 0; k < num; k++)
                            {
                                spaces[i][j][k] = 0;
                            }
                        }
                    }

                    spaces[0][0][0] = 1;    //Player cube at 0,0,0
                    playerX = 0;
                    playerY = 0;
                    playerZ = 0;
                    cube.SetPosition(start);

                }
                break;

                case ALLEGRO_KEY_S:
                { 
                    if((playerZ > 0) && (spaces[playerX][playerY][playerZ - 1] != 2))
                    {
                        spaces[playerX][playerY][playerZ] = 2;
                        playerZ--;
                        spaces[playerX][playerY][playerZ] = 1;
                        cube.Translate({0,0,-1*speed});
                        if(cube.GetLocation().position.z < 0)
                        {
                            cube.SetPositionZ(0);
                        }
                    } 
                }
                break;

                case ALLEGRO_KEY_W:
                {
                    if((playerZ < num-1) && (spaces[playerX][playerY][playerZ + 1] != 2))
                    {
                        spaces[playerX][playerY][playerZ] = 2;
                        playerZ++;
                        spaces[playerX][playerY][playerZ] = 1;
                        cube.Translate({0,0,1*speed});
                        if(cube.GetLocation().position.z > MAX-size)
                        {
                            cube.SetPositionZ(MAX-size);
                        }
                    }
                    
                }
                break;

                case ALLEGRO_KEY_A:
                {
                    if((playerX > 0) && (spaces[playerX - 1][playerY][playerZ] != 2))
                    {
                        spaces[playerX][playerY][playerZ] = 2;
                        playerX--;
                        spaces[playerX][playerY][playerZ] = 1;
                        cube.Translate({-1*speed,0,0});
                        if(cube.GetLocation().position.x < 0)
                        {
                            cube.SetPositionX(0);
                        }
                    }
                }
                break;

                case ALLEGRO_KEY_D:
                {
                    if((playerX < num-1) && (spaces[playerX + 1][playerY][playerZ] != 2))
                    {
                        spaces[playerX][playerY][playerZ] = 2;
                        playerX++;
                        spaces[playerX][playerY][playerZ] = 1;
                        cube.Translate({1*speed,0,0});
                        if(cube.GetLocation().position.x > MAX-size)
                        {
                            cube.SetPositionX(MAX-size);
                        }
                    }
                }
                break;

                case ALLEGRO_KEY_DOWN:
                {
                    if((playerY > 0) && (spaces[playerX][playerY - 1][playerZ] != 2))
                    {
                        spaces[playerX][playerY][playerZ] = 2;
                        playerY--;
                        spaces[playerX][playerY][playerZ] = 1;
                        cube.Translate({0,-1*speed,0});
                        if(cube.GetLocation().position.y < 0)
                        {
                            cube.SetPositionY(0);
                        }
                    }
                }
                break;

                case ALLEGRO_KEY_UP:
                {
                    if((playerY < num-1) && (spaces[playerX][playerY + 1][playerZ] != 2))
                    {
                        spaces[playerX][playerY][playerZ] = 2;
                        playerY++;
                        spaces[playerX][playerY][playerZ] = 1;
                        cube.Translate({0,1*speed,0});
                        if(cube.GetLocation().position.y > MAX-size)
                        {
                            cube.SetPositionY(MAX-size);
                        }
                    }
                }
                break;

                case ALLEGRO_KEY_LEFT:
                {
                    alpha_deg = alpha_deg + 1;
                    if(alpha_deg > max_alpha_deg)
                    {
                        alpha_deg = max_alpha_deg;
                    }
                }
                break;

                case ALLEGRO_KEY_RIGHT:
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
            al_clear_to_color(WHITE);

            alpha = alpha_deg * (M_PI/180);

            drawGrid();

            drawCubes();

            drawMenu();

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
    al_draw_line(0, height, MAX, height, BLACK, thickness*2);
    al_draw_line(0, height, axis_x, height - axis_y, BLACK, thickness*2);
    al_draw_line(0, height, 0, height - MAX, BLACK, thickness*2);

    //Grid
    float loc = size;
    while(loc <= MAX)
    {
        al_draw_line(loc, height, axis_x + loc, height - axis_y, BLACK, 0);
        loc += size;
    }

    float locX = 0.5*(size)*cos(alpha);
    float locY = height - 0.5*(size)*sin(alpha);
    while(locY >= (height - axis_y - (0.01 * axis_y)))
    {
        al_draw_line(locX, locY, locX + MAX, locY, BLACK, 0);
        locX += 0.5*(size)*cos(alpha);
        locY -= 0.5*(size)*sin(alpha);
    }
}

void drawPlacedCube(CubeClass thisCube)
{
    Location loc = thisCube.GetLocation();
    Point *points3D = thisCube.GetVertices();
    Line *edges = thisCube.GetEdges();

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
    al_draw_filled_polygon(points1, 4, LIGHT_GREY);

    float points2[8] = { (float)points2D[1].x, (float)(height - points2D[1].y), (float)points2D[5].x, (float)(height - points2D[5].y), (float)points2D[7].x, (float)(height - points2D[7].y), (float)points2D[3].x, (float)(height - (float)points2D[3].y) };
    al_draw_filled_polygon(points2, 4, MEDIUM_GREY);

    float points3[8] = { (float)points2D[2].x, (float)(height - points2D[2].y), (float)points2D[3].x, (float)(height - points2D[3].y), (float)points2D[7].x, (float)(height - points2D[7].y), (float)points2D[6].x, (float)(height - (float)points2D[6].y) };
    al_draw_filled_polygon(points3, 4, DARK_GREY);

    //plot edges between 2D points
    for (int i = 0; i < 12; ++i)
    {
        if(i != 2 && i != 8 && i != 9)
        {
            al_draw_line(points2D[edges[i].pointA].x, (height - points2D[edges[i].pointA].y), points2D[edges[i].pointB].x, (height - points2D[edges[i].pointB].y), BLACK, thickness);
        }   
    }
}

void drawCubes()
{
    for(int i = 0; i < num; i++)
    {
        for(int j = 0; j < num; j++)
        {
            for(int k = num-1; k >= 0; k--)
            {
                if(spaces[i][j][k] == 1)
                {
                    drawPlayerCube();
                }
                else if(spaces[i][j][k] == 2)
                {
                    Angle thisAngle = {0, 0, 0};
                    Point thisStart = {size*i, size*j, size*k};
                    Location thisLocation = {thisStart, thisAngle};
                    CubeClass thisCube(size, thisLocation);
                    drawPlacedCube(thisCube);
                }
            }
        }
    }
}

void drawPlayerCube()
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
    al_draw_filled_polygon(points1, 4, LIGHT_GREEN);

    float points2[8] = { (float)points2D[1].x, (float)(height - points2D[1].y), (float)points2D[5].x, (float)(height - points2D[5].y), (float)points2D[7].x, (float)(height - points2D[7].y), (float)points2D[3].x, (float)(height - (float)points2D[3].y) };
    al_draw_filled_polygon(points2, 4, MEDIUM_GREEN);

    float points3[8] = { (float)points2D[2].x, (float)(height - points2D[2].y), (float)points2D[3].x, (float)(height - points2D[3].y), (float)points2D[7].x, (float)(height - points2D[7].y), (float)points2D[6].x, (float)(height - (float)points2D[6].y) };
    al_draw_filled_polygon(points3, 4, DARK_GREEN);

    //plot edges between 2D points
    for (int i = 0; i < 12; ++i)
    {
        if(i != 2 && i != 8 && i != 9)
        {
            al_draw_line(points2D[edges[i].pointA].x, (height - points2D[edges[i].pointA].y), points2D[edges[i].pointB].x, (height - points2D[edges[i].pointB].y), LIME_GREEN, thickness);
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

        al_draw_line(pointsProjection2D[0].x, (height - pointsProjection2D[0].y), pointsProjection2D[1].x, (height - pointsProjection2D[1].y), LIME_GREEN, thickness);
    
        al_draw_line(pointsProjection2D[0].x, (height - pointsProjection2D[0].y), pointsProjection2D[2].x, (height - pointsProjection2D[2].y), LIME_GREEN, thickness);

        al_draw_line(pointsProjection2D[1].x, (height - pointsProjection2D[1].y), pointsProjection2D[3].x, (height - pointsProjection2D[3].y), LIME_GREEN, thickness);

        al_draw_line(pointsProjection2D[2].x, (height - pointsProjection2D[2].y), pointsProjection2D[3].x, (height - pointsProjection2D[3].y), LIME_GREEN, thickness);
    
        float points4[8] = { (float)pointsProjection2D[0].x, (float)(height - pointsProjection2D[0].y), (float)pointsProjection2D[1].x, (float)(height - pointsProjection2D[1].y), (float)pointsProjection2D[3].x, (float)(height - pointsProjection2D[3].y), (float)pointsProjection2D[2].x, (float)(height - (float)pointsProjection2D[2].y) };
        al_draw_filled_polygon(points4, 4, DARK_GREEN);
    }
}

void drawMenu()
{
    al_draw_line(width, 0, width, height, BLACK, thickness*2);
}