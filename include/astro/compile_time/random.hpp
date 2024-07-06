#pragma once

#include <string_view>

#include "../utils/misc.hpp"

namespace astro::ct {
   constexpr static inline auto inits = std::string_view{__TIME__};
   constexpr static inline int defaultseed = (inits[0]-'0')*100000+(inits[1]-'0')*10000+
                                             (inits[3]-'0')*1000+(inits[4]-'0')*100+
                                             (inits[6]-'0')*10+inits[7]-'0';

   template <typename Engine>
   struct init {
      using type = Engine;
      static const auto value = Engine::value;
   };

   template <typename Engine>
   struct eval {
      using type = Engine;
      static const auto value = Engine::value;
   };
 
   template <typename Engine>
   struct random {
      using type = init<Engine>::type;
      static const auto value = type::value;
   };

   template <typename R>
   struct next {
      using type = eval<R>::type;
      static const auto value = type::value;
   };

   template <typename UIntType,
               UIntType seed = defaultseed,
               UIntType a = 16807, 
               UIntType c = 0, 
               UIntType m = 2147483647>
   struct linear_congruential_engine {
      static const UIntType value = seed;
      static const UIntType max   = m-1;
   };

   template <typename UIntType, UIntType seed, UIntType a, UIntType c, UIntType m>
   struct init<linear_congruential_engine<UIntType, seed, a, c, m>> {
      using type = eval<linear_congruential_engine<UIntType, seed, a, c, m>>::type;
      static const UIntType value = type::value;
   };

   template <typename UIntType, UIntType seed, UIntType a, UIntType c, UIntType m>
   struct eval<linear_congruential_engine<UIntType, seed, a, c, m>> {
      static const UIntType value = (a*seed+c)%m;
      using type = linear_congruential_engine<UIntType, (a*seed+c)%m, a, c, m>;
   };

   template <typename Engine, int N, int D, bool val = false>
   struct bernoulli_distribution {
      static const bool value = val;
   };

   template <typename Engine, int N, int D, bool b>
   struct eval<bernoulli_distribution<Engine, N, D, b>> {
      using next_type = next<Engine>::type;
      static const bool value = 
         (static_cast<double>(next_type::value)/next_type::max) 
         < 
         (static_cast<double>(N)/D);
      using type = bernoulli_distribution<next_type, N, D, value>;
   };

} // namespace astro::ct