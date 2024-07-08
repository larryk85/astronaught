#include <string>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <source_location>

//#include <astro/debug.hpp>
#include <astro/info.hpp>
#include <astro/utils.hpp>
#include <astro/compile_time/traits.hpp>
#include <astro/compile_time/meta.hpp>
#include <astro/magic_enum.hpp>

inline void print_function_name(const std::source_location& location = std::source_location::current()) {
   std::cout << "FN NAME " << location.function_name() << std::endl;
}

#define UI32(X) static_cast<uint32_t>(X)
TEST_CASE("Constants Tests", "[constants_tests]") {
   SECTION("Check Architecture") {
      REQUIRE(UI32(astro::info::architecture::unknown) == 0x0);
      REQUIRE(UI32(astro::info::architecture::x86) == 0x1);
      REQUIRE(UI32(astro::info::architecture::amd64) == 0x2);
      REQUIRE(UI32(astro::info::architecture::arm32) == 0x4);
      REQUIRE(UI32(astro::info::architecture::arm64) == 0x8);
      REQUIRE(UI32(astro::info::architecture::sparc32) == 0x10);
      REQUIRE(UI32(astro::info::architecture::sparc64) == 0x20);
      REQUIRE(UI32(astro::info::architecture::mips32) == 0x40);
      REQUIRE(UI32(astro::info::architecture::mips64) == 0x80);
      REQUIRE(UI32(astro::info::architecture::ppc32) == 0x100);
      REQUIRE(UI32(astro::info::architecture::ppc64) == 0x200);
      REQUIRE(UI32(astro::info::architecture::riscv32) == 0x400);
      REQUIRE(UI32(astro::info::architecture::riscv64) == 0x800);
      REQUIRE(UI32(astro::info::architecture::s390) == 0x1000);
      REQUIRE(UI32(astro::info::architecture::s390x) == 0x2000);
      REQUIRE(UI32(astro::info::architecture::wasm32) == 0x4000);
      REQUIRE(UI32(astro::info::architecture::wasm64) == 0x8000);
   }

   SECTION("Check Operating Systems") {
      REQUIRE(UI32(astro::info::operating_system::unknown) == 0x0);
      REQUIRE(UI32(astro::info::operating_system::windows) == 0x1);
      REQUIRE(UI32(astro::info::operating_system::macos) == 0x2);
      REQUIRE(UI32(astro::info::operating_system::ios) == 0x4);
   }

   SECTION("Check Compilers") {
      REQUIRE(UI32(astro::info::compiler::unknown) == 0x0);
      REQUIRE(UI32(astro::info::compiler::gcc) == 0x1);
      REQUIRE(UI32(astro::info::compiler::msvc) == 0x2);
      REQUIRE(UI32(astro::info::compiler::clang) == 0x4);
      REQUIRE(UI32(astro::info::compiler::intel) == 0x8);
      REQUIRE(UI32(astro::info::compiler::cl430) == 0x10);
   }

   SECTION("Check Build Types") {
      REQUIRE(UI32(astro::info::build_type::unknown) == 0x0);
      REQUIRE(UI32(astro::info::build_type::release) == 0x1);
      REQUIRE(UI32(astro::info::build_type::debug) == 0x2);
      REQUIRE(UI32(astro::info::build_type::minimum_size) == 0x4);
      REQUIRE(UI32(astro::info::build_type::profile) == 0x8);
      REQUIRE(UI32(astro::info::build_type::trace) == 0x10);
   }

   SECTION("Check Endianesses") {
      REQUIRE(UI32(astro::info::byte_order::unknown) == 0x0);
      REQUIRE(UI32(astro::info::byte_order::little) == 0x1);
      REQUIRE(UI32(astro::info::byte_order::big) == 0x2);
      REQUIRE(UI32(astro::info::byte_order::pdp) == 0x4);
   }

   SECTION("Check Languages") {
      REQUIRE(UI32(astro::info::language::unknown) == 0x0);
      REQUIRE(UI32(astro::info::language::c) == 0x1);
      REQUIRE(UI32(astro::info::language::cpp) == 0x2);
   }
}

TEST_CASE("build_info Tests", "[build_info_tests]") {
   SECTION("Checking Default Values") {
      astro::info::build_info info;
      REQUIRE(info.arch == astro::info::architecture::unknown);
      REQUIRE(info.order == astro::info::byte_order::unknown);
      REQUIRE(info.os == astro::info::operating_system::unknown);
      REQUIRE(info.comp == astro::info::compiler::unknown);  
      REQUIRE(info.comp_version == 0 );
      REQUIRE(info.build == astro::info::build_type::unknown);
      REQUIRE(info.lang == astro::info::language::unknown);
   }

   SECTION("Checking Compiled Values") {
      std::cout << "COMPILER " << int32_t(ASTRO_COMPILER) << "\n";
      std::cout << "COMP " << static_cast<uint32_t>(astro::info::build_info_v.comp) << "\n";
      std::cout << "V " << astro::info::build_info_v.comp_version << "\n";
      std::cout << ((ASTRO_COMPILER & ASTRO_CLANG_BUILD) == ASTRO_CLANG_BUILD) << std::endl;
   }
}

TEST_CASE("Util Tests", "[util_tests]") {
   using namespace astro::util;
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
}