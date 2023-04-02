#ifndef _SQUARE_H
#define _SQUARE_H

class Square {

public:
  enum Color { NONE, WHITE, BLACK, DEAD };

private:
  Color value;

public:
  Square();

  void set(Color);
  Color get();
  Color get_inverse();
  bool is_empty();

  char to_char();
};

#endif
