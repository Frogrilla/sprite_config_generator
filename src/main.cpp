#include <algorithm>
#include <filesystem>
#include <fstream>
#include <imago2.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace fs = std::filesystem;

using json = nlohmann::json;

struct Color {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 0.0f;
};

struct Point {
  int x = 0;
  int y = 0;
};

bool operator==(const Color &a, const Color &b) {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
};

json GCI_Indexed(img_pixmap *map) {

  std::vector<Color> pallete;
  std::string data = "";

  int length = 0;
  int index = -1;

  auto AddSegment = [&](int size, int col) {
    if (length > 0) {
      data.append(std::to_string(size) + ";" + std::to_string(col + 1) + ";");
      length = 0;
      index = -1;
    }
  };

  auto ContinueSegment = [&]() { length++; };

  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      Color col;
      img_getpixel4f(map, x, y, &col.r, &col.g, &col.b, &col.a);

      int find =
          std::find(pallete.begin(), pallete.end(), col) - pallete.begin();

      if (find == index) {
        length++;
      } else {
        if (find == pallete.size()) {
          pallete.push_back(col);
        }
        AddSegment(length, index);
        index = find;
        length = 1;
      }
    }
    AddSegment(length, index);
  }

  json imageJson;

  imageJson["id"] = map->name;
  imageJson["format"] = "indexed";
  imageJson["width"] = map->width;
  imageJson["height"] = map->height;
  imageJson["pallete"] = {};

  for (int i = 0; i < pallete.size(); i++) {
    json col;
    col["r"] = pallete[i].r;
    col["g"] = pallete[i].g;
    col["b"] = pallete[i].b;
    col["a"] = pallete[i].a;
    imageJson["pallete"].push_back(col);
  }

  imageJson["data"] = data;

  return imageJson;
}

json GCI_Layered(img_pixmap *img) {
  std::vector<Color> pallete;
  std::vector<std::vector<bool>> mask(img->height,
                                      std::vector<bool>(img->width, false));

  json imageJson;

  imageJson["id"] = img->name;
  imageJson["format"] = "indexed";
  imageJson["width"] = img->width;
  imageJson["height"] = img->height;
  imageJson["pallete"] = {};

  for (int i = 0; i < pallete.size(); i++) {
    json col;
    col["r"] = pallete[i].r;
    col["g"] = pallete[i].g;
    col["b"] = pallete[i].b;
    col["a"] = pallete[i].a;
    imageJson["pallete"].push_back(col);
  }

  return imageJson;
}

int main(int argc, char *argv[]) {

  if (argc <= 1) {
    std::cout << "No images supplied | Aborting" << std::endl;
    return -1;
  }

  std::vector<json> images;

  struct img_pixmap img;
  for (int i = 1; i < argc; i++) {
    fs::path path = argv[i];
    if (fs::is_directory(path)) {
      for (const auto &file : fs::recursive_directory_iterator(path)) {
        img_init(&img);
        img_load(&img, file.path().c_str());

        if (img.width > 0 && img.height > 0) {
          images.push_back(GCI_Indexed(&img));
          std::cout << "Encoded " << img.name << " using format 'Indexed'"
                    << std::endl;
        }

        img_destroy(&img);
      }
    } else {
      img_init(&img);
      img_load(&img, path.c_str());

      if (img.width > 0 && img.height > 0) {
        images.push_back(GCI_Indexed(&img));
        std::cout << "Encoded " << img.name << " using format 'Indexed'"
                  << std::endl;
      }
      img_destroy(&img);
    }
  }

  std::cout << "Done encoding " << std::to_string(images.size()) << " image(s)"
            << std::endl;

  json config;
  config["script"] = "sprite/src/main.luau";
  config["customData"];
  config["customData"]["images"] = {};
  for (const json &imageJson : images) {
    config["customData"]["images"].push_back(imageJson);
  }

  std::cout << "Writing to file" << std::endl;

  std::ofstream out("frogrilla.sprite.json");
  out << std::setw(4) << config << std::endl;

  std::cout << "Images written to `./frogrilla.sprite.json`" << std::endl;

  return 1;
}
