/* file Size.cpp
 *
 * author: Roland Conybeare, Sep 2022
 */

#include "Size.hpp"
#include "xo/printjson/PrintJson.hpp"
#include "xo/reflect/Reflect.hpp"

namespace xo {
  using xo::json::PrintJson;
  using xo::json::JsonPrinter;
  using xo::reflect::Reflect;
  using xo::reflect::TaggedPtr;
  using xo::reflect::TypeDescr;

  namespace option {
    namespace {
      /* json printer for xo::option::Size */
      class SizeJsonPrinter : public JsonPrinter {
      public:
    SizeJsonPrinter(PrintJson * pjson) : JsonPrinter(pjson) {}

    virtual void print_json(TaggedPtr tp, std::ostream * p_os) const {
      Size * x = tp.recover_native<Size>();

      if (x) {
        *p_os << x->to_int();
      } else {
        report_internal_type_consistency_error(Reflect::require<Size>(),
                           tp.td(),
                           p_os);
      }
    } /*print_json*/
      }; /*SizeJsonPrinter*/
    } /*namespace*/

    void
    Size::provide_json_printer(PrintJson * p_pjson)
    {
      assert(p_pjson);

      TypeDescr td = Reflect::require<Size>();

      std::unique_ptr<JsonPrinter> pr(new SizeJsonPrinter(p_pjson));

      p_pjson->provide_printer(td, std::move(pr));
    } /*provide_json_printer*/
  } /*namespace option*/
} /*namespace xo*/

/* end Size.cpp */
