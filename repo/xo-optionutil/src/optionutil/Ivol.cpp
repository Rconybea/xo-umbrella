/* @file Ivol.cpp */

#include "Ivol.hpp"
#include "xo/printjson/PrintJson.hpp"
#include "xo/printjson/JsonPrinter.hpp"
#include "xo/reflect/StructReflector.hpp"
#include "xo/indentlog/print/fixed.hpp"
#include <string>

namespace xo {
    using xo::reflect::StructReflector;
    using xo::json::PrintJson;
    using xo::json::JsonPrinter;
    using xo::json::AsStringJsonPrinter;
    using xo::reflect::Reflect;
    using xo::reflect::TypeDescr;
    //using xo::xtag;
    using xo::fixed;
    using xo::tostr;

    namespace option {
        char const *
        IvolErrorUtil::ivol_error_descr(IvolError x)
        {
#    define CASE(x) case IvolError::x: return #x

            switch(x) {
                CASE(invalid);
                CASE(ok);
                CASE(below_intrinsic);
                CASE(above_infinity);
                CASE(root_not_bracketed);
                CASE(convergence_failure);
            }

            /* impossible control path for well-formed x */
            return "???";
        } /*ivol_error_descr*/

        using IvolErrorJsonPrinter = AsStringJsonPrinter<IvolError>;

        void
        IvolErrorUtil::provide_json_printer(PrintJson * pjson)
        {
            assert(pjson);

            TypeDescr td = Reflect::require<IvolError>();

            std::unique_ptr<JsonPrinter> pr(new IvolErrorJsonPrinter(pjson));

            pjson->provide_printer(td, std::move(pr));
        } /*provide_json_printer*/

        void
        Ivol::reflect_self() {
            StructReflector<Ivol> sr;

            if (sr.is_incomplete()) {
                REFLECT_MEMBER(sr, volatility);
                REFLECT_MEMBER(sr, error_code);
            }
        } /*reflect_self*/

        Ivol
        Ivol::below_intrinsic() {
            return Ivol::error(IvolError::below_intrinsic);
        }

        Ivol
        Ivol::above_infinity() {
            return Ivol::error(IvolError::above_infinity);
        }

        Ivol
        Ivol::root_not_bracketed() {
            return Ivol::error(IvolError::root_not_bracketed);
        }

        Ivol
        Ivol::convergence_failure(double x) {
            return Ivol(x, IvolError::convergence_failure);
        }

        void
        Ivol::display_unwrapped(std::ostream & os) const {
            if (this->error_code() == IvolError::ok)
                os << fixed(this->volatility(), 4);
            else
                os << "e:" << this->error_code();
        } /*display_unwrapped*/

        void
        Ivol::display(std::ostream & os) const {
            os << "<Ivol ";
            this->display_unwrapped(os);
            os << ">";
        } /*display*/

        std::string
        Ivol::display_string() const {
            return tostr(*this);
        } /*display_string*/
    } /*namespace option*/
} /*namespace xo*/

/* end Ivol.cpp */
