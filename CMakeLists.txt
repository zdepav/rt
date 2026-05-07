cmake_minimum_required(VERSION 3.25)
project(rt)
set(CMAKE_CXX_STANDARD 20)

include_directories(lib/)
add_compile_definitions(SPNG_USE_MINIZ SPNG_STATIC)

set(Image
    lib/miniz.c
    lib/spng.c
    src/Image.cpp
    src/utils.cpp
)

set(Texture
    src/texture/AngularGradientTexture.cpp
    src/texture/ColorTexture.cpp
    src/texture/DebugTexture.cpp
    src/texture/GradientTexture.cpp
    src/texture/ImageTexture.cpp
    src/texture/LinearGradientTexture.cpp
    src/texture/RadialGradientTexture.cpp
)

set(Shapes
    src/shapes/Circle.cpp
    src/shapes/Cylinder.cpp
    src/shapes/Floor.cpp
    src/shapes/Mesh.cpp
    src/shapes/Plane.cpp
    src/shapes/Shape.cpp
    src/shapes/Sphere.cpp
    src/shapes/Triangle.cpp
)

set(Raytracer
    src/Camera.cpp
    src/Material.cpp
    src/Scene.cpp
    src/TilingMode.cpp
    src/Vec2.cpp
    src/Vec3.cpp
    ${Shapes}
    ${Texture}
)

add_executable(raycast main.cpp ${Image} ${Raytracer})
