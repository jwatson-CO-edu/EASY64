////////// INIT ////////////////////////////////////////////////////////////////////////////////////
///// Imports /////////////////////////////////////////////////////////////
#include "utils.hpp"



////////// JSON ////////////////////////////////////////////////////////////////////////////////////


/// Setters ///
void JSON::set( bool   val ){  leaf = true;  value = (val ? "true" : "false");  }
void JSON::set( double val ){  leaf = true;  value = to_string( val ); /*----*/ }
void JSON::set( llong  val ){  leaf = true;  value = to_string( val ); /*----*/ }
void JSON::set( size_t val ){  leaf = true;  value = to_string( val ); /*----*/ }
void JSON::set( string val ){  leaf = true;  value = val; /*-----------------*/ }

void JSON::set_indent( size_t ndnt ){  
    indent = ndnt;  
    for( pair<string,JSON> item : dict ){  item.second.set_indent( ndnt );  }
}

void JSON::unset(){  leaf = false;  value = uuid::generate_uuid_v4();  }

void JSON::add_child( const string& key, const JSON& val ){  if( leaf ) unset();  dict[ key ] = val;  } // Nest some data

string JSON::as_string( size_t depth = 0 ) const {
    // Return a `string` representation
    string rtnStr = "";
    if( leaf ){
        rtnStr = string( depth*indent, ' ') + value;
    }else{
        rtnStr += string( depth*indent, ' ') + "{\n";
        for( const pair<string,JSON>& item : dict ){
            rtnStr += string( depth*indent, ' ') + item.first + " : " + item.second.as_string( depth+1 ) + ",\n";
        }
        rtnStr += string( depth*indent, ' ') + "},\n";
    }
    return rtnStr;
}

ostream& JSON::operator<<( ostream& os ){
    // Send `string` representation to `stdout`
    os << as_string(0);
    return os;
}

