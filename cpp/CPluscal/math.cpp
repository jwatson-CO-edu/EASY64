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

    
    vstr infix_2_RPN( Context& context, const vstr& infix ){
        // Convert infix expression to RPN using Shunting Yard algorithm
        vstr /*----*/ output;
        stack<string> operatorStack;
        
        for (const string& token : infix) {

            // If token is a number, add to output
            if( p_number( context, token ) ){
                output.push_back(token);

            // If token is an operator
            }else if( p_operator( token ) ){
                // Pop operators with higher precedence from the stack to output
                while( (!operatorStack.empty()) && 
                       p_operator( operatorStack.top() ) && 
                       (precedence[ operatorStack.top() ] >= precedence[token]) ) {
                    output.push_back( operatorStack.top() );
                    operatorStack.pop();
                }
                // Push current operator to stack
                operatorStack.push( token );
            }
            // If token is left parenthesis, push to stack
            else if( token == "(" ){
                operatorStack.push( token );

            // If token is right parenthesis, pop until matching left parenthesis
            }else if( token == ")" ){

                while( !operatorStack.empty() && operatorStack.top() != "(" ){
                    output.push_back( operatorStack.top() );
                    operatorStack.pop();
                }
                
                if( operatorStack.empty() || operatorStack.top() != "(" ){
                    throw runtime_error( "Mismatched parentheses" );
                }
                
                // Pop the left parenthesis
                operatorStack.pop();
            }else{  
                throw runtime_error( "Invalid token: " + token );
            }
        }
        
        // Pop remaining operators from stack to output
        while( !operatorStack.empty() ){
            if( operatorStack.top() == "(" ){  throw std::runtime_error("Mismatched parentheses");  }
            output.push_back( operatorStack.top() );
            operatorStack.pop();
        }
        
        return output;
    }

    
    P_Val eval_RPN( Context& context, const vstr& rpn ){
        // Evaluate RPN expression
        stack<P_Val> values;
        P_Val /*--*/ a, b;
        
        for( const string& token : rpn){
            if( p_number( context, token ) ){
                values.push( context.resolve_primitive_name( token ) );
            }else if( p_operator( token ) ){
                if( values.size() < 2 ){  throw runtime_error("Invalid expression");  }
                
                b = values.top(); values.pop();
                a = values.top(); values.pop();
                
                values.push( apply_operator(token, a, b) );
            }else{  throw runtime_error( "Invalid token in RPN: " + token );  }
        }
        
        if( values.size() != 1 ){  throw runtime_error( "Invalid expression" );  }
        
        return values.top();
    }


    P_Val interpret( Context& context, const vstr& expression ){
        // Convert to RPN
        vstr rpn = infix_2_RPN( context, expression );
        // Evaluate RPN
        return eval_RPN( context, rpn );
    }


    void print_RPN( Context& context, const vstr& expression ){
        // Helper method to print RPN (for debugging)
        vstr rpn = infix_2_RPN( context, expression );
        cout << "RPN: ";
        for( const string& token : rpn ){  cout << token << " ";  }
        std::cout << std::endl;
    }

};