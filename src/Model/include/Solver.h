#ifndef MAZE_MODEL_INCLUDE_SOLVER_H_
#define MAZE_MODEL_INCLUDE_SOLVER_H_

#include "../impl/PathFinder.h"

namespace s21 {

class Solver final {
 public:
  template <TypeObject T>
  std::vector<Point> Solve(const Object<T>& object, const Point& from,
                           const Point& to) const noexcept {
    std::vector<Point> path;
    try {
      impl::PathFinder<T> p_finder;
      if (auto optObj = p_finder.Find(object, from, to); optObj.has_value())
        path = *optObj;
    } catch (const std::exception& e) {
      return {};
    }
    return path;
  }
};

}  // namespace s21

#endif  // MAZE_MODEL_INCLUDE_SOLVER_H_
