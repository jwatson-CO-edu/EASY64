////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// TYPES && SCOPE //////////////////////////////////////////////////////////////////////////
void ValStore::set_builtins(){
    // Set values that Pascal knows about
    prim["maxint"] = P_Val{ LONG_MAX };
}

ValStore::ValStore(){
    // Setup this context
    set_builtins();
}

bool ValStore::p_var_name( const string& name ){
    // Is there a variable stored under this `name`?
    if( prim.find( name ) != prim.end() ){  return true;  }else{  return false;  }
}

bool ValStore::p_alias_name( const string& name ){
    // Is there a variable stored under this `name`?
    if( pAlias.find( name ) != pAlias.end() ){  return true;  }else{  return false;  }
}

bool ValStore::p_arr_name( const string& name ){
    // Is there a variable stored under this `name`?
    if( namedArray.find( name ) != namedArray.end() ){  return true;  }else{  return false;  }
}

bool ValStore::p_num_range_name( const string& name ){
    // Is there a variable stored under this `name`?
    if( valrange.find( name ) != valrange.end() ){  return true;  }else{  return false;  }
}

bool ValStore::p_str_range_name( const string& name ){
    // Is there a variable stored under this `name`?
    if( strrange.find( name ) != strrange.end() ){  return true;  }else{  return false;  }
}

bool ValStore::p_file_name( const string& name ){
    // Is there a variable stored under this `name`?
    if( file.find( name ) != file.end() ){  return true;  }else{  return false;  }
}

bool ValStore::p_record_name( const string& name ){
    // Is there a variable stored under this `name`?
    if( record.find( name ) != record.end() ){  return true;  }else{  return false;  }
}

bool ValStore::p_enum_name( const string& name ){
    // Is there a variable stored under this `name`?
    if( namedEnum.find( name ) != namedEnum.end() ){  return true;  }else{  return false;  }
}

P_Val ValStore::get_var( const string& name ){
    // Get var by `name` if it exists, otherwise return NaN
    if( p_var_name( name ) ){  return prim[ name ];  }else{  return make_nan();  }
}

P_Val ValStore::get_var_or_literal( const string& token ){
    // If the `token` is a variable or a literal, then return its value, Otherwise return NaN
    if( p_var_name( token ) ){  return prim[ token ];  }
    if( p_number_string( token ) ){  return str_2_number( token );  }
    return make_nan();
}

string ValStore::resolve_prim_alias( const string& name ){
    // Return the primitive name associated with the alias, otherwise return an empty string
    if( p_prim_type( name ) ){  return name;  }
    if( pAlias.find( name ) != pAlias.end() ){  return pAlias[ name ];  }
    return "";
}

TypeName ValStore::where_name( const string& name ){
    // Identify what kind of thing the identifier identifies
    if( p_var_name( name )       ){  return PRIMITIVE;     }
    if( p_alias_name( name )     ){  return ALIAS;         }
    if( p_num_range_name( name ) ){  return RANGE_VALUE;   }
    if( p_str_range_name( name ) ){  return RANGE_STRING;  }
    if( p_arr_name( name )       ){  return ARRAY;         }
    if( p_file_name( name )      ){  return P_FILE;        }
    if( p_record_name( name )    ){  return RECORD;        }
    return ERROR;
}



P_Val Context::resolve_primitive_name( const string& name ){
    // Try to convert the name into a primitive literal

    cout << "`resolve_primitive_name`: " << name << " --> " 
         << constants.p_var_name( name ) << " || "
         << vars.p_var_name( name )      << " || "
         << p_number_string( name )      << endl;

    if( constants.p_var_name( name ) ){  return constants.prim[ name ];   }
    if( vars.p_var_name( name ) /**/ ){  return vars.prim[ name ]; /*--*/ }
    if( p_number_string( name ) /**/ ){  return str_2_number( name );  }
    return make_nan();
}

bool Context::p_math_expr( const vstr& expr ){
    // Do these tokens have the ingredients of a math expression?
    bool isPrim = false;
    bool isOper = false;
    bool isSemi = false;
    for( const string& token : expr ){
        isPrim = !p_nan( resolve_primitive_name( token ) );
        isOper = p_infix_op( token );
        isSemi = (token == ";");
        if( (!isPrim) && (!isOper) && (!isSemi) ){  return false;  }
    }
    return true;
}











////////// BUILT-IN FUNCTIONS //////////////////////////////////////////////////////////////////////

Array string_2_char_array( const string& str ){
    // Convert a string to a char array
    Array  rtnArr = Array{ str.size() };
    size_t i /**/ = 0;
    for( const char& ch : str ){
        rtnArr[i] = P_Val{ ch };
        ++i;
    }
    return rtnArr;
}


string char_array_2_string( Array& charArr ){
    // Convert a char array to a string
    string rtnStr = "";
    for( size_t i = 0; i < charArr.size(); ++i ){  rtnStr += get<char>( charArr[i] );  }
    return rtnStr;
}


void writeln( Array& charArr ){
    // Print a char array to stdout
    cout << char_array_2_string( charArr ) << endl;
}



////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////

PascalInterpreter::PascalInterpreter(){};

PascalInterpreter::PascalInterpreter( const Context& context_ ){
    context = context_;
};


void PascalInterpreter::set_IO( bool in, bool out ){
    enableInput  = in;
    enableOutput = out;
}


void PascalInterpreter::init_file( const string& sourcePath ){
    vstr /*---*/ fLines = read_file_to_lines( sourcePath );
    TextPortions fSeg = segment_source_file( fLines );

    // define_types(     context, fSeg.type );  cout << endl;
    // define_constants( context, fSeg.cnst );  cout << endl;
    // define_variables( context, fSeg.var  );  cout << endl;
    
}

