cmake_minimum_required(VERSION 3.25)
project(SkyrimCars VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CommonLibSSE CONFIG REQUIRED)

set(SOURCES
    src/Plugin.cpp
    src/InputManager.cpp
    src/VehicleManager.cpp
    src/VehicleController.cpp
)

set(HEADERS
    src/PCH.h
    src/InputManager.h
    src/VehicleManager.h
    src/VehicleController.h
)

add_commonlibsse_plugin(${PROJECT_NAME}
    AUTHOR Acsetla
    COMPATIBLE_RUNTIMES 1.6.1170
    SOURCES ${SOURCES} ${HEADERS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/SkyrimCars")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${OUTPUT_FOLDER}/SKSE/Plugins"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:${PROJECT_NAME}>"
            "${OUTPUT_FOLDER}/SKSE/Plugins/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )
endif()
