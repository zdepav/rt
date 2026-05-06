# rt

## About

A simple CPU-based raytracing implementation, supports basic meshes (sphere, cylinder, plane,
flat circle, triangle, models imported from .obj files) and textures (images imported from .png and
runtime-generated gradients).

`main.cpp` renders 3 scenes:
- **Winter**: .obj model import
- **Spheres**: skybox and materials
- **Reflection Box**: reflections inside enclosed space

|       Winter        |       Spheres        |      Reflection Box      |
|:-------------------:|:--------------------:|:------------------------:|
| ![](out/winter.png) | ![](out/spheres.png) | ![](out/reflect_box.png) |

## Dependencies

The following 2 libraries are used (both included in the `lib/` directory:
- [libspng](https://libspng.org)
- [Miniz](https://github.com/richgel999/miniz) (dependency of libspng)

## Licensing

Most of this repository is licensed under the MIT license (see [LICENSE](LICENSE)) with the
following exceptions:
- `lib/` directory contains external libraries, see [lib/LICENSE](lib/LICENSE) for links to their
    licenses
- `assets/` directory contains images under CC0 (including non-original ones listed in
    [assets/ATTRIBUTIONS](assets/ATTRIBUTIONS))
