#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
#include <string>
#include <iterator>

#include "square.h"

class Board {

public:
  class iterator {

  protected:
    int inc;
    Square *ptr;

  public:
    iterator();
    iterator(const iterator&);
    iterator(Square*, int);
    //~iterator();
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
    int operator-(const iterator&);

  };

  class Line {

  public:
    iterator start;
    iterator end;
    int datum;

    Line();
    Line(const iterator&, const iterator&);

    std::string to_str();
    bool is_full();
    void count_nones();

    static std::string to_str(iterator, iterator);
    static bool is_full(iterator, iterator);
    static int count_nones(iterator, iterator);
  };

  typedef bool (Board::*FindFunc)(Line&);

private:
  Square* board;
  int width, height;

  iterator row_begin(int);
  iterator row_end(int);
  iterator col_begin(int);
  iterator col_end(int);

public:
  Board();
  Board(int);
  Board(int, int);

  std::string to_str();
  bool is_solved();

  Square* get_square(int, int);
  Square::Color get_color(int, int);
  void set_color(int, int, Square::Color);
  void set_color(int, Square::Color);

  Line fetch_row(int);
  Line fetch_col(int);

  bool find_pairs(Line&);
  bool find_gaps(Line&);
  bool find_halves(Line&);
  bool brute_force(Line&);
  bool solve_by_brute_force();
  bool solve_by_finder(FindFunc);
  void solve();
};

#endif

