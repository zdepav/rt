#include "src/Image.hpp"
#include "src/Material.hpp"
#include "src/Scene.hpp"
#include "src/Vec3.hpp"
#include "src/shapes/Floor.hpp"
#include "src/shapes/Mesh.hpp"
#include "src/shapes/Sphere.hpp"
#include "src/shapes/Triangle.hpp"
#include "src/texture/ImageTexture.hpp"

void winter_scene() {
    // textures
    ImageTexture skybox_texture("assets/snowy_park.png", TilingMode::TILING_MODE_REPEAT);
    ImageTexture tree_texture("assets/tree.png");
    ImageTexture snow_texture(
        "assets/noise.png",
        TilingMode::TILING_MODE_REPEAT,
        TilingMode::TILING_MODE_REPEAT
    );

    // materials
    Material ground_mat, tree_mat;
    ground_mat.texture = &snow_texture;
    tree_mat.texture = &tree_texture;
    Material::DEFAULT.color = Vec3::UNIT;

    // scene
    Scene scene;
    scene.shape(std::make_unique<Floor>(&ground_mat, 0.0f));
    scene.shape(std::make_unique<Mesh>(&tree_mat, "assets/tree.obj"));
    //scene.skybox = &skybox_texture;
    scene.light_direction = Vec3(1.0f, -1.0f, 1.0f).normalize();
    scene.light_color = Vec3(1.0f, 1.0f, 1.0f);
    scene.ambient_light = Vec3(0.2f, 0.2f, 0.2f);
    scene.light_color = Vec3::ORIGIN;
    scene.ambient_light = Vec3::UNIT;

    // rendering
    Camera camera(Vec3(-15.0f, 6.0f, 0.0f), Vec3(0.0f, 5.0f, 0.0f), 60.0f);
    auto img = scene.render(camera, 1024, 1024, 2);
    img->save("out/winter.png");
}

/** 7 floating spheres with different material parameters in a scene with a skybox */
void sphere_scene() {
    // textures
    ImageTexture cozy_texture("assets/sundowner_deck.png");

    // materials
    Material bauble_mat[7];
    bauble_mat[0].glossy(1.0f);
    bauble_mat[1].colored(Vec3(1.0f, 0.2f, 0.2f)).glossy(0.8f).rough(0.5f);
    bauble_mat[2].colored(Vec3(1.0f, 1.0f, 0.2f)).glossy(0.7f).rough(0.75f);
    bauble_mat[3].colored(Vec3(0.2f, 1.0f, 0.2f)).glossy(0.9f).rough(0.25f);
    bauble_mat[4].colored(Vec3(0.2f, 1.0f, 1.0f)).glossy(0.8f).rough(1.0f);
    bauble_mat[5].colored(Vec3(0.2f, 0.2f, 1.0f)).glossy(0.7f).rough(0.5f);
    bauble_mat[6].colored(Vec3(1.0f, 0.2f, 1.0f)).glossy(0.9f).rough(0.75f);

    // scene
    Scene scene;
    scene.shape(std::make_unique<Sphere>(&bauble_mat[0], Vec3(0.0f,  0.0f,  0.0f), 3.0f));
    scene.shape(std::make_unique<Sphere>(&bauble_mat[1], Vec3(0.0f,  7.0f, -1.0f), 3.0f));
    scene.shape(std::make_unique<Sphere>(&bauble_mat[2], Vec3(0.0f,  3.0f, -7.0f), 3.0f));
    scene.shape(std::make_unique<Sphere>(&bauble_mat[3], Vec3(0.0f, -4.0f, -6.0f), 3.0f));
    scene.shape(std::make_unique<Sphere>(&bauble_mat[4], Vec3(0.0f, -7.0f,  1.0f), 3.0f));
    scene.shape(std::make_unique<Sphere>(&bauble_mat[5], Vec3(0.0f, -3.0f,  7.0f), 3.0f));
    scene.shape(std::make_unique<Sphere>(&bauble_mat[6], Vec3(0.0f,  4.0f,  6.0f), 3.0f));
    scene.skybox = &cozy_texture;
    scene.light_direction = Vec3(-1.0f, -1.0f, 1.0f).normalize();
    scene.light_color = Vec3(2.0f, 2.0f, 2.0f);
    scene.ambient_light = Vec3(0.5f, 0.5f, 0.5f);

    // rendering
    Camera camera(Vec3(20.0f, 0.0f, 0.0f), Vec3::ORIGIN, 60.0f);
    printf("Rendering \"spheres\"\n");
    auto img = scene.render(camera, 1024, 1024, 2);
    img->save("out/spheres.png");
}

/** scene showing a box made of mirrors from the inside, chrismas scene is shown on the wall behind
 * the camera, "Merry Christmas" text on the mirror in front of the camera
 */
void reflect_box_scene() {
    const float DIST = 2.0f;

    // textures
    ImageTexture text_texture("assets/christmas-text.png");
    ImageTexture outdoor_texture("assets/christmas-img.png");
    ImageTexture wood_texture(
        "assets/wood.png", TilingMode::TILING_MODE_REPEAT, TilingMode::TILING_MODE_REPEAT
    );

    // materials
    Material back_mat(Vec3::UNIT, &outdoor_texture);
    Material mirror_mat(Vec3(0.5f, 0.75f, 1.0f));
    mirror_mat.glossiness = 1.0f;//0.8f;
    mirror_mat.roughness = 0.4f;
    Material wood_mat(Vec3::UNIT, &wood_texture);
    Material text_mat(Vec3::UNIT, &text_texture);
    text_mat.glossiness = 0.5f;

    // scene
    Scene scene;
    scene.light_color = Vec3::ORIGIN;
    scene.ambient_light = Vec3::UNIT;

    // scene - front
    scene.shape(std::make_unique<Triangle>(
        &text_mat, Vec3(DIST, 1.0f, -1.0f), Vec3(DIST, 1.0f, 1.0f), Vec3(DIST, -1.0f, -1.0f),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::ORIGIN, Vec2::UNIT_X, Vec2::UNIT_Y
    ));
    scene.shape(std::make_unique<Triangle>(
        &text_mat, Vec3(DIST, -1.0f, -1.0f), Vec3(DIST, 1.0f, 1.0f), Vec3(DIST, -1.0f, 1.0f),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::UNIT_Y, Vec2::UNIT_X, Vec2::UNIT
    ));

    // scene - back
    scene.shape(std::make_unique<Triangle>(
        &back_mat, Vec3(-1.0f, 1.0f, 1.0f), Vec3(-1.0f, 1.0f, -1.0f), Vec3(-1.0f, -1.0f, 1.0f),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::ORIGIN, Vec2::UNIT_X, Vec2::UNIT_Y
    ));
    scene.shape(std::make_unique<Triangle>(
        &back_mat, Vec3(-1.0f, -1.0f, 1.0f), Vec3(-1.0f, 1.0f, -1.0f), Vec3(-1.0f, -1.0f, -1.0f),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::UNIT_Y, Vec2::UNIT_X, Vec2::UNIT
    ));

    // scene - floor
    scene.shape(std::make_unique<Triangle>(
        &wood_mat, Vec3(DIST, -1.0f, -1.0f), Vec3(DIST, -1.0f, 1.0f), Vec3(-1.0f, -1.0f, -1.0f),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT,
        Vec2::ORIGIN, Vec2::UNIT_X, Vec2(0.0f, DIST * 0.5f + 0.5f)
    ));
    scene.shape(std::make_unique<Triangle>(
        &wood_mat, Vec3(-1.0f, -1.0f, -1.0f), Vec3(DIST, -1.0f, 1.0f), Vec3(-1.0f, -1.0f, 1.0f),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT,
        Vec2(0.0f, DIST * 0.5f + 0.5f), Vec2::UNIT_X, Vec2(1.0f, DIST * 0.5f + 0.5f)
    ));

    // scene - ceiling
    scene.shape(std::make_unique<Triangle>(
        &mirror_mat, Vec3(DIST, 1.0f, -1.0f), Vec3(DIST, 1.0f, 1.0f), Vec3(-1.0f, 1.0f, -1.0f)
    ));
    scene.shape(std::make_unique<Triangle>(
        &mirror_mat, Vec3(-1.0f, 1.0f, -1.0f), Vec3(DIST, 1.0f, 1.0f), Vec3(-1.0f, 1.0f, 1.0f)
    ));

    // scene - left wall
    scene.shape(std::make_unique<Triangle>(
        &mirror_mat, Vec3(-1.0f, 1.0f, -1.0f), Vec3(DIST, 1.0f, -1.0f), Vec3(-1.0f, -1.0f, -1.0f)
    ));
    scene.shape(std::make_unique<Triangle>(
        &mirror_mat, Vec3(-1.0f, -1.0f, -1.0f), Vec3(DIST, 1.0f, -1.0f), Vec3(DIST, -1.0f, -1.0f)
    ));

    // scene - right wall
    scene.shape(std::make_unique<Triangle>(
        &mirror_mat, Vec3(DIST, 1.0f, 1.0f), Vec3(-1.0f, 1.0f, 1.0f), Vec3(DIST, -1.0f, 1.0f)
    ));
    scene.shape(std::make_unique<Triangle>(
        &mirror_mat, Vec3(DIST, -1.0f, 1.0f), Vec3(-1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, 1.0f)
    ));

    // rendering
    Camera camera(Vec3::ORIGIN, Vec3(DIST, 0.0f, 0.0f), 60.0f);
    printf("Rendering \"reflect box\"\n");
    auto img = scene.render(camera, 1024, 1024, 2);
    img->save("out/reflect_box.png");
}

int main() {
    sphere_scene();
    reflect_box_scene();
    winter_scene();
    return 0;
}
