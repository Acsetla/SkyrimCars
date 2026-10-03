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
        logger::info("SkyrimCars: VehicleManager initialized");
        RE::DebugNotification("SkyrimCars: vehicle system initialized");
    }

    void VehicleManager::Update(float deltaSeconds)
    {
        if (!_initialized) {
            return;
        }

        if (InputManager::GetSingleton()->ToggleRequested()) {
            if (_vehicle) {
                DeleteActiveVehicle();
            } else {
                ToggleVehicle(VehicleType::kMercedes);
            }
        }

        if (_vehicle && _driver) {
            VehicleInput input{};
            _controller.Tick(_driver, _vehicle, deltaSeconds, input);
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

        // Actual NIF/Havok reference creation is the next milestone.
        // We deliberately do not fake a drivable car by teleporting a
        // static object through the world.
        logger::info(
            "SkyrimCars: vehicle spawn requested, type={}",
            static_cast<int>(_type));

        RE::DebugNotification(
            "SkyrimCars: car asset/physics pipeline is next");

        return false;
    }

    void VehicleManager::DeleteActiveVehicle()
    {
        if (_vehicle) {
            _vehicle->Disable();
            _vehicle = nullptr;
        }

        _driver = nullptr;
        _controller.Reset();
        RE::DebugNotification("SkyrimCars: vehicle deleted");
    }
}
