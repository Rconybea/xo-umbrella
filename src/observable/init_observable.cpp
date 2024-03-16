/* @file init_observable.cpp */

#include "init_observable.hpp"
#include "xo/reflect/init_reflect.hpp"
//#include "xo/printjson/PrintJson.hpp"

namespace xo {
    void
    InitSubsys<S_observable_tag>::init()
    {
        //PrintJson * pjson = PrintJsonSingleton::instance().get();

        //XXX::provide_json_printer(pjson);

        //XXX::reflect_self();
    } /*init*/

    InitEvidence
    InitSubsys<S_observable_tag>::require()
    {
        InitEvidence retval;

        /* subsystem dependencies for xo-observable/ */
        retval ^= InitSubsys<S_reflect_tag>::require();

        /* xo-observable's own initialization code */
        retval ^= Subsystem::provide<S_observable_tag>("observable", &init);

        return retval;
    } /*require*/

} /*namespace xo*/

/* end init_observable.cpp */
