/**
 * The village shop game.
 */

#pragma once

#include "game.h"

using namespace ME::Math;

namespace ME {

class Shop {
   public:
    uint8_t level;
    FP cash;
    FP stock;
    FP preference;
    FP discout;
    FP pnl;
    FP loan;
    FP interest;
};

class VillageGame : public Game {
   public:
    VillageGame();
    ~VillageGame();

    void Init(ME::Time::TimeManager *currentTimeManager) override;

    void Update(double deltaTime) override;

    void End() override;

   private:
    uint32_t framesPerDay;
    uint32_t frameCount;
    uint32_t day;

    FP buyPrice;
    FP buyPriceAvg;
    Random buyPriceRandom;

    FP sellPrice;
    FP sellPriceAvg;
    Random sellPriceRandom;

    FP supply;
    FP demand;

    Shop shop;

    void DayChange();
    void BuyStock();
    void RefreshDisplay();
};

}  // namespace ME
