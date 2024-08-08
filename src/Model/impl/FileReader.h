#ifndef MAZE_MODEL_IMPL_FILEREADER_H_
#define MAZE_MODEL_IMPL_FILEREADER_H_

#include <fstream>

#include "../include/Structures.h"

namespace s21::impl {

/* Базовый класс для специализаций FileReader */
class Base {
 public:
  Base() {}

 protected:
  virtual bool readData(std::ifstream &file,
                        data_type &data) const noexcept final {
    for (auto item : data) {
      char bool_symb(0);
      file >> bool_symb;
      if (file.eof()) {
        return false;
      } else if (bool_symb == '0') {
        item = 0;
      } else if (bool_symb == '1') {
        item = 1;
      } else if (bool_symb != ' ' || bool_symb != '\n') {
        return false;
      }
    }

    return true;
  }
};

/* Класс для считывания объекта из файла */
template <TypeObject T>
class FileReader {};

/* Специализация FileReader для объекта kMaze */
template <>
class FileReader<kMaze> final : public Base {
 public:
  std::optional<Maze> Read(std::string_view filename) const noexcept {
    std::ifstream file(filename.data());
    if (!file.is_open()) {
      return std::nullopt;
    }

    Maze maze;
    file >> maze.rows >> maze.cols;
    maze.vertical.resize(maze.rows * maze.cols);
    maze.horizont.resize(maze.rows * maze.cols);

    if (maze.rows < 1 || maze.cols < 1 || maze.rows > 50 || maze.cols > 50) {
      return std::nullopt;
    }

    if (!readData(file, maze.vertical)) return std::nullopt;
    if (!readData(file, maze.horizont)) return std::nullopt;

    if (!maze.IsValid()) {
      return std::nullopt;
    }
    file.close();

    return {maze};
  }
};

/* Специализация FileReader для объекта kCave */
template <>
class FileReader<kCave> final : public Base {
 public:
  std::optional<Cave> Read(std::string_view filename) const noexcept {
    std::ifstream file(filename.data());
    if (!file.is_open()) {
      return std::nullopt;
    }

    Cave cave;
    file >> cave.rows >> cave.cols;
    cave.data.resize(cave.rows * cave.cols);

    if (!cave.rows || !cave.cols || cave.rows > 50 || cave.cols > 50) {
      return std::nullopt;
    }

    if (!readData(file, cave.data)) return std::nullopt;

    if (!cave.IsValid()) {
      return std::nullopt;
    }
    file.close();

    return {cave};
  }
};

}  // namespace s21::impl

#endif  // MAZE_MODEL_IMPL_FILEREADER_H_
