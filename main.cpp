#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "LiteMath.h"
#include "public_camera.h"
#include "public_scene.h"
#include "public_image.h"

#include <cstdio>
#include <cstring>

using LiteMath::float3;
int main(int argc, char **argv)
{
  constexpr int W = 1024;
  constexpr int H = 1024;
  PrimitiveSDFScene scene;
  scene.from_file(argv[1]);
  float z0 = std::stof(argv[2]);
  printf("rendering slice of scene %s at z = %f\n", argv[1], z0);

  std::vector<float> image(3*W*H, 0.0f);

  for (int y = 0; y < H; y++)
  {
    for (int x = 0; x < W; x++)
    {
      float dist = 1000.0f;
      for (int i = 0; i < scene.scene_data.size()/4; i++)
      {
        float3 p = float3(2*(float(x)/W) - 1, 2*(float(y)/H) - 1, z0);
        float3 center = float3(scene.scene_data[i*4], 
                                                   scene.scene_data[i*4+1], 
                                                   scene.scene_data[i*4+2]);
        float r = scene.scene_data[i*4+3];
        dist = std::min(dist, length(p - center) - r);
      }

      float3 color = dist > 0 ? float3(1-dist, 0, 0) : float3(0, 0, 1+dist);
      image[3*(y*W + x)] = color.x;
      image[3*(y*W + x) + 1] = color.y;
      image[3*(y*W + x) + 2] = color.z;
    }
  }

  write_image_rgb("out.png", image, W, H);

  return 0;
}