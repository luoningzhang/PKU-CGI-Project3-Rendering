#pragma once

#include <string>
#include <vector>

#include "graphics/camera.h"
#include "graphics/framebuffer.h"
#include "graphics/platform.h"
#include "graphics/scene.h"

namespace VCL {
enum class BUTTON : unsigned char { Left = 0, Right, Middle, NUM };

class Renderer {
 public:
  VWindow* window_ = nullptr;
  Framebuffer* framebuffer_ = nullptr;
  Camera* camera_ = nullptr;
  Color** buffer = nullptr;
  int camera_mode = 0;
  int light_mode = 0;
  int object_mode = 0;
  bool monte_carlo = false;
  int  bounce_num = 10;


  Scene scene_;

  Vec2f last_mouse_pos_;
  bool button_pressed_[size_t(BUTTON::NUM)] = {};

  int width_ = 800;
  int height_ = 600;
  void Init(const std::string& title, int width, int height);
  void Progress(int &x, int &y, int **cnt, const bool MonteCarlo);
  void MainLoop();
  void Destroy();


  // callbacks
  void MouseBottonCallback(BUTTON button, bool pressed);
};
};  // namespace VCL