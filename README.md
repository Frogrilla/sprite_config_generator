# Sprite Config Generator

Sprite Config Generator is a terminal program that generates config files for my Orion Drift script [Sprite](https://github.com/frogrilla/sprite).
This project uses:
- [Nlohmann's Json](https://github.com/nlohmann/json) (for generating json)
- [libimago](https://github.com/jtsiomb/libimago) (for loading images)
</br>
The releases built by me use libpng for png support but they don't use libjpeg. </br>
If you build this yourself, libimago must be built and installed and in that case png / jpeg support will depend on how you build it.

## Usage:

To use SCG simply open a terminal and run `sprite_config_generator` then type the directories with or the path to your images. E.G.

```shell
./sprite_config_generator.exe ./images ./other-images/some-image.png
```
