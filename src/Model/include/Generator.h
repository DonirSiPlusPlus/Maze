#ifndef MAZE_MODEL_INCLUDE_GENERATOR_H_
#define MAZE_MODEL_INCLUDE_GENERATOR_H_

#include "../impl/Creator.h"

namespace s21 {

class Generator final {
 public:
  template <TypeObject T, typename... Parameters>
  Object<T> Generate(const GenerationSettings<T>& settings,
                     Parameters&&... function_params) const noexcept {
    Object<T> object;
    try {
      auto optObj = impl::Creator<T>::Create(
          settings, std::forward<Parameters>(function_params)...);
      if (optObj.has_value()) object = *optObj;
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
    return object;
  }
};

}  // namespace s21

#endif  // MAZE_MODEL_INCLUDE_GENERATOR_H_
