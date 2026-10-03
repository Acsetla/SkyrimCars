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

        bool Forward() const;
        bool Backward() const;
        bool Left() const;
        bool Right() const;
        bool Handbrake() const;

    private:
        bool _ctrlDown{ false };
        bool _toggleRequested{ false };
        bool _enterRequested{ false };

        bool _forward{ false };
        bool _backward{ false };
        bool _left{ false };
        bool _right{ false };
        bool _handbrake{ false };

        mutable std::mutex _mutex;
    };
}
