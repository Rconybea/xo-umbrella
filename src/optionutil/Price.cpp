/* file Price.cpp
 *
 * author: Roland Conybeare, Sep 2022
 */

#include "Price.hpp"
#include "xo/printjson/PrintJson.hpp"
#include "xo/reflect/Reflect.hpp"
#include "xo/indentlog/print/fixed.hpp"

namespace xo {
    using xo::json::PrintJson;
    using xo::json::JsonPrinter;
    using xo::reflect::Reflect;
    using xo::reflect::TaggedPtr;
    using xo::reflect::TypeDescr;
    using xo::fixed;

    namespace option {
        namespace {
            /* json printer for xo::option::Price */
            class PriceJsonPrinter : public JsonPrinter {
            public:
                PriceJsonPrinter(PrintJson * pjson) : JsonPrinter(pjson) {}

                virtual void print_json(TaggedPtr tp, std::ostream * p_os) const override {
                    Price * x = tp.recover_native<Price>();

                    if (x) {
                        *p_os << fixed(x->to_double(), 2);
                    } else {
                        report_internal_type_consistency_error(Reflect::require<Price>(),
                                                               tp.td(),
                                                               p_os);
                    }
                } /*print_json*/
            }; /*PriceJsonPrinter*/
        } /*namespace*/

        void
        Price::provide_json_printer(PrintJson * p_pjson)
        {
            assert(p_pjson);

            TypeDescr td = Reflect::require<Price>();

            std::unique_ptr<JsonPrinter> pr(new PriceJsonPrinter(p_pjson));

            p_pjson->provide_printer(td, std::move(pr));
        } /*provide_json_printer*/
    } /*namespace option*/

} /*namespace xo*/

/* end Price.cpp */
