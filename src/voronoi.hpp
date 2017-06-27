#define BOOST_POLYGON_NO_DEPS
#define BOOST_NO_USER_CONFIG
#define BOOST_NO_COMPILER_CONFIG
#define BOOST_NO_STDLIB_CONFIG
#define BOOST_NO_PLATFORM_CONFIG
#define BOOST_HAS_STDINT_H

#include "boost/polygon/voronoi.hpp"
#include "boost/polygon/detail/voronoi_ctypes.hpp"

#include <ieee754.h>
typedef long double fpt80;

//using namespace boost::polygon;

struct my_ulp_comparison {
  enum Result {
    LESS = -1,
    EQUAL = 0,
    MORE = 1
  };

  Result operator()(fpt80 a, fpt80 b, unsigned int maxUlps) const {
    if (a == b)
      return EQUAL;
    if (a > b) {
      Result res = operator()(b, a, maxUlps);
      if (res == EQUAL) return res;
      return (res == LESS) ? MORE : LESS;
    }
    ieee854_long_double lhs, rhs;
    lhs.d = a;
    rhs.d = b;
    if (lhs.ieee.negative ^ rhs.ieee.negative)
      return lhs.ieee.negative ? LESS : MORE;
    boost::uint64_t le = lhs.ieee.exponent; le =
        (le << 32) + lhs.ieee.mantissa0;
    boost::uint64_t re = rhs.ieee.exponent; re =
        (re << 32) + rhs.ieee.mantissa0;
    if (lhs.ieee.negative) {
      if (le - 1 > re)
        return LESS;
      le = (le == re) ? 0 : 1;
      le = (le << 32) + lhs.ieee.mantissa1;
      re = rhs.ieee.mantissa1;
      return (re + maxUlps < le) ? LESS : EQUAL;
    } else {
      if (le + 1 < re)
        return LESS;
      le = lhs.ieee.mantissa0;
      re = (le == re) ? 0 : 1;
      re = (re << 32) + rhs.ieee.mantissa1;
      return (le + maxUlps < re) ? LESS : EQUAL;
    }
  }
};

struct my_fpt_converter {
  template <typename T>
  fpt80 operator()(const T& that) const {
    return static_cast<fpt80>(that);
  }

  template <std::size_t N>
  fpt80 operator()(const typename boost::polygon::detail::extended_int<N> &that) const {
    fpt80 result = 0.0;
    for (std::size_t i = 1; i <= (std::min)((std::size_t)3, that.size()); ++i) {
      if (i != 1)
        result *= static_cast<fpt80>(0x100000000ULL);
      result += that.chunks()[that.size() - i];
    }
    return (that.count() < 0) ? -result : result;
  }
};

// Voronoi diagram traits.
struct my_voronoi_diagram_traits {
  typedef fpt80 coordinate_type;
  typedef boost::polygon::voronoi_cell<coordinate_type> cell_type;
  typedef boost::polygon::voronoi_vertex<coordinate_type> vertex_type;
  typedef boost::polygon::voronoi_edge<coordinate_type> edge_type;
  typedef class {
  public:
    enum { ULPS = 128 };
    bool operator()(const vertex_type &v1, const vertex_type &v2) const {
      return (ulp_cmp(v1.x(), v2.x(), ULPS) == my_ulp_comparison::EQUAL &&
              ulp_cmp(v1.y(), v2.y(), ULPS) == my_ulp_comparison::EQUAL);
    }
  private:
    my_ulp_comparison ulp_cmp;
  } vertex_equality_predicate_type;
};

struct c_Vertex {
	double X;
	double Y;

	c_Vertex(double x = 0, double y = 0) : X(x), Y(y) {}
};

struct c_Edge {
	size_t start;
	size_t end;

	bool isPrimary;

	size_t site1;
	size_t site2;

	c_Edge(size_t start = 0, size_t end = 0, bool isPrimary = false, size_t site1 = -1, size_t site2 = -1) {
		this->start = start;
		this->end = end;
		this->isPrimary = isPrimary;
		this->site1 = site1;
		this->site2 = site2;
	}
};

struct my_voronoi_ctype_traits {
  typedef boost::int64_t int_type;
  typedef boost::polygon::detail::extended_int<3> int_x2_type;
  typedef boost::polygon::detail::extended_int<3> uint_x2_type;
  typedef boost::polygon::detail::extended_int<128> big_int_type;
  typedef fpt80 fpt_type;
  typedef fpt80 efpt_type;
  typedef my_ulp_comparison ulp_cmp_type;
  typedef my_fpt_converter to_fpt_converter_type;
  typedef my_fpt_converter to_efpt_converter_type;
};

const unsigned int GENERATED_POINTS = 100;
const boost::int64_t MAX = 0x1000000000000LL;
