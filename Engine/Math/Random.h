#pragma once

namespace nc
{
	void SeedRandom(unsigned int seed);

	float Random(); // 0 to 1

	float RandomRange(float min, float max);

	int RandomInt();
	int RandomInt(int max);

	int RandomRangeInt(int min, int max);

}