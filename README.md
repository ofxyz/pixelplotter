# PixelPlotter

![Implementation Overview](img/overview.png)

## Notes on setting up VS2022

  - All Configurations -> Config Properties -> vcpkg -> Use Vcpkg: No
  - Release -> Config Properties -> Linker -> System -> SubSystem: Windows (/SUBSYSTEM:WINDOWS)
  - Release -> Config Properties -> Linker -> Advanced -> Entry Point: mainCRTStartup

## External Dependencies

- [ofx2d](https://github.com/ofxyz/ofx2d.git) 
- [ofxCv](https://github.com/ofxyz/ofxCv.git) 
- [ofxImGui](https://github.com/ofxyz/ofxImGui.git) 
- [ofxOrnament](https://github.com/ofxyz/ofxOrnament) 
