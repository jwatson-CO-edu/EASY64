////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "include/CPluscal.hpp"



////////// STRING PROCESSING ///////////////////////////////////////////////////////////////////////

size_t vstr_find_index( const vstr& vec, const string& q ){  return vec_find_index( vec, string{q} );  }

std::ostream& operator<<( std::ostream& os , vvstr arr ){ 
    // Print string vector vector
	os << "[\n";
    for( const vstr& row : arr ){
        os << "\t[\n";
        for( const string& str : row ){
            os << "\t\t" << str << ",\n";
        }
        os << "\t]\n";
    }
    os << "]";
	return os;
}

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

vstr attempt_reserved_symbol_merge( const vstr& tokens ){
    // Handle the case when reserved symbols are substrings of each other (2 tokens only!)
    vstr /*----*/ rtnTokens;
    deque<string> accumQue;
    string /*--*/ accumStr = "";
    string /*--*/ validStr = "";
    for( const string& token : tokens ){
        accumQue.push_back( token );
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
            strToken += "'";
            if( strAccum ){  
                tokens.push_back( strToken );  
            }
            strAccum = !strAccum;
        }else if( strAccum ){
            strToken += cStr;

        // B. Case Reserved
        }else if( p_symbol( cStr ) ){  
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



////////// FILE OPERATIONS /////////////////////////////////////////////////////////////////////////

vstr read_file_to_lines( string path ){
    // Read plain text to a vector of strings, separated by newlines
    ifstream     inputFile;
    stringstream buffer;
    string /*-*/ errStr = "Could not open ";
    string /*-*/ line, addLine;
    vstr /*---*/ totLines;
    inputFile.open( path );
    if( !inputFile.is_open() ){
        cerr << "Error opening " << path << "!" << endl;
        errStr += path;
        inputFile.close();
        throw runtime_error( errStr ); // Exit with an error code
    }else{
        buffer << inputFile.rdbuf(); // https://stackoverflow.com/a/2602258
        while( getline( buffer, line, '\n' ) ){ // https://stackoverflow.com/a/13172579
            addLine = line + '\n';
            totLines.push_back( addLine );
        }
        inputFile.close();
    }
    return totLines;
}


string first_existing_file_path( const vstr& possiblePaths ){
    // Return the first existing file path, Otherwise return an empty string if none exist
    for( const string& pPath : possiblePaths ){  if( exists( pPath ) ){  return pPath;  }  }
    return "";
}



////////// LEXER ///////////////////////////////////////////////////////////////////////////////////

LexMachine::LexMachine(){} // Empty Constructor

LexMachine::LexMachine( string fPath ){
    // Init object, Load and Lex Pascal file
    progPath = fPath;
    lines    = read_file_to_lines( progPath );
    vstr tknLin;
    for( const string& line : lines ){  
        tknLin = tokenize( line );
        if( tknLin.size() ){  lineTokens.push_back( tknLin );  }
    }
    // cout << lineTokens << endl;
}