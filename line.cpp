#include "board.h"

Board::Line::Line(const Board::iterator &s, const Board::iterator &e) {
  start = s;
  end = e;
  datum = 0;
}

std::string Board::Line::to_str() { return Board::Line::to_str(start, end); }

bool Board::Line::is_full() { return Board::Line::is_full(start, end); }

void Board::Line::count_nones() { 
  datum = 0;
  for (Board::iterator it = start; it < end; it++)
    if (it->is_empty()) datum++;
}

std::string Board::Line::to_str(Board::iterator start, Board::iterator end) {
  std::string rv = "";
  for (Board::iterator it = start; it < end ; it++)
    rv = rv + it->to_char();
  return rv;
}

bool Board::Line::is_full(Board::iterator start, Board::iterator end) {
  for (Board::iterator it = start; it < end; it++)
    if (it->is_empty()) { return false; }
  return true;
}

int Board::Line::count_nones(Board::iterator start, Board::iterator end) {
  int count = 0;
  for (Board::iterator it = start; it < end; it++)
    if (it->is_empty()) count++;
  return count;
}
