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

        const float speedFactor = std::clamp(std::abs(_speed) / 20.0F, 0.0F, 1.0F);
        const float steerTarget = input.steering * 0.65F;
        const float steerDelta = steerTarget - _steeringAngle;
        _steeringAngle += std::clamp(
            steerDelta,
            -kSteeringRate * deltaSeconds,
            kSteeringRate * deltaSeconds);

        // Temporary model-free vehicle implementation.
        // The player is used as the vehicle reference until the NIF/Havok
        // car assets are installed. This lets the complete input/controller
        // pipeline be tested on a clean Skyrim installation.
        const float heading = vehicle->GetAngleZ();
        const float yawRate = _steeringAngle * (1.8F + 2.2F * speedFactor);
        const float newHeading = heading + yawRate * deltaSeconds * (_speed >= 0.0F ? 1.0F : -1.0F);

        auto angle = vehicle->GetAngle();
        angle.z = newHeading;
        vehicle->data.angle = angle;

        if (std::abs(_speed) > 0.01F) {
            const float distance = _speed * deltaSeconds;
            auto position = vehicle->GetPosition();
            position.x += std::sin(newHeading) * distance;
            position.y += std::cos(newHeading) * distance;
            vehicle->SetPosition(position);
        }
    }
}
