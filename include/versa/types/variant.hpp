#pragma once

#include "../memory/discriminated.hpp"
#include "../utils.hpp"

namespace versa::types {
   namespace detail {
      template <typename T, typename... Us>
      struct which_index;

      template <typename T, typename U, typename... Us>
      struct which_index<T, U, Us...> {
         static constexpr std::size_t value = 1 + which_index<T, Us...>::value;
      };

      template <typename T, typename... Us>
      struct which_index<T, T, Us...> {
         static constexpr std::size_t value = 0;
      };

   } // namespace versa::types::detail

   template <typename T, typename... Ts>
   constexpr inline std::size_t which_index_v = detail::which_index<T, Ts...>::value;

   template <typename... Ts>
   class variant {
      public:
         static_assert(sizeof...(Ts) > 0, "variant must have at least one type");
         static_assert(sizeof...(Ts) < (1<<17), "tag must be able to fit in 17 bits");

         constexpr static inline std::size_t cardinality = sizeof...(Ts);

         variant() = default;
         ~variant() = default;

         template <typename T>
         constexpr variant(T* t) : _data(memory::discriminate<T>(t, which_index_v<T, Ts...>)) {}

         variant(const variant&) = default;
         variant(variant&&) = default;
         variant& operator=(const variant&) = default;
         variant& operator=(variant&&) = default;

         template <typename T>
         constexpr inline variant& operator=(T* t) {
            _data = memory::discriminate<T>(t, which_index_v<T, Ts...>);
            return *this;
         }

         constexpr inline std::uint64_t tag() const noexcept {
            return memory::undiscriminate_tag(_data);
         }

         template <typename T>
         constexpr inline const T* get() const noexcept {
            if (tag() == which_index_v<T, Ts...>) {
               return memory::undiscriminate<T>(_data);
            }
            return nullptr;
         }

         constexpr inline memory::discriminated data() const noexcept {
            return _data;
         }

         constexpr inline uint64_t data_as_uint64() const noexcept {
            return std::bit_cast<uint64_t>(_data);
         }
      private:
         memory::discriminated _data;
   };
} // namespace versa::types