/* @file Size.hpp */

#pragma once

#include <iostream>
#include <cstdint>

namespace xo {
  /* avoiding #include "printjson/PrintJson.hpp" here */
  namespace json { class PrintJson; }

  namespace option {
    /* size (shares / contracts) with typesafety */
    class Size {
    public:
      using PrintJson = xo::json::PrintJson;
      using int32_t = std::int32_t;

    public:
      Size() = default;

      static Size invalid() { return Size(sc_invalid_rep); }
      static Size from_int(int32_t x) { return Size(x); }

      static int32_t compare(Size x, Size y) {
	return (x.to_int() - y.to_int());
      } /*compare*/

      static Size max(Size x, Size y) {
	return Size(std::max(x.rep_, y.rep_));
      } /*max*/

      /* register json-printing for Size values with *p_pjson */
      static void provide_json_printer(PrintJson * p_pjson);

      bool is_valid() const { return rep_ != sc_invalid_rep; }
      bool is_invalid() const { return rep_ == sc_invalid_rep; }
      int32_t to_int() const { return rep_; }

      /* size rounded (down) to multiple of m */
      Size round(uint32_t m) const {
	int32_t n = (this->rep_ / m);

	return from_int(n*m);
      } /*round*/

    private:
      explicit Size(int32_t rep) : rep_{rep} {}

      static constexpr int32_t sc_invalid_rep = -1;

    private:
      int32_t rep_ = sc_invalid_rep;
    }; /*Size*/

    inline bool operator==(Size x, Size y) { return Size::compare(x, y) == 0; }
    inline bool operator< (Size x, Size y) { return Size::compare(x, y) <  0; }
    inline bool operator<=(Size x, Size y) { return Size::compare(x, y) <= 0; }
    inline bool operator> (Size x, Size y) { return Size::compare(x, y) >  0; }
    inline bool operator>=(Size x, Size y) { return Size::compare(x, y) >= 0; }

    inline std::ostream &
    operator<<(std::ostream & os, Size x) {
      os << x.to_int();
      return os;
    } /*operator<<*/

  } /*namespace option*/
} /*namespace xo*/
