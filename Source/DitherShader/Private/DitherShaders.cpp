#include "DitherShaders.h"

IMPLEMENT_GLOBAL_SHADER(FDitherShaderVS, "/DitherShader/Private/DitherShader.usf", "MainVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FDitherShaderPS, "/DitherShader/Private/DitherShader.usf", "MainPS", SF_Pixel);