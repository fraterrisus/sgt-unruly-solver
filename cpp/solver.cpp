#include "board.h"
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

Board* read_game() {
  int h, w;
  char p;
  std::string gameid;
  cin >> h;
  cin >> p;
  if (p != 'x') {
    cerr << "Format error: expected (width)x(height)\n";
    return 0;
  }
  cin >> w;
  //cout << "Detected " << h << "x" << w << " board\n";
  Board *b = new Board(h,w);

  p = ' ';
  while (!cin.eof() && p != ':') { cin >> p; }
  if (p != ':') {
    cerr << "Format error: expected colon between dimensions and game string\n";
    return 0;
  }

  int pos = -1;
  while (1) {
    pos++;
    Square::Color color = Square::NONE;
    cin >> p;
    gameid += p;
    if (cin.eof()) { break; }
    if ((p >= 'a') && (p < 'z')) {
      pos += (p - 'a');
      color = Square::WHITE;
    }
    if (p == 'z') {
      pos += (p - 'a') - 1;
    }
    if ((p >= 'A') && (p < 'Z')) {
      pos += (p - 'A');
      color = Square::BLACK;
    }
    if (p == 'Z') {
      pos += (p - 'A') - 1;
    }
    //cout << "[" << p << "] (" << pos << ") " << color << "\n";
    if (pos < (h * w)) {
      b->set_color(pos, color);
    }
    if (pos > (h * w)) {
      cerr << "Format error: game string " << gameid << " too long\n";
      delete(b);
      return 0;
    }
  }
  if (pos != (h * w)+1) {
    cerr << "Format error: game string too short\n";
    delete(b);
    return 0;
  }

  return b;
}

Board* test_game() {
  Board *board = new Board(10);
  board->set_color(0,0,Square::WHITE);
  board->set_color(1,0,Square::BLACK);

  board->set_color(4,1,Square::WHITE);
  board->set_color(9,1,Square::WHITE);

  board->set_color(2,2,Square::WHITE);
  board->set_color(6,2,Square::WHITE);
  board->set_color(7,2,Square::WHITE);

  board->set_color(0,3,Square::WHITE);
  board->set_color(2,3,Square::WHITE);
  board->set_color(6,3,Square::WHITE);
  board->set_color(9,3,Square::WHITE);

  board->set_color(4,4,Square::WHITE);
  board->set_color(7,4,Square::BLACK);

  board->set_color(3,5,Square::WHITE);
  board->set_color(8,5,Square::BLACK);

  board->set_color(6,6,Square::WHITE);

  board->set_color(3,7,Square::BLACK);
  board->set_color(5,7,Square::BLACK);

  board->set_color(1,8,Square::BLACK);
  board->set_color(2,8,Square::BLACK);
  board->set_color(5,8,Square::BLACK);
  board->set_color(8,8,Square::BLACK);
  board->set_color(9,8,Square::BLACK);

  board->set_color(2,9,Square::WHITE);
  board->set_color(8,9,Square::BLACK);

  return board;
}

int main(int argc, char **argv) {
  Board *board;

  board = read_game();
  //board = test_game();
  if (board == 0) { return 1; }
  cout << board->to_str();
  auto then = std::chrono::high_resolution_clock::now();
  board->solve();
  auto now = std::chrono::high_resolution_clock::now();
  long long elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(now - then).count();
  cout << board->to_str();
  cout << "Solving time: " << elapsed_us / 100000.0 << "\n";
  return 0;
}
