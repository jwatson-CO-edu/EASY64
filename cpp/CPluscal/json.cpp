////////// INIT ////////////////////////////////////////////////////////////////////////////////////
///// Imports /////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// JSON ////////////////////////////////////////////////////////////////////////////////////

class JSON{ public:
    // Generic, nested data structure
    bool /*-------*/ leaf;
    string /*-----*/ value;
    map<string,JSON> dict;
    size_t /*-----*/ indent;

    /// Constructors ///
    JSON(){  indent = 2;  }
    JSON( bool   val ) : JSON() {  set( val );  }
    JSON( double val ) : JSON() {  set( val );  }
    JSON( llong  val ) : JSON() {  set( val );  }
    JSON( size_t val ) : JSON() {  set( val );  }
    JSON( string val ) : JSON() {  set( val );  }

    /// Setters ///
    void set( bool   val ){  leaf = true;  value = (val ? "true" : "false");  }
    void set( double val ){  leaf = true;  value = to_string( val ); /*----*/ }
    void set( llong  val ){  leaf = true;  value = to_string( val ); /*----*/ }
    void set( size_t val ){  leaf = true;  value = to_string( val ); /*----*/ }
    void set( string val ){  leaf = true;  value = val; /*-----------------*/ }
    
    void set_indent( size_t ndnt ){  
        indent = ndnt;  
        for( pair<string,JSON> item : dict ){  item.second.set_indent( ndnt );  }
    }

    void unset(){  leaf = false;  value = uuid::generate_uuid_v4();  }

    void add_child( const string& key, const JSON& val ){  unset();  dict[ key ] = val;  } // Nest some data

    string as_string( size_t depth = 0 ) const {
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

    ostream& operator<<( ostream& os ){
        // Send `string` representation to `stdout`
        os << as_string(0);
        return os;
    }
};
