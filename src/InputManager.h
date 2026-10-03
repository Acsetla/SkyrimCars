#pragma once

#include <RE/Skyrim.h>

#include <mutex>

namespace SkyrimCars
{
    class InputManager final : public RE::BSTEventSink<RE::InputEvent*>
    {
    public:
        static InputManager* GetSingleton();

        void Install();
        RE::BSEventNotifyControl ProcessEvent(
            RE::InputEvent* const* events,
            RE::BSTEventSource<RE::InputEvent*>* dispatcher) override;

        bool ToggleRequested();
        bool EnterRequested();

    private:
        bool _ctrlDown{ false };
        bool _toggleRequested{ false };
        bool _enterRequested{ false };
        std::mutex _mutex;
    };
}
