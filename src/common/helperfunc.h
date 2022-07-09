#pragma once

#include "mathtype.h"

namespace VCL {

unsigned char FloatToUChar(float x);
int LerpInt(int a, int b, float t);
void ConvertColor(Vec4f input, unsigned char output[4]);

real rand01();

};
