#include "renderer.h"

#include "common/helperfunc.h"
#include "graphics/globillum.h"

namespace VCL {
void Renderer::Init(const std::string& title, int width, int height) {
  srand(time(0));
  camera_mode = std::rand()%8;
  light_mode = std::rand()%16;
  object_mode = std::rand()%4;
  width_ = width;
  height_ = height;
  InitPlatform();
  window_ = CreateVWindow(title, width_, height_, this);
  framebuffer_ = new Framebuffer(width_, height_);
  buffer = new Color *[height_];
  for (int y = 0; y < height_; y++) {
    buffer[y] = new Color[width_];
    //cnt[y] = new int[width_];
    for (int x = 0; x < width_; x++) {
      buffer[y][x] = Color::Zero();
      //cnt[y][x] = 0;
    }
  }

  camera_ = new Camera;
  const float c_y = 1.5;
  const float c_z = 1.5 + 1.5 * std::sqrt(2);
  // camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
  //                   0.0f*PI_, 0.5f * PI_, Vec3f(0.0f, c_y, 0.0f));
  if(!monte_carlo)printf("Now Witted Style Ray Tracing!\n");
  else printf("Now Monte Carlo Path Tracing!\n");
  printf("camera_mode = %d\n",camera_mode);
  if(camera_mode==0)camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                        0.0f * PI_, 0.5f * PI_, Vec3f(0.0f, 1.5f, -1.0f));
  if(camera_mode==1)camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                        -0.5f * PI_, 0.5f * PI_, Vec3f(-2.0f, 1.5f, -2.0f));
  if(camera_mode==2)camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                        0.5f * PI_, 0.5f * PI_, Vec3f(2.0f, 1.5f, -2.0f));
  if(camera_mode==3)camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                        1.0f * PI_, 0.5f * PI_, Vec3f(0.0f, 1.5f, -3.0f));
  if(camera_mode==4)camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                        0.25f * PI_, 0.5f * PI_, Vec3f(1.2f, 1.5f, -0.8f));
  if(camera_mode==5)camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                        -0.25f * PI_, 0.5f * PI_, Vec3f(-1.2f, 1.5f, -0.8f));
  if(camera_mode==6)camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                        0.75f * PI_, 0.5f * PI_, Vec3f(1.2f, 1.5f, -3.2f));
  if(camera_mode==7)camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                        -0.75f * PI_, 0.5f * PI_, Vec3f(-1.2f, 1.5f, -3.2f));
  // camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, 5.0f,
  //           0.0f, 0.2f * PI_, Vec3f::Zero());

  auto &mats = scene_.mats_;
	auto &objs = scene_.objs_;
  auto &lights = scene_.lights_;
	// Initialize materials.
	mats["ceiling"] = std::make_unique<Material>(Color(32.0f, 32.0f, 255.0f) / 255);
	mats["floor"] = std::make_unique<Material>(Color(32.0f, 255.0f, 32.0f) / 255);
	mats["wall1"] = std::make_unique<Material>(Color(255.0f, 255.0f, 128.0f) / 255);
	mats["wall2"] = std::make_unique<Material>(Color(255.0f, 128.0f, 255.0f) / 255);
	mats["wall3"] = std::make_unique<Material>(Color(128.0f, 255.0f, 255.0f) / 255);
	mats["wall4"] = std::make_unique<Material>(Color(255.0f, 32.0f, 32.0f) / 255);
	mats["light"] = std::make_unique<Material>(Color(3, 3, 3), true);

	mats["wood"] = std::make_unique<Material>(Color(1, 1, 0));
	mats["wood1"] = std::make_unique<Material>(Color(0, 1, 1));
  if(!monte_carlo){
    mats["mirror"] = std::make_unique<Material>(Color(0, 0, 0), Color(.8f, .8f, .8f), 30);
    mats["metal"] = std::make_unique<Material>(Color(0, 0, 0), Color(.8f, .8f, .8f), 30);
  }
  else{
    mats["metal"] = std::make_unique<Material>(Color(0, 0, 0), Color(.8f, .8f, .8f), 30);
    mats["mirror"] = std::make_unique<Material>(Color(37.2f, 24.4f, 13.2f) / 255, Color(.8f, .8f, .8f), -1);
  }


	// Set boundaries.
  float tx=0,ty=0,tz=0,sx=10,sy=10,sz=10;
    
  //////////////////////////////////////////////////////////////////
  tx=0,ty=1.5,tz=-2,sx=2,sy=1.5,sz=2;
  objs.emplace_back(std::make_unique<Plane>(
		mats["floor"].get(),
		Vec3(tx, ty-sy, tz), Vec3(0, 1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["ceiling"].get(),
		Vec3(tx, ty+sy, tz), Vec3(0, -1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wall1"].get(),
		Vec3(tx-sx, ty, tz), Vec3(1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wall2"].get(),
		Vec3(tx+sx, ty, tz), Vec3(-1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["mirror"].get(),
		Vec3(tx-sx*3/4, ty, tz-sz*6/7), Vec3(1, 0, 1), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["mirror"].get(),
		Vec3(tx+sx*3/4, ty, tz-sz*6/7), Vec3(-1, 0, 1), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["mirror"].get(),
		Vec3(tx-sx*3/4, ty, tz+sz*6/7), Vec3(1, 0, -1), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["mirror"].get(),
		Vec3(tx+sx*3/4, ty, tz+sz*6/7), Vec3(-1, 0, -1), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wall3"].get(),
		Vec3(tx, ty, tz-sz), Vec3(0, 0, 1), Vec3(sx,sy,100000)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wall4"].get(),
		Vec3(tx, ty, tz+sz), Vec3(0, 0, -1), Vec3(sx,sy,100000)));


  // Set the light.
	real dl = real(2) / 3;
	real rl = 5;
	real hl = std::sqrt(rl * rl - dl * dl);
  printf("light_mode = %d\n",light_mode);
  {
	objs.emplace_back(std::make_unique<Sphere>(
		mats["light"].get(),
		Vec3(0, 3 + hl, -2), rl));
  lights.emplace_back(std::make_unique<Light>(
    Vec3(0, 3, -2),
    Color(1, 1, 1) * 1.0
  ));
  lights.emplace_back(std::make_unique<Light>(
    Vec3(-0.2, 3, -2.2),
    Color(1, 1, 1) * 1.0
  ));
  lights.emplace_back(std::make_unique<Light>(
    Vec3(-0.2, 3, -1.8),
    Color(1, 1, 1) * 1.0
  ));
  lights.emplace_back(std::make_unique<Light>(
    Vec3(0.2, 3, -2.2),
    Color(1, 1, 1) * 1.0
  ));
  lights.emplace_back(std::make_unique<Light>(
    Vec3(0.2, 3, -1.8),
    Color(1, 1, 1) * 1.0
  ));
  }
  {
	rl = 0.15;
  tx=1.25,ty=2.75,tz=-3.25,sx=0.04,sy=0.25,sz=0.04;
  objs.emplace_back(std::make_unique<Sphere>(
		mats["light"].get(),
		Vec3(tx, ty-sy, tz), rl));
  if(light_mode%2==0){
    printf("Light 1 on!\n");
  lights.emplace_back(std::make_unique<Light>(
    Vec3(tx, ty-sy, tz),
    Color(1, 1, 1) * 1.0
  ));  
  }
	
  objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty-sy, tz), Vec3(0, -1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty+sy, tz), Vec3(0, 1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx-sx, ty, tz), Vec3(-1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx+sx, ty, tz), Vec3(1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty, tz-sz), Vec3(0, 0, -1), Vec3(sx,sy,100000)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty, tz+sz), Vec3(0, 0, 1), Vec3(sx,sy,100000)));
  }
  {
	rl = 0.15;
  tx=1.25,ty=2.75,tz=-0.75,sx=0.04,sy=0.25,sz=0.04;
	objs.emplace_back(std::make_unique<Sphere>(
		mats["light"].get(),
		Vec3(tx, ty-sy, tz), rl));
  if((light_mode/2)%2==0){ 
    printf("Light 2 on!\n");
  lights.emplace_back(std::make_unique<Light>(
    Vec3(tx, ty-sy, tz),
    Color(1, 1, 1) * 1.0
  ));
  }
  objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty-sy, tz), Vec3(0, -1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty+sy, tz), Vec3(0, 1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx-sx, ty, tz), Vec3(-1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx+sx, ty, tz), Vec3(1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty, tz-sz), Vec3(0, 0, -1), Vec3(sx,sy,100000)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty, tz+sz), Vec3(0, 0, 1), Vec3(sx,sy,100000)));
  }
  {
	rl = 0.15;
  tx=-1.25,ty=2.75,tz=-3.25,sx=0.04,sy=0.25,sz=0.04;
	objs.emplace_back(std::make_unique<Sphere>(
		mats["light"].get(),
		Vec3(tx, ty-sy, tz), rl));
  if((light_mode/4)%2==0){
    printf("Light 3 on!\n");
  lights.emplace_back(std::make_unique<Light>(
    Vec3(tx, ty-sy, tz),
    Color(1, 1, 1) * 1.0
  ));
  }
  objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty-sy, tz), Vec3(0, -1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty+sy, tz), Vec3(0, 1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx-sx, ty, tz), Vec3(-1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx+sx, ty, tz), Vec3(1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty, tz-sz), Vec3(0, 0, -1), Vec3(sx,sy,100000)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty, tz+sz), Vec3(0, 0, 1), Vec3(sx,sy,100000)));
  }
  {
	rl = 0.15;
  tx=-1.25,ty=2.75,tz=-0.75,sx=0.04,sy=0.25,sz=0.04;
	objs.emplace_back(std::make_unique<Sphere>(
		mats["light"].get(),
		Vec3(tx, ty-sy, tz), rl));
  if((light_mode/8)%2==0){ 
    printf("Light 4 on!\n");
  lights.emplace_back(std::make_unique<Light>(
    Vec3(tx, ty-sy, tz),
    Color(1, 1, 1) * 1.0
  ));
  }
  objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty-sy, tz), Vec3(0, -1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty+sy, tz), Vec3(0, 1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx-sx, ty, tz), Vec3(-1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx+sx, ty, tz), Vec3(1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty, tz-sz), Vec3(0, 0, -1), Vec3(sx,sy,100000)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood"].get(),
		Vec3(tx, ty, tz+sz), Vec3(0, 0, 1), Vec3(sx,sy,100000)));
  }

	tx=-1,ty=0.3,tz=-3,sx=0.3,sy=0.3,sz=0.3;
  objs.emplace_back(std::make_unique<Plane>(
		mats["metal"].get(),
		Vec3(tx, ty-sy, tz), Vec3(0, -1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["metal"].get(),
		Vec3(tx, ty+sy, tz), Vec3(0, 1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["metal"].get(),
		Vec3(tx-sx, ty, tz), Vec3(-1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["metal"].get(),
		Vec3(tx+sx, ty, tz), Vec3(1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["metal"].get(),
		Vec3(tx, ty, tz-sz), Vec3(0, 0, -1), Vec3(sx,sy,100000)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["metal"].get(),
		Vec3(tx, ty, tz+sz), Vec3(0, 0, 1), Vec3(sx,sy,100000)));



	tx=1,ty=0.3,tz=-1,sx=0.3,sy=0.3,sz=0.3;
  objs.emplace_back(std::make_unique<Plane>(
		mats["wood1"].get(),
		Vec3(tx, ty-sy, tz), Vec3(0, -1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood1"].get(),
		Vec3(tx, ty+sy, tz), Vec3(0, 1, 0), Vec3(sx,100000,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood1"].get(),
		Vec3(tx-sx, ty, tz), Vec3(-1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood1"].get(),
		Vec3(tx+sx, ty, tz), Vec3(1, 0, 0), Vec3(100000,sy,sz)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood1"].get(),
		Vec3(tx, ty, tz-sz), Vec3(0, 0, -1), Vec3(sx,sy,100000)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wood1"].get(),
		Vec3(tx, ty, tz+sz), Vec3(0, 0, 1), Vec3(sx,sy,100000)));

  // Set internal objects.
  objs.emplace_back(std::make_unique<Sphere>(
			mats["wood"].get(),
			Vec3(0.6, real(.7), -2.6), real(.7)));
  objs.emplace_back(std::make_unique<Sphere>(
    mats["metal"].get(),
    Vec3(-0.8, real(.5), -1.2), real(.5)));


  
  scene_.ambient_light_ = Color(0.05, 0.05, 0.05);

}

void Renderer::Progress(int &x, int &y, int **cnt, const bool MonteCarlo) {
  const real dx = real(1) / width_;
	const real dy = real(1) / height_;
  const real lx = dx * x;
  const real ly = dy * y;
  const real sx = lx + rand01() * dx;
  const real sy = ly + rand01() * dy;
  if (!MonteCarlo) {
    buffer[y][x] += (GlobIllum::RayTrace(scene_, camera_->GenerateRay(sx, sy), bounce_num) - buffer[y][x]) / (++cnt[y][x]);    
  }
  else {
    buffer[y][x] += (GlobIllum::PathTrace(scene_, camera_->GenerateRay(sx, sy), bounce_num) - buffer[y][x]) / (++cnt[y][x]);
  }
  int idx = (y * width_ + x) * 4;
  for (int i = 0; i < 3; i++) framebuffer_->color_[idx + i] = std::round(std::pow(std::clamp(buffer[y][x][i], 0.0f, 1.0f), 1 / 2.2f) * 255);
  x++;
  if (x == width_) {
    x=0;y++;
    if (y==height_)y=0;
  }
}

void Renderer::MainLoop() {
  // switch between ray tracing and path tracing
  const bool MonteCarlo = monte_carlo;

  int x;
  int y;

  // Color **buffer = new Color *[height_];
  int **cnt = new int *[height_];

  for (y = 0; y < height_; y++) {
    cnt[y] = new int[width_];
    for (x = 0; x < width_; x++) {
      cnt[y][x] = 0;
    }
  }

  x = y = 0;
  int idx = 0;
  const int buffer_size = height_ * width_;
  int patch_size = 50000;
  patch_size = patch_size > buffer_size ? buffer_size : patch_size;
  while (!window_->should_close_) {
    PollInputEvents();
    # pragma omp parallel for
    for (int i = 0; i < patch_size; ++i) {
      int p = (idx + i) % buffer_size;
      int px = p % width_;
      int py = p / width_;
      Progress(px, py, cnt, MonteCarlo);
    }
    idx = (idx + patch_size) % buffer_size;

    window_->DrawBuffer(framebuffer_);
  }

  for (y = 0; y < height_; y++) {
    delete[] buffer[y];
    delete[] cnt[y];
  }
  delete[] buffer;
  delete[] cnt;
}

void Renderer::Destroy() {
  if (camera_) delete camera_;
  if (framebuffer_) delete framebuffer_;
  window_->Destroy();
  if (window_) delete window_;
  DestroyPlatform();
}
};  // namespace VCL