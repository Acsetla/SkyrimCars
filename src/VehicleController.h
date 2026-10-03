#pragma once

#include <RE/Skyrim.h>

namespace SkyrimCars
{
    struct VehicleInput
    {
        float throttle{ 0.0F };
        float steering{ 0.0F };
        bool handbrake{ false };
    };

    class VehicleController
    {
    public:
        void Reset();
        void Tick(RE::Actor* driver, RE::TESObjectREFR* vehicle, float deltaSeconds, const VehicleInput& input);
        float Speed() const { return _speed; }

    private:
        float _speed{ 0.0F };
        float _steeringAngle{ 0.0F };

        static constexpr float kMaxForwardSpeed = 44.0F;
        static constexpr float kMaxReverseSpeed = 15.0F;
        static constexpr float kAcceleration = 24.0F;
        static constexpr float kBrakeAcceleration = 42.0F;
        static constexpr float kDrag = 4.0F;
        static constexpr float kSteeringRate = 2.6F;
    };
}
