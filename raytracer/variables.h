#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 300

#define IMAGE_WIDTH 100
#define IMAGE_HEIGHT 100

#define PERSPECTIVE 8 // the lower the stronger
#define LIGHT_STRENGTH 0.55 // diffuse, higher the more defined

#define LIGHT_SIZE 30 // light size 
#define LIGHT_TOLERANCE 10 // experimental 

#define RAY_DURATION 500
#define STEP_QUALITY 20 // higher the slower the ray moves
#define DEEPEST_Z -100
#define NEAREST_Z 200

#define NATURAL_LIGHT 0.5 //0.2

#define MAX_BOUNCES 5

#define EXPERIMENTAL_XY_GRID 1 // use the rendered colors for more accurate renders


#define NO_GLOBAL_BOUNCE 1
//#define LIGHT_STOPS_RAY  // stops the ray if it hits the light source
// --------------------------


#define PIX_SIZE_W SCREEN_WIDTH/IMAGE_WIDTH
#define PIX_SIZE_H SCREEN_HEIGHT/IMAGE_HEIGHT


#define RAW_RED 255, 0, 0
#define RAW_GREEN 0, 255, 0
#define RAW_BLUE 0, 0, 255

#define RED (color_t){.r = 1, .g = 0, .b = 0, .a = 1}
#define GREEN (color_t){.r = 0, .g = 1, .b = 0, .a = 1}
#define BLUE (color_t){.r = 0, .g = 0, .b = 1, .a = 1}
#define BLACK (color_t){.r = 0, .g = 0, .b = 0, .a = -1}

typedef struct {
  double r;
  double g;
  double b;
  double a;
} color_t;

typedef struct {
  char use;
  double x;
  double y;
  double z;
  int radius;
  color_t color;
} circle_t;

typedef struct {
  double x;
  double y;
  double z;
} vector_t;


circle_t objects[20];
int current_object = 0;

