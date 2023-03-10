#include <algorithm>
#include <array>
#include <cstdio>
#include <type_traits>
#include <vector>
#include "fmt/format.h"
#include <chrono>
#include <cassert> // assert()

#ifdef USER_PARAMS
#include <cstdlib> // atol()
#endif

#ifdef NO_CONSTEXPR
#define CONSTEXPR
#else
#define CONSTEXPR constexpr
#endif

// necessary to keep the same algorithm as Python
CONSTEXPR auto floor_modulo(auto dividend, auto divisor)
{
  return ((dividend % divisor) + divisor) % divisor;
}

// if we wanted to push this further, we would make the width and height compile-time constants
// that is for a future episode.
struct Automata
{
  using index_t = std::make_signed_t<std::size_t>;

  std::size_t width;
  std::size_t height;
  std::array<bool, 9> born;
  std::array<bool, 9> survives;

  std::vector<bool> data = std::vector<bool>(width * height);

  CONSTEXPR Automata(std::size_t width_, std::size_t height_, std::array<bool, 9> born_, std::array<bool, 9> survives_)
    : width(width_), height(height_), born(born_), survives(survives_) {}

  struct Point
  {
    index_t x;
    index_t y;

    [[nodiscard]] CONSTEXPR Point operator+(Point rhs) const
    {
      return Point{ x + rhs.x, y + rhs.y };
    }
  };

  [[nodiscard]] CONSTEXPR std::size_t index(Point p) const
  {
    return floor_modulo(p.y, static_cast<index_t>(height)) * width + floor_modulo(p.x, static_cast<index_t>(width));
  }

  [[nodiscard]] CONSTEXPR bool get(Point p) const { return data[index(p)]; }

  CONSTEXPR void set(Point p) { data[index(p)] = true; }

#ifdef NO_CONSTEXPR
  static std::array<Point, 8> neighbors;
#else
  constexpr static std::array<Point, 8> neighbors{
    Point{ -1, -1 },
    Point{ 0, -1 },
    Point{ 1, -1 },
    Point{ -1, 0 },
    Point{ 1, 0 },
    Point{ -1, 1 },
    Point{ 0, 1 },
    Point{ 1, 1 }
  };
#endif

  CONSTEXPR std::size_t count_neighbors(Point p) const
  {
    return static_cast<std::size_t>(std::ranges::count_if(
      neighbors, [&](auto offset) { return get(p + offset); }));
  }

  [[nodiscard]] CONSTEXPR Automata next() const
  {
    Automata result{ width, height, born, survives };

    for (std::size_t y = 0; y < height; ++y) {
      for (std::size_t x = 0; x < width; ++x) {
        Point p{ static_cast<index_t>(x), static_cast<index_t>(y) };
        const auto neighbors = count_neighbors(p);
        if (get(p)) {
          if (survives[neighbors]) {
            result.set(p);
          }
        } else {
          if (born[neighbors]) {
            result.set(p);
          }
        }
      }
    }

    return result;
  }
  CONSTEXPR void add_glider(Point p)
  {
    set(p);
    set(p + Point{1, 1});
    set(p + Point{2, 1});
    set(p + Point{0, 2});
    set(p + Point{1, 2});
  }
};

#ifdef NO_CONSTEXPR
std::array<Automata::Point, 8> Automata::neighbors {
    Automata::Point{ -1, -1 },
    Automata::Point{ 0, -1 },
    Automata::Point{ 1, -1 },
    Automata::Point{ -1, 0 },
    Automata::Point{ 1, 0 },
    Automata::Point{ -1, 1 },
    Automata::Point{ 0, 1 },
    Automata::Point{ 1, 1 }
  };
#endif

// convert time to floating point milliseconds. (E.g. higher resolution (microseconds, nanoseconds, etc.) to ms.) 
inline std::chrono::duration<float, std::milli> to_ms_float(auto t) { 
  //return std::chrono::duration_cast<std::chrono::duration<float, std::milli>(t); 
  return std::chrono::duration<float, std::milli>(t);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
#ifdef USER_PARAMS
  assert(argc > 4);
  long warg = atol(argv[1]);
  long harg = atol(argv[2]);
  long xarg = atol(argv[3]);
  long yarg = atol(argv[4]);
  assert(warg > 0);
  assert(harg > 0);
  assert(xarg >= 0);
  assert(yarg >= 0);
  size_t w = (size_t)warg;
  size_t h = (size_t)harg;
  Automata::index_t x = (Automata::index_t)xarg;
  Automata::index_t y = (Automata::index_t)yarg;
  fmt::print("Using parameters from command line: game size ({} x {}), glider start position ({}, {}).\n", w, h, x, y);
#else
  const size_t w = 40;
  const size_t h = 20;
  const Automata::index_t x = 0;
  const Automata::index_t y = 18;  
  fmt::print("Note, compiled to use constant parameters: game size ({} x {}), glider start position ({}, {}).\n", w, h, x, y);
#endif

  using clock = std::chrono::steady_clock;
  auto start = clock::now();

  auto obj = Automata(w, h, { false, false, false, true, false, false, false, false, false }, { false, false, true, true, false, false, false, false, false });

  obj.add_glider(Automata::Point{x, y});

  auto setup_time_elapsed = clock::now() - start;
  auto start_simulation = clock::now();

  for (int i = 0; i < 10'000; ++i) {
    obj = obj.next();
  }

  auto simulation_time_elapsed = clock::now() - start_simulation;
  auto start_output = clock::now();

  for (size_t y = 0; y < obj.height; ++y) {
    for (size_t x = 0; x < obj.width; ++x) {
      auto p = Automata::Point {
        static_cast<Automata::index_t>(x),
        static_cast<Automata::index_t>(y)
      };
      if (obj.get(p)) {
        std::putchar('X');
      } else {
        std::putchar('.');
      }
    }
    std::puts("");
  }

  auto output_time_elapsed = clock::now() - start_output;
  auto total_time_elapsed = clock::now() - start;

  fmt::print("\n");
  fmt::print("setup      {:>10.2f} ms\n", to_ms_float(setup_time_elapsed).count());
  fmt::print("simulation {:>10.2f} ms\n", to_ms_float(simulation_time_elapsed).count());
  fmt::print("output     {:>10.2f} ms\n", to_ms_float(output_time_elapsed).count());
  fmt::print("total      {:>10.2f} ms\n", to_ms_float(total_time_elapsed).count());

  return 0;
}

