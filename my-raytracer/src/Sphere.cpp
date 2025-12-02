#include "Sphere.h"
#include "Ray.h"
bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // [(d.d)t^2] + 2[d.(e-c)]t + [(e-c).(e-c)-r^2] = 0
  const double A = ray.direction.dot(ray.direction);
  const double B = 2 * ray.direction.dot(ray.origin - this->center);
  const double C = (ray.origin - this->center).dot(ray.origin - this->center) - this->radius * this->radius;

  const double discriminant = B * B - 4 * A * C;
  if (discriminant < 0) {
    return false;
  }

  const double sqrt_discriminant = sqrt(discriminant);
  const double t1 = (-B - sqrt_discriminant) / (2 * A);
  const double t2 = (-B + sqrt_discriminant) / (2 * A);
  const Eigen::Vector3d n1 = (ray.origin + t1 * ray.direction - this->center).normalized();
  const Eigen::Vector3d n2 = (ray.origin + t2 * ray.direction - this->center).normalized();

  // both greater than min_t
  if (t1 >= min_t && t2 >= min_t) {
    if (t1 < t2) {
      t = t1;
      n = n1;
    } 
    // also covers t1 == t2
    else {
      t = t2;
      n = n2;
    }
    return true;
  }
  // one of t1 or t2 must be greater than min_t
  else if (t1 >= min_t) {
    t = t1;
    n = n1;
    return true;
  } else if (t2 >= min_t) {
    t = t2;
    n = n2;
    return true;
  }

  return false;
  ////////////////////////////////////////////////////////////////////////////
}

