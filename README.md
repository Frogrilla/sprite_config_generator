# Sprite Config Generator

Sprite Config Generator is a terminal program that generates config files for my Orion Drift script [Sprite](https://github.com/frogrilla/sprite).
This project uses:
- [Nlohmann's Json](https://github.com/nlohmann/json) (for generating json)
- [libimago](https://github.com/jtsiomb/libimago) (for loading images)
- [libpng](https://github.com/pnggroup/libpng) (for png support)

## Usage:

To use SCG simply open a terminal and run `sprite_config_generator` then type the directories with or the path to your images. E.G.
```shell
./sprite_config_generator ./images ./other-images/some-image.png
```
</br>

Images that libimago supports are compatible apart from jpeg.
