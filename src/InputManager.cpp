#include "InputManager.h"

namespace SkyrimCars
{
    InputManager* InputManager::GetSingleton()
    {
        static InputManager instance;
        return std::addressof(instance);
    }

    void InputManager::Install()
    {
        if (auto* input = RE::BSInputDeviceManager::GetSingleton()) {
            input->AddEventSink(this);
            logger::info("SkyrimCars: InputManager installed");
        } else {
            logger::error("SkyrimCars: BSInputDeviceManager unavailable");
        }
    }

    RE::BSEventNotifyControl InputManager::ProcessEvent(
        RE::InputEvent* const* events,
        RE::BSTEventSource<RE::InputEvent*>*)
    {
        if (!events) {
            return RE::BSEventNotifyControl::kContinue;
        }

        std::scoped_lock lock(_mutex);

        for (auto* event = *events; event; event = event->next) {
            if (event->GetEventType() != RE::INPUT_EVENT_TYPE::kButton) {
                continue;
            }

            const auto* button = event->AsButtonEvent();
            if (!button || button->GetDevice() != RE::INPUT_DEVICE::kKeyboard) {
                continue;
            }

            const auto id = button->GetIDCode();
            const bool down = button->IsDown();

            // DirectInput keyboard scan codes used by Skyrim:
            // Ctrl=29, E=18, W=17, A=30, S=31, D=32, Space=57.
            switch (id) {
            case 29:
                _ctrlDown = down;
                break;
            case 23: // I
                if (down && _ctrlDown) {
                    _toggleRequested = true;
                }
                break;
            case 18: // E
                if (down) {
                    _enterRequested = true;
                }
                break;
            case 17:
                _forward = down;
                break;
            case 31:
                _backward = down;
                break;
            case 30:
                _left = down;
                break;
            case 32:
                _right = down;
                break;
            case 57:
                _handbrake = down;
                break;
            default:
                break;
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    bool InputManager::ToggleRequested()
    {
        std::scoped_lock lock(_mutex);
        const bool result = _toggleRequested;
        _toggleRequested = false;
        return result;
    }

    bool InputManager::EnterRequested()
    {
        std::scoped_lock lock(_mutex);
        const bool result = _enterRequested;
        _enterRequested = false;
        return result;
    }

    bool InputManager::Forward() const
    {
        std::scoped_lock lock(_mutex);
        return _forward;
    }

    bool InputManager::Backward() const
    {
        std::scoped_lock lock(_mutex);
        return _backward;
    }

    bool InputManager::Left() const
    {
        std::scoped_lock lock(_mutex);
        return _left;
    }

    bool InputManager::Right() const
    {
        std::scoped_lock lock(_mutex);
        return _right;
    }

    bool InputManager::Handbrake() const
    {
        std::scoped_lock lock(_mutex);
        return _handbrake;
    }
}
