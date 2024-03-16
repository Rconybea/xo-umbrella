/* @file ImpliedVol.hpp */

#pragma once

#include <iostream>
#include <limits>

namespace xo {
  namespace json { class PrintJson; }

  namespace option {
    enum class IvolError {
      /* sentinel for otherwise uninitialized error code */
      invalid /*sentinel enum*/,
      /* success -- not an error */
      ok,
      /* price below option intrinsic value
       * (tv at zero volatility higher than target price)
       */
      below_intrinsic,
      /* tv at infinite volatility lower than target price */
      above_infinity,
      /* tv not bracketed by initial volatility interval */
      root_not_bracketed,
      /* solver failed to converge within budget */
      convergence_failure
    }; /*IvolError*/

    class IvolErrorUtil {
    public:
      using PrintJson = xo::json::PrintJson;
      
    public:
      /* e.g. ivol_error_descr(IvolError::below_intrinsic) = "below_intrinsic" */
      static char const * ivol_error_descr(IvolError x);

      /* install json printer for IvolError */
      static void provide_json_printer(PrintJson * pjson);
    }; /*IvolErrorUtil*/

    inline std::ostream &
    operator<<(std::ostream & os, IvolError x) {
      os << IvolErrorUtil::ivol_error_descr(x);
      return os;
    } /*operator<<*/

    /* encapsulate implied volatility result
     * using class to provision for error codes
     */
    class Ivol {
    public:
      Ivol() = default;
      explicit Ivol(double x, IvolError err) : volatility_(x), error_code_(err) {}

      static Ivol success(double x) { return Ivol(x, IvolError::ok); }
      static Ivol error(IvolError err) { return Ivol(std::numeric_limits<double>::quiet_NaN(), err); }

      static Ivol below_intrinsic();
      static Ivol above_infinity();
      static Ivol root_not_bracketed();
      static Ivol convergence_failure(double x);
      static Ivol invalid();

      static void reflect_self();

      double volatility() const { return volatility_; }
      IvolError error_code() const { return error_code_; }

      bool is_ok() const { return error_code_ == IvolError::ok; }

      void display_unwrapped(std::ostream & os) const;
      void display(std::ostream & os) const;
      std::string display_string() const;

    private:
      /* volatility in annual units.
       * e.g. 0.5 means variance of 0.25 / year
       */
      double volatility_ = 0.0;
      /* ok|root_not_bracketed|convergence_failure */
      IvolError error_code_ = IvolError::invalid;
    }; /*Ivol*/

    inline std::ostream &
    operator<<(std::ostream & os, Ivol const & x) {
      x.display(os);
      return os;
    } /*operator<<*/
  } /*namespace option*/
} /*namespace xo*/

/* end ImpliedVol.hpp */
  
