////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// PRIMITIVE TYPES /////////////////////////////////////////////////////////////////////////

P_Val make_nan(){  return P_Val{ nan("") };  }

bool p_nan( const P_Val& q ){
    return (holds_alternative<double>( q ) && isnan( get<double>(q) ));
}

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


void operator+=( P_Val& lhs, const P_Val& rhs ){
    // Increment a numeric variant
    P_Val nuVal = lhs + rhs;
    lhs = nuVal;
}


void print_variant( const P_Val& v ){
    // Helper function to print variant contents
    visit( [](auto&& arg){  cout << arg;  }, v );
}


ostream& operator<<( ostream& os , const P_Val& v ){
    // Helper function to stream variant contents
    visit( [&](auto&& arg){  os << arg;  }, v );
    return os;
}


////////// COMPOUND TYPES //////////////////////////////////////////////////////////////////////////
Enum::Enum(){};
Enum::Enum( const vstr& values ){  items = values;  };
Enum Enum::copy(){  return Enum{ items };  } // Make a copy of the enum
