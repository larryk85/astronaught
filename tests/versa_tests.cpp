#include <string>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>

#include <versa/constants.hpp>
#include <versa/utils.hpp>

#define UI32(X) static_cast<uint32_t>(X)
TEST_CASE("Constants Tests", "[constants_tests]") {
   SECTION("Check Architecture") {
      REQUIRE(UI32(versa::info::architectures::unknown) == 0x0);
      REQUIRE(UI32(versa::info::architectures::x86) == 0x1);
      REQUIRE(UI32(versa::info::architectures::x64) == 0x2);
      REQUIRE(UI32(versa::info::architectures::arm32) == 0x4);
      REQUIRE(UI32(versa::info::architectures::arm64) == 0x8);
      REQUIRE(UI32(versa::info::architectures::sparc32) == 0x10);
      REQUIRE(UI32(versa::info::architectures::sparc64) == 0x20);
      REQUIRE(UI32(versa::info::architectures::mips32) == 0x40);
      REQUIRE(UI32(versa::info::architectures::mips64) == 0x80);
      REQUIRE(UI32(versa::info::architectures::ppc32) == 0x100);
      REQUIRE(UI32(versa::info::architectures::ppc64) == 0x200);
      REQUIRE(UI32(versa::info::architectures::riscv32) == 0x400);
      REQUIRE(UI32(versa::info::architectures::riscv64) == 0x800);
      REQUIRE(UI32(versa::info::architectures::s390) == 0x1000);
      REQUIRE(UI32(versa::info::architectures::s390x) == 0x2000);
      REQUIRE(UI32(versa::info::architectures::wasm32) == 0x4000);
      REQUIRE(UI32(versa::info::architectures::wasm64) == 0x8000);
   }

   SECTION("Check Operating Systems") {
      REQUIRE(UI32(versa::info::operating_systems::unknown) == 0x0);
      REQUIRE(UI32(versa::info::operating_systems::windows) == 0x1);
      REQUIRE(UI32(versa::info::operating_systems::macos) == 0x2);
      REQUIRE(UI32(versa::info::operating_systems::ios) == 0x4);
   }

   SECTION("Check Compilers") {
      REQUIRE(UI32(versa::info::compilers::unknown) == 0x0);
      REQUIRE(UI32(versa::info::compilers::msvc) == 0x1);
      REQUIRE(UI32(versa::info::compilers::gcc) == 0x2);
      REQUIRE(UI32(versa::info::compilers::clang) == 0x4);
      REQUIRE(UI32(versa::info::compilers::cl430) == 0x8);
      REQUIRE(UI32(versa::info::compilers::intel) == 0x10);
   }

   SECTION("Check Build Types") {
      REQUIRE(UI32(versa::info::build_types::unknown) == 0x0);
      REQUIRE(UI32(versa::info::build_types::debug) == 0x1);
      REQUIRE(UI32(versa::info::build_types::release) == 0x2);
      REQUIRE(UI32(versa::info::build_types::release_with_debug_info) == 0x4);
      REQUIRE(UI32(versa::info::build_types::profile) == 0x8);
      REQUIRE(UI32(versa::info::build_types::trace) == 0x10);
      REQUIRE(UI32(versa::info::build_types::minimum_size) == 0x20);
   }

   SECTION("Check Endianesses") {
      REQUIRE(UI32(versa::info::endianesses::unknown) == 0x0);
      REQUIRE(UI32(versa::info::endianesses::little) == 0x1);
      REQUIRE(UI32(versa::info::endianesses::big) == 0x2);
   }

   SECTION("Check Languages") {
      REQUIRE(UI32(versa::info::languages::unknown) == 0x0);
      REQUIRE(UI32(versa::info::languages::c) == 0x1);
      REQUIRE(UI32(versa::info::languages::cpp) == 0x2);
   }
}

TEST_CASE("build_info Tests", "[build_info_tests]") {
   SECTION("Checking Default Values") {
      versa::info::build_info info;
      REQUIRE(info.arch == versa::info::architectures::unknown);
      REQUIRE(info.endianess == versa::info::endianesses::unknown);
      REQUIRE(info.os == versa::info::operating_systems::unknown);
      REQUIRE(info.compiler == versa::info::compilers::unknown);  
      REQUIRE(info.compiler_version == 0 );
      REQUIRE(info.build == versa::info::build_types::unknown);
      REQUIRE(info.language == versa::info::languages::unknown);
   }

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

}