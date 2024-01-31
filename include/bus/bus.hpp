#pragma once

#include <cstdint>

namespace glue {

class bus {
public:
  bus() = default;

  constexpr bool is_native();

  void init();
  void tx(void *buf, size_t len);
};

} // namespace glue
