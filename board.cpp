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

std::string Board::line_to_str(Board::iterator start, Board::iterator end) {
  std::string rv = "";
  Board::iterator *it = new Board::iterator(start);
  for (; *it < end ; (*it)++) {
    rv = rv + (*it)->to_char();
  }
  return rv;
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

bool Board::find_pairs(Board::iterator start, Board::iterator end) {
  bool changes = false;
  for (Board::iterator it = start; it < end; it++)
    if (it->is_empty()) { changes = true; break; }
  if (!changes) { return false; } // short-circuit

  changes = false;
  for (Board::iterator x = start; x < (end - 1); x++) {
    Board::iterator y = x+1;
    if ((! x->is_empty()) && (x->get() == y->get())) {
      Square::Color newcol = x->get_inverse();
      Board::iterator w = x-1;
      Board::iterator z = x+2;
      if ((w >= start) && (w->is_empty())) { w->set(newcol); changes = true; }
      if ((z <  end  ) && (z->is_empty())) { z->set(newcol); changes = true; }
    }
  }
  return changes;
}

bool Board::find_gaps(Board::iterator start, Board::iterator end) {
  bool changes = false;
  for (Board::iterator it = start; it < end; it++)
    if (it->is_empty()) { changes = true; break; }
  if (!changes) { return false; } // short-circuit

  changes = false;
  for (Board::iterator x = start; x < (end - 2); x++) {
    Board::iterator y = x+1;
    Board::iterator z = x+2;
    if (!x->is_empty() && y->is_empty() && !z->is_empty() && (x->get() == z->get())) {
      //std::cout << "found " << x->to_char() << y->to_char() << z->to_char() << "\n";
      y->set(x->get_inverse()); changes = true;
    }
  }
  return changes;
}

bool Board::find_halves(Board::iterator start, Board::iterator end) {
  std::cout << "find_halves\n";
  bool changes = false;
  int count = end - start;
  int blacks=0, whites=0, nones=0;
  for (Board::iterator it = start; it < end; it++) {
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
    for (Board::iterator it = start; it < end; it++)
      if (it->is_empty()) { it->set(newcol); changes = true; }
  return changes;
}

bool Board::solve_by_finder(Board::FindFunc ff) {
  bool changes = false;
  for (int x = 0; x < width; x++) {
    Board::iterator start = col_begin(x);
    Board::iterator end = col_end(x);
    std::cout << "c" << x << ": " << line_to_str(start, end) << "\n";
    changes = (this->*ff)(start, end) || changes;
    std::cout << "    " << line_to_str(start, end) << "\n";
  }
  for (int y = 0; y < height; y++) {
    Board::iterator start = row_begin(y);
    Board::iterator end = row_end(y);
    std::cout << "r" << y << ": " << line_to_str(start, end) << "\n";
    changes = (this->*ff)(start, end) || changes;
    std::cout << "    " << line_to_str(start, end) << "\n";
  }
  return changes;
}

void Board::solve() {
  bool changes = true;
  while (changes) {
    changes = false;
    changes = changes || solve_by_finder(&Board::find_pairs);
    changes = changes || solve_by_finder(&Board::find_gaps);
    changes = changes || solve_by_finder(&Board::find_halves);
    std::cout << to_str();
  }
}
