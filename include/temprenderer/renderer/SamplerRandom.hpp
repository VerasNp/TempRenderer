#include <random>

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(-0.5, 0.5);
    static std::mt19937 generator;
    return distribution(generator);
}

