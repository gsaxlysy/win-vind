#ifndef _HINTS_HPP
#define _HINTS_HPP

#include <string>
#include <vector>

#include "core/keycode.hpp"
#include "core/keycodedef.hpp"

namespace vind
{
    namespace util
    {
        using Hint = std::vector<core::KeyCode> ;

        void assign_identifier_hints(
            std::size_t target_count,
            std::vector<Hint>& hints) ;

        void convert_hints_to_strings(
            const std::vector<Hint>& hints,
            std::vector<std::string>& hint_strings) ;
    }
}

#endif
