#pragma once
#include <random>

namespace Utils
{
    namespace Random
    {
        float generateRandomNumber(float a, float b) {
            // Tworzenie generatora liczb pseudolosowych
            std::random_device rd; // Urz¹dzenie do generowania seed'a
            std::mt19937 gen(rd()); // Generator Mersenne Twister u¿ywaj¹cy seed'a z random_device
            std::uniform_real_distribution<float> distrib(a, b); // Dystrybucja równomierna typu float od a do b

            // Generowanie i zwracanie losowej liczby
            return distrib(gen);
        }
    }
}