#pragma once

#include <cstdint>

#include "bus/bus.hpp"

namespace glue {

template <typename SPIType>
class spi: public glue::bus, public SPIType {
public:
  using SPIType::SPIType;

  constexpr bool is_native() { return false; }

  void init() {}

  void tx(void *buf, size_t len) {}
};

} // namespace glue
