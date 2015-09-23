#ifndef _SQUARE_H
#define _SQUARE_H

class Square {

public:
  enum Color { NONE, WHITE, BLACK };

private:
  Color value;

public:
  Square();

  void set(Color);
  Color get();

  char to_char();
};

#endif
