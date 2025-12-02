#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Dense>

bool Triangle::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // triangle vectors
  const Eigen::Vector3d p0 = std::get<0>(corners);
  const Eigen::Vector3d t1 = std::get<1>(corners) - p0;
  const Eigen::Vector3d t2 = std::get<2>(corners) - p0;

  // e = p0 + alpha*t1 + beta*t2 - d*t
  // e  - p0 = [t1 t2 t3] x [alpha beta -t]^T
  // A = [t1 t2 -d]
  // e - p0 = Ax
  // x = A^-1 x (e - p0)
  double alpha, beta, t_intersection;

  // construct A
  Eigen::Matrix3d A;
  A.col(0) = t1;
  A.col(1) = t2;
  A.col(2) = -ray.direction;
  if (fabs(A.determinant()) < 1e-6) return false; // check if determinate is 0

  // compute X
  Eigen::Vector3d x = A.fullPivLu().solve(ray.origin - p0);

  // solve for alpha, beta, t
  alpha = x(0);
  beta = x(1);
  t_intersection = x(2);

  if (t_intersection < min_t || alpha < 0 || beta < 0 || alpha + beta > 1) return false;
  t = t_intersection;
  n = t1.cross(t2).normalized();
  return true;

  ////////////////////////////////////////////////////////////////////////////
}