#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <versa/info.hpp>
#include <versa/utils.hpp>
#include <versa/memory.hpp>

using namespace versa::memory;

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

consteval static inline std::size_t page_size() {
   return 4096;
}

int global_value = 0;
/*
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

      for (std::size_t i = 0; i < 10; ++i) {
         reinterpret_cast<int*>(mp)[i] = i;
      }

      om = mm.protect<10>(mp, access_mode::read);

      for (std::size_t i = 0; i < 10; ++i) {
         CHECK(reinterpret_cast<int*>(mp)[i] == i);
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