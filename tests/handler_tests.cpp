#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <astro/async.hpp>

using namespace astro::async;

#ifndef SIGSEGV
#define SIGSEGV EXCEPTION_ACCESS_VIOLATION
#endif

TEST_CASE("Handler Tests", "[handler_tests]") {
   SECTION("Check handler creation") {
      handler<signal::access_violation> av_hndlr([](auto info) {
         CHECK(info.sig == signal::access_violation);
         CHECK(info.code == SIGSEGV);
      });

      handler<signal::float_error> fe_hndlr([](auto info) {
         CHECK(info.sig == signal::float_error);
         CHECK(info.code == SIGFPE);
      });

      handler<signal::abort> ab_hndlr([](auto info) {
         CHECK(info.sig == signal::abort);
         CHECK(info.code == SIGABRT);
      });

      auto exec = executor(av_hndlr, fe_hndlr);
      auto fail = []() {
         volatile int* p = nullptr;
         p[0] = 0;
      };

      CHECK_THROWS_MATCHES((exec(fail)), std::runtime_error, Catch::Matchers::Message("signal handler failure"));

      int a = 0;
      auto test = [&]() {
         a = 42;
      };

      exec(test);

      CHECK(a == 42);

      ASTRO_TRY(exec) {
         a = 24;
      } ASTRO_CATCH {
         CHECK(false); // This should never be called
      }

      CHECK(a == 24);


      ASTRO_TRY(exec) {
         fail();
      } ASTRO_CATCH {
         CHECK(true);
         a = 0xdeadbeef;
      }

      CHECK(a == 0xdeadbeef);

      ASTRO_TRY(exec) {
         fail();
      } ASTRO_CATCH {
         CHECK(true);
         a = 0xbeefbeef;
      }

      CHECK(a == 0xbeefbeef);

      auto fail2 = []() -> int {
         volatile int a = 0;
         a = a/a;
         CHECK(false); // This should never be called
         return a; 
      };

      CHECK_THROWS_MATCHES(exec(fail2), std::runtime_error, Catch::Matchers::Message("signal handler failure"));

      auto fail3 = []() {
         abort();
         CHECK(false); // This should never be called
      };

      CHECK_THROWS_MATCHES(exec(fail3), std::runtime_error, Catch::Matchers::Message("signal handler failure"));
   }
}