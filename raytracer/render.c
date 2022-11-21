#include <simple2d.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "variables.h"

color_t vbuff[SCREEN_HEIGHT][SCREEN_WIDTH];

#include "functions.c"

void update();
vector_t bounceAngle(circle_t circle);

S2D_Window *window;

double closestZ = DEEPEST_Z;

double ray_x;
double ray_y;
double ray_z = NEAREST_Z;

double ray_x_mov;
double ray_y_mov;
double ray_z_mov;

double light_x = SCREEN_WIDTH / 2;
double light_y = 10;
double light_z = 60;

circle_t cobj;
color_t closestColor;

double pixelBrightness = 1;
int pixel_x;
int pixel_y;
int rayAge;

// color_t displayBuffer[IMAGE_WIDTH * IMAGE_HEIGHT];
// color_t bufferAddr = 0;
// // example 1
// void init()
// {
//   addCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 50, RED);
//   // TODO; BROKEN ONLY LIGHTING ONE SPHERE
//   addCircle(SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2 - 5, 35, 50, GREEN);
//   light_x = window->mouse.x;
//   light_y = window->mouse.y;
//   light_z = 60;
//   //addCircle(light_x, light_y, light_z, 50, GREEN);
// }

// void init()
// {
//   addCircle(50, SCREEN_HEIGHT / 2, 0, 10, RED);
//   // TODO; BROKEN ONLY LIGHTING ONE SPHERE
//   addCircle(window->mouse.x, SCREEN_HEIGHT / 2, 0, 20, GREEN);
//   light_z = 0;
//   light_x = 20;
//   light_y = SCREEN_HEIGHT / 2;
//   //addCircle(light_x, light_y, light_z, 50, GREEN);
// }

// void init()
// {
//   addCircle(50, SCREEN_HEIGHT / 2, 0, 20 , RED);
//   // TODO; BROKEN ONLY LIGHTING ONE SPHERE
//   addCircle(window->mouse.x, window->mouse.y, 0, 40, GREEN);
//   //addCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 40, GREEN);
//   light_z = 0;
//   light_x = 5;
//   light_y = SCREEN_HEIGHT / 2;
//   //addCircle(light_x, light_y, light_z, 50, GREEN);
// }

void init()
{
  addCircle(50, SCREEN_HEIGHT / 2, 0, 20, RED);
  addCircle(100, 220, 0, 40, (color_t){.r = .3, .g = 1, .b = .3, .a = 1});
  addCircle(window->mouse.x, window->mouse.y, 0, 40, BLUE);
  light_z = 0;
  light_x = 5;
  light_y = SCREEN_HEIGHT / 2;
}

// void init()
// {
//   addCircle(100, SCREEN_HEIGHT / 2, 0, 50 , RED);
//   // TODO; BROKEN ONLY LIGHTING ONE SPHERE
//   addCircle(window->mouse.x, window->mouse.y, 0, 50, GREEN);
//   //addCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 40, GREEN);
//   light_z = 0;
//   light_x = 160;
//   light_y = SCREEN_HEIGHT / 2;
//   //addCircle(light_x, light_y, light_z, 50, GREEN);
// }

void calculate()
{
  closestZ = DEEPEST_Z;
  closestColor = BLACK;
  char bounced = 0;
  char bounces = 0;

  ray_x = pixel_x;
  ray_y = pixel_y;
  ray_z = NEAREST_Z;

  ray_x_mov = 0;
  ray_y_mov = 0;
  ray_z_mov = -1;

  rayAge = 0;

  color_t first_bounce_color = BLACK;

  double first_bounce_rayx = light_x;
  double first_bounce_rayy = light_y;
  double first_bounce_rayz = light_z;

  int exclude = 21;

  // ======================== TRACE ========================
  while (rayAge < RAY_DURATION) // HERE !@# QWERTY 123 NEED TO BE PUT OUTSIDE FOR LOOP SO IT IS TRACING WHILE CH
  {

    for (current_object = 0; current_object < 20; current_object++) // for each object
    {
      if (objects[current_object].use == 1) // only if in use
      {

        cobj = objects[current_object]; // assign current object

        // If intersecting the circle and closest to the screen
        if (Distance(cobj.x, cobj.y, cobj.z, ray_x, ray_y, ray_z) < cobj.radius + (cobj.z / PERSPECTIVE) && ray_z > closestZ)
        {
          // ======================== INTERSECTION ========================
          closestZ = ray_z; // set closest position

          // experimental xy color grid
          if (EXPERIMENTAL_XY_GRID && vbuff[pixel_y][pixel_x].r != 0 && vbuff[pixel_y][pixel_x].g != 0 && vbuff[pixel_y][pixel_x].b != 0)
          {
            closestColor = (color_t){
                .r = vbuff[pixel_y][pixel_x].r,
                .g = vbuff[pixel_y][pixel_x].g,
                .b = vbuff[pixel_y][pixel_x].b,
                .a = vbuff[pixel_y][pixel_x].a};
          }
          else
          {
            //double red = (int)(ray_x - cobj.x) % 255 + ray_x;
            closestColor = (color_t){
                //.r = red,
                .r = cobj.color.r,
                .g = cobj.color.g,
                .b = cobj.color.b,
                .a = cobj.color.a,
            };
          }

          if (bounced)
            bounces++;
          else
            first_bounce_color = closestColor;

          // ======================== BOUNCE =============================

          // get bounce off sphere
          vector_t rayDirectionResult = bounceAngle((circle_t){.x = cobj.x, .y = cobj.y, .z = cobj.z, .radius = cobj.radius});
          ray_x_mov = rayDirectionResult.x;
          ray_y_mov = rayDirectionResult.y;
          ray_z_mov = rayDirectionResult.z;
          if (bounced == 0)
          {
            first_bounce_rayx = ray_x;
            first_bounce_rayy = ray_y;
            first_bounce_rayz = ray_z;
            exclude = current_object;
          }
          bounced = 1;
        }

        if (bounced)
        {
          double dLight = Distance(ray_x, ray_y, ray_z, light_x, light_y, light_z);
          if (dLight < (double)LIGHT_SIZE)
          {

            pixelBrightness = (LIGHT_SIZE - dLight) / LIGHT_SIZE + 0.7;
            vector_t rayDirectionResult = bounceAngle((circle_t){.x = light_x, .y = light_y, .z = light_z, .radius = LIGHT_SIZE});
            ray_x_mov = rayDirectionResult.x;
            ray_y_mov = rayDirectionResult.y;
            ray_z_mov = rayDirectionResult.z;
          }
        }
      }
    }

    ray_x += ray_x_mov;
    ray_y += ray_y_mov;
    ray_z += ray_z_mov;
    rayAge++;
  }

  // ======================== SHADOWS ========================
  ray_x = first_bounce_rayx;
  ray_y = first_bounce_rayy;
  ray_z = first_bounce_rayz;
  // go to the first hit, and point towards the light
  ray_x_mov = (light_x - ray_x) / STEP_QUALITY;
  ray_y_mov = (light_y - ray_y) / STEP_QUALITY;
  ray_z_mov = (light_z - ray_z) / STEP_QUALITY;
  // trace the line until the light
  while (Distance(ray_x, ray_y, ray_z, light_x, light_y, light_z) > LIGHT_SIZE)
  {
    for (current_object = 0; current_object < 20; current_object++) // for each object
    {
      if (objects[current_object].use == 1) // only if in use
      {
        cobj = objects[current_object]; // assign current object
        // If intersecting the circle and closest to the screen
        if (current_object != exclude && Distance(cobj.x, cobj.y, cobj.z, ray_x, ray_y, ray_z) < cobj.radius + (cobj.z / PERSPECTIVE))
        {
          pixelBrightness = NATURAL_LIGHT / 2;
          goto escape;
        }
      }
    }

    ray_x += ray_x_mov;
    ray_y += ray_y_mov;
    ray_z += ray_z_mov;
  }
escape:
  if (pixelBrightness == NATURAL_LIGHT && !NO_GLOBAL_BOUNCE)
  { // did not intersect
    closestColor = first_bounce_color;
  }
}

vector_t bounceAngle(circle_t circle)
{
  // vectors: camera->intersection and spherecenter->intersection
  // flip angle
  // return 180.0 + VectorAngleDifference(pixel_x, pixel_y, 100, ray_x, ray_y, ray_z, circle.x, circle.y, circle.z, ray_x, ray_y, ray_z);

  // result should be how much the ray should move on each x/y/z per step
  vector_t cam2intersection = (vector_t){
      .x = ray_x - pixel_x,
      .y = ray_y - pixel_y,
      .z = ray_z - 100};

  vector_t sphere2intersection = (vector_t){
      .x = ray_x - circle.x,
      .y = ray_y - circle.y,
      .z = ray_z - circle.z,
  };

  sphere2intersection = normalizeVector(sphere2intersection.x, sphere2intersection.y, sphere2intersection.z, circle.radius);

  //  r=d−2(d⋅n)n where n is the normalized normal
  double dpRes = 2 * (dotProduct(cam2intersection, sphere2intersection));
  sphere2intersection.x = (cam2intersection.x - (sphere2intersection.x * dpRes)) / STEP_QUALITY;
  sphere2intersection.y = (cam2intersection.y - (sphere2intersection.y * dpRes)) / STEP_QUALITY;
  sphere2intersection.z = (-1 * (cam2intersection.z - (sphere2intersection.z * dpRes))) / STEP_QUALITY;

  return sphere2intersection;
}

// double pixelBrightness() {
//   double d = Distance(ray_x, ray_y, ray_z, light_x, light_y, light_z) / LIGHT_STRENGTH;
//   return 1 / (d * d);
// }

char cyclem2 = 0;
void render()
{
  // reset objects
  for (current_object = 0; current_object < 20; current_object++)
  {
    objects[current_object] = (circle_t){.use = 0};
  }
  current_object = 0;
  init();

  if(cyclem2++ % MAX_BOUNCES == 0)
  {
    // clear vbuff
    memset(vbuff, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(color_t));
  }
  for (pixel_y = 0; pixel_y < SCREEN_HEIGHT; pixel_y += PIX_SIZE_H)
  {
    for (pixel_x = 0; pixel_x < SCREEN_WIDTH; pixel_x += PIX_SIZE_W)
    {
      pixelBrightness = NATURAL_LIGHT;
      calculate();
      putPixel(pixel_x, pixel_y, closestColor.r, closestColor.g, closestColor.b, pixelBrightness);
    }
  }
}

int main()
{
  window = S2D_CreateWindow(
      "Drawing",
      SCREEN_WIDTH,
      SCREEN_HEIGHT, update, render, 0);
  // window->fps_cap = 15;
  S2D_HideCursor();

  S2D_Show(window);
  return 0;
}

void update()
{
}
