#pragma once

#include <cstdint>
#include <source_location>
#include <string>
#include <string_view>

//#include <format>

#include "../utils/misc.hpp"
#include "../frozen/traits.hpp"

namespace versa::logger {
   namespace detail {
      constexpr static inline auto only_file_name(std::string_view path) noexcept {
         for (std::size_t i=path.size()-1; i>0; --i) {
            if (path[i] == '\\' || path[i] == '/') {
               return path.substr(i+1, path.size()-i);
            }
         }
         return path;
      }

      constexpr static inline auto only_path(std::string_view path) noexcept {
         for (std::size_t i=path.size()-1; i>0; --i) {
            if (path[i] == '\\' || path[i] == '/') {
               return path.substr(0, i);
            }
         }
         return path;
      }
   } // namespace detail

   struct call_info {
      constexpr inline call_info(std::string_view path=__builtin_FILE(),
                                 std::string_view file=__builtin_FILE(), 
                                 std::string_view func=__builtin_FUNCTION(), 
                                 std::uint32_t line=__builtin_LINE(),
                                 std::uint32_t column=__builtin_COLUMN()) noexcept 
         : path(detail::only_path(path)), file(detail::only_file_name(file)), func(func), line(line), column(column) {}
      std::string_view path;
      std::string_view file;
      std::string_view func;
      std::uint32_t    line;
      std::uint32_t    column;
   };
} // namespace versa::util