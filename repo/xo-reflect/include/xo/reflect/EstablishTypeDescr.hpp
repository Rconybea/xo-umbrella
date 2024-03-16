/* file EstablishTypeDescr.hpp
 *
 * author: Roland Conybeare, Aug 2022
 */

#pragma once

#include "TypeDescr.hpp"
#include "TaggedPtr.hpp"

namespace xo {
  namespace reflect {
    class EstablishTypeDescr {
    public:
      /* implementation method; expect this to be used only within reflect/ library.
       * avoids some otherwise-cyclic #include paths
       * between specialized headers such as vector/VectorTdx.hpp and this
       * EstablishTypeDescr.hpp
       */
#ifdef OBSOLETE
      template<typename T>
      static TaggedPtr establish_tp(T * x) { return TaggedPtr(establish<T>(), x); }
#endif
      template<typename T>
      static TaggedPtr establish_most_derived_tp(T * x) { return establish<T>()->most_derived_self_tp(x); }

      template<typename T>
      static TypeDescrW establish() {
    TypeDescrW td = TypeDescrBase::require(&typeid(T),
                           type_name<T>(),
                           nullptr);

#ifdef NOT_USING
    std::function<TaggedPtr (void *)> to_self_tp;

    if (std::is_base_of_v<SelfTagging, T>) {
      /* T is a descendant of SelfTagging (or T = SelfTagging);
       * use SelfTagging.self_tp()
       */
      to_self_tp = [](void * x) { return reinterpret_cast<T *>(x)->self_tp(); };
    } else {
      /* T is not a descendant of SelfTagging.
       * want to return
       */
      to_self_tp = [td](void * x) { return TaggedPtr(td, x); };
    }

    td->assign_to_self_tp(to_self_tp);
#endif
    return td;
      }
    }; /*EstablishTypeDescr*/

    template<typename T>
    inline TaggedPtr establish_most_derived_tp(T * x) {
      return EstablishTypeDescr::establish_most_derived_tp<T>(x);
    }
  } /*namespace reflect*/
} /*namespace xo*/


/* end EstablishTypeDescr.hpp */
