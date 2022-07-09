#pragma once

#include "graphics/material.h"

namespace VCL {

class Object
{
  public:
    const Material *mat_;

  public:
    Object(const Material *const mat) : mat_(mat) { };
    virtual ~Object() = default;
    const Material *Mat() const { return mat_; }
    virtual real Intersect(const Ray &ray) const = 0;
    virtual Vec3 ClosestNormal(const Vec3 &pos) const = 0;
};

class Plane : public Object
{
  protected:
    const Vec3 pos_;
    const Vec3 n_;
    const Vec3 wh_;

  public:
    Plane(const Material *const mat, const Vec3 &pos, const Vec3 &dir, const Vec3 &wh) :
      Object(mat),
      pos_(pos),
      n_(dir.normalized()),
      wh_(wh)
    { }

    virtual ~Plane() = default;
    virtual real Intersect(const Ray &ray) const override
    {
      // if(n_.dot(ray.dir_)==0)return std::numeric_limits<real>::infinity();
      // real t = n_.dot(pos_ - ray.ori_)/(n_.dot(ray.dir_));
      // if(t<=0.0001f)return std::numeric_limits<real>::infinity();
      // else {
      //   Vec3 tmp = (ray.ori_+ray.dir_*t)-pos_;
      //   if(std::abs(tmp[0])>wh_[0]||std::abs(tmp[1])>wh_[1]||std::abs(tmp[2])>wh_[2]){
      //     return std::numeric_limits<real>::infinity();
      //   }
      //   return t;
      // }
      real num = (pos_ - ray.ori_).dot(n_);
      real den = ray.dir_.dot(n_);
      if (den > -0.00001f||num/den<=0.001f) return std::numeric_limits<real>::infinity();
      Vec3 tmp = (ray.ori_+ray.dir_*(num / den))-pos_;
      if(std::abs(tmp[0])>wh_[0]||std::abs(tmp[1])>wh_[1]||std::abs(tmp[2])>wh_[2]){
         return std::numeric_limits<real>::infinity();
      }
      return (num / den);
    }
    virtual Vec3 ClosestNormal(const Vec3 &position) const { return n_; }
};


class Sphere : public Object
{
  protected:
    const Vec3 cen_;
    const real rad_;

  public:

    Sphere(const Material *const mat, const Vec3 &cen, const real rad) :
      Object(mat),
      cen_(cen),
      rad_(rad)
    { }

    virtual ~Sphere() = default;

    virtual real Intersect(const Ray &ray) const override
    {
      real a = ray.dir_.dot(ray.dir_);
      real b = 2*(ray.dir_).dot(ray.ori_ - cen_);
      real c = (ray.ori_ - cen_).dot(ray.ori_ - cen_) - rad_*rad_;
      if(b*b-4*a*c < 0) return std::numeric_limits<real>::infinity();
      real t1 = (-b+sqrt(b*b-4*a*c))/(2*a);
      real t2 = (-b-sqrt(b*b-4*a*c))/(2*a);
      if(t2>=0.0)return  t2;
      else if(t1>=0.0)return t1;//std::numeric_limits<real>::infinity();
      else return std::numeric_limits<real>::infinity();
    }
    virtual Vec3 ClosestNormal(const Vec3 &pos) const { return (pos - cen_).normalized(); }
  };

}