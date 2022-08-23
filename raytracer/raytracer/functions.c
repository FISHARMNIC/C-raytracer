// point vectors are the substraction of the end with the start
vector_t getVector(int32_t x, int32_t y, int32_t z, int32_t x2, int32_t y2, int32_t z2){
  return (vector_t) {
    .x = x2-x,
    .y = y2-y,
    .z = z2-z};
} 

normal_vector_t normalizeVector( int32_t x,int32_t y, int32_t z,int32_t len) {
  return (normal_vector_t) {
    .x = (float)x/(float)len, 
    .y = (float)y/(float)len, 
    .z = (float)z/(float)len};
}

int32_t dotProduct(normal_vector_t v1,normal_vector_t v2) {
  return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

void putPixel(int x, int y, float r, float g, float b, float a)
{
  r = r / 255;
  g = g / 255;
  b = b / 255;
  S2D_DrawQuad(
      x, y, r, g, b, a,
      x, y + PIX_SIZE_H, r, g, b, a,
      x + PIX_SIZE_W, y, r, g, b, a,
      x + PIX_SIZE_W, y + PIX_SIZE_H, r, g, b, a);
}

void addCircle(int32_t x, int32_t y, int32_t z, int radius, color_t color)
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

float Distance(int32_t x, int32_t y, int32_t z, int32_t x2, int32_t y2, int32_t z2)
{
  return sqrt((float)((x - x2) * (x - x2) + (y - y2) * (y - y2) + (z - z2) * (z - z2)));
}

// float VectorAngleDifference(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2, int32_t x3, int32_t y3, int32_t z3, int32_t x4, int32_t y4, int32_t z4)
// { // 1 and 2 = first coord, 3 and 4 = second coord
//   return acos(
//       ((x2 - x1) * (x4 - x3) + (y2 - y1) * (y4 - y3) + (z2 - z1) * (z4 - z3)) /
//       (sqrt((x2 - x1)* (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1)) * sqrt((x4 - x3) * (x4 - x3) + (y4 - y3) * (y4 - y3) + (z4 - z3) * (z4 - z3))));
// }