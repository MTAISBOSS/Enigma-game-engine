cmake_minimum_required(VERSION 3.20)
project(Enigma)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Enigma
        src/Enigma.cpp
        src/Game/Game.cpp
        src/Game/Game.h
        src/Enigma.cpp
        src/Logger/Logger.cpp
        src/Logger/Logger.h
        src/Components/TransformComponent.h
        src/Systems/MovementSystem.h
        src/ECS/ECS.h
        src/ECS/ECS.cpp
        src/Components/RigidBodyComponent.h
        src/Systems/RenderSystem.h
        src/Components/SpriteComponent.h
        src/AssetStore/AssetStore.cpp
        src/AssetStore/AssetStore.h
)

target_include_directories(Enigma PRIVATE
        "packages/imgui"
        "packages/lua"
        "packages/sol"
        "packages/glm.1.0.3/build/native/include"
        "packages/sdl2.nuget.2.32.8/build/native/include"
        "packages/sdl2_image.nuget.2.8.8/build/native/include"
        "packages/sdl2_mixer.nuget.2.8.1/build/native/include"
        "packages/sdl2_ttf.nuget.2.24.0/build/native/include"
        "packages/spdlog/include"
)

target_link_libraries(Enigma PRIVATE
        "${CMAKE_SOURCE_DIR}/packages/sdl2.nuget.2.32.8/build/native/lib/x64/SDL2main.lib"
        "${CMAKE_SOURCE_DIR}/packages/sdl2.nuget.2.32.8/build/native/lib/x64/SDL2.lib"
        "${CMAKE_SOURCE_DIR}/packages/sdl2_image.nuget.2.8.8/build/native/lib/x64/SDL2_image.lib"
        "${CMAKE_SOURCE_DIR}/packages/sdl2_mixer.nuget.2.8.1/build/native/lib/x64/SDL2_mixer.lib"
        "${CMAKE_SOURCE_DIR}/packages/sdl2_ttf.nuget.2.24.0/build/native/lib/x64/SDL2_ttf.lib"
)


file(COPY
        "packages/sdl2.nuget.redist.2.32.8/build/native/bin/x64/sdl2.dll"
        DESTINATION ${CMAKE_BINARY_DIR}
)
