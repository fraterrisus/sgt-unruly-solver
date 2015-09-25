#include "board.h"

Board::Line::Line(
    const Board::iterator &s, 
    const Board::iterator &e)
{
  begin = s;
  end = e;
  datum = 0;
}

std::string Board::Line::to_str() 
{ return Board::Line::to_str(begin, end); }

bool Board::Line::is_full() 
{ return Board::Line::is_full(begin, end); }

int Board::Line::count()
{ return end - begin; }

int Board::Line::count_colors(
    Square::Color col) 
{ return Board::Line::count_colors(begin, end, col); }

bool Board::Line::is_valid()
{
  for (Board::iterator x = begin; x < (end - 2); x++) {
    Board::iterator y = x + 1;
    Board::iterator z = x + 2;
    if (!x->is_empty() && x->get() == y->get() && x->get() == z->get()) {
      return false;
    }
  }
  return true;
}

/***********************/

std::string Board::Line::to_str(
    const Board::iterator &begin, 
    const Board::iterator &end) 
{
  std::string rv = "";
  for (Board::iterator it = begin; it < end ; it++)
    rv = rv + it->to_char();
  return rv;
}

bool Board::Line::is_full(
    const Board::iterator &begin, 
    const Board::iterator &end) 
{
  for (Board::iterator it = begin; it < end; it++)
    if (it->is_empty()) { return false; }
  return true;
}

int Board::Line::count_colors(
    const Board::iterator &begin, 
    const Board::iterator &end, 
    Square::Color col) 
{
  int count = 0;
  for (Board::iterator it = begin; it < end; it++)
    if (it->get() == col) count++;
  return count;
}

bool Board::Line::ordering(
    const Board::Line &a, 
    const Board::Line &b) 
{
  return (a.datum < b.datum);
}

