# PixelPlotter

![Screenshot of emptyExample](preview.png)


## Roadmap

### Colour
  - Swatch generators / helper tools. (You can break down colours using tertiary colors of different variety)
  - Split colours into primary colors, secondary colors and tertiary colors.
  
    B = M + C
    G = C + Y
    R = Y + M

    Green, Orange, Violet

    Red-Orange
    Red-Violet
    Blue-Violet
    Blue-Green
    Yellow-Green
    Yellow-Orange

### Geometry
  - Implement Cairo render with threading for now. If it turns out to be too slow for prototyping we will need to move to NV_path_rendering.
  - Ability to interpolate shapes
  - Add pattern generators, copy, rotate etc. EG. Plaid Generator, Stripe/grid generator.

### Pixelate Filter
  - Create pixel class 
      -> Input Colour 
      -> Output colour seperation composition that becomes that colour seen from a large distance.
  - Pixel Designer UI

## Notes on setting up VS2022

  - All Configurations -> Config Properties -> vcpkg -> Use Vcpkg: No
  - Release -> Config Properties -> Linker -> System -> SubSystem: Windows (/SUBSYSTEM:WINDOWS)
  - Release -> Config Properties -> Linker -> Advanced -> Entry Point: mainCRTStartup

## dependencies

- ofxCv
- ofxOpenCv
- ofxImGui
- ofxPosterize

- [K-Lite Codec Pack](https://files3.codecguide.com/K-Lite_Codec_Pack_1750_Basic.exe)
