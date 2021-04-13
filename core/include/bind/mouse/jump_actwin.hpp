#ifndef _JUMP_ACTWIN_HPP
#define _JUMP_ACTWIN_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct Jump2ActiveWindow : public BindedFuncCreator<Jump2ActiveWindow> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
