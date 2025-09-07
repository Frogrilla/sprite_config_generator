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

  auto AddSegment = [&]() {
    if (length > 0) {
      data.append(std::to_string(length) + ";" + std::to_string(index + 1) +
                  ";");
      length = 0;
      index = -1;
    }
  };

  auto ContinueSegment = [&]() { length++; };

  auto ContinueOrAddSegment = [&](int nextIndex) {
    if (nextIndex == index || length <= 0) {
      ContinueSegment();
      index = nextIndex;
    } else {
      AddSegment();
    }
  };

  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      Color col;
      img_getpixel4f(map, x, y, &col.r, &col.g, &col.b, &col.a);

      auto find = std::find(pallete.begin(), pallete.end(), col);

      if (find != pallete.end()) {
        ContinueOrAddSegment(find - pallete.begin());
      } else {
        AddSegment();
        length = 1;
        index = pallete.size();
        pallete.push_back(col);
      }
    }
    AddSegment();
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

  json config;
  config["script"] = "sprite/src/main.luau";
  config["customData"];
  config["customData"]["images"] = {};
  config["customData"]["images"].push_back(imageJson);

  std::ofstream out("frogrilla.sprite.json");
  out << std::setw(4) << config << std::endl;

  img_destroy(&img);
  return 1;
}
