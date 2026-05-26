#include <chrono>
// #include <fstream>
#include <iostream>

#include "application/config_parser.h"
#include "application/scene_parser.h"
#include "camera/camera.h"
#include "container/canvas.h"

using namespace std;

int main() {
  // Parse scene from JSON
  auto scene_data = SceneParser::createFromJSON("scene.json");
  auto world = std::move(scene_data.world);

  auto start = chrono::steady_clock::now();
  auto canvas = ConfigParser::createFromJSON("config.json");
  canvas.renderParallel(std::move(world));
  canvas.save();

  auto end = chrono::steady_clock::now();
  auto elapsed = chrono::duration_cast<chrono::minutes>(end - start);
  auto seconds = chrono::duration_cast<chrono::seconds>(end - start);
  auto remaining_seconds = seconds.count() % 60;
  cout << "..............................." << endl;
  cout << "Total elapsed time: " << elapsed.count() << " min "
       << remaining_seconds << " s" << endl;
  cout << "..............................." << endl;

  return 0;
}
