# Сборка SkyrimCars

Цель: Skyrim Special Edition / Anniversary Edition runtime 1.6.1170.

## Инструменты

1. Visual Studio 2022 с Desktop development with C++.
2. CMake 3.25+.
3. vcpkg.
4. SKSE64 для runtime 1.6.1170.
5. Address Library for SKSE Plugins.
6. Интернет для загрузки CommonLibSSE-NG и зависимостей.

## Конфигурация

Проект использует vcpkg.json и vcpkg-configuration.json с Color-Glass registry.

Пример:

cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

Затем:

cmake --build build --config Release

## Важно

Репозиторий сейчас содержит исходный код проекта, а не готовый DLL-релиз. Следующий шаг — собрать DLL на Windows и подключить реальные NIF/Havok assets.

После появления car asset pipeline DLL будет автоматически копироваться в:

SKSE/Plugins/SkyrimCars.dll
