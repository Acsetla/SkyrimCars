#include "PCH.h"

#include "InputManager.h"
#include "VehicleManager.h"

namespace
{
    std::atomic_bool g_running{ false };
    std::atomic_bool g_tickQueued{ false };

    void QueueTick()
    {
        if (g_tickQueued.exchange(true, std::memory_order_acq_rel)) {
            return;
        }

        if (auto* task = SKSE::GetTaskInterface()) {
            task->AddTask([]() {
                g_tickQueued.store(false, std::memory_order_release);

                static auto lastTick = std::chrono::steady_clock::now();
                const auto now = std::chrono::steady_clock::now();
                const auto dt = std::chrono::duration<float>(now - lastTick).count();
                lastTick = now;

                SkyrimCars::VehicleManager::GetSingleton()->Update(
                    std::clamp(dt, 0.0F, 0.1F));
            });
        } else {
            g_tickQueued.store(false, std::memory_order_release);
        }
    }

    void StartTickThread()
    {
        if (g_running.exchange(true, std::memory_order_acq_rel)) {
            return;
        }

        std::thread([] {
            while (g_running.load(std::memory_order_acquire)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                QueueTick();
            }
        }).detach();
    }

    void InitializeLogging()
    {
        auto logDir = SKSE::log::log_directory();
        if (!logDir) {
            return;
        }

        *logDir /= "SkyrimCars.log";

        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            logDir->string(), true);
        auto log = std::make_shared<spdlog::logger>(
            "SkyrimCars",
            std::move(sink));

        spdlog::set_default_logger(std::move(log));
        spdlog::set_level(spdlog::level::info);
        spdlog::flush_on(spdlog::level::info);
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitializeLogging();
    SKSE::Init(skse);

    SKSE::log::info(
        "SkyrimCars loading for Skyrim AE/SE runtime 1.6.1170");

    SkyrimCars::InputManager::GetSingleton()->Install();
    SkyrimCars::VehicleManager::GetSingleton()->Initialize();
    StartTickThread();

    SKSE::log::info("SkyrimCars loaded");
    return true;
}
