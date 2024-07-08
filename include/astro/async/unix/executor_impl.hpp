#pragma once

#include <cctype>
#include <array>
#include <string>
#include <vector>

namespace astro::async {
} // namespace astro::async
inline static int32_t execute(std::string_view prog, const std::vector<std::string>& args) {
   std::string cmd{prog};
   for (const auto& arg : args) {
      cmd += " " + arg;
   }

   FILE* h = popen(cmd.c_str(), "r");
   if (h == nullptr) {
      system::error_log("internal failure, program {0} not found.", prog);
      return EX_SOFTWARE;
   }

   constexpr size_t             array_size = 64;
   std::array<char, array_size> buff{};

   std::size_t n = 0;

   while ((n = fread(buff.data(), 1, buff.size(), h)) > 0) {
      fwrite(buff.data(), 1, n, stdout);
   }

   auto rv = pclose(h);

   return WEXITSTATUS(rv);
}

inline static std::optional<std::string> execute_quiet(std::string_view prog, const std::vector<std::string>& args) {
   std::string cmd{prog};
   for (const auto& arg : args) {
      cmd += " " + arg;
   }
   // redirect stderr to stdout in case caller wants to parse the error
   cmd += " 2>&1";

   FILE* h = popen(cmd.c_str(), "r");
   if (h == nullptr) {
      system::error_log("internal failure, program {0} not found.", prog);
      return std::nullopt;
   }

   constexpr size_t             array_size = 1024;
   std::array<char, array_size> buff{};

   std::size_t n       = 0;
   std::string ret_val = "";

   while ((n = fread(buff.data(), 1, buff.size(), h)) > 0) {
      ret_val += std::string(buff.data(), n);
   }

   auto rv = pclose(h);

   return WEXITSTATUS(rv) == 0 ? std::optional<std::string>{ret_val} : std::nullopt;
}