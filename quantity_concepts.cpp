#include <BoostUnitDefinitions/Units.hpp>
#include <boost/units/quantity.hpp>
#include <concepts>
#include <iostream>
#include <type_traits>

namespace t = boost::units::t;
namespace i = boost::units::i;
using boost::units::quantity;

template <typename T, typename U>
struct have_same_dimensions {
  static const bool value = std::is_same<typename T::dimension_type,
                                         typename U::dimension_type>::value;
};

namespace c
{
template <typename U>
concept Length = have_same_dimensions<U, t::cm>::value;
template <typename U>
concept Area = have_same_dimensions<U, t::cm_p2>::value;
template <typename U>
concept Time = have_same_dimensions<U, t::s>::value;
}  // namespace c

template <typename R, typename U1, typename U2>
quantity<R> area1(quantity<U1> a_L, quantity<U2> a_W)
{
  // don't do any checks, just try to convert to the return unit
  return quantity<R>(a_L * a_W);
}

template <typename R, typename U1, typename U2>
quantity<R> area2(quantity<U1> a_L, quantity<U2> a_W)
{
  // check that everything has the correct dimensions with static asserts
  static_assert(have_same_dimensions<U1, t::cm>::value,
                "a_L must be a length.");
  static_assert(have_same_dimensions<U2, t::cm>::value,
                "a_W must be a length.");
  static_assert(have_same_dimensions<R, t::cm_p2>::value, "R must be an area.");
  return quantity<R>(a_L * a_W);
}

template <c::Area R, c::Length U1, c::Length U2>
quantity<R> area3(quantity<U1> a_L, quantity<U2> a_W)
{
  return quantity<R>(a_L * a_W);
}

int main()
{
  quantity<t::cm> L = 20 * i::cm;
  quantity<t::mm> W = 20 * i::mm;
  quantity<t::ms> t = 20 * i::ms;

  {
    auto A = area1<t::cm_p2>(L, W);
    std::cout << A << std::endl;
  }
  {
    auto A = area2<t::cm_p2>(L, W);
    std::cout << A << std::endl;
  }
  {
    auto A = area3<t::cm_p2>(L, W);
    std::cout << A << std::endl;
  }
  {
      /* auto A = area1<t::cm_p2>(L, t); */
      // This gives a GIANT message about `converstion_factor_helper`.
      // Basically, the function is called, the arguments are multiplied, and we
      // don't get an error until we try to convert the result to t::cm_p2
  } {
      /* auto A = area2<t::cm_p2>(L, t); */
      // This gives a GIANT message about `converstion_factor_helper`.
      // Basically, the function is called, the arguments are multiplied, and we
      // don't get an error until we try to convert the result to t::cm_p2
  } {
    auto A = area3<t::cm_p2>(L, t);
    // This gives a much shorter error message that says there is no
    // matching function call, and at the end is says that a concept expression
    // evaluated to 'false'
    /* auto A = area2<t::cm>(L, W); */
  }

  return 0;
}
