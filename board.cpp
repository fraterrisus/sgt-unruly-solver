#include "board.h"

Board::Board() {
  this->board = 0;
  this->height = 0;
  this->width = 0;
}

Board::Board(int dim) {
  int size = dim * dim;
  this->height = dim;
  this->width = dim;
  this->board = new Square[size];
}

Board::Board(int x, int y) {
  int size = x * y;
  this->height = y;
  this->width = x;
  this->board = new Square[size];
}

std::string Board::to_str() {
  Square* p = board;
  std::string rv = "";
  for (int y=0; y<this->height; y++) {
    for (int x=0; x<this->width; x++) {
      rv = rv + p->to_char() + ' ';
      p++;
    }
    rv = rv + "\n";
  }
  return rv;
}

Square* Board::get_square(int x, int y) {
  return this->board + x + (y * this->width);
}

Square::Color Board::get_color(int x, int y) {
  return this->get_square(x, y)->get();
}

void Board::set_color(int x, int y, Square::Color val) {
  this->get_square(x, y)->set(val);
}

void Board::set_color(int loc, Square::Color val) {
  Square* s = this->board + loc;
  s->set(val);
}

void Board::solve() {
}
