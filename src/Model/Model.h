#ifndef MAZE_MODEL_MODEL_H_
#define MAZE_MODEL_MODEL_H_

#include "include/FileManager.h"
#include "include/Generator.h"
#include "include/Solver.h"

namespace s21 {

class Model final {
 public:
  template <TypeObject T>
  Object<T> ReadFile(std::string_view filename) const noexcept {
    return fileManager_.Read<T>(filename);
  }

  template <TypeObject T, typename... Parameters>
  Object<T> Generate(const GenerationSettings<T>& settings,
                     Parameters&&... function_params) const noexcept {
    return generator_.Generate<T>(settings,
                                  std::forward<Parameters>(function_params)...);
  }

  template <TypeObject T>
  bool Save(const Object<T>& data, std::string_view path) const noexcept {
    return fileManager_.Save(data, path);
  }

  template <TypeObject T>
  std::vector<Point> Solve(const Object<T>& data, const Point& from,
                           const Point& to) const noexcept {
    return solver_.Solve(data, from, to);
  }

 private:
  FileManager fileManager_;
  Generator generator_;
  Solver solver_;
};

}  // namespace s21

#endif  // MAZE_MODEL_MODEL_H_
