# PixelPlotter

![Screenshot of emptyExample](preview.png)

## Notes on setting up VS2022

  - All Configurations -> Config Properties -> vcpkg -> Use Vcpkg: No
  - Release -> Config Properties -> Linker -> System -> SubSystem: Windows (/SUBSYSTEM:WINDOWS)
  - Release -> Config Properties -> Linker -> Advanced -> Entry Point: mainCRTStartup

## dependencies

- ofxCv
- ofxOpenCv
- ofxImGui (Docking Branch)
- ofxIconFontCppHeaders
- [K-Lite Codec Pack](https://files3.codecguide.com/K-Lite_Codec_Pack_1750_Basic.exe)
