#include "TriangleSoup.h"
#include "Ray.h"
// Hint
#include "first_hit.h"

bool TriangleSoup::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  int hit_id;
  if (!first_hit(ray, min_t, this->triangles, hit_id, /*out*/ t, /*out*/ n)) {
    return false;
  }
  return true;
  ////////////////////////////////////////////////////////////////////////////
}
