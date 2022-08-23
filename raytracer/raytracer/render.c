/*
Todo:

cast rays from the camera to the objects, calculate the bounce and see if it
is in proximity to the light after tracing its bounced path

take vector coming from center of sphere to intersection and vector of camera to
point of intersection. Caculate the differnce in angles


*/

#include <simple2d.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "variables.h"
#include "functions.c"

void update();
vector_t bounceAngle(circle_t circle);

S2D_Window *window;

int32_t closestZ = DEEPEST_Z;

int32_t ray_x;
int32_t ray_y;
int32_t ray_z = NEAREST_Z;

// maybe rewrite for floating point?
int ray_x_mov;
int ray_y_mov;
int ray_z_mov;

int32_t light_x = SCREEN_WIDTH / 2;
int32_t light_y = 10;
int32_t light_z = 60;

circle_t cobj;

int pixel_x;
int pixel_y;

int rayAge;

color_t closestColor;

float pixelBrightness = 1;

void init()
{
  addCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 50, RED);
  // TODO; BROKEN ONLY LIGHTING ONE SPHERE
  addCircle(SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2 - 5, 35, 50, GREEN);
  //addCircle(window->mouse.x, SCREEN_HEIGHT / 2 - 10, window->mouse.y - 100, 50, GREEN);
  light_x = window->mouse.x;
  light_y = window->mouse.y;
}

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
  while (rayAge < RAY_DURATION) // HERE !@# QWERTY 123 NEED TO BE PUT OUTSIDE FOR LOOP SO IT IS TRACING WHILE CH
  {

    for (current_object = 0; current_object < 20; current_object++) // for each object
    {
      if (objects[current_object].use == 1) // only if in use
      {

        cobj = objects[current_object]; // assign current object

        // If intersecting the circle and closest to the screen
        // printf("d: %i ||", Distance(cobj.x, cobj.y, cobj.z, ray_x, ray_y, ray_z));
        if (Distance(cobj.x, cobj.y, cobj.z, ray_x, ray_y, ray_z) < cobj.radius + (cobj.z / PERSPECTIVE) && ray_z > closestZ)
        {
          // ======================== INTERSECTION ========================
          closestZ = ray_z; // set closest position

          // set color
          closestColor = (color_t){
              .r = cobj.color.r,
              .g = cobj.color.g,
              .b = cobj.color.b,
              .a = cobj.color.a,
          };

          if (bounced) {
            bounces++;
            if(bounces >= MAX_BOUNCES) {
              current_object = 20;
              rayAge = RAY_DURATION;
            }
          }

          // ======================== BOUNCE =============================

          // get bounce off sphere
          vector_t rayDirectionResult = bounceAngle((circle_t){.x = cobj.x, .y = cobj.y, .z = cobj.z, .radius = cobj.radius});
          ray_x_mov = rayDirectionResult.x;
          ray_y_mov = rayDirectionResult.y;
          ray_z_mov = rayDirectionResult.z;
          bounced = 1;
        }

        if (bounced)
        {

          // rayAge = RAY_DURATION; // kill the ray
          // current_object = 20;

          float dLight = Distance(ray_x, ray_y, ray_z, light_x, light_y, light_z);
          if (dLight < (float)LIGHT_TOLERANCE)
          {
            // printf("%f\n", dLight / LIGHT_TOLERANCE);
            pixelBrightness = (LIGHT_TOLERANCE - dLight) / LIGHT_STRENGTH + NATURAL_LIGHT;
            // pixelBrightness = (LIGHT_TOLERANCE - dLight) / LIGHT_TOLERANCE + NATURAL_LIGHT;
          }
        }

        ray_x += ray_x_mov;
        ray_y += ray_y_mov;
        ray_z += ray_z_mov;
        rayAge++;
      }
    }
  }
}

vector_t bounceAngle(circle_t circle)
{
  // vectors: camera->intersection and spherecenter->intersection
  // flip angle
  // return 180.0 + VectorAngleDifference(pixel_x, pixel_y, 100, ray_x, ray_y, ray_z, circle.x, circle.y, circle.z, ray_x, ray_y, ray_z);

  // result should be how much the ray should move on each x/y/z per step
  normal_vector_t cam2intersection = (normal_vector_t){
      .x = ray_x - pixel_x,
      .y = ray_y - pixel_y,
      .z = ray_z - 100};

  normal_vector_t sphere2intersection = (normal_vector_t){
      .x = ray_x - circle.x,
      .y = ray_y - circle.y,
      .z = ray_z - circle.z,
  };

  sphere2intersection = normalizeVector(sphere2intersection.x, sphere2intersection.y, sphere2intersection.z, circle.radius);

  // printf("|| %i %i %i ||", sphere2intersection.x, sphere2intersection.y, sphere2intersection.z);
  //  r=d−2(d⋅n)n where n is the normalized normal
  float dpRes = 2 * (dotProduct(cam2intersection, sphere2intersection));
  sphere2intersection.x = cam2intersection.x - (sphere2intersection.x * dpRes);
  sphere2intersection.y = cam2intersection.y - (sphere2intersection.y * dpRes);
  sphere2intersection.z = -1 * (cam2intersection.z - (sphere2intersection.y * dpRes));

  return (vector_t){
      .x = (int32_t)sphere2intersection.z,
      .y = (int32_t)sphere2intersection.y,
      .z = (int32_t)sphere2intersection.z
  }; // returns movement for each axis
}

// float pixelBrightness() {
//   float d = Distance(ray_x, ray_y, ray_z, light_x, light_y, light_z) / LIGHT_STRENGTH;
//   return 1 / (d * d);
// }

void render()
{

  // reset objects
  for (current_object = 0; current_object < 20; current_object++)
  {
    objects[current_object] = (circle_t){.use = 0};
  }
  current_object = 0;
  init();

  for (pixel_y = 0; pixel_y < SCREEN_HEIGHT; pixel_y += PIX_SIZE_H)
  {
    for (pixel_x = 0; pixel_x < SCREEN_WIDTH; pixel_x += PIX_SIZE_W)
    {
      pixelBrightness = NATURAL_LIGHT;
      // printf("x: %i || y: %i ", pixel_x, pixel_y);
      calculate();
      // putPixel(pixel_x, pixel_y, 0,0,0,1);
      if (closestColor.a != -1)
        putPixel(pixel_x, pixel_y, closestColor.r, closestColor.g, closestColor.b, pixelBrightness);
    }
  }
  // S2D_DrawCircle(window->mouse.x,window->mouse.y, 25, 20, RAW_RED, 1);
}

int main()
{
  window = S2D_CreateWindow(
      "Drawing",
      SCREEN_WIDTH,
      SCREEN_HEIGHT, update, render, 0);
  // window->fps_cap = 15;
  S2D_Show(window);
  return 0;
}

void update()
{
}
