#include "board.h"
#include <iostream>
#include <string>

using namespace std;

Board* read_game() {
  int h, w;
  char p;
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

  int pos = 0;
  while (1) {
    Square::Color color = Square::NONE;
    cin >> p;
    if (cin.eof()) { break; }
    if ((p >= 'a') && (p < 'z')) {
      pos += (p - 'a');
      color = Square::WHITE;
    }
    if (p == 'z') {
      pos += (p - 'a');
    }
    if ((p >= 'A') && (p < 'Z')) {
      pos += (p - 'A');
      color = Square::BLACK;
    }
    if (p == 'Z') {
      pos += (p - 'A');
    }
    //cout << "[" << p << "] (" << pos << ") " << color << "\n";
    if (pos < (h * w)) {
      b->set_color(pos, color);
    }
    if (pos > (h * w)) {
      cerr << "Format error: game string too long\n";
      delete(b);
      return 0;
    }
    pos++;
  }
  if (pos != (h * w)+1) {
    cerr << "Format error: game string too short\n";
    delete(b);
    return 0;
  }

  return b;
}

int main(int argc, char **argv) {
  Board *board = read_game();
  if (board == 0) { return 1; }
  cout << board->to_str();
  board->solve();
  cout << board->to_str();
  return 0;
}
