#include "VehicleController.h"

#include <algorithm>
#include <cmath>

namespace SkyrimCars
{
    void VehicleController::Reset()
    {
        _speed = 0.0F;
        _steeringAngle = 0.0F;
    }

    void VehicleController::Tick(
        RE::Actor* driver,
        RE::TESObjectREFR* vehicle,
        float deltaSeconds,
        const VehicleInput& input)
    {
        if (!driver || !vehicle || deltaSeconds <= 0.0F) {
            return;
        }

        const float throttle = std::clamp(input.throttle, -1.0F, 1.0F);
        const float targetSpeed = throttle >= 0.0F
            ? throttle * kMaxForwardSpeed
            : throttle * kMaxReverseSpeed;

        const float rate = std::abs(targetSpeed) > std::abs(_speed)
            ? kAcceleration
            : kBrakeAcceleration;

        const float delta = targetSpeed - _speed;
        const float maxStep = rate * deltaSeconds;
        _speed += std::clamp(delta, -maxStep, maxStep);

        if (std::abs(throttle) < 0.01F) {
            const float drag = std::min(std::abs(_speed), kDrag * deltaSeconds);
            _speed -= std::copysign(drag, _speed);
        }

        if (input.handbrake) {
            _speed *= std::max(0.0F, 1.0F - 8.0F * deltaSeconds);
        }

        const float steerTarget = input.steering * 0.65F;
        const float steerDelta = steerTarget - _steeringAngle;
        _steeringAngle += std::clamp(
            steerDelta,
            -kSteeringRate * deltaSeconds,
            kSteeringRate * deltaSeconds);
    }
}
