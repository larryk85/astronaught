#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include "util.hpp"

#include <astro/io.hpp>
#include <astro/utils.hpp>

using namespace astro;
using namespace astro::io;
using namespace astro::test;

//TEST_CASE("File Sink Tests", "[sink][file_sink]") {
//   SECTION("Check stdio sink types") {
//      auto sink_so = astro::io::native::get_stdio_sink<astro::io::stdio::out>();
//      auto sink_se = astro::io::native::get_stdio_sink<astro::io::stdio::err>();
//      auto sink_sl = astro::io::native::get_stdio_sink<astro::io::stdio::log>();
//
//      auto result = capture<stdio::out>([&]() { sink_so.write("Hello, World!, stdout\n"); });
//      CHECK((result == "Hello, World!, stdout\n"));
//      CHECK(capture_and_compare<stdio::err>([&]() { sink_se.write("Hello, World!, stderr\n"); },
//         "Hello, World!, stderr\n"));
//      CHECK(capture_and_compare<stdio::log>([&]() { sink_sl.write("Hello, World!, stdlog\n"); },
//         "Hello, World!, stdlog\n"));
//
//      CHECK(capture_and_compare<stdio::out>([&]() { sink_so.write("Hello, World!, stdout again\n"); },
//         "Hello, World!, stdout again\n"));
//
//      CHECK(capture_and_compare<stdio::err>([&]() { sink_se.write("Hello, World!, stderr again\n"); },
//         "Hello, World!, stderr again\n"));
//   }
//}