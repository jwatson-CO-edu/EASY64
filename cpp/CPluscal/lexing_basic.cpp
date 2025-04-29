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


bool p_section_header( const string& q ){ // WARNING, ASSUMPTION: SECTION HEADINGS OCCUPY THEIR OWN LINE
    // Return True if `q` matches a section name, otherwise return False
    for( const string& nam : SECTION_NAMES ){  if( q == nam ){  return true;  }  }
    return false;
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


vstr tokenize( const string& totStr, char sepChr = ' ' ){
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



////////// FILE LEXING /////////////////////////////////////////////////////////////////////////////

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


vstr p_program_header( const vstr& line ){
    // Attempt to parse program name and input / output requirements
    vstr rtnHeader, ioTerms;
    if( p_vec_has( line, string{"program"} ) ){
        try{
            rtnHeader.push_back( line[1] );
            ioTerms = get_balanced_parenthetical_tokens( line );
            if( p_vec_has( ioTerms, string{"input"}  ) ){  rtnHeader.push_back("true");  }else{  rtnHeader.push_back("false");  }
            if( p_vec_has( ioTerms, string{"output"} ) ){  rtnHeader.push_back("true");  }else{  rtnHeader.push_back("false");  }
            return rtnHeader;
        }catch(...){
            return vstr{};
        }
    }
    return vstr{};
}


TextPortions segment_source_file( const vstr& totLines ){
    // Load sections of the program into the struct so that we can lex/interpret them
    enum Section{ BEGIN, TYPE, CONST, VAR, COMMENT, STRING, OTHER };
    /**/ string /*--*/ trimLine;
    /**/ vstr /*----*/ tokenLine;
    /**/ string /*--*/ line;
    /**/ Section /*-*/ mode     = BEGIN;
    /**/ Section /*-*/ lastMode = BEGIN;
    /**/ size_t /*--*/ cmmntBgn = string::npos;
    /**/ size_t /*--*/ cmmntEnd = string::npos;
    /**/ size_t /*--*/ N /*--*/ = totLines.size();
    /**/ size_t /*--*/ i /*--*/ = 0;
    /**/ deque<string> qLines;
    /**/ TextPortions  rtnSctns;

    auto check_section_switch = [&]{  
        if( p_section_header( trimLine ) ){ // WARNING, ASSUMPTION: SECTION HEADINGS OCCUPY THEIR OWN LINE
            if( trimLine == "type" ){
                cout << endl << "segment_source_file: Defining `type`s!" << endl << endl;
                mode = TYPE;
            }else if( trimLine == "const" ){
                cout << endl << "segment_source_file: Defining `const`ants!" << endl << endl;
                mode = CONST;
            }else if( trimLine == "var" ){
                cout << endl << "segment_source_file: Defining `var`iables!" << endl << endl;
                mode = VAR;
            }
            return true;
        }else{  return false;  }
    };

    // 0. Load queue with lines
    for( const string& vLine : totLines ){  qLines.push_back( vLine );  }

    // 1. Process each line
    while( qLines.size() ){

        // 2. Fetch line
        line = qLines.at(0);
        qLines.pop_front();
        trimLine  = strip( line );
        tokenLine = tokenize( trimLine );

        // 3. Handle empty line
        if( trimLine.size() < MIN_CHAR_S ){  continue;  }

        if( mode != COMMENT ){  

            // 5. Handle comments
            cmmntBgn = q_line_begins_comment( trimLine );
            if( cmmntBgn == 0 ){  
                lastMode = mode;
                mode     = COMMENT;  
                continue;
            }else if( cmmntBgn != string::npos ){
                cout << "BEGIN comment!: " << trimLine << endl;
                qLines.push_front( trimLine.substr( cmmntBgn ) );
                qLines.push_front( trimLine.substr( 0, cmmntBgn ) );
                continue;
            }

            // 6. Handle definition part
            if( check_section_switch() ){  continue;  }  

            // 6. Handle end of program
            if( p_str_has( trimLine, string{"end."} ) ){  
                cout << "Program ENDED: " << trimLine << endl << endl;
                break;  
            }
        }

        // 7. Hande State Transitions
        switch( mode ){
            case BEGIN:  
                // 4. Handle header
                if( p_program_header( tokenLine ).size() == 3 ){  
                    rtnSctns.header = tokenLine;
                    
                    
                    = trimLine;  
                    mode /*------*/ = OTHER;
                    continue;
                }
            case OTHER:  
                // 3. Handle section headers
                rtnSctns.prog += line;  
                break;  
            case TYPE:   
                if( p_vec_has( tokenLine, string{"begin"} ) ){  mode = OTHER;  }else{
                    rtnSctns.type += line;  
                }
                break;
            case CONST:  
                if( p_vec_has( tokenLine, string{"begin"} ) ){  mode = OTHER;  }else{
                    rtnSctns.cnst += line;          
                }
                break;
            case VAR:    
                if( p_vec_has( tokenLine, string{"begin"} ) ){  mode = OTHER;  }else{
                    rtnSctns.var  += line;          
                }
                break;
            case COMMENT: 
                cmmntEnd = q_line_ends_comment( trimLine );
                cout << "COMMENT Mode, Comment end?: " << cmmntEnd << endl;
                if( cmmntEnd != string::npos ){  
                    cout << "END comment!: " << trimLine << endl;
                    qLines.push_front( trimLine.substr(cmmntEnd) );  
                    mode     = lastMode;
                    lastMode = OTHER;
                    cout << "... ending comment ..." << endl;
                }else{  cout << "... skipping comment ..." << endl;  }
                break;
            default:
                cout << "`segment_source_file`: This should NOT happen!, UNHANDLED LINE!!:" << endl << trimLine << endl;
                break;
        }
    }
    return rtnSctns;
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



