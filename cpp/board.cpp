#include <algorithm>
#include <vector>

#include "board.h"

#define USE_HEURISTICS

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

bool Board::is_solved() {
  for (int x = 0; x < width; x++) {
    if (! fetch_col(x).is_full()) return false;
  }
  for (int y = 0; y < height; y++) {
    if (! fetch_row(y).is_full()) return false;
  }
  return true;
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

Board::Line Board::fetch_row(int y) {
  return Board::Line(row_begin(y), row_end(y));
}

Board::Line Board::fetch_col(int x) {
  return Board::Line(col_begin(x), col_end(x));
}

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
void showint(int i) {
  std::cout << (i-1);
}

bool Board::brute_force(Board::Line &line) {
  // Count the number of each color in the line.
  int blacks=0, whites=0, nones=0;
  for (Board::iterator it = line.begin; it < line.end; it++) {
    switch(it->get()) {
      case Square::NONE:
        nones++; break;
      case Square::BLACK:
        blacks++; break;
      case Square::WHITE:
        whites++; break;
    }
  }
  // If the line is complete, why are we here?
  if (nones == 0) { return false; }

  bool changes = false;
  int i=0;
  int count = line.count();
  // The only reason this is a vector<int> and not vector<Color> is for easier
  // printing. This is almost certainly fixable.
  std::vector< int > perm;

  //std::cout << "Baseline: ";
  //std::cout << line.to_str() << "\n";

  // Temporary line, holding a copy of the current line plus the permuted
  // possible additional values
  Square *tempboard = new Square[count];

  // Temporary line, holding the reduction of all valid permutations
  Square *results = new Square[count];
  for (i=0; i<count; i++) { results[i].set(Square::NONE); }

  // Iterators over tempboard
  Board::iterator tempstart(tempboard, 1);
  Board::iterator tempend(tempboard + count, 1);
  Board::Line templine(tempstart, tempend);

  // Determine how many white and black squares are left to be added to this
  // line (game rule: exactly half of the line is each color)
  for (i=0; i<(count / 2) - whites; i++) { perm.push_back(Square::WHITE); }
  for (i=0; i<(count / 2) - blacks; i++) { perm.push_back(Square::BLACK); }

  // Permutation loop.
  do {
    i = 0;
    // Write {templine} with (line with empty tiles) + (permutation of tiles).
    // In other words, for every NONE square in the current line, grab a tile
    // off the permutation and drop it in place.
    std::vector< int >::iterator perm_ptr = perm.begin();
    for (Board::iterator line_ptr = line.begin; line_ptr != line.end; line_ptr++) {
      Square::Color col = line_ptr->get();
      if (col == Square::NONE) {
        tempboard[i].set((Square::Color)*perm_ptr);
        perm_ptr++;
      } else {
        tempboard[i].set(col);
      }
      i++;
    }
    //std::cout << "Applied:  " << templine.to_str() << "  perm: ";
    //std::for_each(perm.begin(), perm.end(), &showint);

    /* If the applied combination of tiles is valid (has no triplets), then try
     * to "save" its value onto the {results} line. This is a reduce()
     * operation; the goal is to say, if every valid permutation has a WHITE in
     * index 6, then index 6 *must* be WHITE. But if one valid perm has WHITE
     * and another valid perm has BLACK, then we don't know what index 6 should
     * be, so don't write anything. */
    if (templine.is_valid()) {
      //std::cout << " valid";
      for (i=0; i<count; i++) {
        Square::Color res, tmp;
        res = results[i].get();
        tmp = tempboard[i].get();
        /* If the results square is NONE, then write the temporary value.
         * If the results square is DEAD, then leave it DEAD.
         * If the results square is different than the temporary value, mark
         *   the results square DEAD.
         * If the results square is the same, leave it as it is. */
        if (res == Square::NONE) 
          results[i].set(tmp);
        else if (res != tmp)
          results[i].set(Square::DEAD);
      }
    }

    //std::cout << "\n";

  // This is the magic permutation function (thanks, STL!)
  } while(std::next_permutation(perm.begin(), perm.end()));

  /*
  Board::iterator resstart(results, 1);
  Board::iterator resend(results + count, 1);
  Board::Line resline(resstart, resend);
  std::cout << "Changes:  " << resline.to_str();
  */

  // Iterate over the {results} line. Any square that (a) isn't marked DEAD (b)
  // isn't already present in {line} should be updated with the {result}.
  i = 0;
  for (Board::iterator line_ptr = line.begin; line_ptr != line.end; line_ptr++) {
    Square::Color res = results[i].get();
    if (res != Square::DEAD && res != line_ptr->get()) {
      line_ptr->set(res);
      changes = true;
    }
    i++;
  }

  /*
  if (! changes) {
    std::cout << "  none applied";
  }
  std::cout << "\n";
  */

  // Clean up after yourself and report whether you did anything.
  delete(tempboard);
  delete(results);
  return changes;
}

/* Collect all Lines, sort based on the number of blank squares, eliminate
 * everything below a certain threshold, then try to brute-force each line. As
 * soon as one is successful, stop. */
bool Board::solve_by_brute_force() {
#ifdef USE_HEURISTICS
  int cutoff = 3;
#else
  int cutoff = 1;
#endif
  std::vector< Board::Line > lines;
  for (int x = 0; x < width; x++) {
    Board::Line col = fetch_col(x);
    col.datum = col.count_colors(Square::NONE);
    if (col.datum >= cutoff) { lines.push_back(col); }
  }
  for (int y = 0; y < height; y++) {
    Board::Line row = fetch_row(y);
    row.datum = row.count_colors(Square::NONE);
    if (row.datum >= cutoff) { lines.push_back(row); }
  }
  std::sort(lines.begin(), lines.end(), &Board::Line::ordering);
  for (std::vector< Board::Line >::iterator it = lines.begin(); it != lines.end(); it++)
    if (brute_force(*it)) return true;
  return false;
}

/*******************/

bool Board::find_pairs(Board::Line &line) {
  if (line.is_full()) { return false; }
  bool changes = false;
  for (Board::iterator x = line.begin; x < (line.end - 1); x++) {
    Board::iterator y = x+1;
    if ((! x->is_empty()) && (x->get() == y->get())) {
      Square::Color newcol = x->get_inverse();
      Board::iterator w = x-1;
      Board::iterator z = x+2;
      if ((w >= line.begin) && (w->is_empty())) { w->set(newcol); changes = true; }
      if ((z <  line.end  ) && (z->is_empty())) { z->set(newcol); changes = true; }
    }
  }
  return changes;
}

bool Board::find_gaps(Board::Line &line) {
  if (line.is_full()) { return false; }
  bool changes = false;
  for (Board::iterator x = line.begin; x < (line.end - 2); x++) {
    Board::iterator y = x+1;
    Board::iterator z = x+2;
    if (!x->is_empty() && y->is_empty() && !z->is_empty() && (x->get() == z->get())) {
      y->set(x->get_inverse()); changes = true;
    }
  }
  return changes;
}

bool Board::find_halves(Board::Line &line) {
  bool changes = false;
  int blacks=0, whites=0, nones=0;
  for (Board::iterator it = line.begin; it < line.end; it++) {
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
  if (blacks == line.count() / 2) newcol = Square::WHITE;
  if (whites == line.count() / 2) newcol = Square::BLACK;
  if (newcol != Square::NONE)
    for (Board::iterator it = line.begin; it < line.end; it++)
      if (it->is_empty()) { it->set(newcol); changes = true; }
  return changes;
}

bool Board::solve_by_finder(Board::FindFunc ff) {
  bool changes = false;
  for (int x = 0; x < width; x++) {
    Board::Line col = fetch_col(x);
    //std::cout << "c" << x << ": " << col->to_str() << "\n";
    changes = (this->*ff)(col) || changes;
    //std::cout << "    " << col->to_str() << "\n";
  }
  for (int y = 0; y < height; y++) {
    Board::Line row = fetch_row(y);
    //std::cout << "r" << y << ": " << row->to_str() << "\n";
    changes = (this->*ff)(row) || changes;
    //std::cout << "    " << row->to_str() << "\n";
  }
  return changes;
}

/*******************/

void Board::solve() {
  bool changes;
  while (!is_solved()) {
#ifdef USE_HEURISTICS
    changes = true;
    while (changes) {
      changes = false;
      changes = changes || solve_by_finder(&Board::find_pairs);
      changes = changes || solve_by_finder(&Board::find_gaps);
      changes = changes || solve_by_finder(&Board::find_halves);
      //if (changes) std::cout << to_str();
    }
#endif
    changes = solve_by_brute_force();
    // Either we're done, or something went really really wrong.
    if (!changes) { return; }
    //std::cout << to_str();
  }
  std::cout << to_str();
}
