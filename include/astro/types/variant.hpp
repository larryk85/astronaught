#pragma once

#include "../memory/tagged_ptr.hpp"
#include "../utils.hpp"

namespace astro::types {
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

   } // namespace astro::types::detail

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
         constexpr variant(T* t) : _data(t, which_index_v<T, Ts...>) {}

         variant(const variant&) = default;
         variant(variant&&) = default;
         variant& operator=(const variant&) = default;
         variant& operator=(variant&&) = default;

         template <typename T>
         constexpr inline variant& operator=(T* t) {
            _data = {t, which_index_v<T, Ts...>};
            return *this;
         }

         constexpr inline std::uint64_t tag() const noexcept {
            return _data.tag();
         }

         template <typename T>
         constexpr inline T* ptr() noexcept {
            if (tag() == which_index_v<T, Ts...>) {
               return _data.as_ptr<T>();
            }
            return nullptr;
         }

         template <typename T>
         constexpr inline const T* ptr() const noexcept {
            if (tag() == which_index_v<T, Ts...>) {
               return _data.as_ptr<T>();
            }
            return nullptr;
         }
         
         template <typename T>
         constexpr inline const T& as() const {
            if (tag() == which_index_v<T, Ts...>) {
               return _data.as<T>();
            }
            throw std::bad_cast{};
         }

         template <typename T>
         constexpr inline T& as() {
            if (tag() == which_index_v<T, Ts...>) {
               return _data.as<T>();
            }
            throw std::bad_cast{};
         }

      private:
         memory::tagged_ptr _data;
   };
} // namespace astro::types