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

ME::Color ME::Color::RandomColorPretty() {
    ME::Random random;
    float h = random.NextDouble();
    float s = 0.7f + 0.3f * random.NextDouble();
    float v = 0.6f + 0.4f * random.NextDouble();
    return FromHSV(h, s, v);
}

ME::Color ME::Color::RandomColorPretty(ME::Random& random) {
    float h = random.NextDouble();
    float s = 0.7f + 0.3f * random.NextDouble();
    float v = 0.6f + 0.4f * random.NextDouble();
    return FromHSV(h, s, v);
}

ME::Color ME::Color::FromHSV(float h, float s, float v) {
    float r, g, b;
    int i = int(h * 6.0f);
    float f = h * 6.0f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);
    switch (i % 6) {
        case 0:
            r = v, g = t, b = p;
            break;
        case 1:
            r = q, g = v, b = p;
            break;
        case 2:
            r = p, g = v, b = t;
            break;
        case 3:
            r = p, g = q, b = v;
            break;
        case 4:
            r = t, g = p, b = v;
            break;
        case 5:
            r = v, g = p, b = q;
            break;
    }

    return ME::Color(r, g, b, 1.0f);
}
