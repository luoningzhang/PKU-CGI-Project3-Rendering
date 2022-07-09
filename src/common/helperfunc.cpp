#include "helperfunc.h"

#include <random>

namespace VCL {

unsigned char FloatToUChar(float x) { return (unsigned char)(x * 255.999); }

int LerpInt(int a, int b, float t) { return int(a + (b - a) * t); }

void ConvertColor(Vec4f input, unsigned char output[4]) {
  output[0] = FloatToUChar(input[0]);
  output[1] = FloatToUChar(input[1]);
  output[2] = FloatToUChar(input[2]);
  output[3] = FloatToUChar(input[3]);
}

real rand01() {
	static std::uniform_real_distribution<real> dist;
	static std::mt19937_64 rng;
	return dist(rng);
}

};  // namespace VCL
