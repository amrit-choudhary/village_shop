# The Village Shop

<p align="center">
<img src="/screenshots/survivors_final.gif" width="700px" />
</p>

<p align="center">
<img src="/screenshots/2025_06_13.gif" width="500px" />
</p>

A multiplayer village-shop simulation. Run a shop in a village where every villager is simulated with
their own needs (food, shelter, luxury goods), compete with AI shops and other players, and watch the
village grow or shrink with how well it is served. Design notes: [design/v1/design_v1.md](design/v1/design_v1.md).

Built on the [Moksha engine](https://github.com/amrit-choudhary/moksha). A fresh start on the engine: the
client opens the village window and the server accepts players; the simulation comes next. The screenshots
are from the earlier prototype.

## Building
Requires CMake 3.31+ and a C++20 compiler; on Windows the resource cook also needs `dxc` and `texconv` on
`PATH`.

```
cmake --preset pinned
cmake --build --preset pinned
```

This builds `VillageShop_Client` and `VillageShop_Server` against the engine commit in `MOKSHA_PIN` (root
`CMakeLists.txt`). Server-only machines (Linux, Raspberry Pi): add `-DMOKSHA_BUILD_CLIENT=OFF` and build the
`VillageShop_Server` target. The game server listens on UDP 9310 (`server/resources/config/settings.ini`).

To work on the engine at the same time, see the engine README ("Starting a new game", daily workflow): a
gitignored `CMakeUserPresets.json` with a `live` preset builds your local engine clone at
`../../moksha/moksha`. Open `village_shop.code-workspace` to have both in one VS Code window.

## License
MIT, see [LICENSE](LICENSE).
