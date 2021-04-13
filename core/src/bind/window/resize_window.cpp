#include "bind/window/resize_window.hpp"

#include "bind/base/char_logger.hpp"
#include "bind/base/key_logger_base.hpp"
#include "bind/base/ntype_logger.hpp"
#include "coreio/i_params.hpp"
#include "io/screen_metrics.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"
#include "bind/window/window_utility.hpp"

namespace
{
    using namespace vind ;

    //
    // Resize the width of a foreground window in a foreground monitor.
    // [1] Desired width
    // [2] Foreground Window Structure
    //
    inline void resize_in_monitor_width(
            LONG desired_width,
            const windowutil::ForegroundInfo& fginfo) {

        screenmetrics::MonitorInfo minfo ;
        screenmetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

        const auto window_height = screenmetrics::height(fginfo.rect) ;
        const auto monitor_width = screenmetrics::width(minfo.work_rect) ;

        if(desired_width >= monitor_width) {
            //a maximum width is a width of a foreground monitor.
            windowutil::resize(
                    fginfo.hwnd,
                    minfo.work_rect.left, fginfo.rect.top,
                    monitor_width, window_height) ;
        }
        else {
            //If cannot move a lower-right point anymore, it move a upper-left point.
            auto right_max_width = minfo.work_rect.right - fginfo.rect.left ;

            if(desired_width <= right_max_width) {
                windowutil::resize(
                        fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
                        desired_width, window_height) ;
            }
            else {
                auto left_shift_delta = desired_width - right_max_width ;
                windowutil::resize(
                        fginfo.hwnd, fginfo.rect.left - left_shift_delta, fginfo.rect.top,
                        desired_width, window_height) ;
            }
        }
    }

    // Resize the height of a foreground window in a foreground monitor.
    // [1] Desired height
    // [2] Foreground Window Structure
    //
    inline void resize_in_monitor_height(
            LONG desired_height,
            const windowutil::ForegroundInfo& fginfo) {

        screenmetrics::MonitorInfo minfo ;
        screenmetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

        const auto window_width = screenmetrics::width(fginfo.rect) ;
        const auto monitor_height = screenmetrics::height(minfo.work_rect) ;

        if(desired_height >= monitor_height) {
            //a maximum height is a height of a foreground monitor.
            windowutil::resize(
                    fginfo.hwnd,
                    fginfo.rect.left, minfo.work_rect.top,
                    window_width, monitor_height) ;
        }
        else {
            //If cannot move a lower-right point anymore, it move a upper-left point.
            auto lower_max_height = minfo.work_rect.bottom - fginfo.rect.top ;

            if(desired_height <= lower_max_height) {
                windowutil::resize(
                        fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
                        window_width, desired_height) ;
            }
            else {
                auto left_shift_delta = desired_height - lower_max_height ;
                windowutil::resize(
                        fginfo.hwnd, fginfo.rect.left, fginfo.rect.top - left_shift_delta,
                        window_width, desired_height) ;
            }
        }
    }
}

namespace vind
{
    //ResizeWindowWidth
    const std::string ResizeWindowWidth::sname() noexcept {
        return "resize_window_width" ;
    }
    void ResizeWindowWidth::sprocess(long width) {
        if(width <= 0) return ;
        windowutil::ForegroundInfo fginfo ;
        resize_in_monitor_width(width, fginfo) ;
    }
    void ResizeWindowWidth::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ResizeWindowWidth::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) return ;
        sprocess(keyloggerutil::extract_num(str)) ;
    }


    //IncreaseWindowWidth
    const std::string IncreaseWindowWidth::sname() noexcept {
        return "increase_window_width" ;
    }
    void IncreaseWindowWidth::sprocess(long delta) {
        if(delta <= 0) return ;

        windowutil::ForegroundInfo fginfo ;
        auto width = screenmetrics::width(fginfo.rect) + delta ;
        resize_in_monitor_width(width, fginfo) ;
    }
    void IncreaseWindowWidth::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(iparams::get_l("window_width_delta") * parent_lgr.get_head_num()) ;
        }
    }
    void IncreaseWindowWidth::sprocess(const CharLogger& parent_lgr) {
        sprocess(keyloggerutil::extract_num(parent_lgr.to_str())) ;
    }


    //DecreaseWindowWidth
    const std::string DecreaseWindowWidth::sname() noexcept {
        return "decrease_window_width" ;
    }
    void DecreaseWindowWidth::sprocess(long delta)  {
        if(delta <= 0) return ;
        windowutil::ForegroundInfo fginfo ;

        auto width = screenmetrics::width(fginfo.rect) - delta ;
        if(width <= 0) { 
            VirtualCmdLine::msgout("e: Width below zero") ;
            return ;
        }

        windowutil::resize(
                fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
                width, screenmetrics::height(fginfo.rect)) ;
    }
    void DecreaseWindowWidth::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(iparams::get_l("window_width_delta") * parent_lgr.get_head_num()) ;
        }
    }
    void DecreaseWindowWidth::sprocess(const CharLogger& parent_lgr) {
        sprocess(keyloggerutil::extract_num(parent_lgr.to_str())) ;
    }


    //ResizeWindowHeight
    const std::string ResizeWindowHeight::sname() noexcept {
        return "resize_window_height" ;
    }
    void ResizeWindowHeight::sprocess(long height) {
        if(height == 0) return ;
        windowutil::ForegroundInfo fginfo ;
        resize_in_monitor_height(height, fginfo) ;
    }
    void ResizeWindowHeight::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ResizeWindowHeight::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) return ;
        sprocess(keyloggerutil::extract_num(str)) ;
    }


    //IncreaseWindowHeight
    const std::string IncreaseWindowHeight::sname() noexcept {
        return "increase_window_height" ;
    }
    void IncreaseWindowHeight::sprocess(long delta) {
        if(delta <= 0) return ;
        windowutil::ForegroundInfo fginfo ;

        auto height = screenmetrics::height(fginfo.rect) + delta ;
        resize_in_monitor_height(height, fginfo) ;
    }
    void IncreaseWindowHeight::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(iparams::get_l("window_height_delta") * parent_lgr.get_head_num()) ;
        }
    }
    void IncreaseWindowHeight::sprocess(const CharLogger& parent_lgr) {
        sprocess(keyloggerutil::extract_num(parent_lgr.to_str())) ;
    }


    //DecreaseWindowHeight
    const std::string DecreaseWindowHeight::sname() noexcept {
        return "decrease_window_height" ;
    }
    void DecreaseWindowHeight::sprocess(long delta) {
        if(delta <= 0) return ;

        windowutil::ForegroundInfo fginfo ;

        auto height = screenmetrics::height(fginfo.rect) - delta ;
        if(height <= 0) { 
            VirtualCmdLine::msgout("e: Height below zero") ;
            return ;
        }

        windowutil::resize(
                fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
                screenmetrics::width(fginfo.rect), height) ;
    }
    void DecreaseWindowHeight::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(iparams::get_l("window_height_delta") * parent_lgr.get_head_num()) ;
        }
    }
    void DecreaseWindowHeight::sprocess(const CharLogger& parent_lgr) {
        sprocess(keyloggerutil::extract_num(parent_lgr.to_str())) ;
    }
}
