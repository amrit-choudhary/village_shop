#include "village_game.h"

#include "util.h"

ME::VillageGame::VillageGame() {}

ME::VillageGame::~VillageGame() {}

void ME::VillageGame::Init(ME::Time::TimeManager* currentTimeManager) {
    ME::Game::Init(currentTimeManager);
    ME::Log("Village Game Start!");

    framesPerDay = 10;
    frameCount = 0;
    day = 0;

    cost = FP{10.0f};
    const char* seed1 = "cost_seed";
    costRandom = Random(seed1);

    price = FP{15.0f};
    const char* seed2 = "price_seed";
    priceRandom = Random(seed2);

    supply = FP{100'000};
    demand = FP{100.0f};

    shop = Shop{};
    shop.level = 1;
    shop.cash = FP{1'000.0f};
    shop.stock = FP{10.0f};
    shop.preference = FP{1.0f};
    shop.discout = FP{0.0f};
    shop.pnl = FP{0.0f};
    shop.loan = FP{0.0f};
    shop.interest = FP{0.0f};
}

void ME::VillageGame::Update(double deltaTime) {
    ME::Game::Update(deltaTime);

    ++frameCount;
    if (frameCount >= framesPerDay) {
        frameCount = 0;
        DayChange();
    }
}

void ME::VillageGame::End() {
    ME::Log("Village Game End!");
    ME::Game::End();
}

void ME::VillageGame::DayChange() {
    ++day;

    // Cost.
    float costVar = costRandom.NextRange(0, 200);
    costVar = (costVar - 100.0f) / 100.0f;
    cost += costVar;

    // Price.
    float priceVar = priceRandom.NextRange(0, 200);
    priceVar = (priceVar - 100.0f) / 100.0f;
    price += priceVar;

    // Shop.

    std::cout << "\x1b[2J";
    std::cout << "Day: " << day << '\n';
    std::cout << "Cost: " << cost.ToFloat() << "  (" << costVar << ")\n";
    std::cout << "Price: " << price.ToFloat() << "  (" << priceVar << ")\n";
    std::cout << "\x1b[H";
}
