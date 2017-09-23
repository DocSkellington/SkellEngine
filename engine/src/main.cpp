// A simple program that computes the square root of a number
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "sol.hpp"

#include "config.h"

using namespace sf;
int main (int argc, char *argv[])
{
  std::cout << ENGINE_VERSION_MAJOR << '\n';

  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);
  lua.script("print('bbkjbkbkh')");

  RenderWindow window (VideoMode(800, 600, 32), "TEST");

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed)
        window.close();
    }

    window.display();
  }
  return 0;
}