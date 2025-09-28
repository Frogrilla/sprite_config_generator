# Sprite Config Generator

Sprite Config Generator is a terminal program that generates config files for my Orion Drift script [Sprite](https://github.com/frogrilla/sprite).
This project uses:
- [Nlohmann's Json](https://github.com/nlohmann/json) for generating json
- [stb_image](https://github.com/nothings/stb) for loading images
</br>

## Usage:

To use SCG simply open a terminal and run `sprite_config_generator` then type the directories with or the paths to your images. E.G.</br>
The output config will be written to `frogrilla.sprite.json` in the directory the program is run from, so if you want it to be in your config folder automatically, run it from there.

```shell
./sprite_config_generator.exe ./images ./other-images/some-image.png
```
