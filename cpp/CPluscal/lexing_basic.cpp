////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// STRING PROCESSING ///////////////////////////////////////////////////////////////////////

bool p_special( const string& q ){
    // Return True if `q` matches a symbol, otherwise return False
    for( const string& sym : SYMBOLS ){  if( q == sym ){  return true;  }  }
    return false;
}


bool p_infix_op( const string& q ){
    // Return True if `q` matches an infix arithmetic operator, otherwise return False
    string sym;
    for( size_t i = 0; i < NUM_INFIX; ++i ){
        sym = SYMBOLS[i];
        if( q == sym ){  return true;  }
    }
    return false;
}


bool p_reserved( const string& q ){
    // Return True if `q` matches a symbol, otherwise return False
    for( const string& sym : RESERVED ){  if( q == sym ){  return true;  }  }
    return false;
}


bool p_prim_type( const string& q ){
    // Return True if `q` matches a primtive type name, otherwise return False
    for( const string& typ : PRIM_TYPE_NAMES ){  if( q == typ ){  return true;  }  }
    return false;
}


bool p_symbol( const string& q ){  return p_special(q) || p_reserved(q);  }


bool p_letter( const char& q ){
    // Return True if `q` matches a letter, otherwise return False
    for( const char& ltr : LETTERS ){  if( q == ltr ){  return true;  }  }
    return false;
}


bool p_digit( const char& q ){
    // Return True if `q` matches a digit, otherwise return False
    for( const char& dgt : DIGITS ){  if( q == dgt ){  return true;  }  }
    return false;
}


bool p_identifier( const string& q ){
    // Return True for strings that are: 1. Not symbols, 2. Begin with a letter, 3. Are composed of alphanumeric chars
    if( p_special(q) ){  return false;  }
    if( p_reserved(q) ){  return false;  }
    if( !p_letter( q[0] ) ){  return false;  }
    for( const char& ch : q ){
        if( (!p_letter( ch ))&&(!p_digit( ch )) ){  return false;  }
    }
    return true;
}




bool p_string_token( const string& q ){
    // Return True if the string represents a string token, otherwise return False
    size_t N = q.size();
    if( N < 2 ){  return false;  }
    if((q[0] == '\'') && (q[N-1] == '\'')){  return true;  };
    return false;
}


bool p_str_has( const string& str, const string& q ){  return (str.find(q) != string::npos);  }


string strip( const string& inputStr ){
    // Remove whitespace from the beginning and end of a string
    // Author: Kemin Zhou, https://stackoverflow.com/a/36169979
    if( inputStr.empty() ){  return inputStr;  }
    string::size_type i,j;
    i = 0;
    while( i < inputStr.size() && isspace( inputStr[i]) ){  ++i;  }
    if( i == inputStr.size() ){  return string();  } // empty string
    j = inputStr.size() - 1;
    while( isspace( inputStr[j] ) ){  --j;  }
    return inputStr.substr( i, j-i+1 );
}


size_t q_line_begins_comment( const string& q ){
    // Does the stripped line begin a comment?
    string stripped = strip(q);
    cout << "Stripped Line: \"" << stripped << "\", " << stripped.size() << endl;
    size_t rtnIndex = string::npos;
    rtnIndex = stripped.find( COMMENT_OPEN[0], 0 );
    if( rtnIndex != string::npos ){  
        cout << "Comment BEGIN!: " << rtnIndex << endl;
        return rtnIndex;  
    }
    rtnIndex = stripped.find( COMMENT_OPEN[1], 0 );
    if( rtnIndex != string::npos ){  
        cout << "Comment BEGIN!: " << rtnIndex << endl;
        return rtnIndex;  
    }
    return string::npos;
}


size_t q_line_ends_comment( const string& q ){
    // Does the stripped line end a comment?
    string stripped = strip(q);
    size_t rtnIndex = string::npos;
    rtnIndex = stripped.find( COMMENT_CLOS[0], 0 );
    if( rtnIndex != string::npos ){  return rtnIndex + COMMENT_CLOS[0].size();  }
    rtnIndex = stripped.find( COMMENT_CLOS[1], 0 );
    if( rtnIndex != string::npos ){  return rtnIndex + COMMENT_CLOS[1].size();  }
    return string::npos;
}


array<size_t,2> q_str_has_symbol( const string& q ){
    // Find a symbol in the string, if it exists
    array<size_t,2> rtnIndex = {string::npos, 0};
    for( const string& sym : SYMBOLS ){
        rtnIndex[0] = q.find( sym );
        rtnIndex[1] = sym.size();
        if( rtnIndex[0] != string::npos ){  return rtnIndex;  }
    }
    return {string::npos, 0};
}


vstr attempt_reserved_symbol_merge( const vstr& tokens ){
    // Handle the case when reserved symbols are substrings of each other (2 tokens only!)
    vstr /*----*/ rtnTokens;
    deque<string> accumQue;
    string /*--*/ accumStr = "";
    string /*--*/ validStr = "";

    for( const string& token : tokens ){
        // while( accumQue.size() > 1 ){  accumQue.pop_front();  }
        accumQue.push_back( token );
        // cout << '\t' << accumQue;
        if( accumQue.size() >= 2 ){  
            accumStr = accumQue[0] + accumQue[1];  
            if( p_special( accumStr ) ){
                rtnTokens.push_back( accumStr );
                accumQue.clear();
            }else{
                rtnTokens.push_back( accumQue[0] );
                accumQue.pop_front();
            }
        }
        // cout << " : " << rtnTokens << endl;
    }
    while( accumQue.size() ){  
        rtnTokens.push_back( accumQue[0] );
        accumQue.pop_front();  
    }
    return rtnTokens;
}


vstr tokenize( const string& totStr, char sepChr ){
    // Return a vector of tokenized strings that a separated by whitespace within `expStr`
    vstr   tokens;
    char   c;
    string cStr;
    string token  = "";
    bool   sepWs  = (sepChr == ' ');
    string expStr = totStr + sepChr; // Apply the postfix hack
    bool   strAccum = false;
    string strToken = "";

    // Helpers //
    auto stow_token = [&]{  tokens.push_back( token );  token = "";  };
    auto stow_char  = [&]{  tokens.push_back( string(1,c) );  };
    auto cache_char = [&]{  token.push_back( c );  };

    // 1. For every character in the string
    for( char ch_i : expStr ){
        // 2. Fetch character
        c    = ch_i;
        cStr = string(1,c);
        // 3. Either add char to present token or create a new one
        // A. Case String Begin/End
        if( cStr == "'" ){
            strToken += '\'';
            if( strAccum ){
                tokens.push_back( strToken );
                strToken = "";
            }
            strAccum = !strAccum;

        // B. Case Reserved
        }else if( p_symbol( cStr ) ){  
            // cout << "Reserved Token: " << find_reserved_token( cStr ) << endl;
            if( token.size() > 0 ){  stow_token();  }
            stow_char();  
        // C. Case separator
        }else if( (c == sepChr) || (sepWs && isspace(c)) ){
            if(token.size() > 0){  stow_token();  }
        // D. Case any other char
        }else{  cache_char();  } 
    }
    // N. Return the vector of tokens
    return attempt_reserved_symbol_merge( tokens );
}


vvstr segment_statements( const vstr& tokens_ ){
    // Separate a token vector into a vector of token vectors, each representing a statement (';' included!)
    vector<vstr> statements;
    vstr /*---*/ statement;
    vstr /*---*/ tokens = tokens_;
    tokens.push_back( ";" ); // Terminator Hack
    for( const string& token : tokens ){
        statement.push_back( token );
        if( token == ";" ){
            if( statement.size() > 1 ){
                statements.push_back( statement );
                statement.clear();
            }
        }
    }
    return statements;
}


vvstr text_block_to_tokenized_statements( const string& textBlock ){
    // Render a large block of text as a series of statements, each made of string tokens
    return segment_statements( tokenize( textBlock ) );
}

string concat( const vstr& parts, char sepChar ){
    // Concat the elements of a string vector
    string rtnStr = "";
    size_t N = parts.size();
    size_t i = 0;
    for( const string& part : parts ){
        rtnStr += part;
        if( (sepChar > 0)&&(i<(N-1)) ){  rtnStr += sepChar;  }
    }
    return rtnStr;
}





///// Lex Primitives //////////////////////////////////////////////////////
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


P_Val str_2_number( const string& q ){
    // Return interpret `q` as a primitive literal and return it
    /// Handle `long` ///
    try {
        return P_Val{ stol(q) };
    }catch( const std::invalid_argument& e ){
        cerr << "Invalid argument: " << e.what() << endl;
    } catch( const std::out_of_range& e ){
        cerr << "Out of range: " << e.what() << endl;
    }
    /// Handle `double` ///
    try {
        return P_Val{ stod(q) };
    }catch( const std::invalid_argument& e ){
        cerr << "Invalid argument: " << e.what() << endl;
    }catch( const std::out_of_range& e ){
        cerr << "Out of range: " << e.what() << endl;
    }
    /// Handle NaN ///
    return P_Val{ nan("") };
};
    


////////// TOKEN LEXING ////////////////////////////////////////////////////////////////////////////

bool p_has_balanced_parenthetical( const vstr& tokens ){
    // Get parentheses contents
    vstr contents;
    size_t open = 0;
    size_t clos = 0;
    for( const string& token : tokens ){
        if( token == "(" ){  ++open;  }
        else if( token == ")" ){  ++clos;  }
    }
    return ((open > 0)&&(open == clos));
}


bool p_has_balanced_bracketed( const vstr& tokens ){
    // Get parentheses contents
    vstr contents;
    size_t open = 0;
    size_t clos = 0;
    for( const string& token : tokens ){
        if( token == "[" ){  ++open;  }
        else if( token == "]" ){  ++clos;  }
    }
    return ((open > 0)&&(open == clos));
}


vstr get_balanced_parenthetical_tokens( const vstr& tokens ){
    // Get parentheses contents
    vstr   contents;
    bool   inside  = false;
    size_t depth = 0;
    for( const string& token : tokens ){
        if( inside ){  contents.push_back( token );  }
        if( token == "(" ){  
            ++depth;  
            if( !inside ){  inside = true;  }
        }else if( token == ")" ){  
            --depth;  
            if( depth == 0 ){  inside = false;  }
        }
    }
    return contents;
}


vstr get_balanced_bracketed_tokens( const vstr& tokens ){
    // Get square brackets contents
    vstr   contents;
    bool   inside  = false;
    size_t depth = 0;
    for( const string& token : tokens ){
        if( inside ){  contents.push_back( token );  }
        if( token == "[" ){  
            ++depth;  
            if( !inside ){  inside = true;  }
        }else if( token == "]" ){  
            --depth;  
            if( depth == 0 ){  inside = false;  }
        }
    }
    return contents;
}



