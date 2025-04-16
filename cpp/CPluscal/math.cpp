////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// SHUNTING YARD ALGORITHM /////////////////////////////////////////////////////////////////

// FIXME, START HERE: MOVE DECLARATIONS TO THE HPP

class ShuntingYardInterpreter { public:
    // Handle infix operations
    // Inspired by: https://claude.ai/share/1f2d2598-c133-4851-a946-63f55ab70390
    map<string,ubyte> precedence;

    ShuntingYardInterpreter(){
        // Initialize operator precedence
        precedence["+"] = 0;
        precedence["-"] = 0;
        precedence["*"] = 1;
        precedence["/"] = 1;
        precedence["^"] = 2;
    }

    // Check if token is an operator
    bool p_operator( const string& token) const {  return precedence.find(token) != precedence.end();  }

    
    bool p_number( Context& context, const string& token ){
        // Check if token is a number
        P_Val res = context.resolve_primitive_name( token );
        return (!p_nan( res ));
    }

    
    P_Val apply_operator( const std::string& op, const P_Val& a, const P_Val& b ){
        // Apply operator to values
        if( op == "+" ){  return a + b;  }
        if( op == "-" ){  return a - b;  }
        if( op == "*" ){  return a * b;  }
        if( op == "/" ){
            if( b == P_Val{ 0.0 } ){  throw std::runtime_error("Division by zero");  }
            return (a/b);
        }
        if( op == "**" ){  return pow( a, b );  }
        throw runtime_error( "Unknown operator: " + op );
    }

};