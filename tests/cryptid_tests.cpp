#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <iostream>
#include <iomanip>

#include <astro/compile_time.hpp>
#include <astro/cryptid/uint128.hpp>

using namespace astro;
using namespace astro::cryptid;


struct none_t {
   using derived_type = none_t;
};

template <typename Derived, typename Parent=none_t>
struct reflectable {
   using parent_type = Parent;
   constexpr static inline bool is_reflectable = !std::is_same_v<Derived, none_t>;
   constexpr static inline bool has_parent     = !std::is_same_v<Parent, none_t>;
   constexpr static inline std::string_view get_name() noexcept {
      constexpr auto name = ct::nameof<Derived>();
      return name;
   }
   constexpr static inline std::string_view get_parent_name() noexcept {
      if constexpr (has_parent) {
         constexpr auto name = ct::nameof<Parent>();
         return name;
      } else {
         return "none_t";
      }
   }
};

template <typename Derived>
struct reflectable<Derived, none_t> {
   using parent_type = Derived;
   constexpr static inline bool is_reflectable = !std::is_same_v<Derived, none_t>;
   constexpr static inline bool has_parent     = false;
   constexpr static inline std::string_view get_name() noexcept {
      constexpr auto name = ct::nameof<Derived>();
      return name;
   }
   constexpr static inline std::string_view get_parent_name() noexcept {
      return "none_t";
   }
};

//#define vreflect(...) public reflectable<decltype(*this), __VA_ARGS__>

//struct reflectable {
//   using derived_type = Derived;
//   constexpr static bool is_reflectable = !std::is_same_v<Derived, none_t>;
//   constexpr static bool is_none = std::is_same_v<Derived, none_t>;
//
//   auto* self() noexcept {
//      return this;
//   }
//
//   const auto* self() const noexcept {
//      return this;
//   }
//
//   auto* parent() noexcept {
//      return static_cast<derived_type*>(this)->self();
//   }
//
//   const auto* parent() const noexcept {
//      return static_cast<const derived_type*>(this)->self();
//   }
//
//   constexpr static auto get_name() {
//      constexpr auto name = ct::nameof<Derived>();
//      return name;
//   }
//};


struct foo : public reflectable<foo> {
};

struct bar : public reflectable<bar, foo> {
};

TEST_CASE("Cryptid UINT128 Tests", "[cryptid_uint128_tests]") {
   SECTION("Check uint128_t") {
      uint128_t il = low_value{54};
      uint128_t ih = high_value{45};

      CHECK(il.low() == 54);
      CHECK(il.high() == 0);
      CHECK(ih.low() == 0);
      CHECK(ih.high() == 45);

      using namespace astro::literals;

      uint128_t i2 = 0x000000000000002d0000000000000036_ui128;

      CHECK(i2.low() == 54);
      CHECK(i2.high() == 45);

      uint64_t l = 0xffffffffffffffff;
      uint64_t h = 0xffffffffffffffff;

      uint128_t i3 = uint128_t{l, h};
      uint128_t i4 = 0xffffffffffffffffffffffffffffffff_ui128;

      CHECK(i3.low() == l);
      CHECK(i3.high() == h);
      CHECK(i4.low() == l);
      CHECK(i4.high() == h);
      CHECK(i3 == i4);

      CHECK((i3+1 == 0x0_ui128));
      CHECK((i3-1 == 0xfffffffffffffffffffffffffffffffe_ui128));

      CHECK(i3.to_string() == "0xffffffffffffffffffffffffffffffff");
      CHECK(uint128_t(9876543210987654321ULL, 12345678901234567890ULL).to_string() ==
            "0xab54a98ceb1f0ad2891087b8e3b70cb1");

      uint64_t l2 = 0xffffffffffffffff;
      std::cout << std::hex << "L2 " << (l2 + 34) << "\n";

      uint128_t i5 = {l, 0};
      i5 += 1;
      CHECK(i5.low() == 0);
      CHECK(i5.high() == 1);

      i5 -= 1;

      CHECK(i5.low() == l);

      foo f;
      std::cout << f.get_name() << "\n";
      std::cout << std::boolalpha << f.is_reflectable << "\n";
      std::cout << f.get_name() << "\n";
      std::cout << f.get_parent_name() << "\n";
      bar b;
      std::cout << b.get_name() << "\n";
      std::cout << b.get_parent_name() << "\n";

      //CHECK(i5.high() == 0);
   }
}