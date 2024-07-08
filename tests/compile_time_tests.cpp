#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <iostream>

#include <astro/compile_time.hpp>

using namespace astro;
using namespace astro::ct;
using namespace astro::util;

struct TestStruct {
   int a;
   float b;
   double c;
};

template <typename T>
struct TestStruct2 {
   int a;
   float b;
   double c;
};

enum TestEnum {
   A,
   B,
   C
};

enum class TestEnum2 {
   A,
   B,
   C
};

struct foo : public reflectable<foo> {
   std::string name;
   double value;
};

struct bar : public reflectable<bar, foo> {
   int a;
   float b;
};

struct baz : public reflectable<baz, bar> {
   std::string_view sv;
   TestEnum e;
};

template <typename... Ts>
struct nn {
   constexpr nn(Ts&&... ts) : t(std::forward<Ts>(ts)...) {}
   std::tuple<Ts...> t;
};

struct no : public nn<int, float, char> {
   constexpr no() : nn(1, 2, 3) {}
};

template <typename... Ptrs>
struct ptrs {
   constexpr ptrs(Ptrs&&... ptrs){
      util::unused(std::forward<Ptrs>(ptrs)...);
   }
   using ptr_types = std::tuple<Ptrs...>;
};

struct foobar : public reflectable<foobar> {
   int a;
   float b;
   double c;
   ASTRO_REFL(a,b,c)
};

TEST_CASE("Compile_Time Tests", "[compile_time_tests]") {
   SECTION("Check names") {
      constexpr int a = 10;

      CHECK(nameof<int>() == "int");
      CHECK(nameof<decltype(a)>() == "const int");
      CHECK((nameof<decltype(&a)>() == "const int*" || nameof<decltype(&a)>() == "const int *"));
      CHECK((valueof<a>() == "0xa" || valueof<a>() == "10"));

      CHECK(nameof<TestStruct>() == "TestStruct");
      CHECK(nameof<TestStruct2<int>>() == "TestStruct2");
      CHECK(nameof<TestStruct2<TestStruct2<int>>>() == "TestStruct2");

      CHECK(nameof<TestEnum>() == "TestEnum");
      CHECK(nameof<TestEnum2>() == "TestEnum2");
   }

   SECTION("Check traits") {
      CHECK(is_templated_v<TestStruct2<int>>);
      CHECK(!is_templated_v<int>);
      CHECK(!is_templated_v<TestEnum>);
      CHECK(!is_templated_v<TestEnum2>);
   }

   SECTION("Check values") {
      constexpr int a = 10;
      constexpr double c = 3.14;

      CHECK((value_of_v<a> == "0xa" || value_of_v<a> == "10"));
      CHECK(type_name_v<decltype(c)> == "const double");

      constexpr TestEnum e = TestEnum::A;
      constexpr TestEnum2 e2 = TestEnum2::B;

      CHECK(value_of_v<e> == "A");
      CHECK(value_of_v<e2> == "TestEnum2::B");
      CHECK(enum_name_v<TestEnum, e> == "A");
      CHECK(enum_name_v<TestEnum2, e2> == "B");
   }

   SECTION("Check reflecting") {
      CHECK(!is_reflectable_v<TestStruct>);
      CHECK(!is_reflectable_v<TestStruct2<int>>);
      CHECK(!is_reflectable_v<TestEnum>);
      CHECK(!is_reflectable_v<TestEnum2>);

      CHECK(foo::is_reflectable);
      CHECK(is_reflectable_v<foo>);
      CHECK(is_reflectable_v<bar>);
      CHECK(is_reflectable_v<baz>);

      foobar fb;
      auto names = fb.reflected_names();
      CHECK(names[0] == "a");
      CHECK(names[1] == "b");
      CHECK(names[2] == "c");

      auto addrs = fb.reflected_addrs();
      CHECK(addrs[0] == &fb.a);
      CHECK(addrs[1] == &fb.b);
      CHECK(addrs[2] == &fb.c);

   }
}

template <int cnt, typename R>
struct print_randoms {
   static void print() {
      using rnd_t = next<R>::type;
      std::cout << rnd_t::value << " ";
      print_randoms<cnt-1, rnd_t>::print();
   }
};

template <typename R>
struct print_randoms<0, R> {
   static void print() {
      std::cout << next<R>::value << " " << std::endl;
   }
};

TEST_CASE("Random Tests", "[random_tests]") {
   SECTION("Check random number generator") {
      std::cout << "Default Seed : " << ct::defaultseed << std::endl;
      //print_randoms<10, random<bernoulli_distribution<linear_congruential_engine<uint_fast32_t>, 1, 10>>::type>::print();
   }
}
