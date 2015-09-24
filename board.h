#ifndef _BOARD_H
#define _BOARD_H

#include "square.h"
#include <iostream>
#include <string>
#include <iterator>

class Board {

public:
  class iterator {
  protected:
    int inc;
    Square *ptr;
  public:
    iterator(const iterator&);
    iterator(Square*, int);
    ~iterator();
    iterator& operator=(const iterator&);
    iterator& operator++(); //pre-increment
    iterator operator++(int); //post-increment
    iterator& operator--();
    iterator operator--(int);
    Square& operator*() const;
    Square* operator->() const;
    bool operator==(const iterator&);
    bool operator!=(const iterator&);
    bool operator<(const iterator&);
    bool operator>(const iterator&);
    bool operator<=(const iterator&);
    bool operator>=(const iterator&);
    iterator& operator+=(int);
    iterator operator+(int);
    iterator& operator-=(int);
    iterator operator-(int);
  };

private:
  Square* board;
  int width, height;

public:
  Board();
  Board(int);
  Board(int, int);

  std::string to_str();
  std::string line_to_str(iterator, iterator);

  Square* get_square(int, int);
  Square::Color get_color(int, int);
  void set_color(int, int, Square::Color);
  void set_color(int, Square::Color);

  iterator row_begin(int);
  iterator row_end(int);
  iterator col_begin(int);
  iterator col_end(int);

  bool find_pairs(iterator, iterator);
  bool solve_by_pairs();
  void solve();
};

#endif

