#pragma once

#include "../utils.hpp"

#include "uint128.hpp"

namespace versa::cryptid {

   class ulid {
      public:
         ulid() = default;
         ulid(const ulid&) = default;
         ulid(ulid&&) = default;
         ulid& operator=(const ulid&) = default;
         ulid& operator=(ulid&&) = default;
         ~ulid() = default;
      private:
         uint128 _data;
   };
} // namespace versa::cryptid