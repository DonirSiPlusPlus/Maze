#ifndef MAZE_MODEL_IMPL_FILEWRITER_H_
#define MAZE_MODEL_IMPL_FILEWRITER_H_

#include <fstream>

#include "../include/Structures.h"

namespace s21::impl {

/* Класс для сохранения объекта в виде файла */
template <TypeObject T>
class FileWriter {};

/* Специализация FileWriter для объекта kMaze */
template <>
class FileWriter<kMaze> final {
 public:
  static bool write(const Maze &maze, std::string_view path) noexcept {
    std::ofstream fout(path.data());
    if (!fout.is_open() || maze.empty() || !maze.IsValid()) {
      return false;
    }

    fout << maze.rows << ' ' << maze.cols << '\n';

    for (size_type i(0); i < maze.rows; ++i) {
      for (size_type j(0); j < maze.cols; ++j) {
        fout << maze(i, j);
        char symb = (j == maze.cols - 1) ? '\n' : ' ';
        fout << symb;
      }
    }
    fout << '\n';

    for (size_type i(0); i < maze.rows; ++i) {
      for (size_type j(0); j < maze.cols; ++j) {
        fout << maze(i, j, 1);
        char symb = (j == maze.cols - 1) ? '\n' : ' ';
        fout << symb;
      }
    }

    fout.close();
    return true;
  }
};

/* Специализация FileWriter для объекта kCave */
template <>
class FileWriter<kCave> final {
 public:
  static bool write(const Cave &cave, std::string_view path) noexcept {
    std::ofstream fout(path.data());
    if (!fout.is_open() || cave.empty() || !cave.IsValid()) {
      return false;
    }

    fout << cave.rows << ' ' << cave.cols << '\n';
    for (size_type i(0); i < cave.rows; ++i) {
      for (size_type j(0); j < cave.cols; ++j) {
        fout << cave.data[i * cave.cols + j];
        char symb = (j == cave.cols - 1) ? '\n' : ' ';
        fout << symb;
      }
    }

    fout.close();
    return true;
  }
};

}  // namespace s21::impl

#endif  // MAZE_MODEL_IMPL_FILEWRITER_H_
