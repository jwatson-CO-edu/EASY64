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
        if constexpr ( is_same_v< decltype(left), decltype(right) >) {
            return left + right;
        }else if( (is_same_v<decltype(left),double>)||(is_same_v<decltype(right),double>) ){
            return static_cast<double>(left) + static_cast<double>(right);

        }else if( (is_same_v<decltype(left),long>)||(is_same_v<decltype(right),long>) ){
            return static_cast<long>(left) + static_cast<long>(right);

        }else if( (is_same_v<decltype(left),char>)||(is_same_v<decltype(right),char>) ){
            return static_cast<char>(left) + static_cast<char>(right);

        }else if( (is_same_v<decltype(left),bool>)||(is_same_v<decltype(right),bool>) ){
            return static_cast<long>(left) + static_cast<long>(right);

        // If different types, convert to double for maximum precision
        }else{
            return static_cast<double>(left) + static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val operator-( const P_Val& lhs, const P_Val& rhs ){
    // Add two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( is_same_v< decltype(left), decltype(right) >) {
            return left - right;
        }else if( (is_same_v<decltype(left),double>)||(is_same_v<decltype(right),double>) ){
            return static_cast<double>(left) - static_cast<double>(right);

        }else if( (is_same_v<decltype(left),long>)||(is_same_v<decltype(right),long>) ){
            return static_cast<long>(left) - static_cast<long>(right);

        }else if( (is_same_v<decltype(left),char>)||(is_same_v<decltype(right),char>) ){
            return static_cast<char>(left) - static_cast<char>(right);

        }else if( (is_same_v<decltype(left),bool>)||(is_same_v<decltype(right),bool>) ){
            return static_cast<long>(left) - static_cast<long>(right);

        // If different types, convert to double for maximum precision
        }else{
            return static_cast<double>(left) - static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val operator*( const P_Val& lhs, const P_Val& rhs ){
    // Add two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( is_same_v< decltype(left), decltype(right) >) {
            return left * right;
        }else if( (is_same_v<decltype(left),double>)||(is_same_v<decltype(right),double>) ){
            return static_cast<double>(left) * static_cast<double>(right);

        }else if( (is_same_v<decltype(left),long>)||(is_same_v<decltype(right),long>) ){
            return static_cast<long>(left) * static_cast<long>(right);

        }else if( (is_same_v<decltype(left),char>)||(is_same_v<decltype(right),char>) ){
            return static_cast<char>(left) * static_cast<char>(right);

        }else if( (is_same_v<decltype(left),bool>)||(is_same_v<decltype(right),bool>) ){
            return static_cast<long>(left) * static_cast<long>(right);

        // If different types, convert to double for maximum precision
        }else{
            return static_cast<double>(left) * static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val operator/( const P_Val& lhs, const P_Val& rhs ){
    // Add two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( is_same_v< decltype(left), decltype(right) >) {
            return left / right;
        }else if( (is_same_v<decltype(left),double>)||(is_same_v<decltype(right),double>) ){
            return static_cast<double>(left) / static_cast<double>(right);

        }else if( (is_same_v<decltype(left),long>)||(is_same_v<decltype(right),long>) ){
            return static_cast<long>(left) / static_cast<long>(right);

        }else if( (is_same_v<decltype(left),char>)||(is_same_v<decltype(right),char>) ){
            return static_cast<char>(left) / static_cast<char>(right);

        }else if( (is_same_v<decltype(left),bool>)||(is_same_v<decltype(right),bool>) ){
            return static_cast<long>(left) / static_cast<long>(right);

        // If different types, convert to double for maximum precision
        }else{
            return static_cast<double>(left) / static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val operator==( const P_Val& lhs, const P_Val& rhs ){
    // Add two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( is_same_v< decltype(left), decltype(right) >) {
            return left == right;
        }else if( (is_same_v<decltype(left),double>)||(is_same_v<decltype(right),double>) ){
            return static_cast<double>(left) == static_cast<double>(right);

        }else if( (is_same_v<decltype(left),long>)||(is_same_v<decltype(right),long>) ){
            return static_cast<long>(left) == static_cast<long>(right);

        }else if( (is_same_v<decltype(left),char>)||(is_same_v<decltype(right),char>) ){
            return static_cast<char>(left) == static_cast<char>(right);

        }else if( (is_same_v<decltype(left),bool>)||(is_same_v<decltype(right),bool>) ){
            return static_cast<long>(left) == static_cast<long>(right);

        // If different types, convert to double for maximum precision
        }else{
            return static_cast<double>(left) == static_cast<double>(right);
        }
    }, lhs, rhs);
}


P_Val pow( const P_Val& lhs, const P_Val& rhs ){
    // Add two numeric variants
    // Using std::visit to handle all possible type combinations
    return visit([](auto&& left, auto&& right) -> P_Val {
        // If both are the same type, return that type
        if constexpr ( is_same_v< decltype(left), decltype(right) >) {
            return pow( left, right );
        }else if( (is_same_v<decltype(left),double>)||(is_same_v<decltype(right),double>) ){
            return pow( static_cast<double>(left), static_cast<double>(right) );

        }else if( (is_same_v<decltype(left),long>)||(is_same_v<decltype(right),long>) ){
            return static_cast<double>( pow( static_cast<long>(left), static_cast<long>(right) ) );

        }else if( (is_same_v<decltype(left),char>)||(is_same_v<decltype(right),char>) ){
            return static_cast<char>( pow( static_cast<char>(left), static_cast<char>(right) ) );

        }else if( (is_same_v<decltype(left),bool>)||(is_same_v<decltype(right),bool>) ){
            return static_cast<long>( pow( static_cast<long>(left), static_cast<long>(right) ) );

        // If different types, convert to double for maximum precision
        }else{
            return pow( static_cast<double>(left), static_cast<double>(right) );
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
Enum::Enum(){}
Enum::Enum( const vstr& values ){  items = values;  }
Enum Enum::copy(){  return Enum{ items };  } // Make a copy of the enum


ValRange::ValRange(){
    // Set up the range for iteration
    valMin = make_nan();
    valMax = make_nan();
    incr   = 1;
    valCur = make_nan();
    done   = true;
}

ValRange::ValRange( const P_Val& valMin_, const P_Val& valMax_ ){
    // Set up the range for iteration
    valMin = valMin_;
    valMax = valMax_;
    incr   = 1;
    valCur = valMin_;
    done   = false;
}

ValRange ValRange::copy(){  return ValRange{ valMin, valMax };  }

P_Val ValRange::yield(){
    // Yield the next value and notify if done
    valCur += incr; 
    if( valCur > valMax ){  
        done = true;
        return valMax;  
    }
    return valCur;
}



StrRange::StrRange(){}
StrRange::StrRange( const vstr& values_ ){  values = values_;  }
StrRange StrRange::copy(){  return StrRange{ values };  }



Array::Array(){}
Array::Array( size_t N ){
    values.reserve(N);
}

Array Array::copy_empty(){  return Array{ values.size() };  }

P_Val& Array::operator[]( const size_t& index ){
    return values[ index ].val;
}

size_t Array::size() const {  return values.size();  } 



Record::Record(){}

Record Record::copy(){
    Record rtnRec;
    for( const pair<string,P_Val>& item : pVars ){  rtnRec.pVars[ item.first ] = item.second;  }
    for( const pair<string,vstr>& item  : condFields ){  rtnRec.condFields[ item.first ] = item.second;  }
    return rtnRec;
}



void SetString::restrict( const vstr& allowedStrings ){  allowed = allowedStrings;  }

bool SetString::add( const string& val ){
    if( allowed.size() ){
        if( p_vec_has( allowed, val ) ){
            res = values.insert( val );
            return res.second;
        }else{  return false;  }
    }else{  
        res = values.insert( val );  
        return res.second;
    }
}



bool SetValue::add( const P_Val& val ){
    res = values.insert( val );  
    return res.second;
}