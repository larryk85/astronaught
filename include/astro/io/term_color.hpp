#pragma once

#include <cstdint>

#include <bit>
#include <concepts>
#include <type_traits>
#include <stdexcept>

namespace astro::io {
   enum class style : uint8_t {
      reset = 0,
      bold  = 1,
      dim   = 2,
      italic = 3,
      underline = 4,
      blink = 5,
      rblink = 6,
      reversed = 7,
      conceal = 8,
      crossed = 9,
   };

   enum class term_color : uint8_t {
      black = 0,
      red,
      green,
      yellow,
      blue,
      magenta,
      cyan,
      white,
   };

   template <typename Derived>
   struct color_base {
      color_base() = delete;
      color_base(const color_base&) = default;
      color_base(color_base&&) = default;

      constexpr inline color_base(uint8_t c) : value(c) {}
      constexpr inline color_base(term_color c) : value(static_cast<uint8_t>(c)) {}

      color_base& operator=(const color_base&) = default;
      color_base& operator=(color_base&&) = default;

      constexpr inline term_color base() const noexcept { return static_cast<term_color>(value); }
      constexpr inline uint8_t code() const noexcept { return value + Derived::offset_v; }
      constexpr static inline uint8_t offset() noexcept { return Derived::offset_v; }

      uint8_t value;
   };

   struct fg : public color_base<fg> {
      constexpr static inline uint8_t offset_v = 30;
   };

   struct bg : public color_base<bg> {
      constexpr static inline uint8_t offset_v = 40;
   };

   template <typename Color>
   concept color_type = std::is_base_of_v<color_base<Color>, Color>;

   template <color_type Color>
   struct bright : public color_base<bright<Color>> {
      constexpr static inline uint8_t offset_v = 60 + Color::offset_v;
      constexpr inline bright(Color c) : color_base<bright<Color>>(c.value) {}
   };

   enum class term_color_mode : uint8_t {
      fg = 38,
      bg = 48,
   };

   enum class term_color_reset : uint8_t {
      fg = 39,
      bg = 49,
   };

   enum class term_color_mode_type : uint8_t {
      rgb    = 2,
      normal = 5,
   };

   enum class term_color_reset_type : uint8_t {
      fg = 0,
      bg = 0,
   };

   struct rgba {
      rgba() = default;
      rgba(const rgba&) = default;
      rgba(rgba&&) = default;

      constexpr inline rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a=0) : r(r), g(g), b(b), a(a) {}
      constexpr inline rgba(uint32_t color) : r((color >> 24) & 0xFF), g((color >> 16) & 0xFF), b((color >> 8) & 0xFF), a(color & 0xFF) {}

      rgba& operator=(const rgba&) = default;
      rgba& operator=(rgba&&) = default;

      [[nodiscard]] constexpr inline uint32_t to_int() const noexcept { return std::bit_cast<uint32_t>(*this); }

      [[nodiscard]] constexpr inline bool operator==(const rgba& other) const noexcept { return to_int() == other.to_int(); }
      [[nodiscard]] constexpr inline bool operator!=(const rgba& other) const noexcept { return to_int() != other.to_int(); }

      [[nodiscard]] constexpr inline bool operator<(const rgba& other) const noexcept { return to_int() < other.to_int(); }
      [[nodiscard]] constexpr inline bool operator>(const rgba& other) const noexcept { return to_int() > other.to_int(); }
      [[nodiscard]] constexpr inline bool operator<=(const rgba& other) const noexcept { return to_int() <= other.to_int(); }
      [[nodiscard]] constexpr inline bool operator>=(const rgba& other) const noexcept { return to_int() >= other.to_int(); }

      #pragma warning(suppress: 4244) // MSVC warning about argument conversion erronesouly.
      [[nodiscard]] constexpr inline rgba operator+(const rgba& other) const noexcept { return rgba(r+other.r, g+other.g, b+other.b, a+other.a); }
      [[nodiscard]] constexpr inline rgba operator-(const rgba& other) const noexcept { return rgba(r-other.r, g-other.g, b-other.b, a-other.a); }
      [[nodiscard]] constexpr inline rgba operator*(const rgba& other) const noexcept { return rgba(r*other.r, g*other.g, b*other.b, a*other.a); }
      [[nodiscard]] constexpr inline rgba operator/(const rgba& other) const noexcept { return rgba(r/other.r, g/other.g, b/other.b, a/other.a); }

      [[nodiscard]] constexpr inline rgba operator+(uint8_t val) const noexcept { return rgba(r+val, g+val, b+val, a+val); }
      [[nodiscard]] constexpr inline rgba operator-(uint8_t val) const noexcept { return rgba(r-val, g-val, b-val, a-val); }
      [[nodiscard]] constexpr inline rgba operator*(uint8_t val) const noexcept { return rgba(r*val, g*val, b*val, a*val); }
      #pragma warning(default: 4244)

      std::string to_string() const {
         return std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a);
      }

      std::string format() const {
         return std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b);
      }
      uint32_t r : 8;
      uint32_t g : 8;
      uint32_t b : 8;
      uint32_t a : 8;
   };

   template <auto Color, rgba RGB>
   struct mapping {
      constexpr static inline std::size_t code = Color.code();
      constexpr static inline rgba rgb = RGB;
   };

   template <typename... Mappings>
   struct color_scheme {
      constexpr static inline std::size_t size = sizeof...(Mappings);
      using mappings_t = decltype(std::tuple<Mappings...>{Mappings{}...});

      template <std::size_t N>
      using mapping_t = std::tuple_element_t<N, mappings_t>;

      template <uint8_t Code, std::size_t N=0>
      constexpr static inline rgba rgb_of() {
         if constexpr (N == size) {
            throw std::out_of_range("Color code not found in color scheme");
         } else {
            if constexpr (mapping_t<N>::code == Code) {
               return mapping_t<N>::rgb;
            } else {
               return rgb_of<Code, N+1>();
            }
         }
      }

      template <auto Color>
      requires color_type<std::decay_t<decltype(Color)>>
      constexpr static inline rgba rgb_of() {
         return rgb_of<Color.code()>();
      }
   };

   constexpr static inline auto fg_black_v = fg{term_color::black};
   constexpr static inline auto fg_red_v = fg{term_color::red};
   constexpr static inline auto fg_green_v = fg{term_color::green};
   constexpr static inline auto fg_yellow_v = fg{term_color::yellow};
   constexpr static inline auto fg_blue_v = fg{term_color::blue};
   constexpr static inline auto fg_magenta_v = fg{term_color::magenta};
   constexpr static inline auto fg_cyan_v = fg{term_color::cyan};
   constexpr static inline auto fg_white_v = fg{term_color::white};
   constexpr static inline auto fg_bright_black_v = bright{fg{term_color::black}};
   constexpr static inline auto fg_bright_red_v = bright{fg{term_color::red}};
   constexpr static inline auto fg_bright_green_v = bright{fg{term_color::green}};
   constexpr static inline auto fg_bright_yellow_v = bright{fg{term_color::yellow}};
   constexpr static inline auto fg_bright_blue_v = bright{fg{term_color::blue}};
   constexpr static inline auto fg_bright_magenta_v = bright{fg{term_color::magenta}};
   constexpr static inline auto fg_bright_cyan_v = bright{fg{term_color::cyan}};
   constexpr static inline auto fg_bright_white_v = bright{fg{term_color::white}};

   constexpr static inline auto bg_black_v = bg{term_color::black};
   constexpr static inline auto bg_red_v = bg{term_color::red};
   constexpr static inline auto bg_green_v = bg{term_color::green};
   constexpr static inline auto bg_yellow_v = bg{term_color::yellow};
   constexpr static inline auto bg_blue_v = bg{term_color::blue};
   constexpr static inline auto bg_magenta_v = bg{term_color::magenta};
   constexpr static inline auto bg_cyan_v = bg{term_color::cyan};
   constexpr static inline auto bg_white_v = bg{term_color::white};
   constexpr static inline auto bg_bright_black_v = bright{bg{term_color::black}};
   constexpr static inline auto bg_bright_red_v = bright{bg{term_color::red}};
   constexpr static inline auto bg_bright_green_v = bright{bg{term_color::green}};
   constexpr static inline auto bg_bright_yellow_v = bright{bg{term_color::yellow}};
   constexpr static inline auto bg_bright_blue_v = bright{bg{term_color::blue}};
   constexpr static inline auto bg_bright_magenta_v = bright{bg{term_color::magenta}};
   constexpr static inline auto bg_bright_cyan_v = bright{bg{term_color::cyan}};
   constexpr static inline auto bg_bright_white_v = bright{bg{term_color::white}};


   template <auto color, uint32_t rgb_value>
   using scheme_map = mapping<color, rgba(rgb_value)>;

   using default_scheme = color_scheme <
      scheme_map<fg_black_v,          0x00000000>,
      scheme_map<fg_red_v,            0xcc000000>,
      scheme_map<fg_green_v,          0x4e9a0600>,
      scheme_map<fg_yellow_v,         0xc4a00000>,
      scheme_map<fg_blue_v,           0x729fcf00>,
      scheme_map<fg_magenta_v,        0x75507b00>,
      scheme_map<fg_cyan_v,           0x06989a00>,
      scheme_map<fg_white_v,          0xd3d7cf00>,
      scheme_map<fg_bright_black_v,   0x55575300>,
      scheme_map<fg_bright_red_v,     0xef292900>,
      scheme_map<fg_bright_green_v,   0x8ae23400>,
      scheme_map<fg_bright_yellow_v,  0xfce94f00>,
      scheme_map<fg_bright_blue_v,    0x32afff00>,
      scheme_map<fg_bright_magenta_v, 0xad7fa800>,
      scheme_map<fg_bright_cyan_v,    0x34e2e200>,
      scheme_map<fg_bright_white_v,   0xffffff00>,
      scheme_map<bg_black_v,          0x00000000>,
      scheme_map<bg_red_v,            0xcc000000>,
      scheme_map<bg_green_v,          0x4e9a0600>,
      scheme_map<bg_yellow_v,         0xc4a00000>,
      scheme_map<bg_blue_v,           0x729fcf00>,
      scheme_map<bg_magenta_v,        0x75507b00>,
      scheme_map<bg_cyan_v,           0x06989a00>,
      scheme_map<bg_white_v,          0xd3d7cf00>,
      scheme_map<bg_bright_black_v,   0x55575300>,
      scheme_map<bg_bright_red_v,     0xef292900>,
      scheme_map<bg_bright_green_v,   0x8ae23400>,
      scheme_map<bg_bright_yellow_v,  0xfce94f00>,
      scheme_map<bg_bright_blue_v,    0x32afff00>,
      scheme_map<bg_bright_magenta_v, 0xad7fa800>,
      scheme_map<bg_bright_cyan_v,    0x34e2e200>,
      scheme_map<bg_bright_white_v,   0xffffff00>
   >;
   
   template <typename Scheme=default_scheme>
   class rgb_color {
      public:
         rgb_color() = default;
         rgb_color(const rgb_color&) = default;
         rgb_color(rgb_color&&) = default;

         constexpr inline rgb_color(uint8_t r, uint8_t g, uint8_t b) : _rgba(r,g,b,0) {}
         constexpr inline rgb_color(uint32_t color) : _rgba(color) {}
         constexpr inline rgb_color(const rgba& color) : _rgba(color) {}

         template <color_type Color>
         constexpr inline rgb_color(Color c) {
            (void)c;
         }

         rgb_color& operator=(const rgb_color&) = default;
         rgb_color& operator=(rgb_color&&) = default;

         //rgb(term_bg_color c) : rgb(static_cast<term_color>(static_cast<uint8_t>(c)-10)) {}

         [[nodiscard]] constexpr inline uint8_t red() const noexcept { return _rgba.r; }
         [[nodiscard]] constexpr inline uint8_t green() const noexcept { return _rgba.g; }
         [[nodiscard]] constexpr inline uint8_t blue() const noexcept { return _rgba.b; }

      private:
         rgba _rgba;
   };
} // namespace astro::logger