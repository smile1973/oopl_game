#ifndef APP_UTIL_HPP
#define APP_UTIL_HPP

#include "Character.hpp"

[[nodiscard]]inline bool OnWard(const Character& character) {
    auto position = character.GetPosition();
    return position.y > -90 && position.y < 93 && position.x > 50 && position.x < 233;
}

#endif //APP_UTIL_HPP
