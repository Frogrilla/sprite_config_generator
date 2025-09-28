#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include "json.hpp"
#include "stb_image.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

using json = nlohmann::json;

struct Color {
  uint8_t r = 0.0f;
  uint8_t g = 0.0f;
  uint8_t b = 0.0f;
  uint8_t a = 0.0f;
};

bool operator==(const Color &a, const Color &b) {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
};

json GCI_Indexed(const char *path) {

  int width, height, channels;
  unsigned char *img = stbi_load(path, &width, &height, &channels, 0);

  if (img == NULL) {
    delete (img);
    return NULL;
  }

  std::cout << "Encoding image using 'Indexed' " << path;

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

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Color col = *(Color *)&(img[((y * width) + x) * channels]);
      if (channels < 4) {
        col.a = 255;
      }

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

  delete (img);

  json imageJson;

  imageJson["id"] = path;
  imageJson["format"] = "indexed";
  imageJson["width"] = width;
  imageJson["height"] = height;
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

  std::cout << " -> done" << std::endl;
  return imageJson;
}

int main(int argc, char *argv[]) {

  if (argc <= 1) {
    std::cout << "No images supplied | Aborting" << std::endl;
    return -1;
  }

  std::vector<json> images;

  for (int i = 1; i < argc; i++) {
    fs::path path = argv[i];
    if (fs::is_directory(path)) {
      for (const auto &file : fs::recursive_directory_iterator(path)) {
        json img = GCI_Indexed(file.path().c_str());
        if (img != NULL)
          images.push_back(img);
      }
    } else {
      json img = GCI_Indexed(path.c_str());
      if (img != NULL)
        images.push_back(img);
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
