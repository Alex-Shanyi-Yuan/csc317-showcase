#include <Eigen/Core>

Eigen::Vector3d reflect(const Eigen::Vector3d & in, const Eigen::Vector3d & n)
{
  ////////////////////////////////////////////////////////////////////////////
  // for reflections there are only changes in direction, not magnitude, and the change is in bounce off the normal
  return (in - 2 * (in.dot(n)) * n).normalized(); // dot product is only a scaler, need to go in -n direction twice
  ////////////////////////////////////////////////////////////////////////////
}
