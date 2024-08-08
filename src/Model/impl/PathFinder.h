#ifndef MAZE_MODEL_IMPL_PATHFINDER_H_
#define MAZE_MODEL_IMPL_PATHFINDER_H_

#include "../include/Structures.h"

namespace s21::impl {

/* Структура для хранения соседней точки */
struct NearPoint {
  size_type x, y;
  bool wall;
};

/* Класс ищущий кратчайший путь между двумя точками */
template <TypeObject T>
class PathFinder {};

/* Специализация PathFinder для объекта kMaze */
template <>
class PathFinder<kMaze> final {
 public:
  std::optional<std::vector<Point>> Find(const Maze &maze, const Point &from,
                                         const Point &to) noexcept {
    if (!IsValid(maze, from, to)) return std::nullopt;
    InitializeStartState(maze, from);
    while (!old_wave_.empty()) {
      if (StepWave(maze, to)) break;
    }
    return MakePath(maze, to);
  }

 private:
  std::vector<size_type> path_map_;
  std::vector<Point> wave_, old_wave_;
  size_type wave_step_{0};
  const size_type myEnd_{2501};  //  невозможное количество шагов для решения

  /* Инициализация значений */
  void InitializeStartState(const Maze &maze, const Point &from) noexcept {
    wave_.clear();
    wave_step_ = 0u;
    old_wave_.push_back(from);
    path_map_ = std::vector<size_type>(maze.rows * maze.cols, myEnd_);
    path_map_[from.x * maze.rows + from.y] = wave_step_;
  }

  /* Проверка коректности лабиринта и координат */
  bool IsValid(const Maze &maze, const Point &from,
               const Point &to) const noexcept {
    if (maze.empty() || !maze.IsValid() || from.x >= maze.rows ||
        from.y >= maze.cols || to.x >= maze.rows || to.y >= maze.cols) {
      return false;
    }
    return true;
  }

  /* Построение всех возможных путей */
  bool StepWave(const Maze &maze, const Point &to) noexcept {
    ++wave_step_;
    for (auto [row, col] : old_wave_) {
      for (auto [x, y, wall] :
           {NearPoint{row + 1, col, maze(row, col, 1)},
            NearPoint{row - 1, col, maze((row - 1), col, 1)},
            NearPoint{row, col + 1, maze(row, col)},
            NearPoint{row, col - 1, maze(row, col - 1)}}) {
        if (!wall) {  // если нет стены
          if (path_map_[x * maze.rows + y] == myEnd_) {
            wave_.emplace_back(x, y);
            path_map_[x * maze.rows + y] = wave_step_;
          }
          if (x == to.x && y == to.y) {
            return true;
          }
        }
      }
    }
    old_wave_ = std::move(wave_);
    return false;
  }

  /* Поиск кратчайшего пути */
  std::optional<std::vector<Point>> MakePath(const Maze &maze,
                                             Point to) const noexcept {
    std::vector<Point> path{to};
    auto &[row, col] = to;
    size_type item(path_map_[row * maze.rows + col]);
    for (; item != 0; item = path_map_[row * maze.rows + col]) {
      if (path_map_[row * maze.rows + col - 1] + 1 == item &&
          !maze(row, col - 1) && (col - 1) < maze.cols) {
        --col;
      } else if (path_map_[row * maze.rows + col + 1] + 1 == item &&
                 !maze(row, col) && (col + 1) < maze.cols) {
        ++col;
      } else if (path_map_[(row - 1) * maze.rows + col] + 1 == item &&
                 !maze((row - 1), col, 1) && (row - 1) < maze.rows) {
        --row;
      } else if (path_map_[(row + 1) * maze.rows + col] + 1 == item &&
                 !maze(row, col, 1) && (row + 1) < maze.rows) {
        ++row;
      } else {
        return std::nullopt;
      }
      path.push_back(to);
    }

    std::reverse(path.begin(), path.end());
    return {path};
  }
};

/* Специализация PathFinder для объекта kCave */
template <>
class PathFinder<kCave> final {
 public:
  std::optional<std::vector<Point>> Find(const Cave &cave, const Point &from,
                                         const Point &to) noexcept {
    if (!IsValid(cave, from, to)) return std::nullopt;
    InitializeStartState(cave, from);
    while (!old_wave_.empty()) {
      if (StepWave(cave, to)) break;
    }

    return MakePath(cave, to);
  }

 private:
  std::vector<size_type> path_map_;
  std::vector<Point> wave_, old_wave_;
  size_type wave_step_{0};
  const size_type myEnd_{2501};  //  невозможное количество шагов для решения

  void InitializeStartState(const Cave &cave, const Point &from) noexcept {
    wave_.clear();
    wave_step_ = 0u;
    old_wave_.push_back(from);
    path_map_ = std::vector<size_type>(cave.rows * cave.cols, myEnd_);
    path_map_[from.x * cave.rows + from.y] = wave_step_;
  }

  bool IsValid(const Cave &cave, const Point &from,
               const Point &to) const noexcept {
    if (cave.empty() || !cave.IsValid() || from.x >= cave.rows ||
        from.y >= cave.cols || to.x >= cave.rows || to.y >= cave.cols) {
      return false;
    }
    return true;
  }

  bool StepWave(const Cave &cave, const Point &to) noexcept {
    ++wave_step_;
    for (auto [row, col] : old_wave_) {
      for (auto [x, y, wall] : {NearPoint{row + 1, col, cave(row + 1, col)},
                                NearPoint{row - 1, col, cave(row - 1, col)},
                                NearPoint{row, col + 1, cave(row, col + 1)},
                                NearPoint{row, col - 1, cave(row, col - 1)}}) {
        if (x >= cave.rows || y >= cave.cols) continue;
        if (!wall) {  // если нет стены
          if (path_map_[x * cave.rows + y] == myEnd_) {
            wave_.emplace_back(x, y);
            path_map_[x * cave.rows + y] = wave_step_;
          }
          if (x == to.x && y == to.y) {
            return true;
          }
        }
      }
    }
    old_wave_ = std::move(wave_);
    return false;
  }

  std::optional<std::vector<Point>> MakePath(const Cave &cave,
                                             Point to) const noexcept {
    std::vector<Point> path{to};
    auto &[row, col] = to;
    size_type item(path_map_[row * cave.rows + col]);
    for (; item != 0; item = path_map_[row * cave.rows + col]) {
      if (path_map_[(row + 1) * cave.rows + col] + 1 == item &&
          !cave(row + 1, col) && (row + 1) < cave.rows) {
        ++row;
      } else if (path_map_[(row - 1) * cave.rows + col] + 1 == item &&
                 !cave(row - 1, col) && (row - 1) < cave.rows) {
        --row;
      } else if (path_map_[row * cave.rows + (col + 1)] + 1 == item &&
                 !cave(row, col + 1) && (col + 1) < cave.cols) {
        ++col;
      } else if (path_map_[row * cave.rows + (col - 1)] + 1 == item &&
                 !cave(row, col - 1) && (col - 1) < cave.cols) {
        --col;
      } else {
        return std::nullopt;
      }
      if (row >= cave.rows || col >= cave.cols) continue;
      path.push_back(to);
    }
    std::reverse(path.begin(), path.end());
    return {path};
  }
};

}  // namespace s21::impl

#endif  // MAZE_MODEL_IMPL_PATHFINDER_H_
