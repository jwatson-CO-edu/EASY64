////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// SOURCE TREE /////////////////////////////////////////////////////////////////////////////

ProgNode::ProgNode( NodeType typ, vstr tkns ){
    type   = typ;
    tokens = tkns;
}


////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

CPC_Parser::CPC_Parser(){
    // Default Constructor
}

bool CPC_Parser::load_program_file( string fPath ){
    // Invoke the lexer
    lexer = LexMachine{ fPath };
    return true;
}


bool p_vstr_has_str( const vstr& vec, const string& q ){
    // Return true if the `vstr` contains `q`
    for( const string& str : vec ){  if( str == q ){  return true;  }  }
    return false;
}


bool p_number_string( const string& q ){
    // Return true if the string can represent a primitive
    /// Handle `long` ///
    try {
        stol(q);
        return true;
    }catch( const std::invalid_argument& e ){
        cerr << "Invalid argument: " << e.what() << endl;
    } catch( const std::out_of_range& e ){
        cerr << "Out of range: " << e.what() << endl;
    }
    /// Handle `double` ///
    try {
        stod(q);
        return true;
    }catch( const std::invalid_argument& e ){
        cerr << "Invalid argument: " << e.what() << endl;
    }catch( const std::out_of_range& e ){
        cerr << "Out of range: " << e.what() << endl;
    }
    return false;
}


bool p_math_op( const string& q ){
    // Is this string an infix math operator?
    for( ubyte i = 0; i < NUM_INFIX; ++i ){  if( q == SYMBOLS[i] ){  return true;  }  }
    return false;
}


bool p_literal_math_expr( const vstr& tokens ){
    // Does this expression contain only numbers and infix math operators?
    if( tokens.size() == 0 ){  return false;  }
    for( const string& token : tokens ){
        if( !( p_math_op( token ) || p_number_string( token ) ) ){  return false;  }
    }
    return true;
}


bool p_ident_math_expr( const vstr& tokens ){
    // Does this expression contain only numbers and infix math operators?
    if( tokens.size() == 0 ){  return false;  }
    for( const string& token : tokens ){
        if( !( p_math_op( token ) || p_number_string( token ) || p_identifier( token ) ) ){  return false;  }
    }
    return true;
}


bool p_assignment_statememt( const vstr& tokens, const string& eqSym = ":=" ){
    // Does this statement fit the pattern `<identifier> = <literal math expr>`?
    size_t eqDex  = vstr_find_index( tokens, eqSym );
    size_t expEnd = vstr_find_index( tokens, ";"   );
    vstr   valExpr;
    if( tokens.size() == 0 ){  return false;  }
    // ASSUMPTION: FIRST TOKEN IS THE IDENTIFIER
    if( !p_identifier( tokens[0] ) ){  return false;  }
    if( eqDex  == SIZE_MAX ){  return false;  }
    if( expEnd == SIZE_MAX ){  expEnd = tokens.size();  }
    valExpr = get_sub_vec( tokens, eqDex+1, expEnd );
    return p_ident_math_expr( valExpr );
}


vstr get_RHS( const vstr& expr, const string& eqSym = ":=" ){
    // Return the Right Hand Side of an `expr`ession
    size_t eqDex  = vstr_find_index( expr, eqSym );
    size_t expEnd = vstr_find_index( expr, ";"   );
    if( eqDex  == SIZE_MAX ){  return vstr{};  }
    if( expEnd == SIZE_MAX ){  expEnd = expr.size();  }
    return get_sub_vec( expr, eqDex+1, expEnd );
}


bool p_var_declare_statememt( const vstr& tokens ){
    // Return True if `tokens` is a variable declaration statement
    size_t colonDex = vstr_find_index( tokens, ":" );
    if( colonDex == SIZE_MAX ){  return false;  }
    if( !p_identifier( tokens[0] ) ){  return false;  }
    if( !p_prim_type( tokens[colonDex+1] ) ){  return false;  }
    return true;
}


bool p_function_call( const vstr& expr ){
    // Determine if `expr` is a function call
    // ASSUMPTION: ENTIRE FUNCTION CALL IS ON THE SAME LINE
    size_t openDex = vstr_find_index( expr, "(" );
    size_t closDex = vstr_find_index( expr, ")" );
    if( !p_identifier( expr[0] ) ){  return false;  }
    if( openDex == SIZE_MAX ){  return false;  }
    if( closDex == SIZE_MAX ){  return false;  }
    return true;
}


vstr get_func_args( const vstr& expr ){
    // Return the function arguments in `expr`, Otherwise return an empty vector
    // ASSUMPTION: ENTIRE FUNCTION CALL IS ON THE SAME LINE
    size_t openDex = vstr_find_index( expr, "(" );
    size_t closDex = vstr_find_index( expr, ")" );
    if( openDex == SIZE_MAX ){  return vstr{};  }
    if( closDex == SIZE_MAX ){  return vstr{};  }
    return get_sub_vec( expr, openDex+1, closDex );
}



///// AST Parsing /////////////////////////////////////////////////////////

enum ParseMode{ 
    // What is the parser working on?
    BEGIN, 
    COMMENT, 
    VAR_SECT,
    CONST_SECT,
    MAIN_SECT,
    FOR_CNTX,
};

vstr concat( const vvstr& tokenLines ){
    size_t N = 0;
    vstr   rtnVec;
    for( const vstr& line : tokenLines ){  N += line.size();  }
    rtnVec.reserve( N );
    for( const vstr& line : tokenLines ){  for( const string& token : line ){  rtnVec.push_back( token );  }  }
    return rtnVec;
}


NodePtr CPC_Parser::build_source_tree( const vvstr& lineTokens ){
    // Build a cheap Abstract Source Tree to be executed later
    ParseMode mode    = BEGIN;
    NodePtr   root    = nullptr;
    size_t    chrDex  = SIZE_MAX;
    NodePtr   rtnPtr{ new ProgNode{  PROGRAM, concat( lineTokens )  } };
    NodePtr   conSctn = nullptr;
    NodePtr   varSctn = nullptr;
    NodePtr   typSctn = nullptr;
    
    // For every line of tokens, do ...
    for( const vstr& tknLin : lineTokens ){
        cout << "Line: " << tknLin << endl;

        if( mode == BEGIN ){  cout << "\tDefault Mode!" << endl;  }

        ///// Comment Continue / End /////////////
        if( mode == COMMENT ){
            // ASSUMPTION: COMMENT ENDS AT THE END OF THE LINE
            if( tknLin.back() == "}" ){  mode = BEGIN;  }
            cout << "\tComment End!" << endl;
            continue;

        ///// Program Start //////////////////////
        // ASSUMPTION: PROGRAM DECLARATION IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "program" ) ){
            header = NodePtr{ new ProgNode{ PROGRAM, tknLin } };
            cout << "\tProgram Start!" << endl;
        
        ///// Comment Start //////////////////////
        // ASSUMPTION: COMMENT BEGINS AT THE START OF THE LINE
        }else if( tknLin.front() == "{" ){
            mode = COMMENT;
            cout << "\tComment Start!" << endl;

        ///// Variable Section Start /////////////
        // ASSUMPTION: VAR SECTION DECLARATION IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "var" ) ){
            mode    = VAR_SECT;
            varSctn = NodePtr{ new ProgNode{ VAR_SCTN, tknLin } };
            cout << "\tVariable Start!" << endl;

        ///// Variable Section Start /////////////
        // ASSUMPTION: CONST SECTION DECLARATION IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "const" ) ){
            mode    = CONST_SECT;
            conSctn = NodePtr{ new ProgNode{ CON_SCTN, tknLin } };
            cout << "\tConst Start!" << endl;

        ///// Main Program Start /////////////////
        // ASSUMPTION: PROGRAM START `begin` IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "begin" ) && (tknLin.size() == 1) ){
            cout << "\tProgram Start!" << endl;
            mode = MAIN_SECT;

        ///// Constants Section //////////////////
        }else if( mode == CONST_SECT ){
            if( p_assignment_statememt( tknLin, "=" ) ){
                cout << "\tConst Assignment!" << endl;
                root = NodePtr{ new ProgNode{ ASSIGNMENT, tknLin } };
                root->edges.push_back( NodePtr{ new ProgNode{ IDENTIFIER, get_sub_vec( tknLin, 0, 1 ) } } );
                root->edges.push_back( NodePtr{ new ProgNode{ MATH_EXPR , get_RHS( tknLin, "=" ) } } );
                conSctn->edges.push_back( root );
            }

        ///// Variables Section //////////////////
        }else if( mode == VAR_SECT ){
            if( p_var_declare_statememt( tknLin ) ){
                cout << "\tVar Declaration!" << endl;
                root = NodePtr{ new ProgNode{ VAR_DECL, tknLin } };
                root->edges.push_back( NodePtr{ new ProgNode{ IDENTIFIER, get_sub_vec( tknLin, 0, 1 ) } } );
                chrDex = vstr_find_index( tknLin, ":" );
                root->edges.push_back( NodePtr{ new ProgNode{ TYPENAME, get_sub_vec( tknLin, chrDex, chrDex+1 ) } } );
                varSctn->edges.push_back( root );
            }

        ///// Main Program ///////////////////////
        }else if( mode == MAIN_SECT ){
            ///// Function Call /////
            if( p_function_call( tknLin ) ){
                cout << "\tFunction Call!" << endl;
                root = NodePtr{ new ProgNode{ FUNCTION, tknLin } };
                root->edges.push_back( NodePtr{ new ProgNode{ IDENTIFIER, get_sub_vec( tknLin, 0, 1 ) } } );
                root->edges.push_back( NodePtr{ new ProgNode{ ARGUMENTS , get_func_args( tknLin ) } } );
                rtnPtr->edges.push_back( root );

            ///// For Loop /////
            }else if( p_vstr_has_str( tknLin, "for" ) ){
                cout << "\tFor Loop Begin!" << endl;

            ///// Variable Assignment /////
            }else if( p_assignment_statememt( tknLin, ":=" ) ){
                cout << "\tVar Assignment!" << endl;
                root = NodePtr{ new ProgNode{ ASSIGNMENT, tknLin } };
                root->edges.push_back( NodePtr{ new ProgNode{ IDENTIFIER, get_sub_vec( tknLin, 0, 1 ) } } );
                root->edges.push_back( NodePtr{ new ProgNode{ MATH_EXPR , get_RHS( tknLin, ":=" ) } } );
                rtnPtr->edges.push_back( root );
            }

        ///// Unknown Expression /////////////////
        }else{  
            cout << "Line " << tknLin << " could not be parsed!" << endl;
            break;
        }
    }
    return rtnPtr;
}
