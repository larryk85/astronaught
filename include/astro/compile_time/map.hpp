#pragma once

#include <functional>
#include <optional>

#include "../utils/misc.hpp"

#include "traits.hpp"

namespace astro::ct {

   template <template <typename, typename> class Elem, typename A, typename B, typename... Elems>
   class map {
      public:

         using elem_t       = std::pair<A,B>;
         using iter_t       = elem_t*;
         using const_iter_t = const iter_t;

         struct a_tag { 
            using type = B; 
            constexpr static inline auto first_ptr  = &elem_t::first;
            constexpr static inline auto second_ptr = &elem_t::second;
         };

         constexpr static inline auto a_tag_v = a_tag{};

         struct b_tag { 
            using type = A; 
            constexpr static inline auto first_ptr  = &elem_t::second;
            constexpr static inline auto second_ptr = &elem_t::first;
         };

         constexpr static inline auto b_tag_v = b_tag{};

         template <typename T> using ref_t        = std::reference_wrapper<T>;
         template <typename T> using cref_t       = std::reference_wrapper<const T>;
         template <typename T> using maybe_ref_t  = std::optional<ref_t<T>>;
         template <typename T> using maybe_cref_t = std::optional<cref_t<T>>;

         map()           = delete;
         map(const map&) = default;
         map(map&&)      = default;

         constexpr inline map(const Elems&... elems) : _data({elems...}) {}
         constexpr inline map(Elems&&... elems) : _data({std::forward<Elems>(elems)...}) {}
         
         map& operator=(const map&) = default;
         map& operator=(map&&)      = default;

         constexpr static inline std::size_t size() noexcept { return sizeof...(Elems); }

         constexpr inline const_iter_t begin() const noexcept { return &_data[0]; }
         constexpr inline iter_t begin() noexcept { return &_data[0]; }
         constexpr inline const_iter_t end() const noexcept { return _data.data()+size(); }
         constexpr inline iter_t end() noexcept { return _data.data()+size(); }

         template <typename Key, typename Tag=a_tag>
         constexpr inline maybe_cref_t<typename Tag::type> at(const Key& key, Tag={}) const noexcept {
            for (const auto& e : _data) {
               if (e.*Tag::first_ptr == key) 
                  return std::cref(e.*Tag::second_ptr);
            }
            return {};
         }

         template <typename Key, typename Tag=a_tag>
         constexpr inline maybe_ref_t<typename Tag::type> at(const Key& key, Tag={}) noexcept {
            for (auto& e : _data) {
               if (e.*Tag::first_ptr == key) 
                  return std::ref(e.*Tag::second_ptr);
            }
            return {};
         }

         template <typename K, typename Tag>
         constexpr inline auto operator[](const std::pair<K, Tag>& p) const noexcept { return at(p.first, Tag{}); }
         template <typename K, typename Tag>
         constexpr inline auto operator[](const std::pair<K, Tag>& p) noexcept { return at(p.first, Tag{}); }

         constexpr inline auto operator[](const A& k) const noexcept { return at(k, a_tag{}); }
         constexpr inline auto operator[](const A& k) noexcept { return at(k, a_tag{}); }

      private:
         std::array<elem_t, size()> _data;
   };

   // Deduction guide
   template <template <typename, typename> class Elem, typename A, typename B, typename... Args>
   map(Elem<A, B>, Args...) -> map<Elem, A, B, Elem<A, B>, Args...>;

} // namespace astro::ct