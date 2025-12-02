#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"

#define EPSILON 1e-6
#define MAX_NUM_CALLS 5

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  ////////////////////////////////////////////////////////////////////////////
  if (num_recursive_calls > MAX_NUM_CALLS) {
    return false;
  }
  
  rgb = Eigen::Vector3d(0,0,0);

  int hit_id;
  double t;
  Eigen::Vector3d n;
  bool hit = first_hit(ray, min_t, objects, hit_id, t, n);
  if (hit) {
    // If we hit an object, compute the color at the intersection point
    rgb = blinn_phong_shading(ray, hit_id, t, n, objects, lights);

    // make a mirror ray
    Ray mirror_ray;
    mirror_ray.origin = ray.origin + t * ray.direction;
    mirror_ray.direction = reflect(ray.direction, n).normalized();

    // recursive call
    Eigen::Vector3d mirror_rgb(0,0,0);
    if (raycolor(mirror_ray, EPSILON, objects, lights, num_recursive_calls + 1, mirror_rgb)) {
      rgb += (objects[hit_id]->material->km.array() * mirror_rgb.array()).matrix();
    }
  }

  return hit;
  ////////////////////////////////////////////////////////////////////////////
}
