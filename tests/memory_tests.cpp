#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <astro/info.hpp>
#include <astro/utils.hpp>
#include <astro/memory.hpp>

using namespace astro::memory;

template <std::size_t N>
struct test_alloc : allocator_base< test_alloc<N> > {

   constexpr test_alloc()
      : data(new int[N]), index(0) {}

   ~test_alloc() { delete[] data; }

   template <typename T>
   T* allocate_impl(std::size_t n) { 
      if (index + n > N) {
         throw std::runtime_error("Out of memory");
      }
      index += n;
      return data + index - n;
   }

   template <typename T>
   void deallocate_impl(T* ptr) { 
      if (check_ptr(ptr)) {
         throw std::runtime_error("Invalid pointer");
      }
      index = 0;
   }

   constexpr inline bool check_ptr(int* ptr) const noexcept {
      return ptr >= data && ptr < data + index;
   }

   int*        data  = nullptr;
   std::size_t index = 0;
};

/*
consteval static inline std::size_t page_size() {
   return 4096;
}

int global_value = 0;

LONG CALLBACK ExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
    if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        global_value = 2;
        std::cout << "Segmentation fault occurred!" << std::endl;
        return EXCEPTION_EXECUTE_HANDLER;
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

template <typename Func>
bool should_fail(Func&& func) {
   __try {
      func();
   } __except (ExceptionHandler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {
      return true;
   }
   return false;
}
*/


TEST_CASE("Mapper Tests", "[mapper_tests]") {
   SECTION("Check mapper_base") {
      memory_mapper mm;

      auto mp = mm.map<10>(access_mode::none);
      std::cout << "Page size: " << mm.page_size() << "\n";
      auto om = mm.protect<10>(mp, access_mode::read_write);
      CHECK(om == access_mode::none);
      om = mm.protect<10>(mp, access_mode::read);
      CHECK(om == access_mode::read_write);

      om = mm.protect<10>(mp, access_mode::read_write);

      for (int i = 0; i < 10; ++i) {
         reinterpret_cast<int*>(mp)[i] = i;
      }

      om = mm.protect<10>(mp, access_mode::read);

      for (std::size_t i = 0; i < 10; ++i) {
         CHECK((std::size_t)reinterpret_cast<int*>(mp)[i] == i);
      }

      om = mm.protect<10>(mp, access_mode::none);

      //should_fail([&]() {
      //   reinterpret_cast<int*>(mp)[0] = 1;
      //});

      //CHECK(global_value == 1);
   }
}

TEST_CASE("Allocator Tests", "[allocator_tests]") {
   SECTION("Check allocator_base") {
      test_alloc<10> ta;

      auto ptr = ta.allocate<int>(5);
      CHECK(ptr != nullptr);
      CHECK(ta.check_ptr(ptr) == true);
      CHECK(ta.check_ptr(ptr + 4) == true);
      CHECK(ta.check_ptr(ptr + 5) == false);

      auto ptr2 = ta.allocate<int>(5);
      CHECK(ptr2 != nullptr);
      CHECK(ta.check_ptr(ptr + 5) == true);
      CHECK(ta.check_ptr(ptr2) == true);
      CHECK(ta.check_ptr(ptr2 + 4) == true);
      CHECK(ta.check_ptr(ptr2 + 5) == false);
      CHECK_THROWS_MATCHES(ta.allocate<int>(1), std::runtime_error, Catch::Matchers::Message("Out of memory"));
   }
}

TEST_CASE("discriminant Tests", "[discriminant_tests]") {
   using namespace astro::memory;
   SECTION("Check discriminant") {
      auto ptr = new int(10);
      auto d = tagged_ptr{ptr, 1};
      CHECK(d.ptr() == ptr);
      CHECK(d.tag() == 1);
      d.as<int>() = 20;
      CHECK(*ptr == 20);
      d.tag(11);
      CHECK(d.tag() == 11);
      delete ptr;
   }
   SECTION("Check discriminant with nullptr") {
      auto d = tagged_ptr(nullptr, 1);
      CHECK(d.ptr() == nullptr);
      CHECK(d.tag() == 1);
   }
   SECTION("Check discriminant on static") {
      float f   = 3.14f;
      int32_t i = -10;
      uint16_t b  = 42;

      {
         auto d = tagged_ptr(&f, 1);
         CHECK(d.ptr() == &f);
         CHECK(d.as<float>() == 3.14f);
         CHECK(d.tag() == 1);
      }

      {
         auto d = tagged_ptr(&i, 2);
         CHECK(d.ptr() == &i);
         CHECK(d.as<int32_t>() == -10);
         CHECK(d.tag() == 2);
      }

      {
         auto d = tagged_ptr(&b, 3);
         CHECK(d.ptr() == &b);
         CHECK(d.as<uint16_t>() == 42);
         CHECK(d.tag() == 3);
      } 

      f = 42.4242f;
      i = 100;
      b = 10;

      {
         auto d = tagged_ptr(&f, 1);
         CHECK(d.ptr() == &f);
         CHECK(d.as<float>() == 42.4242f);
         CHECK(d.tag() == 1);
      }

      {
         auto d = tagged_ptr(&i, 2);
         CHECK(d.ptr() == &i);
         CHECK(d.as<int>() == 100);
         CHECK(d.tag() == 2);
      }

      {
         auto d = tagged_ptr(&b, 3);
         CHECK(d.ptr() == &b);
         CHECK(d.as<uint8_t>() == 10);
         CHECK(d.tag() == 3);
      }
   }
}