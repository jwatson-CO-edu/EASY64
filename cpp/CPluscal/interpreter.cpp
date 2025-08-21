////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"


////////// CONTEXT /////////////////////////////////////////////////////////////////////////////////

bool Context::p_name_defined( const string& ident ){
    // Return whether the name exists in this context.
    if( constants.count( ident ) ){  return true;  }
    if( variables.count( ident ) ){  return true;  }
    if( parent ){  return parent->p_name_defined( ident );  }
    return false;
}


P_Val Context::get_value_by_name( const string& ident ){
    // Resolve the name to a value, otherwise return "NaN"
    if( constants.count( ident ) ){  return constants[ ident ];  }
    if( variables.count( ident ) ){  return variables[ ident ];  }
    if( parent ){  return parent->get_value_by_name( ident );  }
    return make_nan();
}


bool Context::set_value_by_name( const string& ident, const P_Val& val ){
    // Attempt to set an identifier's value and return whether the change was made
    if( constants.count( ident ) ){  return false;  }
    if( variables.count( ident ) ){  variables[ ident ] = val;  return true;  }
    if( parent ){  return parent->set_value_by_name( ident, val );  }
    return false;
}


void Context::print_constant_state(){
    // Print all the constants in this context
    cout << "///// Program Constants /////" << endl;
    for( const pair<string,P_Val>& elem : constants ){
        cout << "{" << elem.first << "}: " << elem.second << endl;
    }
}


void Context::print_variable_state(){
    // Print all the variables in this context
    cout << "///// Program Variables /////" << endl;
    for( const pair<string,P_Val>& elem : variables ){
        cout << "{" << elem.first << "}: " << elem.second << endl;
    }
}



////////// BUILT-IN FUNCTIONS //////////////////////////////////////////////////////////////////////

map<string,BuiltInFunction> Built_In_Functions;

bool p_string_token( const string& q ){
    // Does this token represent a string?
    if( q.front() != '\'' ){  return false;  }
    if( q.back()  != '\'' ){  return false;  }
    return true;
}

void writeln( const vstr& args, CntxPtr cntx ){
    // Basic print followed by a newline
    for( const string& arg : args ){
        if( p_string_token( arg ) ){  cout << arg.substr( 1, arg.size()-2 ) << " ";  }
    }
    cout << endl;
}

void load_builtins(){
    // Load all Built-In Functions
    Built_In_Functions[ "writeln" ] = writeln;
}



////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

CPC_Interpreter::CPC_Interpreter(){  
    // Default Constructor
    context = CntxPtr{ new Context{} };  
    load_builtins();
} 


ubyte PEMDAS( const string& q ){
    // Express the priority of the math operator as a number
    vvstr pemdas = {
        {"(",")"}, // Parens
        {"**"}, // -- Exponent
        {"*","/"}, // Mult / Div
        {"+","-"}, // Add / Sub
    };
    ubyte i = 0;
    for( const vstr& lvl : pemdas ){  if( p_vstr_has_str( lvl, q ) ){  return i;  }else{  ++i;  }  }
    return 255;
}


P_Val infix_op( const P_Val& v1, const string& op, const P_Val& v2 ){
    // Invoke the math operator based on the token
    if( op == "**" ){
        return pow( v1, v2 );
    }else if( op == "*" ){
        return v1 * v2;
    }else if( op == "/" ){
        return 1.0 * v1 / v2; // FORCE devision to be float!
    }else if( op == "+" ){
        return v1 + v2;
    }else if( op == "-" ){
        return v1 - v2;
    }else{  return make_nan();  }
}


vstr get_parenthetical( const vstr& expr, size_t bgn = 0 ){
    // Get the contents of balanced parentheses starting at `bgn`
    size_t depth = 1;
    size_t i     = bgn+1;
    size_t N     = expr.size();
    vstr   rtnVec;
    string elem;
    while( i < N ){
        elem = expr[i];
        if( elem == "(" ){  ++depth;  }
        if( elem == ")" ){  --depth;  }
        if( depth > 0 ){  rtnVec.push_back( elem );  }else{  break;  }
        ++i;
    }
    return rtnVec;
}


P_Val CPC_Interpreter::calculate( const vstr& expr, CntxPtr cntx ){
    // Implements a stack-based calculator
    P_Val /*---*/ result = make_nan(); 
    string /*--*/ lastOp = "";
    size_t /*--*/ N /**/ = expr.size();
    size_t /*--*/ i /**/ = 0;
    size_t /*--*/ skip   = 0;
    vstr /*----*/ subExp;
    stack<string> oprs;
    stack<P_Val>  vals;
    P_Val /*---*/ lastVal;
    P_Val /*---*/ prevVal;
    P_Val /*---*/ currVal;
    
    if( p_literal_math_expr( expr ) ){
        // cout << endl << "BGN!-----------------" << endl << endl;
        for( const string& tkn : expr ){

            // cout << "## Iter: " << i << ", Token: " << tkn << ", `lastVal`: " << lastVal << ", `lastOp`: " << lastOp 
            //      << ", Skip: " << skip << " ##" << endl;

            if( skip > 0 ){
                --skip;
                // cout << ">skip!>" << endl;
                ++i; continue;
            }
            if( p_number_string( tkn ) || (tkn == "(") ){
                if( tkn == "(" ){
                    subExp  = get_parenthetical( expr, i );
                    // cout << endl << "PARENTHETICAL: " << subExp << endl << endl;
                    skip    = subExp.size()+1;
                    lastVal = calculate( subExp, cntx );
                }else if( p_number_string( tkn ) ){
                    lastVal = str_2_primitive( tkn );
                }
                // cout << "Got Value: " << lastVal << endl;
                if( oprs.size() && (PEMDAS( lastOp ) <= PEMDAS( oprs.top() )) ){
                    // cout << lastOp << " <= " << oprs.top() << endl;
                    prevVal = vals.top();
                    vals.pop();
                    vals.push( infix_op( prevVal, lastOp, lastVal ) );
                    lastOp = "";
                    lastVal = make_nan();
                }else if( oprs.size() && (PEMDAS( lastOp ) > PEMDAS( oprs.top() )) ){
                    // cout << lastOp << " > " << oprs.top() << endl;
                    currVal = vals.top();
                    vals.pop();
                    prevVal = vals.top();
                    vals.pop();
                    vals.push( infix_op( prevVal, oprs.top(), currVal ) );
                    oprs.pop();
                }
                if( lastOp.size() ){
                    oprs.push( lastOp );
                }
                if( i >= (N-1) ){  
                    if( !p_nan( lastVal ) ){
                        vals.push( lastVal );
                        lastVal = make_nan();
                    }
                }
                
            }else if( p_math_op( tkn ) ){
                lastOp = tkn;
                // cout << "Got Op: " << lastOp << endl;
                if( !p_nan( lastVal ) ){
                    vals.push( lastVal );
                }else{  return lastVal;  }
            }

            // cout << "\n/// Stack State ///" << endl  
            //      << "Values:" << vals << endl
            //      << "Ops: _ " << oprs << endl << endl;

            ++i;
        }
        if( !p_nan( lastVal ) ){
            vals.push( lastVal );
            // cout << vals.size() << " on the VALUE stack!" << endl;
        }
        while( oprs.size() ){
            lastOp  = oprs.top(); oprs.pop();
            lastVal = vals.top(); vals.pop();
            prevVal = vals.top(); vals.pop();
            // cout << prevVal << " " << lastOp << " " << lastVal << endl;
            vals.push( infix_op( prevVal, lastOp, lastVal ) );
        }
    }
    // cout << endl << "------------END!---> " << vals.top() << endl << endl;
    return vals.top();
}



P_Val CPC_Interpreter::interpret( NodePtr sourceTree, CntxPtr cntx ){
    // RUN THE CODE (Source Tree)!
    NodePtr root = sourceTree;
    string  ident;
    P_Val   value;
    string  valStr;
    CntxPtr nextCntx;
    vstr    tknLin;
    P_Val  bgn;
    P_Val  end;
    P_Val  cur;
    if( !cntx ){  cntx = context; }

    cout << "Node with " << root->edges.size() << " child nodes., Code: " << root->tokens << endl;

    switch ( root->type ){

        ///// Program Start ///////////////////////////////////////////////
        case PROGRAM:
            cout << "Run program!" << endl;
        case CODE_BLC:
            for( const NodePtr node : root->edges ){  interpret( node, cntx );  }
            break;

        ///// Constant Declaration ////////////////////////////////////////
        case CON_SCTN:
            for( const NodePtr node : root->edges ){  
                switch( node->type ){

                    case ASSIGNMENT:
                        ident = node->edges.front()->tokens[0];
                        node->edges.pop_front();
                        value = calculate( node->edges.front()->tokens, cntx );
                        node->edges.pop_front();
                        cntx->constants[ ident ] = value;
                        break;
                    
                    default:
                        cerr << "CONSTANTS, CANNOT PROCESS: " << node->tokens << endl;
                        break;
                }
            }
            break;

        ///// Variable Declaration ////////////////////////////////////////
        case VAR_SCTN:
            for( const NodePtr node : root->edges ){  
                switch( node->type ){

                    // NOTE: VAR TYPE IS DYNAMIC AND **NOT** ENFORCED!
                    case VAR_DECL:
                        ident = node->edges.front()->tokens[0];
                        node->edges.pop_front();
                        valStr = node->edges.front()->tokens[0];
                        node->edges.pop_front();
                        if( valStr == "integer" ){
                            cout << "Create `llong`: " << valStr << endl;
                            cntx->variables[ ident ] = make_llong();
                        }else if( valStr == "real" ){
                            cout << "Create `double`: " << valStr << endl;
                            cntx->variables[ ident ] = make_double();
                        }else{
                            cerr << "VARIABLES, TYPE NOT RECOGNIZED: " << valStr << ", " << node->tokens << endl;    
                        }
                        break;

                    default:
                        cerr << "VARIABLES, CANNOT PROCESS: " << node->tokens << endl;
                        break;
                }
            }
            break;

        ///// Function Call ///////////////////////////////////////////////
        case FUNCTION:
            // FIXME: NEED TO BUILD A CONTEXT ACCORDING TO THE ARGS!
            nextCntx = CntxPtr{ new Context{} };
            nextCntx->parent = cntx;
            ident = root->edges.front()->tokens[0];
            root->edges.pop_front();
            if( Built_In_Functions.count( ident ) ){
                tknLin = root->edges.front()->tokens;
                Built_In_Functions[ ident ]( tknLin, nextCntx );
            }
            break;

        ///// For Loop ////////////////////////////////////////////////////
        case FOR_LOOP:
            nextCntx = CntxPtr{ new Context{} };
            nextCntx->parent = cntx;
            tknLin = root->tokens;
            ident  = tknLin[1];
            bgn = nextCntx->get_value_by_name( tknLin[ vstr_find_index( tknLin, ":=" ) +1 ] );
            end = nextCntx->get_value_by_name( tknLin[ vstr_find_index( tknLin, "to" ) +1 ] );
            cur = bgn;
            nextCntx->set_value_by_name( ident, bgn );
            while( cur < end ){
                cout << "\tLoop Iteration " << cur << " < " << end << ", " << root->edges.size() << endl;
                cout << "\tExec: " << root->edges.front()->tokens << endl;
                interpret( root->edges.front(), nextCntx );
                cur += P_Val{1};
                nextCntx->set_value_by_name( ident, cur );
            }
            break;

        ///// Logical Error ///////////////////////////////////////////////
        case INVALID:
        default:
            cerr << "CANNOT PROCESS: " << root->tokens << endl;
            break;
    }
    return make_nan();
}
