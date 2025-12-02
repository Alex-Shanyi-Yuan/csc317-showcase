#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // check if there is an answer
  if (this->normal.dot(ray.direction) == 0) return false;

  // there will be an answer
  t = -this->normal.dot(ray.origin - this->point) / this->normal.dot(ray.direction);
  if (t < min_t) return false;
  
  n = this->normal.normalized();
  return true;
  ////////////////////////////////////////////////////////////////////////////
}

