// point vectors are the substraction of the end with the start
vector_t getVector(float x, float y, float z, float x2, float y2, float z2){
  return (vector_t) {
    .x = x2-x,
    .y = y2-y,
    .z = z2-z};
} 

vector_t normalizeVector( float x,float y, float z,float len) {
  return (vector_t) {
    .x = (float)x/(float)len, 
    .y = (float)y/(float)len, 
    .z = (float)z/(float)len};
}

float dotProduct(vector_t v1,vector_t v2) {
  return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

#define putPixel(x, y, r, g, b, a) vbuff[y][x] = (color_t){(float) r/255, (float) g/255, (float) b/255, a}

void addCircle(float x, float y, float z, int radius, color_t color)
{
  objects[current_object++] = (circle_t){
      .use = 1,
      .x = x,
      .y = y,
      .z = z,
      .radius = radius,
      .color = color,
  };
}

float Distance(float x, float y, float z, float x2, float y2, float z2)
{
  return sqrt((float)((x - x2) * (x - x2) + (y - y2) * (y - y2) + (z - z2) * (z - z2)));
}

// float VectorAngleDifference(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4)
// { // 1 and 2 = first coord, 3 and 4 = second coord
//   return acos(
//       ((x2 - x1) * (x4 - x3) + (y2 - y1) * (y4 - y3) + (z2 - z1) * (z4 - z3)) /
//       (sqrt((x2 - x1)* (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1)) * sqrt((x4 - x3) * (x4 - x3) + (y4 - y3) * (y4 - y3) + (z4 - z3) * (z4 - z3))));
// }