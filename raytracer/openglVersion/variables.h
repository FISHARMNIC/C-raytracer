#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200

#define IMAGE_WIDTH 200
#define IMAGE_HEIGHT 200

#define PERSPECTIVE 8 // the lower the stronger
#define LIGHT_STRENGTH 0.55 // diffuse, higher the more defined
#define LIGHT_TOLERANCE 50 // light size 
#define RAY_DURATION 500
#define STEP_QUALITY 20 // higher the slower the ray moves
#define DEEPEST_Z -100
#define NEAREST_Z 200

#define NATURAL_LIGHT 0.2

#define MAX_BOUNCES 5
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
  float r;
  float g;
  float b;
  float a;
} color_t;

typedef struct {
  char use;
  float x;
  float y;
  float z;
  int radius;
  color_t color;
} circle_t;

typedef struct {
  float x;
  float y;
  float z;
} vector_t;


circle_t objects[20];
int current_object = 0;

