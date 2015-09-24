#include "board.h"

Board::iterator::iterator(Square* p, int i) {
  ptr = p;
  inc = i;
}

Board::iterator::iterator(const Board::iterator &rhs) {
  ptr = rhs.ptr;
  inc = rhs.inc;
}

Board::iterator& Board::iterator::operator=(const Board::iterator &rhs) {
  ptr = rhs.ptr;
  inc = rhs.inc;
}

Square& Board::iterator::operator*() const {
  return *ptr;
}

Square* Board::iterator::operator->() const {
  return ptr;
}

Board::iterator& Board::iterator::operator++() { 
  ptr += inc; 
  return *this; 
}

Board::iterator Board::iterator::operator++(int _junk) { 
  Board::iterator tmp = *this;
  operator++();
  return tmp;
}

Board::iterator& Board::iterator::operator+=(int i) {
  ptr += (inc * i);
  return *this;
}

Board::iterator Board::iterator::operator+(int i) {
  Board::iterator tmp = *this;
  tmp += i;
  return tmp;
}

Board::iterator& Board::iterator::operator--() { 
  ptr -= inc; 
  return *this; 
}

Board::iterator Board::iterator::operator--(int _junk) { 
  Board::iterator tmp = *this;
  ptr -= inc;
  return tmp;
}

Board::iterator& Board::iterator::operator-=(int i) {
  ptr -= (inc * i);
  return *this;
}

Board::iterator Board::iterator::operator-(int i) {
  Board::iterator tmp = *this;
  tmp -= i;
  return tmp;
}

int Board::iterator::operator-(const Board::iterator& rhs) {
  Board::iterator it = rhs;
  int dist = 0;
  while (it != *this) { it++; dist++; }
  return dist;
}

bool Board::iterator::operator==(const Board::iterator& rhs) {
  return (ptr == rhs.ptr);
}

bool Board::iterator::operator!=(const Board::iterator& rhs) {
  return (ptr != rhs.ptr);
}

bool Board::iterator::operator<(const Board::iterator& rhs) {
  return (ptr < rhs.ptr);
}

bool Board::iterator::operator>(const Board::iterator& rhs) {
  return (ptr > rhs.ptr);
}

bool Board::iterator::operator<=(const Board::iterator& rhs) {
  return (ptr <= rhs.ptr);
}

bool Board::iterator::operator>=(const Board::iterator& rhs) {
  return (ptr >= rhs.ptr);
}

