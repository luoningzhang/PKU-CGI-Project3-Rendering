#include "graphics/scene.h"

namespace VCL::GlobIllum {

Color RayTrace(const Scene &scene, Ray ray, int bounce_num);
Color PathTrace(const Scene &scene, Ray ray, int bounce_num);

}
