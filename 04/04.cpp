#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Coord {
  int x;
  int y;
};

struct Direction {
  int dx;
  int dy;
};

struct Grid {
  Grid(std::vector<char>&& grid, int width, int height) : m_grid {grid}, m_width {width}, m_height {height} {}

  char operator[] (int index) const {
    return m_grid[index];
  }

  char at(int x, int y) const {
    if (x >= m_width || y >= m_height || x < 0 || y < 0) {
      return '$';
    }
    return m_grid[y * m_width + x];
  }

  char at(Coord coords) const {
    return this->at(coords.x, coords.y);
  }

  std::vector<char> m_grid;
  int m_width;
  int m_height;
};

Grid grid_from_file(std::string const& filename) {
  std::ifstream file {filename};
  if (!file) {
    std::cout << "Input file cannot be read" << std::endl;
  }

  std::vector<char> grid;
  std::string line;
  int width = 0;
  int height = 0;
  while(file >> line) {
    grid.insert(grid.end(), line.begin(), line.end());
    width = line.size();
    height++;
  }
  return {std::move(grid), width, height};
}

Coord get_next_pos(Grid const& grid, Coord curr_pos, Direction direction) {
  return {curr_pos.x + direction.dx, curr_pos.y + direction.dy};
}

bool char_correct(Grid const& grid, std::string const& word, Coord pos, Direction direction, int count) {
  if (grid.at(pos) != word[count]) {
    return false;
  } else {
    count++;
  }
  if (count == word.size()) {
    return true;
  } else {
    Coord next_pos = get_next_pos(grid, pos, direction);
    return char_correct(grid, word, next_pos, direction, count);
  }
}

int solve_rec(Grid const& grid) {
  int total_count = 0;
  for (int y = 0; y < grid.m_width; y++) {
    for (int x = 0; x < grid.m_height; x++) {
      for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
          if (dx == 0 && dy == 0) {
            continue;
          }
          total_count += char_correct(grid, "XMAS", {x, y}, {dx, dy}, 0);
        }
      }
    }
  }
  return total_count;
}

int main() {
  int width;
  int height;
  Grid grid = grid_from_file("input.txt");
  int result = solve_rec(grid);
  // std::cout << "Solution for part 1: " << result << std::endl;
}
