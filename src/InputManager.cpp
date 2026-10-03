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

            // DirectInput keyboard scan codes:
            // Ctrl=29, I=23, E=18.
            if (id == 29) {
                _ctrlDown = down;
            } else if (id == 23 && down && _ctrlDown) {
                _toggleRequested = true;
            } else if (id == 18 && down) {
                _enterRequested = true;
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
}
