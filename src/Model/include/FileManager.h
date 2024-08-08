#ifndef MAZE_MODEL_INCLUDE_FILEMANAGER_H_
#define MAZE_MODEL_INCLUDE_FILEMANAGER_H_

#include "../impl/FileReader.h"
#include "../impl/FileWriter.h"

namespace s21 {

class FileManager final {
 public:
  template <TypeObject T>
  Object<T> Read(std::string_view filename) const noexcept {
    Object<T> object;
    try {
      impl::FileReader<T> reader;
      if (auto optObj = reader.Read(filename); optObj.has_value())
        object = *optObj;
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
    return object;
  }

  template <TypeObject T>
  bool Save(const Object<T>& data, std::string_view path) const noexcept {
    bool result(false);
    try {
      result = impl::FileWriter<T>::write(data, path);
    } catch (const std::exception& e) {
      return false;
    }
    return result;
  }
};

}  // namespace s21

#endif  // MAZE_MODEL_INCLUDE_FILEMANAGER_H_
