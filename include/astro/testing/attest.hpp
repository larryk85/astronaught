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
      constexpr inline named_object() = default;
      constexpr inline named_object(T&& object)
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
            //constexpr auto pattern = std::get<I>(value).name;
            if constexpr (S.template starts_with<std::get<I>(Patterns{}).name>()) {
               return std::get<I>(Patterns{}).name;   
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
      builtin_eq = 0,
      builtin_ne = 1,
      builtin_le = 2,
      builtin_lt = 3,
      builtin_ge = 4,
      builtin_gt = 5,
      user_op,
      user_val,
      op = 6,
      val = 7,
      end = 8,
      unknown = 9,
      native_op = 10
   };

   namespace detail {
      template <ct::string S>
      consteval inline auto consume_ws() { 
         if constexpr (S.size() <= 0) {
            return S;   
         } else {
            constexpr auto idx = ct::find_first_not<S, ' '>();
            if constexpr (idx == -1) {
               return ct::string{""};
            } else {
               return S.substr(ct::range<idx>());
            }
         }
      }

      template <ct::range Bnds, token_type TokType, ct::string Lex=ct::string{""}, std::int64_t Idx=-1>
      struct token {
         constexpr static inline auto bnds = Bnds;
         constexpr static inline auto type = TokType;
         constexpr static inline auto lex  = Lex;
         constexpr static inline auto idx  = Idx;
      };

      template <ct::string S>
      consteval inline auto tokenize() noexcept {
         using namespace astro::literals;
         if constexpr (S.size() <= 0) {
            return token<ct::range<0,0>{}, token_type::end>{};
         } else if constexpr (S.starts_with("equals"_fs)) {
            return token<ct::range<0, 6>{}, token_type::builtin_eq>{};
         } else if constexpr (S.starts_with("not equals"_fs)) {
            return token<ct::range<0, 10>{}, token_type::builtin_ne>{};
         } else if constexpr (S.starts_with("greater than"_fs)) {
            if constexpr (S.template eq_from<13, "or equal to"_fs>()) {
               return token<ct::range<0, 24>{}, token_type::builtin_ge>{};
            } else {
               return token<ct::range<0, 13>{}, token_type::builtin_gt>{};
            }
         } else if constexpr (S.starts_with("less than"_fs)) {
            if constexpr (S.template eq_from<10, "or equal to"_fs>()) {
               return token<ct::range<0, 22>{}, token_type::builtin_le>{};
            } else {
               return token<ct::range<0, 9>{}, token_type::builtin_lt>{};
            }
         } else if constexpr (S.starts_with("=="_fs)) {
            return token<ct::range<0, 2>{}, token_type::builtin_eq>{};
         } else if constexpr (S.starts_with("!="_fs)) {
            return token<ct::range<0, 2>{}, token_type::builtin_ne>{};
         } else if constexpr (S.starts_with(">="_fs)) {
            return token<ct::range<0, 2>{}, token_type::builtin_ge>{};
         } else if constexpr (S.starts_with(">"_fs)) {
            return token<ct::range<0, 1>{}, token_type::builtin_gt>{};
         } else if constexpr (S.starts_with("<="_fs)) {
            return token<ct::range<0, 2>{}, token_type::builtin_le>{};
         } else if constexpr (S.starts_with("<"_fs)) {
            return token<ct::range<0, 1>{}, token_type::builtin_lt>{};
         } else if constexpr (S[0] == '{') {
            constexpr auto end = ct::find_first<S, '}'>();
            return token<ct::range<0, end+1>{}, token_type::user_val>{};
         } else if constexpr (S[0] == '`') {
            constexpr auto end = ct::find_first<S, '`'>();
            return token<ct::range<0, end+1>{}, token_type::user_op>{};
         } else {
            return token<ct::range<0, 0>{}, token_type::unknown>{};
         }
      }

      template <token Token>
      consteval inline bool is_builtin() noexcept {
         return Token.type == token_type::builtin_eq || Token.type == token_type::builtin_ne ||
                Token.type == token_type::builtin_ge || Token.type == token_type::builtin_gt ||
                Token.type == token_type::builtin_le || Token.type == token_type::builtin_lt;
      }

      template <token Token>
      consteval inline auto get_builtin() noexcept {
         if constexpr (Token.type == token_type::builtin_eq) {
            return [](auto l, auto r) { return l == r; };
         } else if constexpr (Token.type == token_type::builtin_ne) {
            return [](auto l, auto r) { return l != r; };
         } else if constexpr (Token.type == token_type::builtin_ge) {
            return [](auto l, auto r) { return l >= r; };
         } else if constexpr (Token.type == token_type::builtin_gt) {
            return [](auto l, auto r) { return l > r; };
         } else if constexpr (Token.type == token_type::builtin_le) {
            return [](auto l, auto r) { return l <= r; };
         } else if constexpr (Token.type == token_type::builtin_lt) {
            return [](auto l, auto r) { return l < r; };
         } else {
            static_assert(!is_builtin<Token>(), "invalid builtin token");
            util::unreachable();
         }
      }

      template <token Token>
      consteval inline bool is_user_val() noexcept { return Token.type == token_type::val; }

      template <token Token>
      constexpr inline bool is_user_op() noexcept { return Token.type == token_type::op; }

      template <token Token>
      consteval inline bool is_end() noexcept { return Token.type == token_type::end; }

      template <token Token>
      consteval inline bool is_unknown() noexcept { return Token.type == token_type::unknown; }

      template <ct::string S>
      consteval inline auto unwrap() noexcept {
         constexpr auto start = ct::find_first_not<S, ' ', '{', '`'>();
         constexpr auto end   = ct::rfind_first_not<S, ' ', '{', '`'>();
         return S.substr(ct::range<start, end>{});
      }

      template <typename R>
      [[deprecated]] consteval inline void ppp(R&&) {}

      template <bool P, typename A, typename  B>
      consteval inline auto which(A&& a, B&& b) {
         if constexpr (P) return std::forward<A>(a);
         else return std::forward<B>(b);
      }

      template <ct::string S, std::int64_t Idx = 0>
      consteval inline auto parse() noexcept {
         using namespace astro::literals;
         constexpr auto tok = tokenize<S>();
         using null_res_t = std::pair<std::tuple<>, std::tuple<>>;

         ///< First tuple is the operand set and the second is the value set
         if constexpr (is_end<tok>()) {
            return null_res_t{};
         } else if constexpr (is_user_op<tok>() || is_builtin<tok>() || is_user_val<tok>()) {
            constexpr auto next  = consume_ws<S.substr(ct::range<tok.bnds.upper_bound>{})>();
            constexpr auto rest  = parse<next, which<is_builtin<tok>()>(Idx, Idx+1)>();
            constexpr auto lex   = which<is_builtin<tok>()>(S.substr(tok.bnds), unwrap<S.substr(tok.bnds)>());
            constexpr auto arg   = std::make_tuple(token<tok.bnds, tok.type, lex, Idx>{});;
            constexpr auto ops   = which<!is_user_val<tok>()>(std::tuple_cat(arg, rest.first), rest.first);
            constexpr auto vals  = which<is_user_val<tok>()>(std::tuple_cat(arg, rest.second), rest.second);
            return std::make_pair(ops, vals);
         } else {
            constexpr auto lex  = S.substr(tok.bnds);
            using namespace std::literals;
            constexpr auto msg = "parse failure at token ["_fs + lex + "]"_fs;
            static_assert(!is_unknown<tok>(), "parsing failure");
            return null_res_t{};
         }
      }

      template <typename T>
      constexpr inline decltype(auto) invoke_or_fwd(T&& t) noexcept {
         if constexpr (std::is_invocable_v<T>) {
            return std::invoke(std::forward<T>(t));
         } else {
            return std::forward<T>(t);
         }
      }

      template <typename Op, typename... Args>
      constexpr inline auto evaluate(Op&& op, Args&&... args) noexcept {
         return std::invoke(std::forward<Op>(op), invoke_or_fwd(std::forward<Args>(args))...);
      }

      template <ct::string Nm, typename Tuple, std::size_t I = 0>
      constexpr inline std::size_t find_index() noexcept {
         if constexpr (I < std::tuple_size_v<std::decay_t<Tuple>>) {
            if constexpr (std::tuple_element_t<I, std::decay_t<Tuple>>::name == Nm) {
               return I;   
            } else {
               return find_index<Nm, Tuple, I + 1>();
            }
         }
         return std::numeric_limits<std::size_t>::max();
      }

      template <ct::string S, std::size_t I, std::size_t J, class Ops, class Vals, typename Args>
      constexpr inline auto execute(Args&& args) noexcept {
         if constexpr (I < std::tuple_size_v<Ops>) {
            constexpr auto op  = std::tuple_element_t<I, Ops>{}; //std::get<I>(Ops{});
            if constexpr (is_builtin<op>()) {
               //static_assert(J + 2 < std::tuple_size_v<Vals>, "incorrect number of value arguments");
               constexpr auto v0 = std::tuple_element_t<J, Vals>{}; //std::get<J>(Vals);
               constexpr auto v1 = std::tuple_element_t<J + 1, Vals>{}; //std::get<J + 1>(Vals);
               const auto& op_fn = get_builtin<op>();
               std::cout << "Is " << v0.lex.to_string_view() << " " << op.lex.to_string_view() << " " << v1.lex.to_string_view() << " true? ";
               bool res = evaluate(op_fn, std::get<v0.idx>(std::forward<Args>(args)), std::get<v1.idx>(std::forward<Args>(args)));
               std::cout << (res ? "Yes it was" : "No it wasn't") << std::endl;
               return res;
            }
         } else {
            return true;
         }
      }
   } // namespace detail

   template <ct::string Scenario, typename... Args>
   constexpr inline bool attest(Args&&... args) {
      auto tokens = detail::parse<detail::consume_ws<Scenario>()>();
      using ops_t = std::decay_t<decltype(std::get<0>(tokens))>;
      using vals_t = std::decay_t<decltype(std::get<1>(tokens))>;
      return detail::execute<Scenario, 0, 0, ops_t, vals_t>(std::make_tuple(std::forward<Args>(args)...));
   }

} // namespace astro::testing
