#include <algorithm>
#include <array>
#include <cstdio>
#include <type_traits>
#include <vector>

#include "automata.hpp"
#include "rundemos.hpp"



template<typename LHS, typename RHS>
constexpr auto floor_modulo(LHS dividend, RHS divisor)
{
  return ((dividend % divisor) + divisor) % divisor;
}

Automata::Automata(std::size_t width_, std::size_t height_, std::array<bool, 9> born_, std::array<bool, 9> survives_)
  : width(width_), height(height_), born(born_), survives(survives_) {}

Automata::Automata(std::size_t width_, std::size_t height_, const std::vector<bool> &born_, const std::vector<bool> &survives_)
  : width(width_), height(height_)
{
  std::copy_n(std::begin(born_), std::min(born_.size(), born.size()), std::begin(born));
  std::copy_n(std::begin(survives_), std::min(survives_.size(), survives.size()), std::begin(survives));
}

[[nodiscard]] Automata Automata::next() const
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


[[nodiscard]] std::size_t Automata::index(Automata::Point p) const
{
  return floor_modulo(p.y, static_cast<index_t>(height)) * width + floor_modulo(p.x, static_cast<index_t>(width));
}


std::size_t Automata::count_neighbors(Point p) const
{
  return static_cast<std::size_t>(
    std::count_if(neighbors.begin(), neighbors.end(), [&](auto offset) { return get(p + offset); }));
}

size_t run_glider_demo_game() //size_t n, size_t w, size_t h)
{
  const size_t n = 10'000;
  const size_t w = 40;
  const size_t h = 20;
  const Automata::index_t x = 0;
  const Automata::index_t y = 18;
  const std::array<bool, 9> born{ false, false, false, true, false, false, false, false, false }; 
  const std::array<bool, 9> surv{ false, false, true, true, false, false, false, false, false };
  auto obj = Automata(w, h, born, surv);
  obj.add_glider(Automata::Point{x, y});
  for(size_t i = 0; i < n; ++i)
    obj = obj.next();


  size_t occupied_cells = 0;

  for (size_t y = 0; y < obj.height; ++y) {
    for (size_t x = 0; x < obj.width; ++x) {
      auto p = Automata::Point {
        static_cast<Automata::index_t>(x),
        static_cast<Automata::index_t>(y)
      };
      if (obj.get(p)) {
        std::putchar('X');
        ++occupied_cells;
      } else {
        std::putchar('.');
      }
    }
    std::puts("");
  }

  return occupied_cells;
}



