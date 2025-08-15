////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"


ostream& operator<<(ostream& os, const P_Val& v) {
    std::visit([&os](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, char>) {
            os << (int)value;  // Print char as integer
        } else {
            os << value;
        }
    }, v);
    return os;
}