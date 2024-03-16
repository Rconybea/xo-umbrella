/* @file Price.hpp */

#pragma once

#include "xo/indentlog/print/fixed.hpp"
#include <cstdint>

namespace xo {
  /* avoiding #include "printjson/PrintJson.hpp" here */
  namespace json { class PrintJson; }

  namespace option {
    /* price with exact representation
     * note that prices can be negative (for complex products)
     */
    class Price {
    public:
      using PrintJson = xo::json::PrintJson;

    public:
      Price() = default;

      static constexpr Price from_double(double px) { return Price(px * sc_inv_unit); }
      static Price from_rep(int32_t rep) { return Price(rep); }
      static Price zero() { return Price(0); }

      static int32_t compare(Price x, Price y) { return x.rep() - y.rep(); }

      /* register json-printing for Price values with *p_pjson */
      static void provide_json_printer(PrintJson * p_pjson);

      int32_t rep() const { return rep_; }
      double to_double() const { return sc_unit * rep_; }

    private:
      explicit constexpr Price(int32_t rep) : rep_{rep} {}

      static constexpr double sc_unit = 0.0001;
      static constexpr double sc_inv_unit = (1.0 / sc_unit);

    private:
      /* r = .rep represents a price
       *    c_unit * r
       *
       * e.g.
       *   rep=10000 -> price = $1
       *   rep= 1000 -> price = $0.10
       *   rep=  100 -> price = $0.01
       */
      int32_t rep_ = 0;
    }; /*Price*/

    inline bool operator==(Price x, Price y) { return Price::compare(x, y) == 0; }
    inline bool operator!=(Price x, Price y) { return Price::compare(x, y) != 0; }
    inline bool operator< (Price x, Price y) { return Price::compare(x, y) <  0; }
    inline bool operator<=(Price x, Price y) { return Price::compare(x, y) <= 0; }
    inline bool operator> (Price x, Price y) { return Price::compare(x, y) >  0; }
    inline bool operator>=(Price x, Price y) { return Price::compare(x, y) >= 0; }

    inline Price operator+(Price x, Price y) { return Price::from_rep(x.rep() + y.rep()); }
    inline Price operator-(Price x, Price y) { return Price::from_rep(x.rep() - y.rep()); }

    inline Price operator*(double x, Price y) { return Price::from_rep(x * y.rep()); }
    inline Price operator*(Price x, double y) { return Price::from_rep(x.rep() * y); }

    inline std::ostream &
    operator<<(std::ostream & os,
           Price const & px)
    {
      os << fixed(px.to_double(), 2);
      return os;
    } /*operator<<*/

  } /*namespace option*/
} /*namespace xo*/

/* end Price.hpp */
