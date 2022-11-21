// point vectors are the substraction of the end with the start
vector_t getVector(double x, double y, double z, double x2, double y2, double z2){
  return (vector_t) {
    .x = x2-x,
    .y = y2-y,
    .z = z2-z};
} 

vector_t normalizeVector( double x,double y, double z,double len) {
  return (vector_t) {
    .x = x/len, 
    .y = y/len, 
    .z = z/len};
}

double dotProduct(vector_t v1,vector_t v2) {
  return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

void putPixel(int x, int y, double r, double g, double b, double a)
{
  vbuff[y][x] = (color_t){.r = r, .g = g, .b = b, .a = a};
  S2D_DrawQuad(
      x, y, r, g, b, a,
      x, y + PIX_SIZE_H, r, g, b, a,
      x + PIX_SIZE_W, y, r, g, b, a,
      x + PIX_SIZE_W, y + PIX_SIZE_H, r, g, b, a);
}

void addCircle(double x, double y, double z, int radius, color_t color)
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

double Distance(double x, double y, double z, double x2, double y2, double z2)
{
  return sqrt((double)((x - x2) * (x - x2) + (y - y2) * (y - y2) + (z - z2) * (z - z2)));
}

// double VectorAngleDifference(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4)
// { // 1 and 2 = first coord, 3 and 4 = second coord
//   return acos(
//       ((x2 - x1) * (x4 - x3) + (y2 - y1) * (y4 - y3) + (z2 - z1) * (z4 - z3)) /
//       (sqrt((x2 - x1)* (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1)) * sqrt((x4 - x3) * (x4 - x3) + (y4 - y3) * (y4 - y3) + (z4 - z3) * (z4 - z3))));
// }