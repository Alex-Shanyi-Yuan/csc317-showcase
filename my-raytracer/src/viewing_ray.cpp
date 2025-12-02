#include "viewing_ray.h"
#include <Eigen/Core>

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
  ////////////////////////////////////////////////////////////////////////////
  // Camera width and height are the world space dimensions
  const double Nx = width, Ny = height;

  // translate pixel coordinates so that the origin is at the center of the pixel
  const double u = (camera.width/Nx) * (j + 0.5) - camera.width/2.0;
  const double v = - (camera.height/Ny) * (i + 0.5) + camera.height/2.0; // j increases downwards

  // set ray
  ray.origin = camera.e;
  ray.direction = u * camera.u + v * camera.v - camera.d * camera.w;
  ////////////////////////////////////////////////////////////////////////////
}
