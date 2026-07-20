#include "Utils.h"
#include <math.h>

std::mt19937 Utils::mt = std::mt19937((unsigned int)(std::chrono::steady_clock::now().time_since_epoch().count()));

int Utils::getRand(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}
