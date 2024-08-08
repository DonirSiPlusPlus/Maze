#ifndef MAZE_MODEL_INCLUDE_STRUCTURES_H_
#define MAZE_MODEL_INCLUDE_STRUCTURES_H_

#include <iostream>
#include <optional>
#include <vector>

namespace s21 {

using data_type = std::vector<bool>;
using size_type = std::size_t;

/* Типы объектов: лабиринт и пещера */
enum TypeObject { kMaze, kCave };

/* Структура объекта */
template <TypeObject T>
struct Object {};

/* Специализация структуры для типа kMaze */
template <>
struct Object<kMaze> final {
  data_type vertical, horizont;
  size_type rows{0}, cols{0};

  bool IsValid() const noexcept {
    if (vertical.size() != rows * cols || horizont.size() != rows * cols)
      return false;

    if (rows < 1 || cols < 1 || rows > 50 || cols > 50) return false;

    for (size_type row = 0; row < rows; ++row)
      if (!vertical[row * cols + cols - 1]) return false;

    for (size_type col = 0; col < cols; ++col)
      if (!horizont[rows * (cols - 1) + col]) return false;

    return true;
  }

  void clear() noexcept {
    vertical.clear();
    horizont.clear();
    rows = cols = 0;
  }

  bool empty() const noexcept {
    return !rows && !cols && vertical.empty() && horizont.empty();
  }

  std::vector<bool>::reference operator()(size_type row, size_type col,
                                          bool line = 0) noexcept {
    if (line) {
      return horizont[row * rows + col];
    }
    return vertical[row * rows + col];
  }

  bool operator()(size_type row, size_type col, bool line = 0) const noexcept {
    if (row >= rows || col >= cols) {
      return true;
    }
    if (line) {
      return horizont[row * rows + col];
    }
    return vertical[row * rows + col];
  }

  bool operator==(const Object<kMaze> &other) const noexcept {
    if (rows != other.rows || cols != other.cols ||
        vertical != other.vertical || horizont != other.horizont) {
      return false;
    }
    return true;
  }

  // delete later
  void printMaze() const {
    for (size_type i = 0; i < horizont.size(); i++) {
      if (vertical[i] && horizont[i]) {
        std::cout << "_|";
      } else if (vertical[i] && !horizont[i]) {
        std::cout << " |";
      } else if (!vertical[i] && horizont[i]) {
        std::cout << "__";
      } else if (!vertical[i] && !horizont[i]) {
        std::cout << "  ";
      }
      if (!((i + 1) % cols)) std::cout << "\n";
    }
  }
};

/* Специализация структуры для типа kCave */
template <>
struct Object<kCave> final {
  data_type data;
  size_type rows{0}, cols{0};

  // Object<kCave>(const Object<kCave> &other) {}

  bool IsValid() const noexcept {
    if (rows < 1 || cols < 1 || rows > 50 || cols > 50) return false;

    if (data.size() != rows * cols) return false;

    return true;
  }

  bool empty() const noexcept { return !rows && !cols && data.empty(); }

  void clear() noexcept {
    data.clear();
    rows = cols = 0;
  }

  std::vector<bool>::reference operator()(size_type row,
                                          size_type col) noexcept {
    return data[row * cols + col];
  }

  bool operator()(size_type row, size_type col) const noexcept {
    if (row >= rows || col >= cols) return true;
    return data[row * cols + col];
  }

  bool operator==(const Object<kCave> &other) const noexcept {
    if (rows != other.rows || cols != other.cols || data != other.data) {
      return false;
    }
    return true;
  }

  // delete later
  void printCave() const {
    for (size_type i = 0; i < data.size(); i++) {
      if (data[i]) {
        std::cout << "⬛";
      } else {
        std::cout << "⬜";
      }
      if (!((i + 1) % cols)) std::cout << "\n";
    }
  }
};

/* Cтруктура координат точки */
struct Point {
  size_type x, y;

  Point(size_type x = 0, size_type y = 0) : x{x}, y{y} {}

  bool operator==(const Point &other) const noexcept {
    return (x == other.x && y == other.y);
  }
};

using Maze = Object<kMaze>;
using Cave = Object<kCave>;

}  // namespace s21

#endif  // MAZE_MODEL_INCLUDE_STRUCTURES_H_
