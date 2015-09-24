#include "square.h"

Square::Square() {
  value = NONE;
}

void Square::set(Color val) {
  value = val;
}

Square::Color Square::get() {
  return value;
}

Square::Color Square::get_inverse() {
  switch (value) {
    case WHITE:
      return BLACK;
    case BLACK:
      return WHITE;
    default:
      return NONE;
  }
}

bool Square::is_empty() {
  return (value == NONE);
}

char Square::to_char() {
  switch (value) {
    case NONE:
      return '.';
    case WHITE:
      return '0';
    case BLACK:
      return '1';
    default:
      return ' ';
  }
}
