#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include "util.hpp"

#include <astro/testing/assertion.hpp>
#include <astro/utils.hpp>

using namespace astro;
using namespace astro::testing;
using namespace astro::test;

float test_func() {
   return 42;
}

template <std::size_t I, typename Tup>
static inline void print_all(const Tup& tup) {
   if constexpr (I < std::tuple_size_v<Tup>) {
      std::cout << " " << std::get<I>(tup).value.to_string() << " " << static_cast<uint32_t>(std::get<I>(tup).type) << std::endl;
      print_all<I+1>(tup);
   }
}

TEST_CASE("Testing Testing Framework", "[test][testing]") {
   using namespace astro::literals;
   auto equal = [](auto x, auto y) { return x == y; };
   attest<"{x} {op} {y}">(42, equal, test_func);
   using grammar_t = decltype(testing::simple_grammar);
   constexpr auto s1 = "equals {x} {foo} less than{y}"_fs;
   constexpr auto toks = testing::detail::parse<s1, grammar_t>();
   constexpr auto eqtok = testing::detail::tokenize<s1, grammar_t>().value;
   constexpr auto rest0 = testing::detail::consume_ws<s1.substr(ct::range<eqtok.size()>{})>();
   //constexpr auto xtok = testing::detail::tokenize<rest0, grammar_t>().value;
   std::cout << "eqtok " << eqtok.to_string() << std::endl;
   std::cout << "rest0 " << rest0.to_string() << std::endl;

   constexpr auto xtok  = testing::detail::tokenize<rest0, grammar_t>().value;
   constexpr auto rest1 = testing::detail::consume_ws<rest0.substr(ct::range<xtok.size()+2>{})>();
   std::cout << "xtok " << xtok.to_string() << std::endl;
   std::cout << "rest1 " << rest1.to_string() << std::endl;
   print_all<0>(toks);
}