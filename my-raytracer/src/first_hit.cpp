#include "first_hit.h"

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
  ////////////////////////////////////////////////////////////////////////////
  hit_id = -1;
  double smallest_t = std::numeric_limits<double>::infinity();
  // loop throw all the objects
  for (int i = 0; i < static_cast<int>(objects.size()); i++) {
    double time_intersection;
    Eigen::Vector3d normal_intersection;
    // if intersect and the intersection is the smallest one
    if (objects[i] -> intersect(ray, min_t, time_intersection, normal_intersection) && time_intersection < smallest_t) {
      smallest_t = time_intersection;
      hit_id = i;
      t = time_intersection;
      n = normal_intersection;
    }
  }

  if (hit_id == -1) {
    return false;
  }

  return true;
  ////////////////////////////////////////////////////////////////////////////
}
