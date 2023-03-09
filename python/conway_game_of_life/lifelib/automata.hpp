#include <algorithm>
#include <array>
#include <cstdio>
#include <type_traits>
#include <vector>

struct __attribute__ ((visibility ("hidden")))
Automata
{
  using index_t = std::make_signed_t<std::size_t>;

  std::size_t width;
  std::size_t height;
  std::array<bool, 9> born;
  std::array<bool, 9> survives;

  std::vector<bool> data = std::vector<bool>(width * height);

  Automata(std::size_t width_, std::size_t height_, std::array<bool, 9> born_, std::array<bool, 9> survives_);

  Automata(std::size_t width_, std::size_t height_, const std::vector<bool> &born_, const std::vector<bool> &survives_);


  struct Point
  {
    index_t x;
    index_t y;

    [[nodiscard]] constexpr Point operator+(Point rhs) const
    {
      return Point{ x + rhs.x, y + rhs.y };
    }
  };

  [[nodiscard]] std::size_t index(Point p) const;

  [[nodiscard]] bool get(Point p) const { return data[index(p)]; }

  void set(Point p) { data[index(p)] = true; }

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

  std::size_t count_neighbors(Point p) const;

  [[nodiscard]] Automata next() const;

  void add_glider(Point p)
  {
    set(p);
    set(p + Point{ 1, 1 });
    set(p + Point{ 2, 1 });
    set(p + Point{ 0, 2 });
    set(p + Point{ 1, 2 });
  }
};

