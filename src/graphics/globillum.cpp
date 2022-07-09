#include "globillum.h"

#include "common/helperfunc.h"
#include "light.h"

#include <iostream>

namespace VCL::GlobIllum {

Vec3 AxisAngle(const Vec3 &w, const real cos2theta, const real phi){
  // 2*theta的cos
	const real cos_theta = std::sqrt(cos2theta);//theta的cos
	const real sin_theta = std::sqrt(1 - cos2theta);//theta的sin
	const Vec3 u = (std::abs(w[0]) > real(.1) ? Vec3(0, 1, 0) : Vec3(1, 0, 0)).cross(w).normalized();
	const Vec3 v = w.cross(u);
	return (u * std::cos(phi) * sin_theta + v * std::sin(phi) * sin_theta + w * cos_theta).normalized();
}

Vec3 Sample(const Material *const mat, const Vec3 &n, const Vec3 &wi, Color &weight){
  const real R = mat->k_d_.mean() / (mat->k_d_.mean() + mat->k_s_.mean());
  const real r0 = rand01();
  if (r0 < R) { // sample diffuse ray 
    weight = mat->k_d_.any() ? mat->k_d_ / R : Color(0, 0, 0);
    return AxisAngle(n, rand01(), rand01() * 2 * PI_);
  }
  else { // sample specular ray
    if (mat->alpha_ >= 0) {
      const Vec3 d = AxisAngle(n * 2 * n.dot(wi) - wi, std::pow(rand01(), real(2) / (mat->alpha_ + 2)), rand01() * 2 * PI_);
      weight = n.dot(d) <= 0 || !mat->k_s_.any() ? Color(0, 0, 0) : mat->k_s_ / (1 - R);
      return d;
    }
    else { // for ideal mirrors
      weight = mat->k_s_.any() ? mat->k_s_ / (1 - R) : Color(0, 0, 0);
      return n * 2 * n.dot(wi) - wi;
    }
  }
}
real max(real a,real b){
  if(a>b)return a;
  else return b;
}

Color RayTrace(const Scene &scene, Ray ray, int bounce_num)
{
  Color color(0, 0, 0);
  Color weight(1, 1, 1);
  std::vector<Light> lights;
  for (int depth = 0; depth < bounce_num; depth++) {
    lights.clear();
    Vec3 pos;
    const Object *obj = scene.Intersect(ray, pos);
    if (!obj) return color;
    auto mat = obj->Mat();
    const Vec3 n = obj->ClosestNormal(pos);
    // Lights
    for (const auto& tlight : scene.lights_) {
      Vec3 test_pos;
      const Ray test_ray(pos + 0.01 * (tlight->position - pos), (tlight->position - pos).normalized());
      const Object * test_obj = scene.Intersect(test_ray, test_pos);
      if (test_obj && test_obj->Mat()->emissive_) {
        lights.push_back(*tlight);
      }
    }
    // Phong shading
    Color result(0, 0, 0);
    Vec3 norm = n.normalized();
    Vec3 ambient = mat->k_d_ * scene.ambient_light_;
    result += Color(ambient);
    for (const auto& tlight : lights) {
      Vec3 light_dir = (tlight.position - pos).normalized();
      Vec3 view_dir = (ray.ori_ - pos).normalized();
      Vec3 reflect_vector = -light_dir + 2*norm*(norm.dot(light_dir));
      Vec3 half_vector = (light_dir+view_dir).normalized();
      real r2 = (tlight.position - pos).dot(tlight.position - pos);
      Vec3 diffuse = mat->k_d_*(tlight.intensity/r2);
      diffuse *= std::max(0.0f,light_dir.dot(norm));
      Vec3 specular = mat->k_s_*(tlight.intensity/r2);
      specular *= std::pow(std::max(0.0f, norm.dot(half_vector)), mat->alpha_);
      result += Color(diffuse + specular);
    } 
    // accumulate color
    Color R = mat->k_s_ * 0.5;
    color += weight * (Color(1, 1, 1) - R) * result;
    weight *= R;
    // generate new ray
    Vec3 I = ray.dir_.normalized();
    ray.dir_ = I - 2 * norm *(norm.dot(I) );   
    ray.ori_ = pos+ray.dir_*0.01;
  }
  return color;
}

Color PathTrace(const Scene &scene, Ray ray, int bounce_num)
{
  Color weight(1, 1, 1);
  Color color(1, 1, 1);
  for (int depth = 0; depth < 10; depth++) {
    Vec3 pos;
    const Object *obj = scene.Intersect(ray, pos);
    if (!obj) {
      return Color(0,0,0);
    }
    if (obj->Mat()->emissive_) {
      return color * obj->Mat()->k_d_;
    }
    Vec3 wi = (ray.ori_ - pos).normalized();
    ray.dir_ = Sample(obj->Mat(),obj->ClosestNormal(pos),wi,weight);
    ray.ori_ = pos+0.01*ray.dir_;
    
    if (!weight.any()) return weight;
    else {
      color *= weight;
    }
  }
  return Color(0,0,0);
}
}