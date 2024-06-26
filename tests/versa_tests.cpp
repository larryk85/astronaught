#include <string>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <source_location>

#include <versa/info.hpp>
#include <versa/utils.hpp>
#include <versa/frozen/traits.hpp>
#include <versa/frozen/meta.hpp>
#include <versa/magic_enum.hpp>

inline void print_function_name(const std::source_location& location = std::source_location::current()) {
   std::cout << "FN NAME " << location.function_name() << std::endl;
}

#define UI32(X) static_cast<uint32_t>(X)
TEST_CASE("Constants Tests", "[constants_tests]") {
   SECTION("Check Architecture") {
      REQUIRE(UI32(versa::info::architecture::unknown) == 0x0);
      REQUIRE(UI32(versa::info::architecture::x86) == 0x1);
      REQUIRE(UI32(versa::info::architecture::amd64) == 0x2);
      REQUIRE(UI32(versa::info::architecture::arm32) == 0x4);
      REQUIRE(UI32(versa::info::architecture::arm64) == 0x8);
      REQUIRE(UI32(versa::info::architecture::sparc32) == 0x10);
      REQUIRE(UI32(versa::info::architecture::sparc64) == 0x20);
      REQUIRE(UI32(versa::info::architecture::mips32) == 0x40);
      REQUIRE(UI32(versa::info::architecture::mips64) == 0x80);
      REQUIRE(UI32(versa::info::architecture::ppc32) == 0x100);
      REQUIRE(UI32(versa::info::architecture::ppc64) == 0x200);
      REQUIRE(UI32(versa::info::architecture::riscv32) == 0x400);
      REQUIRE(UI32(versa::info::architecture::riscv64) == 0x800);
      REQUIRE(UI32(versa::info::architecture::s390) == 0x1000);
      REQUIRE(UI32(versa::info::architecture::s390x) == 0x2000);
      REQUIRE(UI32(versa::info::architecture::wasm32) == 0x4000);
      REQUIRE(UI32(versa::info::architecture::wasm64) == 0x8000);
   }

   SECTION("Check Operating Systems") {
      REQUIRE(UI32(versa::info::operating_system::unknown) == 0x0);
      REQUIRE(UI32(versa::info::operating_system::windows) == 0x1);
      REQUIRE(UI32(versa::info::operating_system::macos) == 0x2);
      REQUIRE(UI32(versa::info::operating_system::ios) == 0x4);
   }

   SECTION("Check Compilers") {
      REQUIRE(UI32(versa::info::compiler::unknown) == 0x0);
      REQUIRE(UI32(versa::info::compiler::gcc) == 0x1);
      REQUIRE(UI32(versa::info::compiler::msvc) == 0x2);
      REQUIRE(UI32(versa::info::compiler::clang) == 0x4);
      REQUIRE(UI32(versa::info::compiler::intel) == 0x8);
      REQUIRE(UI32(versa::info::compiler::cl430) == 0x10);
   }

   SECTION("Check Build Types") {
      REQUIRE(UI32(versa::info::build_type::unknown) == 0x0);
      REQUIRE(UI32(versa::info::build_type::release) == 0x1);
      REQUIRE(UI32(versa::info::build_type::debug) == 0x2);
      REQUIRE(UI32(versa::info::build_type::minimum_size) == 0x4);
      REQUIRE(UI32(versa::info::build_type::profile) == 0x8);
      REQUIRE(UI32(versa::info::build_type::trace) == 0x10);
   }

   SECTION("Check Endianesses") {
      REQUIRE(UI32(versa::info::byte_order::unknown) == 0x0);
      REQUIRE(UI32(versa::info::byte_order::little) == 0x1);
      REQUIRE(UI32(versa::info::byte_order::big) == 0x2);
      REQUIRE(UI32(versa::info::byte_order::pdp) == 0x4);
   }

   SECTION("Check Languages") {
      REQUIRE(UI32(versa::info::language::unknown) == 0x0);
      REQUIRE(UI32(versa::info::language::c) == 0x1);
      REQUIRE(UI32(versa::info::language::cpp) == 0x2);
   }
}

TEST_CASE("build_info Tests", "[build_info_tests]") {
   SECTION("Checking Default Values") {
      versa::info::build_info info;
      REQUIRE(info.arch == versa::info::architecture::unknown);
      REQUIRE(info.order == versa::info::byte_order::unknown);
      REQUIRE(info.os == versa::info::operating_system::unknown);
      REQUIRE(info.comp == versa::info::compiler::unknown);  
      REQUIRE(info.comp_version == 0 );
      REQUIRE(info.build == versa::info::build_type::unknown);
      REQUIRE(info.lang == versa::info::language::unknown);
   }

   SECTION("Checking Compiled Values") {
      std::cout << "COMPILER " << int32_t(VERSA_COMPILER) << "\n";
      std::cout << "COMP " << static_cast<uint32_t>(versa::info::build_info_v.comp) << "\n";
      std::cout << "V " << versa::info::build_info_v.comp_version << "\n";
      std::cout << ((VERSA_COMPILER & VERSA_CLANG_BUILD) == VERSA_CLANG_BUILD) << std::endl;
   }
}

TEST_CASE("Util Tests", "[util_tests]") {
   using namespace versa::util;
   check(true, "This should not throw an exception");

   try {
      check(false, "This should throw an exception");
   } catch (const std::runtime_error& e) {
      REQUIRE(std::string(e.what()) == "This should throw an exception");
   }

   std::string message = "This should not throw an exception";
   check(true, message);
   message = "This should throw an exception";
   try {
      check(false, message);
   } catch (const std::runtime_error& e) {
      REQUIRE(std::string(e.what()) == message);
   }

   std::string_view message_view = "This should not throw an exception";
   check(true, message_view);
   message_view = "This should throw an exception";
   try {
      check(false, message_view);
   } catch (const std::runtime_error& e) {
      REQUIRE(std::string(e.what()) == message_view);
   }

   int a = 0;
   check(true, [&a]() { a = 1; });

   REQUIRE(a == 0);
   check(false, [&a]() { a = 1; });
   REQUIRE(a == 1);

   enum class foo {
      a,
      b,
      c,
      d = -44,
      e = 100,
      f = 200
   };

   //versa::frozen::detail::enums::values<foo>(std::make_index_sequence<VERSA_ENUM_MAX_ELEMS>());
   constexpr auto mapp = versa::frozen::detail::enums::mappings<foo>();

   constexpr auto nm = mapp[0].first.first;

   //std::cout << "FFD " << versa::frozen::type_name_v<foo, false> << std::endl;
   //std::cout << "FFE " << versa::frozen::enum_name_v<static_cast<foo>(1), false> << std::endl;
   //std::cout << "FFG " << versa::frozen::enum_name_v<static_cast<foo>(1)> << std::endl;

   //std::cout << "name(22) " << versa::frozen::detail::enums::name<foo, 0>() << std::endl;
   //foo fa = foo::a;

   //auto name = magic_enum::enum_name(fa);
   //auto value = magic_enum::enum_integer(fa);
   //auto color2 = magic_enum::enum_cast<foo>("a");
   //auto color3 = magic_enum::enum_cast<foo>(0);
   //auto colors = magic_enum::enum_values<foo>();
   //auto names = magic_enum::enum_names<foo>();

   //std::cout << "name " << name << " value " << value << std::endl;


}