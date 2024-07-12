#pragma once

#include <array>
#include <functional>
#include <string_view>
#include <tuple>
#include <utility>

#include "../utils.hpp"
#include "../compile_time.hpp"
#include "../info.hpp"

namespace astro::testing {

   template <ct::string Name, typename T>
   struct named_object {
      named_object() = default;
      named_object(T&& object)
         : object(std::forward<T>(object)) {}
      constexpr static inline ct::string name = Name;
      T object;
   };

   template <typename... Ts> constexpr static inline bool is_tuple_type(std::tuple<Ts&&...>) noexcept { return true; }
   template <typename... Ts> constexpr static inline bool is_tuple_type(Ts&&...) noexcept { return false; }

   template <typename... Ts>
   concept tuple_type = requires (Ts&&... ts) { is_tuple_type(ts...); };

   template <tuple_type Patterns>
   struct grammar_base {
      using patterns_t = Patterns;
      constexpr static inline auto value = Patterns{};

      consteval static inline std::size_t size() noexcept { return std::tuple_size_v<patterns_t>; }
      
      template <ct::string S, std::size_t I = 0>
      consteval static inline decltype(auto) matches() noexcept {
         if constexpr (I < std::tuple_size_v<patterns_t>) {
            constexpr auto pattern = std::get<I>(value).name;
            if constexpr (S.template starts_with<pattern>()) {
               return pattern;   
            } else {
               return matches<S, I + 1>();
            }
         } else {
            return ct::string{""};
         }
      }

      template <tuple_type T>
      consteval static inline decltype(auto) concat(const grammar_base<T>& other) noexcept {
         return std::tuple_cat(value, other.value);
      }
   };


   template <auto Arg, auto... Args>
   using grammar = std::conditional_t< (sizeof...(Args) == 0 && is_tuple_type(Arg)),
                                          grammar_base<decltype(Arg)>, 
                                          grammar_base<decltype(std::make_tuple(named_object<Arg, void*>{}, named_object<Args, void*>{}...))> >;

   constexpr static inline grammar<
                                   ct::string{"equals"},
                                   ct::string{"not equals"},
                                   ct::string{"greater than or equal to"},
                                   ct::string{"greater than"},
                                   ct::string{"less than or equal to"},
                                   ct::string{"less than"},
                                   ct::string{"=="},
                                   ct::string{"!="},
                                   ct::string{">="},
                                   ct::string{">"},
                                   ct::string{"<="},
                                   ct::string{"<"}
                                   > simple_grammar;
                                   
   enum class token_type : uint8_t {
      native_op,
      op,
      val,
      end
   };

   template <ct::string S, token_type T>
   struct token {
      constexpr static inline auto value = S;
      constexpr static inline auto type  = T;
   };

   template <ct::string S>
   using native_operator_token_t = token<S, token_type::native_op>;

   template <ct::string S>
   using operator_token_t = token<S, token_type::op>;

   template <ct::string S>
   using value_token_t = token<S, token_type::val>;

   using end_token_t = token<ct::string{""}, token_type::end>;

   namespace detail {
      template <ct::string S, std::size_t I=0>
      consteval static inline auto consume_ws() {
         if constexpr (I < S.size()) {
            if constexpr (S[I] == ' ') {
               return consume_ws<S, I + 1>();
            } else {
               return S.substr(ct::range<I, S.size()>());
            }
         } else {
            return S;
         }
      }

      template <ct::string S, class G>
      consteval static inline bool is_operator() noexcept {
         return G::template matches<S>() != ct::string{""};
      }

      template <ct::string S>
      consteval static inline auto named_type() noexcept {
         constexpr auto start1 = ct::find<S, '{'>();
         constexpr auto start2 = ct::find<S, '`'>();

         if constexpr (start1 == start2 && start1 >= S.size()) {
            return token_type::end; // next token is not of concern
         } else if constexpr (start1 < start2) {
            return token_type::val; // next is a value token
         } else {
            return token_type::op;  // next is a operator token
         }
      }

      template <auto... Ns>
      [[deprecated]] constexpr void put() noexcept {}

      template <ct::string S, class G>
      consteval static inline auto tokenize() noexcept {
         if constexpr (is_operator<S, G>()) {
            return native_operator_token_t<G::template matches<S>()>{};
         } else {
            constexpr auto tok_type = named_type<S>();
            if constexpr (tok_type != token_type::end) {
               constexpr auto start = ct::find<S, '{'>()+1; //tok_type == token_type::val ? '{' : '`'>()+1;
               constexpr auto end   = ct::find<S, '}'>();   //tok_type == token_type::val ? '}' : '`'>();
               //put<start, end>();
               static_assert(start < end, "tokenize failure");
               constexpr auto bnds = ct::range<start, end>{};
               //put<S.substr(bnds)>();
               return token<S.substr(bnds), tok_type>{};
            } else {
               return end_token_t{};
            }
         }
      }

      template <ct::string S, class G>
      consteval static inline auto parse() noexcept {
         constexpr auto tok  = tokenize<S, G>();
         if constexpr (tok.type != token_type::end) {
            constexpr auto offset = tok.type == token_type::native_op ? 0 : 2;
            constexpr auto next = consume_ws<S.substr(ct::range<tok.value.size()+offset>{})>();
            return std::tuple_cat(std::make_tuple(tok), parse<next, G>());
         } else {
            return std::tuple{};
         }
      }

      template <ct::string S>
      constexpr static inline auto extract() noexcept {
         constexpr auto npos  = std::string_view::npos;
         constexpr auto start = ct::find<S, '{'>();
         constexpr auto end   = ct::find<S, '}', start+1>();
         return ct::range<start, end>{};
      }

      template <ct::string S, typename Arg, typename... Args>
      constexpr static inline auto created_named_objects(Arg arg, Args&&... args) noexcept {
         constexpr auto bnds = extract<S>();
         constexpr auto name = S.substr(ct::range<bnds.lower_bound+1, bnds.upper_bound - bnds.lower_bound>{});
         static_assert(name.size() > 0, "placeholder name cannot be empty.");
         auto obj = std::tuple{named_object<name, Arg>{std::forward<Arg>(arg)}};
         if constexpr (sizeof...(Args) > 0) {
            return std::tuple_cat(obj, created_named_objects<S.substr(ct::range<bnds.upper_bound + 2>{})>(std::forward<Args>(args)...));
         } else {
            return obj;
         }
      }

      template <typename T>
      constexpr static inline decltype(auto) invoke_or_fwd(T&& t) noexcept {
         if constexpr (std::is_invocable_v<T>) {
            return std::invoke(std::forward<T>(t));
         } else {
            return std::forward<T>(t);
         }
      }

      template <typename Op, typename T, typename U>
      constexpr static inline auto evaluate(Op&& op, T&& t, U&& u) noexcept {
         return std::invoke(op, invoke_or_fwd(std::forward<T>(t)), invoke_or_fwd(std::forward<U>(u)));
      }

      template <ct::string Nm, typename Tuple, std::size_t I = 0>
      constexpr static inline std::size_t find_index() noexcept {
         if constexpr (I < std::tuple_size_v<std::decay_t<Tuple>>) {
            if constexpr (std::tuple_element_t<I, std::decay_t<Tuple>>::name == Nm) {
               return I;   
            } else {
               return find_index<Nm, Tuple, I + 1>();
            }
         }
         return std::numeric_limits<std::size_t>::max();
      }

   } // namespace detail

   template <ct::string Scenario, typename... Args>
   static inline void attest(Args&&... args) {
      auto placeholders_args = detail::created_named_objects<Scenario>(std::forward<Args>(args)...);
      using placeholders_t   = decltype(placeholders_args);
      std::cout << "p " << std::get<0>(placeholders_args).name.to_string() << std::endl;
      std::cout << "p " << std::get<0>(placeholders_args).object << std::endl;
      std::cout << "p " << std::get<1>(placeholders_args).name.to_string() << std::endl;
      std::cout << "p " << std::get<2>(placeholders_args).name.to_string() << std::endl;
      std::cout << "I " << detail::find_index<ct::string{"x"}, placeholders_t>() << std::endl;
      auto x = std::get<detail::find_index<ct::string{"x"}, placeholders_t>()>(placeholders_args).object;

      //auto op = detail::find<0>("op", placeholders_args);
      //auto x = std::get<0>(placeholders_args).object;
      //auto y = std::get<2>(placeholders_args).object;
      //auto result = detail::evaluate(op, x, y);
      //std::cout << scenario << " " << detail::invoke_or_fwd(x) << " " << detail::invoke_or_fwd(y) << " " << std::boolalpha <<  result << std::endl;
   }

} // namespace astro::testing