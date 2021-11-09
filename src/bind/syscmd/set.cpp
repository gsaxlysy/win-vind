#include "set.hpp"

#include <sstream>
#include <string>

#include "bind/binded_func.hpp"
#include "bind/binded_func_creator.hpp"
#include "core/char_logger.hpp"
#include "core/entry.hpp"
#include "core/g_params.hpp"
#include "core/rc_parser.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    SyscmdSet::SyscmdSet()
    : BindedFuncCreator("system_command_set")
    {}

    void SyscmdSet::sprocess(
            const std::string& args,
            bool reload_config) {

        if(args.find("=") != std::string::npos) { // set option_name = value
            auto [key, val] = rcparser::divide_key_and_value(args, "=") ;

            if(key.empty()) {
                if(val.empty()) {
                    opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of option yet")) ;
                }
                else {
                    opt::VCmdLine::print(opt::ErrorMessage("E: Invalid syntax")) ;
                }
                return ;
            }

            key = util::A2a(key) ;

            if(val.empty()) {
                gparams::set(key, val) ;
            }
            else if(val.find_first_not_of("0123456789.") == std::string::npos) {
                gparams::set(key, std::stod(val)) ;
            }
            else {
                gparams::set(key, val) ;
            }
        }
        else { // set option_name
            auto key = rcparser::extract_single_arg(util::A2a(args)) ;

            bool flag_value = true ;
            if(key.size() > 2 && key[0] == 'n' && key[1] == 'o') {
                key.erase(0, 2) ;
                flag_value = false ;
            }

            // reference of value
            if(key.back() == '?') {
                auto name_last_pos = key.find_last_not_of(" ", key.size() - 2) ;
                key.erase(name_last_pos + 1) ;

                if(key.find(" ") != std::string::npos) {
                    opt::VCmdLine::print(opt::ErrorMessage("E: Unknown option: " + key)) ;
                    return ;
                }

                std::stringstream ss ;
                switch(gparams::get_type(key)) {
                    using namespace gparams ;

                    case ValueType::BOOL:
                        if(gparams::get_b(key)) {
                            ss << key ;
                        }
                        else {
                            ss << "no" << key ;
                        }
                        break ;

                    case ValueType::NUMBER: {
                        ss << key << "=" ;
                        auto v_d = gparams::get_d(key) ;
                        auto v_z = gparams::get_z(key) ;
                        if(v_d == v_z) {
                            ss << std::to_string(v_z) ;
                        }
                        else {
                            ss << std::to_string(v_d) ;
                        }
                        break ;
                    }

                    case ValueType::STRING:
                        ss << key << "=" << gparams::get_s(key) ;
                        break ;

                    default:
                        ss << "E: Unknown option: " << key ;
                        break ;
                }

                opt::VCmdLine::print(opt::ErrorMessage(ss.str())) ;
                return ;
            }

            if(key.find(" ") != std::string::npos) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Unknown option: " + key)) ;
                return ;
            }
            gparams::set(key, flag_value) ;
        }

        if(reload_config) {
            vind::reconstruct_all_components() ;
        }
    }

    void SyscmdSet::sprocess(NTypeLogger&) {
        return ;
    }

    void SyscmdSet::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            return ;
        }

        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        sprocess(args, true) ;
    }
}
