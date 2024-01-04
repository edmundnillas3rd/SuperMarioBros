#include "Pathfinding.h"
#include <iostream>
#include <cmath>

float MoveTowards(float current, float target, float maxDelta)
{
	if (std::fabs(target - current) <= maxDelta)
	{
		return target;
	}

	float length = std::sqrt(std::pow(target, 2.0f) - std::pow(current, 2.0f));
	int normalized = length / length;

	return current + normalized * maxDelta;
}