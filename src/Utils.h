#pragma once

#include <random>
#include <chrono>

class Utils
{
private:
	static std::mt19937 mt;

public:
	Utils() = delete;
	static int getRand(int min, int max);
};
