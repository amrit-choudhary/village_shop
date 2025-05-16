#include "color.h"

#include "../../../../shared/src/random/random_engine.h"

ME::Color ME::Color::RandomColor() {
    ME::Random random;
    float r = random.NextDouble();
    float g = random.NextDouble();
    float b = random.NextDouble();
    float a = 1.0f;
    return Color{r, g, b, a};
}

ME::Color ME::Color::RandomColor(ME::Random& random) {
    float r = random.NextDouble();
    float g = random.NextDouble();
    float b = random.NextDouble();
    float a = 1.0f;
    return Color{r, g, b, a};
}
