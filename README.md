# PixelPlotter

![Screenshot of emptyExample](preview.png)

## Notes on setting up VS2022

  - All Configurations -> Config Properties -> vcpkg -> Use Vcpkg: No
  - Release -> Config Properties -> Linker -> System -> SubSystem: Windows (/SUBSYSTEM:WINDOWS)
  - Release -> Config Properties -> Linker -> Advanced -> Entry Point: mainCRTStartup

## External Dependencies

- [ofx2d](https://github.com/ofxyz/ofx2d.git)
- [ofxCv](https://github.com/ofxyz/ofxCv.git)
- [ofxImGui](https://github.com/ofxyz/ofxImGui.git)
- [ofxIconFontCppHeaders](https://github.com/ofxyz/ofxIconFontCppHeaders)
- [K-Lite Codec Pack](https://files3.codecguide.com/K-Lite_Codec_Pack_1750_Basic.exe)
