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
    size_t  idx;
    size_t  i = 0;
    size_t  N = tokenLines.size();
    vstr    statement, tokens;
    vvstr   blockTokens;
    bool    accum = false;
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