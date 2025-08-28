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

string repeat_char( size_t N, char chr = ' ' ){
    string rtnStr = "";
    for( size_t ii = 0; ii < N; ++ii ){  rtnStr += chr;  }
    return rtnStr;
}


P_Obj CPC_Interpreter::writeln( const vobj& args, CntxPtr cntx ){
    // Basic print followed by a newline
    P_Val /*--*/ val;
    P_Val /*--*/ vArg;
    stringstream valStrm;
    string /*-*/ valStr;
    string /*-*/ nptStr;
    string /*-*/ pad;
    string /*-*/ sArg;
    size_t /*-*/ wInt   = SIZE_MAX;
    size_t /*-*/ nInt   = 0;
    size_t /*-*/ wDec   = SIZE_MAX;
    size_t /*-*/ nDec   = 0;
    size_t /*-*/ wPad   = 0;
    size_t /*-*/ decPos = string::npos;
    size_t /*-*/ i /**/ = 0;
    size_t /*-*/ N /**/ = args.size();
    bool /*---*/ getInt = false;
    bool /*---*/ getDec = false;
    bool /*---*/ _VERB  = false;
    bool /*---*/ pStr   = false;
    bool /*---*/ pVal   = false;

    if( _VERB ) cout << "`writeln` got args: " << args << endl;

    auto format_value = [&](){
        getInt = false;
        getDec = false;
        if( _VERB ) cout << "\tAbout to format: " << valStr << ", Tot. Width: " << wInt << ":" << nInt << ", Precision: " << wDec << ":" << nDec << endl;
        if( valStr.size() ){
            if( wInt < SIZE_MAX ){
                if( nInt < wInt ){
                    wPad   = wInt-nInt;
                    if( _VERB ) cout << "\t\tAdd Pad: " << wPad << endl;
                    valStr = repeat_char( wPad, ' ' ) + valStr;
                }
            }
            if( wDec < SIZE_MAX ){
                if( nDec > wDec ){
                    if( _VERB ) cout << "\t\tExcessive Precision: " << nDec << " --to-> " << wDec << endl;
                    valStr = valStr.substr( 0, valStr.size()-(nDec-wDec) );
                }else if( nDec < wDec ){
                    if( nDec > 0 ){
                        valStr += repeat_char( wDec-nDec, '0' );
                    }else{
                        decPos = valStr.size();
                        valStr += "." + repeat_char( wDec, '0' );
                    }
                }
                if( valStr.size() > wInt ){

                    if((nInt + 1 + wDec) < wInt){
                        valStr = valStr.substr( wPad-(wInt-(nInt+1+wDec)) );
                    }else{
                        valStr = valStr.substr( wPad );
                    }
                        
                }else{
                    valStr = repeat_char( wInt-valStr.size(), ' ' ) + valStr;
                }
            }
            if( _VERB ) cout << "\tFormatted Number: " << valStr << endl;
            cout << valStr;
        }else{  cout << ' ';  }
    };

    for( const P_Obj& arg : args ){

        if( holds_alternative<string>( arg ) ){  
            sArg = get<string>( arg );  
            pStr = true;
            pVal = false;
        }else if( holds_alternative<P_Val>( arg ) ){  
            vArg = get<P_Val>( arg );  
            pStr = false;
            pVal = true;
        }

        if( pStr && p_string_token( sArg ) ){  
            getInt = false;
            getDec = false;
            cout << sArg.substr( 1, sArg.size()-2 ) << " ";  
        }else if( pVal ){  

            if( _VERB ) cout << "\tProcess Number: " << vArg << endl;

            valStrm << vArg;
            nptStr = valStrm.str();
            valStrm.flush();
            valStrm.str("");
            valStrm.clear();

            
            if( getInt ){  
                wInt = strtoull( nptStr.c_str(), NULL, 0 );  
                if( _VERB ) cout << "\tGot Width: " << nptStr << " --to-> " << wInt << endl;
            }else if( getDec ){  
                wDec = strtoull( nptStr.c_str(), NULL, 0 );  
                if( _VERB ) cout << "\tGot Precision: "<< nptStr << " --to-> " << wDec << endl;
            }else{  
                valStr = nptStr;  
                decPos = nptStr.find( '.' );
                if( decPos != string::npos ){
                    nInt = decPos;
                    nDec = valStr.size() - nInt - 1;
                }else{
                    nInt = valStr.size();
                    nDec = 0;
                }
                if( _VERB ) cout << "\tGot Value: "<< nptStr << " --to-> " << valStr << endl;
            }
            
        }else if( pStr && (sArg == ":") ){  
            if( !getInt ){
                if( _VERB ) cout << "\tLook for width!" << endl;
                getInt = true;
                getDec = false;
            }else{
                if( _VERB ) cout << "\tLook for precision!" << endl;
                getInt = false;
                getDec = true;
            }
        }else if( pStr && (sArg == ",") ){
            format_value();
        }

        if( i >= (N-1) ){  format_value();  }

        ++i;
    }
    cout << endl;
    return P_Val{true};
}


P_Obj CPC_Interpreter::sqrt( const vobj& args, CntxPtr cntx ){
    // Compute the square root of the expression
    return P_Val{ std::sqrt( as_double( calculate( args, cntx ) ) ) };
}


vstr vstr_remove( const vstr& vec, const string& remStr ){
    vstr rtnVec;
    for( const string& q : vec ){  if( q != remStr ){  rtnVec.push_back( q );  }  }
    return rtnVec;
}

vstr vstr_filter( const vstr& vec, bool (*filterFunc)(const string &) ){
    vstr rtnVec;
    for( const string& q : vec ){  if( filterFunc( q ) ){  rtnVec.push_back( q );  }  }
    return rtnVec;
}


string unwrap_string( const string& strStr ){
    if( p_string_token( strStr ) ){  return strStr.substr( 1, strStr.size()-2 );  }else{  return strStr;  }
}

vstr unwrap_strings( const vstr& vStrStr ){
    vstr rtnVec;
    rtnVec.reserve( vStrStr.size() );
    for( const string& str : vStrStr ){  rtnVec.push_back( unwrap_string( str ) );  }
    return rtnVec;
}


P_Obj CPC_Interpreter::read( const vobj& args, CntxPtr cntx ){
    // Read input from the user
    string input;
    string ident;
    vstr   names = unwrap_strings( vstr_filter( as_vstr( args ), p_string_token ) );
    vstr   values;
    size_t N = 0;
    size_t i = 0;
    P_Obj  rtnVal = P_Obj{ make_nan() };
    // Read Input
    cout << endl << "Input for " << args << "separated by commas,\nthen press [Enter]: ";
    cin >> input;
    // Discard the remaining newline character after reading the number
    cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
    cout << endl;

    values = vstr_remove( tokenize( input ), ",");
    N      = names.size();

    for( const string& npt_i : values ){
        if( (i<N) && p_number_string( npt_i ) ){
            cntx->set_value_by_name( names[i], str_2_primitive( npt_i ) );
            rtnVal = P_Obj{ cntx->get_value_by_name( names[i] ) };
        }
        ++i;
    }
    return rtnVal;
}



////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////


CPC_Interpreter::CPC_Interpreter(){  
    // Default Constructor
    context = CntxPtr{ new Context{} };  
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





P_Val CPC_Interpreter::calculate( const vobj& expr, CntxPtr cntx ){
    // Implements a stack-based calculator
    bool /*----*/ _VERB    = true;
    if( _VERB ) cout << endl << "............ about to calc ...." << endl;
    vstr /*----*/ sExpr  = as_vstr( expr );
    if( _VERB ) cout << endl << "............ default return val ...." << endl;
    P_Val /*---*/ result = make_nan(); 
    string /*--*/ lastOp = "";
    if( _VERB ) cout << endl << "............ how many? ...." << endl;
    size_t /*--*/ N /**/ = expr.size();
    size_t /*--*/ i /**/ = 0;
    size_t /*--*/ skip   = 0;
    vobj /*----*/ subExp;
    stack<string> oprs;
    stack<P_Val>  vals;
    P_Val /*---*/ lastVal;
    P_Val /*---*/ prevVal;
    P_Val /*---*/ currVal;
    NodePtr /*-*/ funcCall = nullptr;
    if( _VERB ) cout << endl << "............ start? ...." << sExpr << endl;
    // cout << p_func_math_expr( sExpr ) << endl;
    
    if( p_func_math_expr( sExpr ) ){
        if( _VERB ) cout << endl << "BGN!-----------------" << endl << endl;
        for( const string& tkn : sExpr ){

            if( _VERB ) cout << "## Iter: " << i << ", Token: " << tkn << ", `lastVal`: " << lastVal << ", `lastOp`: " << lastOp 
                             << ", Skip: " << skip << " ##" << endl;

            if( skip > 0 ){
                --skip;
                if( _VERB ) cout << ">skip!>" << endl;
                ++i; continue;
            }
            if( p_number_string( tkn ) || (tkn == "(") || p_identifier( tkn ) ){
                if( tkn == "(" ){
                    subExp  = get_parenthetical( expr, i );
                    if( _VERB ) cout << endl << "PARENTHETICAL: " << subExp << endl << endl;
                    skip    = subExp.size()+1;
                    lastVal = calculate( subExp, cntx );
                }else if( p_number_string( tkn ) ){
                    lastVal = str_2_primitive( tkn );
                }else if( p_identifier( tkn ) ){

                    /// Function Call ///
                    if( ((i+1)<N) && (sExpr[i+1] == "(") ){
                        subExp   = get_parenthetical( expr, i );
                        cout << "Fetch Args: " << subExp << endl;
                        funcCall = NodePtr{ new ProgNode{ FUNCTION, get_sub_vec( expr, i, i+subExp.size()+2 ) } };
                        funcCall->edges.push_back( NodePtr{ new ProgNode{ IDENTIFIER, vstr{ tkn } } } );
                        funcCall->edges.push_back( NodePtr{ new ProgNode{ ARGUMENTS , subExp } } );
                        lastVal  = get<P_Val>( interpret( funcCall, cntx ) );
                        funcCall = nullptr;

                    /// Variable / Constant ///
                    }else{  
                        cout << "Fetch value stored in \"" << tkn << "\"" << endl;
                        lastVal = cntx->get_value_by_name( tkn );  
                    }

                }else{
                    cerr << "MATH TOKEN NOT RECOGNIZED: " << tkn << endl;
                    return make_nan();
                }
                if( _VERB ) cout << "Got Value: " << lastVal << endl;
                if( oprs.size() && (PEMDAS( lastOp ) <= PEMDAS( oprs.top() )) ){
                    if( _VERB ) cout << lastOp << " <= " << oprs.top() << endl;
                    prevVal = vals.top();
                    vals.pop();
                    vals.push( infix_op( prevVal, lastOp, lastVal ) );
                    lastOp = "";
                    lastVal = make_nan();
                }else if( oprs.size() && (PEMDAS( lastOp ) > PEMDAS( oprs.top() )) ){
                    if( _VERB ) cout << lastOp << " > " << oprs.top() << endl;
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
                if( _VERB ) cout << "Got Op: " << lastOp << endl;
                if( !p_nan( lastVal ) ){
                    vals.push( lastVal );
                }else{  return lastVal;  }
            }

            if( _VERB ) cout << "\n/// Stack State ///" << endl  
                             << "Values:" << vals << endl
                             << "Ops: _ " << oprs << endl << endl;

            ++i;
        }
        if( !p_nan( lastVal ) ){
            vals.push( lastVal );
        }
        while( oprs.size() ){
            lastOp  = oprs.top(); oprs.pop();
            lastVal = vals.top(); vals.pop();
            prevVal = vals.top(); vals.pop();
            if( _VERB ) cout << prevVal << " " << lastOp << " " << lastVal << endl;
            vals.push( infix_op( prevVal, lastOp, lastVal ) );
        }
    }
    if( _VERB ) cout << endl << "------------END!---> " << endl << endl;
    return (vals.size() ? vals.top() : make_nan());
}



P_Obj CPC_Interpreter::interpret( NodePtr root, CntxPtr cntx ){
    // RUN THE CODE (Source Tree)!
    P_Obj   ident;
    string  sIdent;
    P_Val   value;
    string  valStr;
    CntxPtr nextCntx;
    NodePtr node;
    vobj    tknLin;
    vobj    calcdArgs;
    vstr    strLin;
    P_Val   bgn;
    P_Val   end;
    P_Val   cur;
    P_Obj   rtnVal = make_nan();
    size_t  i = 0;
    size_t  N = 0;
    bool    _VERB = true;
    if( !cntx ){  cntx = context; }

    if( _VERB ) cout << "Node with " << root->edges.size() << " child nodes., Code: " << root->tokens << endl;

    switch ( root->type ){

        ///// Program Start ///////////////////////////////////////////////
        case PROGRAM:
            if( _VERB ) cout << "Run program!" << endl;
            for( const NodePtr node : root->edges ){  interpret( node, cntx );  }
            break;

        case CODE_BLC:
            if( _VERB ) cout << "Run code block!" << endl;
            for( const NodePtr node : root->edges ){  interpret( node, cntx );  }
            break;

        case EXPRESSION:
            if( _VERB ) cout << "Calc expression!: " << root->tokens << endl;
            if( p_func_math_expr( as_vstr( root->tokens ) ) ){
                rtnVal = P_Obj{ calculate( root->tokens, cntx ) };
            }else if( (root->tokens.size() == 1) && p_string_token( root->tokens[0] ) ){
                rtnVal = root->tokens[0];
            }else{
                cerr << "BAD EXPRESSION: " << root->tokens << endl;
                rtnVal = P_Obj{ make_err_syntax() };
            }
            break;

        ///// Constant Declaration ////////////////////////////////////////
        case CON_SCTN:
            for( const NodePtr node : root->edges ){  
                switch( node->type ){

                    case ASSIGNMENT:
                        ident  = node->edges[0]->tokens[0];
                        if( type_of( ident ) == STRING ){
                            value = calculate( node->edges[1]->tokens, cntx );
                            cntx->set_value_by_name( as_string( ident ), value );
                        }
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
                        ident  = node->edges[0]->tokens[0];
                        valStr = as_string( node->edges[1]->tokens[0] );
                        if( valStr == "integer" ){
                            if( _VERB ) cout << "Create `llong`: " << valStr << endl;
                            if( p_identifier( as_string( ident ) ) ){
                                cntx->variables[ as_string( ident ) ] = make_llong();
                            }
                            
                        }else if( valStr == "real" ){
                            if( _VERB ) cout << "Create `double`: " << valStr << endl;
                            if( p_identifier( as_string( ident ) ) ){
                                cntx->variables[ as_string( ident ) ] = make_double();
                            }
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

        ///// Variable Assignment /////////////////////////////////////////
        case ASSIGNMENT:
            // cout << "Assignment!, " << root->edges.size() << " edges!" << endl;
            ident = root->edges[0]->tokens[0];
            // cout << "Assignment!, Ident: " << ident << endl;
            value = calculate( root->edges[1]->tokens, cntx );
            if( _VERB ) cout << "Assignment, Ident: " << ident << ", Value: "<< root->edges[1]->tokens << " = " << value << endl;
            if( !(cntx->set_value_by_name( as_string( ident ), value )) ){  
                cerr << "Name NOT in context!: " << ident << endl;
                return make_nan();  
            }
            break;

        ///// Function Call ///////////////////////////////////////////////
        case FUNCTION:
            // FIXME: NEED TO BUILD A CONTEXT ACCORDING TO THE ARGS!
            
            nextCntx = CntxPtr{ new Context{} };
            nextCntx->parent = cntx;
            ident  = root->edges[0]->tokens[0];
            sIdent = as_string( ident );
            if( _VERB ) cout << "Exec Function, " << sIdent << "!" << endl;
            N /**/ = root->edges.size();
            calcdArgs.clear();
            calcdArgs.reserve( N-1 );
            if( _VERB ) cout << "\tProcess " << (N-1) << " args!" << endl;
            for( i = 1; i < N; ++i ){  
                calcdArgs.push_back( interpret( root->edges[i], cntx ) );  
                if( _VERB ) cout << "\tArg " << i << " : " << root->edges[i]->tokens << " --to-> " << calcdArgs.back() << endl;
            }
            if( sIdent == "writeln" ){
                writeln( calcdArgs, nextCntx );
            }else if( sIdent == "sqrt" ){
                rtnVal = sqrt( calcdArgs, nextCntx );
            }else if( sIdent == "read" ){
                rtnVal = read( calcdArgs, nextCntx );
            }
            break;


        ///// For Loop ////////////////////////////////////////////////////
        case FOR_LOOP:
            nextCntx = CntxPtr{ new Context{} };
            nextCntx->parent = cntx;
            tknLin = root->tokens;
            ident  = tknLin[1];
            sIdent = as_string( ident );
            strLin = as_vstr( tknLin );
            bgn = nextCntx->get_value_by_name( strLin[ vstr_find_index( strLin, ":=" ) +1 ] );
            end = nextCntx->get_value_by_name( strLin[ vstr_find_index( strLin, "to" ) +1 ] );
            cur = bgn;
            nextCntx->set_value_by_name( sIdent, bgn );
            while( cur < end ){
                if( _VERB ) cout << "\tLoop Iteration " << cur << " < " << end << ", " << root->edges.size() << endl;
                if( _VERB ) cout << "\tExec: " << root->edges[0]->tokens << endl;
                interpret( root->edges[0], nextCntx );
                cur += P_Val{1};
                nextCntx->set_value_by_name( sIdent, cur );
            }
            break;

        ///// Logical Error ///////////////////////////////////////////////
        case INVALID:
        default:
            cerr << "CANNOT PROCESS: " << root->tokens << endl;
            break;
    }
    return rtnVal;
}
