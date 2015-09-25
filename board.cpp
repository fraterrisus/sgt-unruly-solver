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

/*******************/

std::string Board::to_str() {
  Square* p = board;
  std::string rv = "-----\n";
  for (int y=0; y<this->height; y++) {
    for (int x=0; x<this->width; x++) {
      rv = rv + p->to_char() + ' ';
      p++;
    }
    rv = rv + "\n";
  }
  return rv;
}

bool Board::is_solved() {
  for (int x = 0; x < width; x++) {
    if (! fetch_col(x).is_full()) return false;
  }
  for (int y = 0; y < height; y++) {
    if (! fetch_row(y).is_full()) return false;
  }
  return true;
}

/*******************/

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

/*******************/

Board::Line Board::fetch_row(int y) {
  return Board::Line(row_begin(y), row_end(y));
}

Board::Line Board::fetch_col(int x) {
  return Board::Line(col_begin(x), col_end(x));
}

Board::iterator Board::row_begin(int y) {
  return Board::iterator(this->get_square(0,y), 1);
}

Board::iterator Board::row_end(int y) {
  return Board::iterator(this->get_square(this->width,y), 1);
}

Board::iterator Board::col_begin(int x) {
  return Board::iterator(this->get_square(x,0), this->width);
}

Board::iterator Board::col_end(int x) {
  return Board::iterator(this->get_square(x,this->height), this->width);
}

/*******************/

bool Board::brute_force(Board::Line &line) {
  return false;
}

/* The difference in this method is that it exits as soon as any changes are
 * made. solve_by_finder() continues iterating over every row and column even
 * if it makes changes. */
bool Board::solve_by_brute_force() {
  for (int x = 0; x < width; x++) {
    Board::Line col = fetch_col(x);
    //std::cout << "c" << x << ": " << col->to_str() << "\n";
    if (brute_force(col)) { return true; }
    //std::cout << "    " << col->to_str() << "\n";
  }
  for (int y = 0; y < height; y++) {
    Board::Line row = fetch_row(y);
    //std::cout << "r" << y << ": " << row->to_str() << "\n";
    if (brute_force(row)) { return true; }
    //std::cout << "    " << row->to_str() << "\n";
  }
  return false;
}

/*******************/

bool Board::find_pairs(Board::Line &line) {
  if (line.is_full()) { return false; }
  bool changes = false;
  for (Board::iterator x = line.start; x < (line.end - 1); x++) {
    Board::iterator y = x+1;
    if ((! x->is_empty()) && (x->get() == y->get())) {
      Square::Color newcol = x->get_inverse();
      Board::iterator w = x-1;
      Board::iterator z = x+2;
      if ((w >= line.start) && (w->is_empty())) { w->set(newcol); changes = true; }
      if ((z <  line.end  ) && (z->is_empty())) { z->set(newcol); changes = true; }
    }
  }
  return changes;
}

bool Board::find_gaps(Board::Line &line) {
  if (line.is_full()) { return false; }
  bool changes = false;
  for (Board::iterator x = line.start; x < (line.end - 2); x++) {
    Board::iterator y = x+1;
    Board::iterator z = x+2;
    if (!x->is_empty() && y->is_empty() && !z->is_empty() && (x->get() == z->get())) {
      y->set(x->get_inverse()); changes = true;
    }
  }
  return changes;
}

bool Board::find_halves(Board::Line &line) {
  bool changes = false;
  int count = line.end - line.start;
  int blacks=0, whites=0, nones=0;
  for (Board::iterator it = line.start; it < line.end; it++) {
    switch(it->get()) {
      case Square::NONE:
        nones++; break;
      case Square::BLACK:
        blacks++; break;
      case Square::WHITE:
        whites++; break;
    }
  }
  if (nones == 0) return false; // short-circuit

  changes = false;
  Square::Color newcol = Square::NONE;
  if (blacks == count / 2) newcol = Square::WHITE;
  if (whites == count / 2) newcol = Square::BLACK;
  if (newcol != Square::NONE)
    for (Board::iterator it = line.start; it < line.end; it++)
      if (it->is_empty()) { it->set(newcol); changes = true; }
  return changes;
}

bool Board::solve_by_finder(Board::FindFunc ff) {
  bool changes = false;
  for (int x = 0; x < width; x++) {
    Board::Line col = fetch_col(x);
    //std::cout << "c" << x << ": " << col->to_str() << "\n";
    changes = (this->*ff)(col) || changes;
    //std::cout << "    " << col->to_str() << "\n";
  }
  for (int y = 0; y < height; y++) {
    Board::Line row = fetch_row(y);
    //std::cout << "r" << y << ": " << row->to_str() << "\n";
    changes = (this->*ff)(row) || changes;
    //std::cout << "    " << row->to_str() << "\n";
  }
  return changes;
}

/*******************/

void Board::solve() {
  bool changes;
  while (!is_solved()) {
    changes = true;
    while (changes) {
      changes = false;
      changes = changes || solve_by_finder(&Board::find_pairs);
      changes = changes || solve_by_finder(&Board::find_gaps);
      changes = changes || solve_by_finder(&Board::find_halves);
      //if (changes) std::cout << to_str();
    }
    changes = solve_by_brute_force();
    // Either we're done, or something went really really wrong.
    if (!changes) { return; }
    //std::cout << to_str();
  }
}
