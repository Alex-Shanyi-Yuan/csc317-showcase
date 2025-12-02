#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "first_hit.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <random>
#include <iomanip>

int main(int argc, char *argv[])
{
  Camera camera;
  std::vector<std::shared_ptr<Object>> objects;
  std::vector<std::shared_ptr<Light>> lights;
  // Read a camera and scene description from given .json file
  read_json(
      argc <= 1 ? "../data/sphere-and-plane.json" : argv[1],
      camera,
      objects,
      lights);

  int width = 640;
  int height = 360;
  std::vector<unsigned char> rgb_image(3 * width * height);

  // Configuration for Showcase
  int num_samples = 50;   // Samples per pixel for Anti-Aliasing
  double aperture = 0.05; // Size of the lens aperture (0.0 = pinhole/sharp)

  // Random number generators
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(-0.5, 0.5);      // For pixel jitter
  std::uniform_real_distribution<> dis_lens(-1.0, 1.0); // For lens sampling

  // Auto-Focus: Cast a ray through the center to find focal distance
  double t_focus = 10.0; // Default
  {
    Ray center_ray;
    viewing_ray(camera, height / 2, width / 2, width, height, center_ray);
    int hit_id;
    double t;
    Eigen::Vector3d n;
    if (first_hit(center_ray, 1.0, objects, hit_id, t, n))
    {
      t_focus = t;
      std::cout << "Auto-focus locked at distance: " << t_focus << std::endl;
    }
    else
    {
      std::cout << "Auto-focus failed (infinity), using default: " << t_focus << std::endl;
    }
  }

  // For each pixel (i,j)
  for (unsigned i = 0; i < height; ++i)
  {
    // Progress bar
    if (i % 10 == 0)
    {
      std::cout << "\rRendering: " << std::fixed << std::setprecision(1) << (100.0 * i / height) << "%" << std::flush;
    }

    for (unsigned j = 0; j < width; ++j)
    {
      // Set background color
      Eigen::Vector3d pixel_color(0, 0, 0);

      for (int s = 0; s < num_samples; ++s)
      {
        // 1. Jitter pixel coordinates for Anti-Aliasing
        double u_jitter = (j + 0.5 + dis(gen));
        double v_jitter = (i + 0.5 + dis(gen));

        // Replicate viewing_ray logic with jittered coordinates
        // Camera width and height are the world space dimensions
        const double Nx = width, Ny = height;
        const double u_cam = (camera.width / Nx) * u_jitter - camera.width / 2.0;
        const double v_cam = -(camera.height / Ny) * v_jitter + camera.height / 2.0;

        Ray ray;
        ray.origin = camera.e;
        ray.direction = u_cam * camera.u + v_cam * camera.v - camera.d * camera.w;

        // 2. Depth of Field (Defocus Blur)
        if (aperture > 0.0)
        {
          // Point in focus (where the original ray hits the focal plane)
          Eigen::Vector3d p_focus = ray.origin + t_focus * ray.direction;

          // Jitter the origin on the lens disk
          Eigen::Vector3d lens_offset = aperture * (dis_lens(gen) * camera.u + dis_lens(gen) * camera.v);
          // Ideally sample in a disk, square is okay for simple effect

          ray.origin = camera.e + lens_offset;
          ray.direction = p_focus - ray.origin;
        }

        // Shoot ray and collect color
        Eigen::Vector3d sample_color;
        raycolor(ray, 1.0, objects, lights, 0, sample_color);
        pixel_color += sample_color;
      }

      // Average samples
      pixel_color /= double(num_samples);

      // Write double precision color into image
      auto clamp = [](double s)
      { return std::max(std::min(s, 1.0), 0.0); };
      rgb_image[0 + 3 * (j + width * i)] = 255.0 * clamp(pixel_color(0));
      rgb_image[1 + 3 * (j + width * i)] = 255.0 * clamp(pixel_color(1));
      rgb_image[2 + 3 * (j + width * i)] = 255.0 * clamp(pixel_color(2));
    }
  }
  std::cout << "\rRendering: 100.0% - Done!" << std::endl;

  write_ppm("piece.ppm", rgb_image, width, height, 3);
}
