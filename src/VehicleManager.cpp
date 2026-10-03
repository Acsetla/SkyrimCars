#include "VehicleManager.h"

#include "InputManager.h"

namespace SkyrimCars
{
    VehicleManager* VehicleManager::GetSingleton()
    {
        static VehicleManager instance;
        return std::addressof(instance);
    }

    void VehicleManager::Initialize()
    {
        if (_initialized) {
            return;
        }

        _initialized = true;
        SKSE::log::info("SkyrimCars: VehicleManager initialized");
        RE::DebugNotification("SkyrimCars: Ctrl+I = car stub");
    }

    void VehicleManager::Update(float deltaSeconds)
    {
        if (!_initialized) {
            return;
        }

        auto* input = InputManager::GetSingleton();

        if (input->ToggleRequested()) {
            if (_vehicle) {
                DeleteActiveVehicle();
            } else {
                ToggleVehicle(VehicleType::kMercedes);
            }
        }

        if (input->EnterRequested()) {
            if (_vehicle && !_driver) {
                _driver = RE::PlayerCharacter::GetSingleton();
                RE::DebugNotification("SkyrimCars: driving ON (model stub)");
            } else if (_driver) {
                _driver = nullptr;
                _controller.Reset();
                RE::DebugNotification("SkyrimCars: driving OFF");
            }
        }

        if (_vehicle && _driver) {
            VehicleInput vehicleInput{};

            if (input->Forward()) {
                vehicleInput.throttle += 1.0F;
            }
            if (input->Backward()) {
                vehicleInput.throttle -= 1.0F;
            }
            if (input->Left()) {
                vehicleInput.steering -= 1.0F;
            }
            if (input->Right()) {
                vehicleInput.steering += 1.0F;
            }

            vehicleInput.handbrake = input->Handbrake();

            _controller.Tick(
                _driver,
                _vehicle,
                deltaSeconds,
                vehicleInput);
        }
    }

    bool VehicleManager::ToggleVehicle(VehicleType type)
    {
        if (_vehicle) {
            return false;
        }

        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return false;
        }

        _type = type;
        _vehicle = player;
        _driver = nullptr;
        _stubVehicle = true;
        _controller.Reset();

        SKSE::log::info(
            "SkyrimCars: model-free vehicle stub activated, type={}",
            static_cast<int>(_type));

        RE::DebugNotification(
            "SkyrimCars: car stub ready - press E, then W/A/S/D");

        return true;
    }

    void VehicleManager::DeleteActiveVehicle()
    {
        // The v1 test stub uses the player as the temporary vehicle reference,
        // so never Disable() this reference. Real car references will be
        // disabled/removed here once the NIF/Havok assets are installed.
        _vehicle = nullptr;
        _driver = nullptr;
        _stubVehicle = false;
        _controller.Reset();
        RE::DebugNotification("SkyrimCars: vehicle deleted");
    }
}
