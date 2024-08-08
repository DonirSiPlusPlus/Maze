#ifndef MAZE_MODEL_IMPL_CREATOR_H_
#define MAZE_MODEL_IMPL_CREATOR_H_

#include <algorithm>
#include <chrono>
#include <random>

#include "../include/Structures.h"

namespace s21 {

template <TypeObject T>
struct GenerationSettings final {};

template <>
struct GenerationSettings<kMaze> final {
  size_type rows{0}, cols{0};
};

template <>
struct GenerationSettings<kCave> final {
  GenerationSettings<kCave>(size_type rows_, size_type cols_,
                            size_type live_chance_, size_type born_limit_,
                            size_type death_limit_)
      : rows(rows_), cols(cols_), live_chance(live_chance_) {
    GenerationSettings<kCave>(born_limit_, death_limit_);
  }

  GenerationSettings<kCave>(size_type born_limit_, size_type death_limit_) {
    if (born_limit_ < 8) born_limit = born_limit_;
    if (death_limit_ < 8) death_limit = death_limit_;
  }

  size_type rows = 0, cols = 0;
  size_type live_chance = 0;
  size_type born_limit = 3, death_limit = 3;
  const size_type generation_count{40};
};

using CaveSettings = GenerationSettings<kCave>;
using MazeSettings = GenerationSettings<kMaze>;

namespace impl {

/* Класс генерирующий объект в случайном виде */
template <TypeObject T>
class Creator {};

/* Специализация Сreator для объекта kMaze */
template <>
class Creator<kMaze> final {
 public:
  static std::optional<Maze> Create(const MazeSettings &s) noexcept {
    if (s.rows < 1 || s.cols < 1 || s.rows > 50 || s.cols > 50) {
      return std::nullopt;
    }

    Maze maze{data_type(s.rows * s.cols), data_type(s.rows * s.cols), s.rows,
              s.cols};

    size_type set_count(1);
    std::vector<size_type> line(s.cols);
    std::transform(line.begin(), line.end(), line.begin(),
                   [&](size_type el) { return !el ? set_count++ : el; });

    auto merge = [&](size_type i) {
      size_type merged_item = line[i + 1];
      for (size_type c(0); c != line.size(); ++c) {
        if (line[c] == merged_item) {
          line[c] = line[i];
        }
      }
    };

    srand(time(0));
    for (size_type i(0); i < s.rows; ++i) {
      std::transform(
          line.begin(), line.end(), line.begin(),
          [&set_count](size_type el) { return !el ? set_count++ : el; });

      for (size_type c(0); c < line.size(); ++c) {
        bool wall = (rand() % 2) ? 1 : 0;
        if (wall || line[c] == line[c + 1]) {
          maze(i, c) = 1;
        } else {
          merge(c);
        }
      }
      maze(i, s.cols - 1) = 1;

      for (size_type j(0); j < line.size(); ++j) {
        bool wall = (rand() % 2) ? 1 : 0;
        if (wall) {
          size_type wallsCount(0);
          for (size_type k(0); k < line.size(); ++k) {
            if (line[k] == line[j] && !maze(i, k, 1)) {
              ++wallsCount;
            }
          }

          if (wallsCount != 1) {
            maze(i, j, 1) = 1;
          }
        }
      }

      if (i + 1 == s.rows) break;

      for (size_type c(0); c < s.cols; ++c) {
        if (maze(i, c, 1)) {
          line[c] = 0;
        }
      }
    }

    for (size_type c(0); c < s.cols; ++c) {
      maze((s.rows - 1), c, 1) = 1;
      if (c + 1 != s.cols && line[c] != line[c + 1]) {
        maze((s.rows - 1), c) = 0;
        merge(c);
      }
    }

    return {maze};
  }
};

/* Специализация Сreator для объекта kCave */
template <>
class Creator<kCave> final {
 public:
  /* Генерация с использованием клеточного автомата */
  static std::optional<Cave> Create(const CaveSettings &s) noexcept {
    if (s.rows < 1 || s.cols < 1 || s.rows > 50 || s.cols > 50) {
      return std::nullopt;
    }

    Cave cave{Initializer(s.rows, s.cols, s.live_chance)};
    return Create(s, cave);
  }

  /* Генерирование нового поколения */
  static std::optional<Cave> Create(const CaveSettings &s,
                                    Cave &cave) noexcept {
    std::optional<Cave> optCave(cave);
    Cave temp{cave.data, cave.rows, cave.cols};

    for (size_type i(0); i < s.generation_count; ++i) {
      for (size_type row(0); row < cave.rows; ++row) {
        for (size_type col(0); col < cave.cols; ++col) {
          size_type count_living(CountLiveNeighbours(row, col, cave));
          if (cave(row, col) && count_living < s.death_limit) {
            temp(row, col) = false;
          } else if (!cave(row, col) && count_living > s.born_limit) {
            temp(row, col) = true;
          }
        }
      }
      cave = temp;
    }

    return optCave;
  }

 private:
  /* Генерирование 1-ого поколения */
  static Cave Initializer(size_type rows, size_type cols,
                          size_type live_chance) noexcept {
    std::default_random_engine re(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_type> dist(0, 100);

    data_type generation(rows * cols);
    std::generate(generation.begin(), generation.end(),
                  [&dist, &re, live_chance] {
                    size_type chance = dist(re);
                    return (chance <= live_chance) ? 1 : 0;
                  });

    return {generation, rows, cols};
  }

  // Подсчет живых соседей(клеток)
  static size_type CountLiveNeighbours(size_type row, size_type col,
                                       const Cave &cave) noexcept {
    size_type count(0);
    for (auto item :
         {cave(row, col - 1), cave(row, col + 1), cave(row - 1, col),
          cave(row + 1, col), cave(row - 1, col - 1), cave(row - 1, col + 1),
          cave(row + 1, col - 1), cave(row + 1, col + 1)}) {
      if (item) count++;
    }

    return count;
  }
};

}  // namespace impl
}  // namespace s21

#endif  // MAZE_MODEL_IMPL_CREATOR_H_
