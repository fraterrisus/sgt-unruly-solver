#!/usr/bin/env ruby
# frozen_string_literal: true

require_relative 'lib/board'

game_id = ARGV.shift

board = Board.parse_game_id(game_id)
puts board
puts

while board.solve
  puts board
  puts
  raise unless board.check
end
