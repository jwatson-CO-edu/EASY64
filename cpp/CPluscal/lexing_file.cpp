////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// STRING PROCESSING ///////////////////////////////////////////////////////////////////////

bool p_section_header( const string& q ){ // WARNING, ASSUMPTION: SECTION HEADINGS OCCUPY THEIR OWN LINE
    // Return True if `q` matches a section name, otherwise return False
    for( const string& nam : SECTION_NAMES ){  if( q == nam ){  return true;  }  }
    return false;
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
                    rtnSctns.tokens.header = tokenLine;
                    mode /*------*/ = OTHER;
                    continue;
                }
            case OTHER:  
                // 3. Handle section headers
                vec_extend( rtnSctns.tokens.prog, tokenLine );  
                break;  
            case TYPE:   
                if( p_vec_has( tokenLine, string{"begin"} ) ){  mode = OTHER;  }else{
                    vec_extend( rtnSctns.tokens.type, tokenLine );  
                }
                break;
            case CONST:  
                if( p_vec_has( tokenLine, string{"begin"} ) ){  mode = OTHER;  }else{
                    vec_extend( rtnSctns.tokens.cnst, tokenLine );
                }
                break;
            case VAR:    
                if( p_vec_has( tokenLine, string{"begin"} ) ){  mode = OTHER;  }else{
                    vec_extend( rtnSctns.tokens.var, tokenLine );
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
