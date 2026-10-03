#include "PCH.h"

#include "InputManager.h"
#include "VehicleManager.h"

namespace
{
    std::chrono::steady_clock::time_point g_lastTick;

    class UpdateSink final : public RE::BSTEventSink<RE::TESUpdateEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(
            const RE::TESUpdateEvent* event,
            RE::BSTEventSource<RE::TESUpdateEvent>*) override
        {
            if (!event) {
                return RE::BSEventNotifyControl::kContinue;
            }

            const auto now = std::chrono::steady_clock::now();
            if (g_lastTick.time_since_epoch().count() == 0) {
                g_lastTick = now;
            }

            const auto dt =
                std::chrono::duration<float>(now - g_lastTick).count();
            g_lastTick = now;

            SkyrimCars::VehicleManager::GetSingleton()->Update(
                std::clamp(dt, 0.0F, 0.1F));

            return RE::BSEventNotifyControl::kContinue;
        }
    };

    UpdateSink g_updateSink;
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    auto* logDir = SKSE::log::log_directory();
    if (logDir) {
        *logDir /= "SkyrimCars.log";
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            logDir->string(), true);
        auto log = std::make_shared<spdlog::logger>("SkyrimCars", sink);
        spdlog::set_default_logger(log);
        spdlog::set_level(spdlog::level::info);
    }

    SKSE::Init(skse);

    logger::info(
        "SkyrimCars loading for Skyrim AE/SE runtime 1.6.1170");

    SkyrimCars::InputManager::GetSingleton()->Install();
    SkyrimCars::VehicleManager::GetSingleton()->Initialize();

    if (auto* tes = RE::TES::GetSingleton()) {
        tes->AddEventSink(&g_updateSink);
    }

    logger::info("SkyrimCars loaded");
    return true;
}
