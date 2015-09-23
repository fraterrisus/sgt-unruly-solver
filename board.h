#ifndef _BOARD_H
#define _BOARD_H

#include "square.h"
#include <iostream>
#include <string>

class Board {

private:
  Square* board;
  int width, height;

public:
  Board();
  Board(int);
  Board(int, int);

  void solve();
  std::string to_str();

  Square* get_square(int, int);
  Square::Color get_color(int, int);
  void set_color(int, int, Square::Color);
  void set_color(int, Square::Color);
};

#endif

