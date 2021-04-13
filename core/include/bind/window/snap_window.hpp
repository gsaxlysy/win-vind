#ifndef _SNAP_WINDOW_HPP
#define _SNAP_WINDOW_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct SnapCurrentWindow2Left : public BindedFuncCreator<SnapCurrentWindow2Left> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct SnapCurrentWindow2Right : public BindedFuncCreator<SnapCurrentWindow2Right> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct SnapCurrentWindow2Top : public BindedFuncCreator<SnapCurrentWindow2Top> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct SnapCurrentWindow2Bottom : public BindedFuncCreator<SnapCurrentWindow2Bottom> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
