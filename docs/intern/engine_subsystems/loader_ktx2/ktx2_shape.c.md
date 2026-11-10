# src/engine_subsystems/loader_ktx2/ktx2_shape.c

This file checks the shape of a texture against the types of the rendering part.

## The checks

The checks run in this order, and the first fault gives the reason:

1. `vkFormat` is not 0, and `supercompressionScheme` is 0.
2. `pixelWidth` and `pixelHeight` are not 0, so the texture is not a 1D texture.
3. `faceCount` is 1 or 6. A cube is square and has no `pixelDepth`.
4. A cube or a volume has no `layerCount`.
5. The levels are at most the levels of the largest side.

`count_levels` counts the bits of the largest side, so a side of 4 has 3 levels: 4, 2 and 1.

## Functions

| Function | Result |
| --- | --- |
| `count_levels` | The number of levels of the largest side. |
| `check_kind` | The reason of a fault of the format, of the scheme or of the size. |
| `check_faces` | The reason of a fault of the faces or of the layers. |
| `ktx2_check_shape` | The first reason, or nothing. |
