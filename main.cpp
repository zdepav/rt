#include "src/Image.hpp"
#include "src/Material.hpp"
#include "src/Scene.hpp"
#include "src/Vec3.hpp"
#include "src/shapes/Cylinder.hpp"
#include "src/shapes/Floor.hpp"
#include "src/shapes/Mesh.hpp"
#include "src/shapes/Plane.hpp"
#include "src/shapes/Sphere.hpp"
#include "src/shapes/Triangle.hpp"
#include "src/texture/ImageTexture.hpp"

void winter_scene() {
    // textures
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

    // scene
    Scene scene;
    scene.shape<Floor>(&ground_mat, 0);
    scene.shape<Mesh>(&tree_mat, "assets/tree.obj");
    scene.light_direction = Vec3(1, -1, 1).normalize();
    scene.light_color = Vec3::UNIT;
    scene.ambient_light = Vec3(0.2f, 0.2f, 0.2f);
    scene.light_color = Vec3::ORIGIN;
    scene.ambient_light = Vec3::UNIT;

    // rendering
    Camera camera(Vec3(-15, 6, 0), Vec3(0, 5, 0), 60);
    printf("Rendering \"winter\"\n");
    auto img = scene.render(camera, 1024, 1024, 4);
    img->save("out/winter.png");
}

/** 7 floating spheres with different material parameters in a scene with a skybox */
void sphere_scene() {
    // textures
    ImageTexture cozy_texture("assets/sundowner_deck.png");

    // materials
    Material bauble_mat[7];
    bauble_mat[0].glossy(1);
    bauble_mat[1].colored(Vec3(1, 0.2f, 0.2f)).glossy(0.8f).rough(0.5f);
    bauble_mat[2].colored(Vec3(1, 1, 0.2f)).glossy(0.7f).rough(0.75f);
    bauble_mat[3].colored(Vec3(0.2f, 1, 0.2f)).glossy(0.9f).rough(0.25f);
    bauble_mat[4].colored(Vec3(0.2f, 1, 1)).glossy(0.8f).rough(1);
    bauble_mat[5].colored(Vec3(0.2f, 0.2f, 1)).glossy(0.7f).rough(0.5f);
    bauble_mat[6].colored(Vec3(1, 0.2f, 1)).glossy(0.9f).rough(0.75f);

    // scene
    Scene scene;
    scene.shape<Sphere>(&bauble_mat[0], Vec3(0,  0,  0), 3);
    scene.shape<Sphere>(&bauble_mat[1], Vec3(0,  7, -1), 3);
    scene.shape<Sphere>(&bauble_mat[2], Vec3(0,  3, -7), 3);
    scene.shape<Sphere>(&bauble_mat[3], Vec3(0, -4, -6), 3);
    scene.shape<Sphere>(&bauble_mat[4], Vec3(0, -7,  1), 3);
    scene.shape<Sphere>(&bauble_mat[5], Vec3(0, -3,  7), 3);
    scene.shape<Sphere>(&bauble_mat[6], Vec3(0,  4,  6), 3);
    scene.skybox = &cozy_texture;
    scene.light_direction = Vec3(-1, -1, 1).normalize();
    scene.light_color = Vec3(2, 2, 2);
    scene.ambient_light = Vec3(0.5f, 0.5f, 0.5f);

    // rendering
    Camera camera(Vec3(20, 0, 0), Vec3::ORIGIN, 60);
    printf("Rendering \"spheres\"\n");
    auto img = scene.render(camera, 1024, 1024, 4);
    img->save("out/spheres.png");
}

/** scene showing a box made of mirrors from the inside, chrismas scene is shown on the wall behind
 * the camera, "Merry Christmas" text on the mirror in front of the camera
 */
void reflect_box_scene() {
    constexpr float DIST = 2;

    // textures
    ImageTexture text_texture("assets/christmas-text.png");
    ImageTexture outdoor_texture("assets/christmas-img.png");
    ImageTexture wood_texture(
        "assets/wood.png", TilingMode::TILING_MODE_REPEAT, TilingMode::TILING_MODE_REPEAT
    );

    // materials
    Material back_mat(Vec3::UNIT, &outdoor_texture);
    Material mirror_mat(Vec3(0.5f, 0.75f, 1));
    mirror_mat.glossiness = 1;//0.8f;
    mirror_mat.roughness = 0.4f;
    Material wood_mat(Vec3::UNIT, &wood_texture);
    Material text_mat(Vec3::UNIT, &text_texture);
    text_mat.glossiness = 0.5f;

    // scene
    Scene scene;
    scene.light_color = Vec3::ORIGIN;
    scene.ambient_light = Vec3::UNIT;

    // scene - front
    scene.shape<Triangle>(
        &text_mat, Vec3(DIST, 1, -1), Vec3(DIST, 1, 1), Vec3(DIST, -1, -1),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::ORIGIN, Vec2::UNIT_X, Vec2::UNIT_Y
    );
    scene.shape<Triangle>(
        &text_mat, Vec3(DIST, -1, -1), Vec3(DIST, 1, 1), Vec3(DIST, -1, 1),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::UNIT_Y, Vec2::UNIT_X, Vec2::UNIT
    );

    // scene - back
    scene.shape<Triangle>(
        &back_mat, Vec3(-1, 1, 1), Vec3(-1, 1, -1), Vec3(-1, -1, 1),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::ORIGIN, Vec2::UNIT_X, Vec2::UNIT_Y
    );
    scene.shape<Triangle>(
        &back_mat, Vec3(-1, -1, 1), Vec3(-1, 1, -1), Vec3(-1, -1, -1),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::UNIT_Y, Vec2::UNIT_X, Vec2::UNIT
    );

    // scene - floor
    scene.shape<Triangle>(
        &wood_mat, Vec3(DIST, -1, -1), Vec3(DIST, -1, 1), Vec3(-1, -1, -1),
        Vec3::UNIT, Vec3::UNIT, Vec3::UNIT, Vec2::ORIGIN, Vec2::UNIT_X, Vec2(0, DIST * 0.5f + 0.5f)
    );
    scene.shape<Triangle>(
        &wood_mat, Vec3(-1, -1, -1), Vec3(DIST, -1, 1), Vec3(-1, -1, 1), Vec3::UNIT, Vec3::UNIT,
        Vec3::UNIT, Vec2(0, DIST * 0.5f + 0.5f), Vec2::UNIT_X, Vec2(1, DIST * 0.5f + 0.5f)
    );

    // scene - ceiling
    scene.shape<Triangle>(&mirror_mat, Vec3(DIST, 1, -1), Vec3(DIST, 1, 1), Vec3(-1, 1, -1));
    scene.shape<Triangle>(&mirror_mat, Vec3(-1, 1, -1), Vec3(DIST, 1, 1), Vec3(-1, 1, 1));

    // scene - left wall
    scene.shape<Triangle>(&mirror_mat, Vec3(-1, 1, -1), Vec3(DIST, 1, -1), Vec3(-1, -1, -1));
    scene.shape<Triangle>(&mirror_mat, Vec3(-1, -1, -1), Vec3(DIST, 1, -1), Vec3(DIST, -1, -1));

    // scene - right wall
    scene.shape<Triangle>(&mirror_mat, Vec3(DIST, 1, 1), Vec3(-1, 1, 1), Vec3(DIST, -1, 1));
    scene.shape<Triangle>(&mirror_mat, Vec3(DIST, -1, 1), Vec3(-1, 1, 1), Vec3(-1, -1, 1));

    // rendering
    Camera camera(Vec3::ORIGIN, Vec3(DIST, 0, 0), 60);
    printf("Rendering \"reflect box\"\n");
    auto img = scene.render(camera, 1024, 1024, 4);
    img->save("out/reflect_box.png");
}

void tetrahedron_scene() {
    // TODO: fix cylinders randomly flipping normals

    // textures
    ImageTexture cozy_texture("assets/sundowner_deck.png");
    ImageTexture glass_texture("assets/glass.png");

    // materials
    Material mirror_mat(Vec3(0.65f, 0.58f, 0.91f));
    mirror_mat.glossy(0.6f).rough(0.1f);
    Material sphere_mat(Vec3::UNIT_X);
    sphere_mat.rough(0.5f);
    Material bar_mat(Vec3(0.75f, 0.75f, 0.75f));
    sphere_mat.rough(0.75f).glossy(0.5f);

    // scene
    Scene scene;
    scene.shape<Cylinder>(&bar_mat, Vec3(1.73f, -2.83f, 1), Vec3(3.46f, 0, -6), 3.46, 0.5f);
    scene.shape<Cylinder>(&bar_mat, Vec3(-1.73f, -2.83f, 1), Vec3(-3.46f, 0, -6), 3.46, 0.5f);
    scene.shape<Cylinder>(&bar_mat, Vec3(0, 0, 2), Vec3(0, 5.66f, -4), 3.46, 0.5f);
    scene.shape<Cylinder>(&bar_mat, Vec3(0, -2.83f, -2), Vec3(-6.93f, 0, 0), 3.46, 0.5f);
    scene.shape<Cylinder>(&bar_mat, Vec3(1.73f, 0, -1), Vec3(-3.46f, 5.66f, 2), 3.46, 0.5f);
    scene.shape<Cylinder>(&bar_mat, Vec3(-1.73f, 0, -1), Vec3(3.46f, 5.66f, 2), 3.46, 0.5f);
    scene.shape<Sphere>(&sphere_mat, Vec3(0, -2.83f, 4), 1);
    scene.shape<Sphere>(&sphere_mat, Vec3(3.46f, -2.83f, -2), 1);
    scene.shape<Sphere>(&sphere_mat, Vec3(-3.46f, -2.83f, -2), 1);
    scene.shape<Sphere>(&sphere_mat, Vec3(0, 2.83f, 0), 1);
    scene.shape<Plane>(&mirror_mat, Vec3::UNIT_X * 5, -Vec3::UNIT_X);
    scene.skybox = &cozy_texture;
    scene.light_direction = Vec3(1, -1, -1).normalize();
    scene.light_color = Vec3(1, 1, 1);
    scene.ambient_light = Vec3(0.75f, 0.75f, 0.75f);

    // rendering
    Camera camera(Vec3(-2, 0, 16), Vec3(2, 0, 0), 60);
    printf("Rendering \"tetrahedron\"\n");
    auto img = scene.render(camera, 1024, 768, 4);
    img->save("out/tetrahedron.png");
}


int main() {
    sphere_scene();
    reflect_box_scene();
    winter_scene();
    tetrahedron_scene();
    return 0;
}
