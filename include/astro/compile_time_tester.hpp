#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <type_traits>

namespace astro::util {
   template <auto... Vs>
   struct param_pack {
      static constexpr std::size_t size = sizeof...(Vs);
      using pack_type = std::tuple<decltype(Vs)...>;

      constexpr static inline auto values_v = std::make_tuple(Vs...);

      template <std::size_t I>
      using type = typename std::tuple_element<I, pack_type>::type;

      template <std::size_t I>
      constexpr static inline auto value = std::get<I>(pack_type{Vs...});
   };

   namespace detail {
      template <template<auto...> class C, param_pack Pack, std::size_t... Is>
      constexpr static inline auto make_from_param_pack(std::index_sequence<Is...>) -> C<Pack.template value<Is>...>;
   } // namespace astro::util::detail

   template <template<auto...> class C, param_pack Pack>
   constexpr static inline auto make_from_param_pack() -> decltype(detail::make_from_param_pack<C, Pack>(std::make_index_sequence<Pack.size>{}));

   template <template<auto...> class C, param_pack Pack>
   using make_from_param_pack_t = decltype(make_from_param_pack<C, Pack>);

   template < template<auto...> class C, param_pack, typename = std::void_t<> >
   struct is_valid_value : std::false_type {};

   template < template<auto...> class C, param_pack Pack >
   struct is_valid_value<C, Pack, std::void_t<make_from_param_pack_t<C,Pack>>> : std::true_type {};
} // end namespace astro::test