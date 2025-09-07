#include <algorithm>
#include <fstream>
#include <imago2.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

using json = nlohmann::json;

struct Color {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 0.0f;
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

int main(int argc, char *argv[]) {
  struct img_pixmap img;

  img_init(&img);
  img_load(&img, "foo.png");
  json imageJson = GCI_Indexed(&img);
  img_destroy(&img);

  img_init(&img);
  img_load(&img, "orion.png");
  json orionJson = GCI_Indexed(&img);
  img_destroy(&img);

  json config;
  config["script"] = "sprite/src/main.luau";
  config["customData"];
  config["customData"]["images"] = {};
  config["customData"]["images"].push_back(imageJson);
  config["customData"]["images"].push_back(orionJson);

  std::ofstream out("frogrilla.sprite.json");
  out << std::setw(4) << config << std::endl;

  return 1;
}
