#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "Shape.hpp"

#include "render.h"

//
// CLASS
//   Surface
//

class Surface : public Shape {
public:
  Surface(Material& material, int nx, int nz, double* x, double* z, double* y);
  void draw(RenderContext* renderContext);
private:
  void setNormal(int ix, int iz);

  VertexArray vertexArray;
  TexCoordArray texCoordArray;
  int nx, nz;
};

#endif // SURFACE_HPP