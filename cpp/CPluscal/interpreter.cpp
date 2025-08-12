////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// SOURCE TREE /////////////////////////////////////////////////////////////////////////////

ProgNode::ProgNode( NodeType typ, vstr tkns ){
    type   = typ;
    tokens = tkns;
}


////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

bool CPC_Interpreter::load_program_file( string fPath ){
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


bool p_assignment_statememt( const vstr& tokens ){
    // Does this statement fit the pattern `<identifier> = <literal math expr>`?
    size_t eqDex  = vstr_find_index( tokens, "=" );
    size_t expEnd = vstr_find_index( tokens, ";" );
    vstr   valExpr;
    if( tokens.size() == 0 ){  return false;  }
    // ASSUMPTION: FIRST TOKEN IS THE IDENTIFIER
    if( !p_identifier( tokens[0] ) ){  return false;  }
    if( eqDex  == SIZE_MAX ){  return false;  }
    if( expEnd == SIZE_MAX ){  expEnd = tokens.size();  }
    valExpr = get_sub_vec( tokens, eqDex+1, expEnd );
    return p_literal_math_expr( valExpr );
}


vstr get_RHS( const vstr& expr ){
    // Return the Right Hand Side of an `expr`ession
    size_t eqDex  = vstr_find_index( expr, "=" );
    size_t expEnd = vstr_find_index( expr, ";" );
    if( eqDex  == SIZE_MAX ){  return vstr{};  }
    if( expEnd == SIZE_MAX ){  expEnd = expr.size();  }
    return get_sub_vec( expr, eqDex+1, expEnd );
}


bool p_var_declare_statememt( const vstr& tokens ){
    size_t colonDex = vstr_find_index( tokens, ":" );
    if( colonDex == SIZE_MAX ){  return false;  }
    if( !p_identifier( tokens[0] ) ){  return false;  }
    if( !p_prim_type( tokens[colonDex+1] ) ){  return false;  }
    return true;
}


enum ParseMode{ 
    BEGIN, 
    COMMENT, 
    VAR_SECT,
    CONST_SECT,
    MAIN_SECT,
};


bool CPC_Interpreter::build_source_tree(){
    // Build a cheap Abstract Source Tree to be executed later
    // State flags: Should probably be an enum!
    ParseMode mode   = BEGIN;
    NodePtr   root   = nullptr;
    size_t    chrDex = SIZE_MAX;

    

    // For every line of tokens, do ...
    for( const vstr& tknLin : lexer.lineTokens ){
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
            program.push_back( NodePtr{ new ProgNode{ PROGRAM, tknLin } } );
            cout << "\tProgram Start!" << endl;
        
        ///// Comment Start //////////////////////
        // ASSUMPTION: COMMENT BEGINS AT THE START OF THE LINE
        }else if( tknLin.front() == "{" ){
            mode = COMMENT;
            cout << "\tComment Start!" << endl;

        ///// Variable Section Start /////////////
        // ASSUMPTION: VAR SECTION DECLARATION IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "var" ) ){
            mode = VAR_SECT;
            cout << "\tVariable Start!" << endl;

        ///// Variable Section Start /////////////
        // ASSUMPTION: CONST SECTION DECLARATION IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "const" ) ){
            mode = CONST_SECT;
            cout << "\tConst Start!" << endl;

        ///// Main Program Start /////////////////
        // ASSUMPTION: PROGRAM START `begin` IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "begin" ) && (tknLin.size() == 1) ){
            cout << "\tProgram Start!" << endl;

        ///// Constants Section //////////////////
        }else if( mode == CONST_SECT ){
            if( p_assignment_statememt( tknLin ) ){
                cout << "\tConst Assignment!" << endl;
                root = NodePtr{ new ProgNode{ ASSIGNMENT, tknLin } };
                root->edges.push_back( NodePtr{ new ProgNode{ IDENTIFIER, get_sub_vec( tknLin, 0, 1 ) } } );
                root->edges.push_back( NodePtr{ new ProgNode{ MATH_EXPR , get_RHS( tknLin ) } } );
                context.constants.push_back( root );
            }

        ///// Variables Section //////////////////
        }else if( mode == VAR_SECT ){
            if( p_var_declare_statememt( tknLin ) ){
                cout << "\tVar Declaration!" << endl;
                root = NodePtr{ new ProgNode{ VAR_DECL, tknLin } };
                root->edges.push_back( NodePtr{ new ProgNode{ IDENTIFIER, get_sub_vec( tknLin, 0, 1 ) } } );
                chrDex = vstr_find_index( tknLin, ":" );
                root->edges.push_back( NodePtr{ new ProgNode{ TYPENAME, get_sub_vec( tknLin, chrDex, chrDex+1 ) } } );
                context.variables.push_back( root );
            }

        ///// Unknown Expression /////////////////
        }else{  
            cout << "Line " << tknLin << " could not be parsed!" << endl;
            return false;  
        }
    }
    return true;
}
