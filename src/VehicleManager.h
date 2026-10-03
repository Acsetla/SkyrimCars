#pragma once

#include "VehicleController.h"

namespace SkyrimCars
{
    enum class VehicleType : std::uint8_t
    {
        kMercedes,
        kJeep
    };

    class VehicleManager
    {
    public:
        static VehicleManager* GetSingleton();

        void Initialize();
        void Update(float deltaSeconds);

        bool ToggleVehicle(VehicleType type);
        void DeleteActiveVehicle();

        bool HasActiveVehicle() const { return _vehicle != nullptr; }

    private:
        VehicleManager() = default;

        RE::TESObjectREFR* _vehicle{ nullptr };
        RE::Actor* _driver{ nullptr };
        VehicleType _type{ VehicleType::kMercedes };
        VehicleController _controller;
        bool _initialized{ false };
    };
}
