# frozen_string_literal: true

class Board

  GAME_ID_REGEX = Regexp.compile(/\A(\d+)x(\d+):([a-zA-Z]+)\z/)

  def initialize(x, y)
    @dim_x = x
    @dim_y = y
    @squares = []
    @dim_y.times { @squares << [:clear] * @dim_x }
  end

  def check
    check_horizontal
    check_vertical
  end

  def assign_opposite(x1, y1, x2, y2)
    a = get(x1, y1)
    b = get(x2, y2)
    if a && b && a == b
      Board.opposite(a)
    end
  end

  def solve
    changes = solve_by_adjacency
    changes |= solve_by_count
    changes
  end

  def self.opposite(color)
    case color
    when :white
      :black
    when :black
      :white
    end
  end

  def maybe_set(x, y, value)
    raise "Illegal address #{x},#{y}" if x < 0 || y < 0 || x >= @dim_x || y >= @dim_y

    @squares[y][x] = value if @squares[y][x] == :clear
  end

  def set(x, y, value)
    raise "Illegal address #{x},#{y}" if x < 0 || y < 0 || x >= @dim_x || y >= @dim_y

    @squares[y][x] = value
  end

  def get(x, y)
    if x < 0 || y < 0 || x >= @dim_x || y >= @dim_y
      nil
    else
      @squares[y][x]
    end
  end

  def to_s
    rows = []
    @dim_y.times do |y|
      cols = []
      @dim_x.times do |x|
        cols << case get(x, y)
                when :clear
                  '.'
                when :black
                  'x'
                when :white
                  'o'
                else
                  raise "Unexpected symbol at #{x},#{y} = #{get(x, y)}"
                end
      end
      rows << cols.join('')
    end
    rows.join("\n")
  end

  def self.parse_game_id(game_id)
    unless (match_data = Board::GAME_ID_REGEX.match(game_id))
      raise "Game ID string couldn't be parsed"
    end

    dim_x = match_data[1].to_i || -1
    dim_y = match_data[2].to_i || -1

    board = Board.new(dim_x, dim_y)

    board_data = match_data[3] || ''

    raise 'Board dimensions must be even' if dim_x.odd? || dim_y.odd?

    coherent = false

    # Start the pointer just off the top-left edge of the board
    ptr_x = -1
    ptr_y = 0

    # Game IDs are a series of letters
    board_data.each_char do |ch|
      incr = 0
      color = :clear

      # The ord of the letter tells us how many squares to advance (a/A=1)
      if 'A' <= ch && ch <= 'Z'
        # UPCASE letters are for black squares
        incr = ch.ord - 64
        color = :black
      elsif 'a' <= ch && ch <= 'z'
        # downcase letters are for white squares
        incr = ch.ord - 96
        color = :white
      end

      ptr_x += incr
      # If we wrap off the right edge of the board, advance to the next line
      while ptr_x >= dim_x
        ptr_x -= dim_x
        ptr_y += 1
      end

      # The last char should drop us just past the last square
      if ptr_y == dim_y && ptr_x == 0
        coherent = true
        break
      end

      board.set(ptr_x, ptr_y, color)
    end

    raise "Game ID string didn't result in a good board" unless coherent

    board
  end

  private

  def check_horizontal
    @dim_y.times do |y|
      num_white = 0
      num_black = 0
      @dim_x.times do |x|
        sq = get(x, y)
        case sq
        when :clear
          next
        when :white
          num_white += 1
          return false if get(x+1, y) == :white && get(x+2, y) == :white
        when :black
          num_black += 1
          return false if get(x+1, y) == :black && get(x+2, y) == :black
        else
          raise "Unrecognized color #{sq}"
        end
      end
      max_num = @dim_x / 2
      return false if num_white > max_num || num_black > max_num
    end
    true
  end

  def check_vertical
    @dim_x.times do |x|
      num_white = 0
      num_black = 0
      @dim_y.times do |y|
        sq = get(x, y)
        case sq
        when :clear
          next
        when :white
          num_white += 1
          return false if get(x, y+1) == :white && get(x, y+2) == :white
        when :black
          num_black += 1
          return false if get(x, y+1) == :black && get(x, y+2) == :black
        else
          raise "Unrecognized color #{sq}"
        end
      end
      max_num = @dim_y / 2
      return false if num_white > max_num || num_black > max_num
    end
    true
  end

  def solve_by_adjacency
    changes = false
    @dim_y.times do |y|
      @dim_x.times do |x|
        sq = get(x, y)
        next unless sq == :clear

        col = assign_opposite(x-1, y, x+1, y) ||
              assign_opposite(x, y-1, x, y+1) ||
              assign_opposite(x-1, y, x-2, y) ||
              assign_opposite(x+1, y, x+2, y) ||
              assign_opposite(x, y-1, x, y-2) ||
              assign_opposite(x, y+1, x, y+2)

        if col
          changes = true
          set(x, y, col)
        end
      end
    end
    changes
  end

  def solve_by_count
    changes = false

    @dim_y.times do |y|
      num_black = 0
      num_white = 0
      @dim_x.times do |x|
      end
      if num_black == @dim_x / 2 && num_white < num_black
        changes = true
        @dim_x.times { |x| maybe_set(x, y, :white) }
      end
      if num_white == @dim_x / 2 && num_black < num_white
        changes = true
        @dim_x.times { |x| maybe_set(x, y, :black) }
      end
    end

    @dim_x.times do |x|
      num_black = 0
      num_white = 0
      @dim_y.times do |y|
      end
      if num_black == @dim_y / 2 && num_white < num_black
        changes = true
        @dim_y.times { |y| maybe_set(x, y, :white) }
      end
      if num_white == @dim_y / 2 && num_black < num_white
        changes = true
        @dim_y.times { |y| maybe_set(x, y, :black) }
      end
    end

    changes
  end

end
