#pragma once

#include "Noises.h"

FastNoiseLite* gen;

void initializeNoise3d(int seed) {
	gen = new FastNoiseLite(seed);
	gen->SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
	gen->SetFrequency(0.05);
	gen->SetFractalOctaves(10);
}

float getNoise3d(float x, float y, float z) {
	float ab = gen->GetNoise(x, y);
	float bc = gen->GetNoise(y, z);
	float ac = gen->GetNoise(x, z);

	float ba = gen->GetNoise(y, x);
	float cb = gen->GetNoise(z, y);
	float ca = gen->GetNoise(z, z);

	return (ab + bc + ac + ba + cb + ca) / 6.0f;
}
