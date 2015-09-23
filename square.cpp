#include "square.h"

Square::Square() {
  this->value = NONE;
}

void Square::set(Color val) {
  this->value = val;
}

Square::Color Square::get() {
  return this->value;
}

char Square::to_char() {
  switch (this->value) {
    case NONE:
      return '.';
    case WHITE:
      return '0';
    case BLACK:
      return '1';
  }
  return ' ';
}
