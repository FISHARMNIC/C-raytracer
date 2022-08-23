#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

#define IMAGE_WIDTH 100
#define IMAGE_HEIGHT 100

#define PERSPECTIVE 8 // the lower the stronger
#define LIGHT_STRENGTH 30 // diffuse, higher the more defined
#define LIGHT_TOLERANCE 50 // light size 
#define RAY_DURATION 300

#define DEEPEST_Z -100
#define NEAREST_Z 200

#define NATURAL_LIGHT 0.15

#define MAX_BOUNCES 0
// --------------------------


#define PIX_SIZE_W SCREEN_WIDTH/IMAGE_WIDTH
#define PIX_SIZE_H SCREEN_HEIGHT/IMAGE_HEIGHT


#define RAW_RED 255, 0, 0
#define RAW_GREEN 0, 255, 0
#define RAW_BLUE 0, 0, 255

#define RED (color_t){.r = 255, .g = 0, .b = 0, .a = 1}
#define GREEN (color_t){.r = 0, .g = 255, .b = 0, .a = 1}
#define BLUE (color_t){.r = 0, .g = 0, .b = 255, .a = 1}
#define BLACK (color_t){.r = 0, .g = 0, .b = 0, .a = -1}

typedef struct {
  int r;
  int g;
  int b;
  int a;
} color_t;

typedef struct {
  char use;
  int32_t x;
  int32_t y;
  int32_t z;
  int radius;
  color_t color;
} circle_t;

typedef struct {
  int32_t x;
  int32_t y;
  int32_t z;
} vector_t;

typedef struct {
  float x;
  float y;
  float z;
} normal_vector_t;

circle_t objects[20];
int current_object = 0;

