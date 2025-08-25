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

ostream& operator<<(ostream& os, const P_Obj& v){
    if( holds_alternative<string>( v ) ){
        os << get<string>( v );
    }else if( holds_alternative<P_Val>( v ) ){
        os << get<P_Val>( v );
    }
    return os;
}


DataType type_of( const P_Obj& obj ){
    if( holds_alternative<string>( obj ) ){  return STRING;   }
    if( holds_alternative<P_Val >( obj ) ){  return LITERAL;  }
    return ERROR;
}


P_Val make_nan(){     return P_Val{ nan("") };    }
P_Val make_double(){  return P_Val{ 0.0 }; /*--*/ }
P_Val make_llong(){   return P_Val{ (llong) 0 };  }
P_Val make_char(){    return P_Val{ (char) 0 };   }
P_Val make_bool(){    return P_Val{ false }; /**/ }


bool p_nan( const P_Val& q ){
    return (holds_alternative<double>( q ) && isnan( get<double>(q) ));
}


bool p_has_substring( const string& str, const string& sub ){
    // Find the substring
    return (str.find( sub ) != string::npos);
}


P_Val str_2_primitive( const string& q ){
    // Return interpret `q` as a primitive literal and return it
    /// Handle `bool` ///
    if( q == "true"  ){  return P_Val{ true  };  }
    if( q == "false" ){  return P_Val{ false };  }
    /// Handle `double` ///
    if( p_has_substring( q, "." ) ){
        try {
            return P_Val{ stod(q) };
        }catch( const std::invalid_argument& e ){
            // NO-OP //
        }catch( const std::out_of_range& e ){
            // NO-OP //
        }
    }
    /// Handle `long` ///
    try {
        return P_Val{ stol(q) };
    }catch( const std::invalid_argument& e ){
        // NO-OP //
    } catch( const std::out_of_range& e ){
        // NO-OP //
    }
    /// Handle `char` ///
    if( q.size() == 1 ){  return P_Val{ q[0] };  }
    
    /// Handle NaN ///
    return P_Val{ nan("") };
};


P_Val operator+( const P_Val& lhs, const P_Val& rhs ){
    // Add two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( std::is_same_v< decltype(left), decltype(right) >) {
            return left + right;
        }
        // FIXME: WANT A MORE GRANULAR TYPE ESCALATION HERE
        // If different types, convert to double for maximum precision
        else {
            return static_cast<double>(left) + static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val operator+=( P_Val& lhs, const P_Val& rhs ){
    // Add two numeric variants and store in `lhs`
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( std::is_same_v< decltype(left), decltype(right) >) {
            left = left + right;
            return left;
        }
        // FIXME: WANT A MORE GRANULAR TYPE ESCALATION HERE
        // If different types, convert to double for maximum precision
        else {
            left = static_cast<double>(left) + static_cast<double>(right);
            return left;
        }
    }, lhs, rhs);
}


P_Val operator-( const P_Val& lhs, const P_Val& rhs ){
    // Subtract two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( std::is_same_v< decltype(left), decltype(right) >) {
            return left - right;
        }
        // FIXME: WANT A MORE GRANULAR TYPE ESCALATION HERE
        // If different types, convert to double for maximum precision
        else {
            return static_cast<double>(left) - static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val operator*( const P_Val& lhs, const P_Val& rhs ){
    // Multiply two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( std::is_same_v< decltype(left), decltype(right) >) {
            return left * right;
        }
        // FIXME: WANT A MORE GRANULAR TYPE ESCALATION HERE
        // If different types, convert to double for maximum precision
        else {
            return static_cast<double>(left) * static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val operator/( const P_Val& lhs, const P_Val& rhs ){
    // Divide two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( std::is_same_v< decltype(left), decltype(right) >) {
            return left / right;
        }
        // FIXME: WANT A MORE GRANULAR TYPE ESCALATION HERE
        // If different types, convert to double for maximum precision
        else {
            return static_cast<double>(left) / static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val pow( const P_Val& lhs, const P_Val& rhs ){
    // Raise `lhs` to the `rhs` power
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        return pow( static_cast<double>(left), static_cast<double>(right) );
    }, lhs, rhs);
}


double as_double( const P_Val& val ){
    // Raise `lhs` to the `rhs` power
    // Using std::visit to handle all possible type combinations
    double rtnDbbl;
    visit( [&](auto&& rtnVal) -> void {  rtnDbbl = static_cast<double>( rtnVal );  }, val );
    return rtnDbbl;
}