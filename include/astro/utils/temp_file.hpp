#pragma once

#include <filesystem>
#include <random>
#include <string>
#include <string_view>
#include <iostream>

#include "misc.hpp"
#include "../compile_time/string.hpp"

namespace astro::util {
   constexpr static inline std::string_view default_random_string_charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

   static inline std::string generate_random_string(std::string tmpl, std::string_view charset = default_random_string_charset) {
      std::random_device rd;
      std::default_random_engine rng(rd());
      std::uniform_int_distribution<std::size_t> dist(0, charset.size() - 1);
      std::seed_seq sd{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
      std::mt19937 mt(sd);
      for (std::size_t i=0 ; i < tmpl.size(); i++) {
         if (tmpl[i] == '%')
            tmpl[i] = charset[dist(mt)];
      }
      return tmpl;
   }

   template <std::size_t N>
   static inline std::string generate_random_string(astro::ct::string<N> tmpl, std::string_view charset = default_random_string_charset) {
      return generate_random_string(tmpl.to_string(), charset); 
   }

   template <std::size_t N>
   static inline std::string generate_random_string(const char (&tmpl)[N], std::string_view charset = default_random_string_charset) {
      return generate_random_string(ct::string(tmpl)); 
   }

   template <typename S>
   requires (astro::ct::string_type<S> || util::string_type<S> || util::cstring_type<S>)
   static inline std::string generate_temp_file_name(S tmpl, std::string_view charset = default_random_string_charset) {
      std::string fn = generate_random_string(tmpl, charset);
      return (std::filesystem::temp_directory_path()/fn).string();
   }  

   /*
   template <typename FileType, typename S>
   requires (astro::ct::string_type<S> || util::string_type<S> || util::cstring_type<S>)
   static inline std::string generate_temp_file(S tmpl, std::string_view charset = default_random_string_charset) {
      const auto fn = generate_temp_file_name<S>(tmpl, charset);
      if constexpr (std::is_same_v<FileType, FILE*) {
         FILE* fp = fopen(fn.c_str(), "w");
         fclose(fp);
      } 
   }
   */

}

namespace astro::literals {
   template <astro::ct::string S>
   inline std::string operator""_rs() noexcept { 
      return astro::util::generate_random_string(S);
   }
}