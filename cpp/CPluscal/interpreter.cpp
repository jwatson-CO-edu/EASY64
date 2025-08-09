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


bool CPC_Interpreter::build_source_tree(){
    // Build a cheap Abstract Source Tree to be executed later
    // State flags: Should probably be an enum!
    bool openComment = false;
    bool readVars    = false;
    bool readConst   = false;
    // For every line of tokens, do ...
    for( const vstr& tknLin : lexer.lineTokens ){

        ///// Comment Continue / End /////////////
        if( openComment ){
            // ASSUMPTION: COMMENT ENDS AT THE END OF THE LINE
            if( tknLin.back() == "}" ){  openComment = false;  }
            continue;

        ///// Program Start //////////////////////
        // ASSUMPTION: PROGRAM DECLARATION IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "program" ) ){
            program.push_back( NodePtr{ new ProgNode{ PROGRAM, tknLin } } );
        
        ///// Comment Start //////////////////////
        // ASSUMPTION: COMMENT BEGINS AT THE START OF THE LINE
        }else if( tknLin.front() == "{" ){
            openComment = true;
            readVars    = false;
            readConst   = false;

        ///// Variable Section Start /////////////
        // ASSUMPTION: VAR SECTION DECLARATION IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "var" ) ){
            readVars  = true;
            readConst = false;

        ///// Variable Section Start /////////////
        // ASSUMPTION: CONST SECTION DECLARATION IS ON ITS OWN LINE
        }else if( p_vstr_has_str( tknLin, "const" ) ){
            readConst = true;
            readVars  = false;

        ///// Constants Section //////////////////
        }else if( readConst ){
            

        }else{  
            cout << "Line " << tknLin << " could not be parsed!" << endl;
            return false;  
        }
        return true;
    }
}
