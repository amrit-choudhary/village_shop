#include "village_game.h"

#include "util.h"

ME::VillageGame::VillageGame() {}

ME::VillageGame::~VillageGame() {}

void ME::VillageGame::Init(ME::Time::TimeManager* currentTimeManager) {
    ME::Game::Init(currentTimeManager);
    ME::Log("Village Game Start!");

    framesPerDay = 180;
    frameCount = 0;
    day = 0;

    buyPrice = FP{10.0f};
    buyPriceAvg = FP{10.0f};
    const char* seed1 = "buy_price_seed";
    buyPriceRandom = Random(seed1, true);

    sellPrice = FP{11.0f};
    sellPriceAvg = FP{11.0f};
    const char* seed2 = "sell_price_seed";
    sellPriceRandom = Random(seed2, true);

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

    if (InputManager::GetKeyDown(ME::Input::KeyCode::A)) {
        BuyStock();
    }

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

void ME::VillageGame::BuyStock() {
    shop.stock += shop.cash / buyPrice;
    shop.cash = FP{0.0f};

    RefreshDisplay();
}

void ME::VillageGame::DayChange() {
    ++day;

    // Cost.
    float costVar = buyPriceRandom.NextRange(0, 200);
    costVar = (costVar - 100.0f) / 100.0f;
    buyPrice += costVar;

    // Price.
    float priceVar = sellPriceRandom.NextRange(0, 200);
    priceVar = (priceVar - 100.0f) / 100.0f;
    sellPrice += priceVar;

    // Shop.
    shop.cash += shop.stock * sellPrice;
    shop.stock = FP{0.0f};

    RefreshDisplay();

    // Create a stringstream to build the string
    std::stringstream ss;
    ss << "Day: " << day << '\n';

    // Get the string as a char*
    char* result = new char[ss.str().length() + 1];  // +1 for null terminator
    strcpy(result, ss.str().c_str());

    // Don't forget to free the memory when done
    connection->SendMessage(result);
    delete[] result;
}

void ME::VillageGame::RefreshDisplay() {
    std::cout << "\x1b[2J";
    std::cout << "Day: " << day << '\n';
    std::cout << "Cost: " << buyPrice.ToFloat() << "  (" << (buyPrice - buyPriceAvg).ToFloat() << ")\n";
    std::cout << "Price: " << sellPrice.ToFloat() << "  (" << (sellPrice - sellPriceAvg).ToFloat() << ")\n";
    std::cout << "Shop\n";
    std::cout << "Stock: " << shop.stock.ToFloat() << '\n';
    std::cout << "Cash: " << shop.cash.ToFloat() << '\n';
    std::cout << "\x1b[H";
}
