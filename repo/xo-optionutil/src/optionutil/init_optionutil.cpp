/* file init_optionutil.cpp
 *
 * author: Roland Conybeare, Aug 2022
 */

#include "init_option_util.hpp"
#include "xo/printjson/init_printjson.hpp"
#include "xo/printjson/PrintJson.hpp"
#include "Size2.hpp"
#include "Px2.hpp"
#include "PxSizeTime2.hpp"
#include "PxSizeTime.hpp"
#include "Ivol2.hpp"
#include "xo/subsys/Subsystem.hpp"

namespace xo {
    using xo::option::Size2;
    using xo::option::Px2;
    using xo::option::PxSize2;
    using xo::option::PxSizeTime2;
    using xo::option::Price;
    using xo::option::Size;
    using xo::option::PxSize;
    using xo::option::PxSizeTime;
    using xo::option::Ivol;
    using xo::option::Ivol2;
    using xo::option::IvolErrorUtil;
    using xo::json::PrintJsonSingleton;
    using xo::json::PrintJson;

    void
    InitSubsys<S_option_util_tag>::init()
    {
        PrintJson * pjson = PrintJsonSingleton::instance().get();

        Size::provide_json_printer(pjson);
        Price::provide_json_printer(pjson);
        IvolErrorUtil::provide_json_printer(pjson);

        Size2::reflect_self();
        Px2::reflect_self();
        PxSize2::reflect_self();
        PxSizeTime2::reflect_self();
        PxSize::reflect_self();
        PxSizeTime::reflect_self();
        Ivol::reflect_self();
        Ivol2::reflect_self();
    } /*init*/

    InitEvidence
    InitSubsys<S_option_util_tag>::require()
    {
        InitEvidence retval;

        /* direct subsystem dependencies for option_util/ */
        retval ^= InitSubsys<S_printjson_tag>::require();

        /* option_util/'s own initialization code */
        retval ^= Subsystem::provide<S_option_util_tag>("option_util", &init);

        return retval;
    } /*require*/
} /*namespace xo*/

/* end init_optionutil.cpp */
