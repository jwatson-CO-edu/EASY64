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
    if( p_primitive_string( token ) ){  return str_2_primitive( token );  }
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
    bool srchConst = constants.p_var_name( name );
    bool srchVars  = vars.p_var_name( name );
    if( srchConst ){  return constants.prim[ name ];  }
    if( srchVars ){   return vars.prim[ name ];       }
    return make_nan();
}



////////// TYPE DEFINITION PART ////////////////////////////////////////////////////////////////////
void define_types( Context& context, const string& defText ){
    // Specify aliases and user-defined types
    vvstr  typStatements = text_block_to_tokenized_statements( defText );
    string name, pName;
    P_Val  bgnRange, endRange;
    vstr   expr;
    size_t span;
    bool   accum = false;
    vvstr  mltExp;
    Record rec;
    cout << "Types:" << endl << typStatements << endl;
    for( const vstr& statement : typStatements ){

        cout << "Processing: " << statement << endl;

        if( accum ){
            vec_extend( expr, statement );
            if( p_vec_has( statement, string{"end"} ) ){  accum = false;  }

            if( !accum ){
                /// Handle Record End ///
                if( p_vec_has( expr, string{"record"} ) ){
                    expr   = vec_ltrim( expr, 1 ); // Remove "record"
                    expr   = vec_rtrim( expr, 2 ); // Remove ["end", ";",]
                    mltExp = segment_statements( expr );
                    cout << mltExp << endl;
                    for( const vstr& sttmnt_j : mltExp ){
                        if( context.types.resolve_prim_alias( sttmnt_j[2] ).size() ){
                            rec = Record{};
                            rec.pVars[ sttmnt_j[0] ] = make_nan();
                            context.types.record[ name ] = rec;
                        }
                    }
                }
            }

        }else if( p_vec_has( statement, string{"="} ) ){ // WARNING, ASSUMPTION: ALL TYPEDEFS CONTAIN AN '='
            name = statement[0];
            expr = vec_ltrim( statement, 2 );

            /// Handle Range ///
            if( p_vec_has( expr, string{".."} ) ){
                bgnRange = context.types.get_var_or_literal( expr[0] );
                endRange = context.types.get_var_or_literal( expr[2] );
                if( (!p_nan( bgnRange ))&&(!p_nan( endRange )) ){
                    context.types.valrange[ name ] = ValRange( bgnRange, endRange );
                }

            /// Handle Alias ///
            }else if( (expr.size() == 2) && p_prim_type( expr[0] ) ){
                context.types.pAlias[ name ] = expr[0];

            /// Handle Enum ///
            }else if( p_vec_has( expr, string{"("} ) && p_vec_has( expr, string{")"} ) ){
                context.types.namedEnum[ name ] = Enum{ get_parenthetical_tokens( expr ) };
            
            /// Handle Array ///
            }else if( p_vec_has( expr, string{"array"} ) ){
                expr = get_bracketed_tokens( expr );
                if( (expr.size() == 1)&&(context.types.p_num_range_name( expr[0] )) ){
                    span = get<long>( context.types.valrange[ expr[0] ].valMax )
                           -
                           get<long>( context.types.valrange[ expr[0] ].valMin );
                    context.types.namedArray[ name ] = Array{ span };
                }else if( expr.size() == 3 ){
                    bgnRange = context.types.get_var_or_literal( expr[0] );
                    endRange = context.types.get_var_or_literal( expr[2] );
                    span     = get<long>( endRange ) - get<long>( bgnRange );
                    context.types.namedArray[ name ] = Array{ span };    
                }else{
                    cout << "Malformed array!: " << statement << endl;
                }

            /// Handle File ///
            }else if( p_vec_has( expr, string{"file"} ) ){
                context.types.file[ name ] = P_File{};
            
            /// Handle Record Begin ///
            }else if( p_vec_has( expr, string{"record"} ) ){
                accum = true;

            /// Handle OTHER? ///
            }else{
                cout << "`define_types`, WARNING: COULD NOT PARSE THE FOLLOWING LINE:\n" << statement << endl;
            }

        }
    }
}



////////// CONSTANT DEFINITION PART ////////////////////////////////////////////////////////////////
void define_constants( Context& context, string defText ){
    // Specify values that should not change
    vvstr  conStatements = text_block_to_tokenized_statements( defText );
    string name, pName;
    P_Val  bgnRange, endRange;
    vstr   expr;
    cout << "Constants:" << endl << conStatements << endl;
    for( const vstr& statement : conStatements ){

        cout << "Processing: " << statement << endl;

        if( p_vec_has( statement, string{"="} ) ){ // WARNING, ASSUMPTION: ALL CONSTANT DEFS CONTAIN AN '='
            name = statement[0];
            expr = vec_ltrim( statement, 2 );

            /// Handle Primtive ///
            if( (expr.size() == 2) && p_prim_type( expr[0] ) ){
                context.types.pAlias[ name ] = expr[0];
            }else{
                cout << "`define_constants`, WARNING: COULD NOT PARSE THE FOLLOWING LINE:\n" << statement << endl;
            }
        }
    }
}



////////// VARIABLE DEFINITION PART ////////////////////////////////////////////////////////////////
void define_variables( Context& context, string defText ){
    vvstr    varStatements = text_block_to_tokenized_statements( defText );
    vvstr    parts;
    vstr     names, tExpr;
    string   type, errStr;
    P_Val    bgnRange, endRange;
    size_t   span, idx;
    SetString nuSetStr;

    cout << "Variables:" << endl << varStatements << endl;
    for( const vstr& statement : varStatements ){

        cout << "Processing: " << statement << endl;

        if( p_vec_has( statement, string{":"} ) ){ // WARNING, ASSUMPTION: ALL VARIABLE DEFS CONTAIN AN ':'
            parts = vec_split( statement, string{":"} );
            names = vec_remove( parts[0], string{","} );
            tExpr  = vec_remove( parts[1], string{";"} );
            cout << names << " : " << tExpr << endl;

            /// Handle Named Types ///
            if( tExpr.size() == 1 ){

                cout << "Type expression has one element: " << tExpr << endl;

                type = context.types.resolve_prim_alias( tExpr[0] );
                if( type.size() ){  
                    for( const string& name : names ){  context.vars.prim[ name ] = make_nan();  }  
                }else{
                    switch( context.types.where_name( tExpr[0] ) ){
                        case PRIMITIVE:
                        case ALIAS:
                            for( const string& name : names ){
                                context.vars.prim[ name ] = make_nan();
                            }
                            break;
                        case RANGE_VALUE:
                            for( const string& name : names ){
                                context.vars.valrange[ name ] = context.types.valrange[ tExpr[0] ].copy();
                            }
                            break;
                        case RANGE_STRING:
                            for( const string& name : names ){
                                context.vars.strrange[ name ] = context.types.strrange[ tExpr[0] ].copy();
                            }
                            break;
                        case ENUM:
                            for( const string& name : names ){
                                context.vars.namedEnum[ name ] = context.types.namedEnum[ tExpr[0] ].copy();
                            }
                            break;
                        case ARRAY:
                            for( const string& name : names ){
                                context.vars.namedArray[ name ] = context.types.namedArray[ tExpr[0] ].copy_empty();
                            }
                            break;
                        case P_FILE:
                            for( const string& name : names ){
                                context.vars.file[ name ] = P_File{};
                            }
                            break;
                        case RECORD:
                            for( const string& name : names ){
                                context.vars.record[ name ] = context.types.record[ tExpr[0] ].copy();
                            }
                            break;
                        case ERROR:
                            cerr << tExpr[0] << " is not a defined name!" << endl;
                            break;
                        default:
                            cerr << "THIS SHOULD NOT HAPPEN!\nThere was an ERROR in interpreting " << statement << "!" << endl;
                            errStr = "`define_variables`: Could not process the following!:\t" + concat( statement, ',' ) + "\n\n";
                            throw runtime_error( errStr );
                    }
                }

            /// Handle Numeric Range Variables ///
            }else if( p_vec_has( tExpr, string{".."} ) ){
                bgnRange = context.types.get_var_or_literal( tExpr[0] );
                endRange = context.types.get_var_or_literal( tExpr[2] );
                if( (!p_nan( bgnRange ))&&(!p_nan( endRange )) ){
                    for( const string& name : names ){
                        context.vars.valrange[ name ] = ValRange( bgnRange, endRange );
                    }
                }

            /// Handle Enum ///
            }else if( p_vec_has( tExpr, string{"("} ) && p_vec_has( tExpr, string{")"} ) ){
                tExpr = get_parenthetical_tokens( tExpr );
                for( const string& name : names ){
                    context.vars.namedEnum[ name ] =  Enum{ tExpr };
                }
            
            /// Handle Array ///
            }else if( p_vec_has( tExpr, string{"array"} ) ){
                tExpr = get_bracketed_tokens( tExpr );
                if( (tExpr.size() == 1)&&(context.types.p_num_range_name( tExpr[0] )) ){
                    span = get<long>( context.types.valrange[ tExpr[0] ].valMax )
                           -
                           get<long>( context.types.valrange[ tExpr[0] ].valMin );
                    for( const string& name : names ){
                        context.types.namedArray[ name ] = Array{ span };
                    }
                }else if( tExpr.size() == 3 ){
                    bgnRange = context.types.get_var_or_literal( tExpr[0] );
                    endRange = context.types.get_var_or_literal( tExpr[2] );
                    span     = get<long>( endRange ) - get<long>( bgnRange );
                    for( const string& name : names ){
                        context.types.namedArray[ name ] = Array{ span };    
                    }
                }else{
                    cout << "Malformed array!: " << statement << endl;
                }

            /// Handle File ///
            }else if( p_vec_has( tExpr, string{"file"} ) ){
                for( const string& name : names ){
                    context.types.file[ name ] = P_File{};
                }
            
            /// Handle Set ///
            }else if( p_vec_has( tExpr, string{"set"} ) && p_vec_has( tExpr, string{"of"} ) ){
                idx  = vec_find( tExpr, string{"of"} ) + 1;
                type = context.types.resolve_prim_alias( tExpr[ idx ] );
                if( type.size() ){
                    for( const string& name : names ){
                        context.types.setvalue[ name ] = SetValue{};
                    }
                }else if( context.types.p_enum_name( tExpr[ idx ] ) ){
                    for( const string& name : names ){
                        nuSetStr = SetString{};
                        nuSetStr.restrict( context.types.namedEnum[ tExpr[ idx ] ].items );
                        context.types.setstring[ name ] = nuSetStr;
                    }
                }
            }else{
                cerr << "`define_variables`, WARNING: CANNOT LEX STATEMENT: " << tExpr << "!" << endl;
                errStr = "`define_variables`: Could not process the following!:\t" + concat( tExpr, ',' ) + "\n\n";
                throw runtime_error( errStr );
            }
        }
    }
}



////////// NODE ////////////////////////////////////////////////////////////////////////////////////

enum NodeType{
    // Types of syntax nodes
    MAIN,
    FOR,
    STATEMENT,
};

map<NodeType,string> NTypeStr = {
    {MAIN, /**/ "MAIN"},
    {FOR, /*-*/ "FOR"},
    {STATEMENT, "STATEMENT"},
};

struct ST_Node{ 
    // Holds code as a tree
    NodeType /*--*/ type;
    vstr /*------*/ statement;
    vector<ST_Node> block;
};


ST_Node tokens_2_nodes( const vvstr& tokenLines, NodeType type_ = MAIN ){
    // Recursively distribute tokenized text to nodes
    ST_Node rtnNode;
    size_t idx;
    size_t i = 0;
    size_t N = tokenLines.size();
    vstr   statement, tokens;
    vvstr  blockTokens;
    bool   accum = false;
    rtnNode.type = type_;
    while( i < N ){
        tokens = tokenLines[i];
        if( accum ){
            /// Recursive Case: Block End ///
            if( p_vec_has( tokens, string{"end"} ) ){
                rtnNode.block.push_back(  tokens_2_nodes( blockTokens, FOR )  ); // FIXME: THERE ARE OTHER BLOCK TYPES!
                blockTokens.clear();
            /// Base Case: Block Accumulate ///
            }else{
                blockTokens.push_back( tokens );
            }
        /// Base Case: Block Start ///
        }else if( p_vec_has( tokens, string{"begin"} ) ){
            idx /*-*/ = vec_find( tokens, string{"begin"} );
            statement = vec_sub( tokens, 0, idx );
            accum     = true;
        /// Base Case: Normal Statement ///
        }else{
            rtnNode.block.push_back( ST_Node{ STATEMENT, tokens, vector<ST_Node>{} } );
        }
        /// Increment ///
        ++i;
    }
    return rtnNode;
}


void pprint_node( const ST_Node& node, size_t lvl = 0 ){
    // Recursively print node contents
    for( size_t i = 0; i < lvl; ++i ){  cout << '\t';  }
    cout << NTypeStr[ node.type ] << " : " << node.statement << endl;
    for( const ST_Node& n_i : node.block ){
        pprint_node( n_i, lvl+1 );
    }
}



////////// INTERPRETER /////////////////////////////////////////////////////////////////////////////


class PascalInterpreter{ public:
    // The actual interpreter

    string /*-*/ progName;
    bool /*---*/ enableInput;
    bool /*---*/ enableOutput;
    TextPortions code;

    PascalInterpreter(){};

    PascalInterpreter( const TextPortions& code_ ){
        code = code_;
    };


    void set_IO( bool in, bool out ){
        enableInput  = in;
        enableOutput = out;
    }
};
