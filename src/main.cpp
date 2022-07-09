#include <iostream>
#include "renderer/renderer.h"
#include <spdlog/spdlog.h>

using namespace VCL;

int main() {
  spdlog::set_pattern("[%^%l%$] %v");
#ifdef NDEBUG
  spdlog::set_level(spdlog::level::info);
#else
  spdlog::set_level(spdlog::level::debug);
#endif
  Renderer renderer;
  renderer.Init("Visual Computing", 800, 600);
  renderer.MainLoop();
  renderer.Destroy();
  return 0;
}