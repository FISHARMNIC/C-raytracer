#include <GLUT/glut.h>

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "variables.h"
color_t vbuff[IMAGE_HEIGHT][IMAGE_WIDTH];
#include "functions.c"

void update();
vector_t bounceAngle(circle_t circle);

float closestZ = DEEPEST_Z;

float ray_x;
float ray_y;
float ray_z = NEAREST_Z;

float ray_x_mov;
float ray_y_mov;
float ray_z_mov;

float light_x = SCREEN_WIDTH / 2;
float light_y = 10;
float light_z = 60;

circle_t cobj;
color_t closestColor;

float pixelBrightness = 1;
int pixel_x;
int pixel_y;
int rayAge;

int mouseX = 0;
int mouseY = 0;

void init()
{
  addCircle(50, SCREEN_HEIGHT / 2, 0, 20, RED);
  // TODO; BROKEN ONLY LIGHTING ONE SPHERE
  addCircle(mouseX, mouseY, 0, 40, GREEN);
  // addCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 40, GREEN);
  light_z = 0;
  light_x = 5;
  light_y = SCREEN_HEIGHT / 2;
  // addCircle(light_x, light_y, light_z, 50, GREEN);
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

          set color
          closestColor = (color_t){
              .r = cobj.color.r,
              .g = cobj.color.g,
              .b = cobj.color.b,
          };

          // experimental xy color grid
          // closestColor = (color_t){
          //     .r = vbuff[pixel_y][pixel_x].r,
          //     .g = vbuff[pixel_y][pixel_x].g,
          //     .b = vbuff[pixel_y][pixel_x].b,
          // };

          if (bounced)
          {
            // closestColor = BLACK;
            // current_object = 20;
            // rayAge = RAY_DURATION;
            // printf("ret %i %i", pixel_x, pixel_y);
            // return;
            bounces++;
            if (bounces >= MAX_BOUNCES)
            {
              return;
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
            pixelBrightness = (LIGHT_TOLERANCE - dLight) / LIGHT_TOLERANCE + LIGHT_STRENGTH;

            // OLD ENGINE
            // pixelBrightness = (LIGHT_TOLERANCE - dLight) / LIGHT_STRENGTH + NATURAL_LIGHT;
            // if(LIGHT_STOPS_RAY)
            // return;
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

  // printf("|| %i %i %i ||", sphere2intersection.x, sphere2intersection.y, sphere2intersection.z);
  //  r=d−2(d⋅n)n where n is the normalized normal
  float dpRes = 2 * (dotProduct(cam2intersection, sphere2intersection));
  sphere2intersection.x = (cam2intersection.x - (sphere2intersection.x * dpRes)) / STEP_QUALITY;
  sphere2intersection.y = (cam2intersection.y - (sphere2intersection.y * dpRes)) / STEP_QUALITY;
  sphere2intersection.z = (-1 * (cam2intersection.z - (sphere2intersection.z * dpRes))) / STEP_QUALITY;

  // printf("|| %f %f %f ||", sphere2intersection.x, sphere2intersection.y, sphere2intersection.z);
  return sphere2intersection;

  // return (vector_t){
  //     .x = (float)sphere2intersection.z,
  //     .y = (float)sphere2intersection.y,
  //     .z = (float)sphere2intersection.z
  // }; // returns movement for each axis
}

// float pixelBrightness() {
//   float d = Distance(ray_x, ray_y, ray_z, light_x, light_y, light_z) / LIGHT_STRENGTH;
//   return 1 / (d * d);
// }
void My_mouse_routine(int x, int y)
{
  mouseX = x;
  mouseY = y;
  glutPostRedisplay();
  // printf("%i %i\n", mouseX, mouseY);
}

void render()
{

  // reset objects
  for (current_object = 0; current_object < 20; current_object++)
  {
    objects[current_object] = (circle_t){.use = 0};
  }
  current_object = 0;
  // printf("%i\n", objects[1].x); fflush(stdout);
  init();

  glBegin(GL_POINTS);
  for (pixel_y = 0; pixel_y <= SCREEN_HEIGHT; pixel_y += PIX_SIZE_H)
  {
    for (pixel_x = 0; pixel_x <= SCREEN_WIDTH; pixel_x += PIX_SIZE_W)
    {
      pixelBrightness = NATURAL_LIGHT;
      // printf("x: %i || y: %i ", pixel_x, pixel_y);
      calculate();
      glColor4f(0, 0, 0, 1);
      glVertex2i(pixel_x, pixel_y);
      if (closestColor.a != -1)
        glColor4f(closestColor.r, closestColor.g, closestColor.b, pixelBrightness);
      glVertex2i(pixel_x, pixel_y);
    }
  }
  glEnd();
  glFlush();
  // printf("done"); fflush(stdout);
  // glutPostRedisplay();
  //  S2D_DrawCircle(window->mouse.x,window->mouse.y, 25, 20, RAW_RED, 1);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  glutInitWindowPosition(80, 80);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutCreateWindow("A Simple Triangle");

  glClear(GL_COLOR_BUFFER_BIT);
  glutPassiveMotionFunc(My_mouse_routine);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glutDisplayFunc(render);
  glutMainLoop();
  return 0;
}
