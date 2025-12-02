#include "blinn_phong_shading.h"

// Hint:
#include "first_hit.h"
#include <iostream>

#define EPSILON 1e-6

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
  ////////////////////////////////////////////////////////////////////////////
  // ambient + diffuse + specular = phong reflection
  Eigen::Vector3d result(0,0,0);
  Eigen::Vector3d ka = objects[hit_id]->material->ka;
  Eigen::Vector3d kd = objects[hit_id]->material->kd;
  Eigen::Vector3d ks = objects[hit_id]->material->ks;
  double p = objects[hit_id]->material->phong_exponent;

  // result starts with ambient light
  result += (Eigen::Vector3d(0.1, 0.1, 0.1).array() * ka.array()).matrix();

  // create and define the intersection point
  Ray point_to_light;
  point_to_light.origin = ray.origin + t * ray.direction + EPSILON * n; // offset a bit to avoid self-intersection
  double light_distance;

  // loop through each light source
  for (size_t i = 0; i < lights.size(); i++) {
    // get the direction from intersection point to light source
    lights[i]->direction(point_to_light.origin, point_to_light.direction, light_distance);

    // check if the light is blocked by any object
    int temp_hit_id;
    double object_distance;
    Eigen::Vector3d temp_n;
    if (first_hit(point_to_light, EPSILON, objects, temp_hit_id, object_distance, temp_n) && object_distance < light_distance) {
      continue; // light is blocked, skip to next light source
    }

    // diffuse shading
    result += (kd.array() * lights[i]->I.array()).matrix() * std::max(0.0, n.dot(point_to_light.direction.normalized()));
    // specular shading
    const Eigen::Vector3d h = (-ray.direction.normalized() + point_to_light.direction.normalized()).normalized();
    result += (ks.array() * lights[i]->I.array()).matrix() * std::pow(std::max(0.0, n.dot(h)), p);
  }

  return result;
  ////////////////////////////////////////////////////////////////////////////
}
