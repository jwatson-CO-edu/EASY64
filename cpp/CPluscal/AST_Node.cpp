////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"



////////// NODE ////////////////////////////////////////////////////////////////////////////////////


AST_Node make_ast_program( const string& name ){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type   = PROGRAM;
    rtnNode.tokens = {name,};
    return rtnNode;
}


AST_Node make_ast_type_def(){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type = TYPE_DEF;
    return rtnNode;
}


AST_Node make_ast_const_def( const string& name ){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type = CONSTANT_DEF;
    return rtnNode;
}


AST_Node make_ast_var_def( const string& name ){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type = VARIABLE_DEF;
    return rtnNode;
}


AST_Node make_ast_type_name( const string& name ){
    AST_Node rtnNode{};
    rtnNode.type   = TYPE_NAME;
    rtnNode.tokens = {name,};
    return rtnNode;
}


AST_Node make_ast_identifier( const string& name ){
    // Identifier Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type   = IDENTIFIER;
    rtnNode.tokens = {name,};
    return rtnNode;
}


AST_Node make_ast_number_literal( const string& literalStr ){
    // Number Literal Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type   = NUM_LITERAL;
    rtnNode.tokens = {literalStr,};
    rtnNode.value  = str_2_number( literalStr );
    return rtnNode;
}


AST_Node make_ast_assignment( AST_Node left, AST_Node right ){
    // Number Assignment Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type  = ASSIGNMENT;
    rtnNode.edges = {left, right,};
    return rtnNode;
}


AST_Node make_ast_binary_op( const string& op, AST_Node left, AST_Node right ){
    // Number Assignment Node, to be evaluated later
    AST_Node rtnNode{};
    rtnNode.type   = BINARY_OP;
    rtnNode.tokens = {op,};
    rtnNode.edges  = {left, right,};
    return rtnNode;
}



////////// PARSER //////////////////////////////////////////////////////////////////////////////////

void AST_Parser::load_program_tokens( const vstr& tokens_ ){
    // Load tokenized program into the parser
    input.clear();
    for( const string& token : tokens_ ){  input.push_back( token );  }
    N = tokens_.size();
}


AST_Parser::AST_Parser( const vstr& tokens_ ){
    // Load tokenized program into the parser
    load_program_tokens( tokens_ );
}

// Move to next token string
bool AST_Parser::advance(){    
    if( input.size() ){
        curTkn = input.front();
        input.pop_front();
        return true;
    }else{
        curTkn = "";
        return false;
    }
    
}


bool AST_Parser::match( const string& expected ){
    // Return True if the current token string matches what is `expected`
    return (curTkn == expected);
}


void AST_Parser::expect( const string& expected ){
    // Throw a `runtime_error` if `match` fails
    if( !match( expected ) ){
        throw runtime_error("Expected '" + expected + "', got '" + curTkn + "'");
    }
}


AST_Node AST_Parser::parse_term(){
    // Parse one expression term as an AST node
    // NOTE: This function assumes that the token has been advanced to the term to be parsed

    AST_Node value;
    if( !curTkn.size() ){  throw runtime_error( "Unexpected end of input" );  }
    
    // Handle numbers
    if( p_number_string( curTkn ) ){
        value = make_ast_number_literal( curTkn );
        advance();
        return value;
    }
    
    // Handle identifiers
    if( p_identifier( curTkn ) ){
        value = make_ast_identifier( curTkn );
        advance();
        return value;
    }

    // Handle type names
    if( p_prim_type( curTkn ) ){
        value = make_ast_type_name( curTkn );
        advance();
        return value;
    }
    
    // Handle parenthesized expressions
    if( match("(") ){
        value = parse_expression();
        expect(")");
        return value;
    }

    throw runtime_error( "Unexpected token: " + curTkn );
}


AST_Node AST_Parser::parse_expression(){
    // Read tokens until an expression subtree is obtained
    // NOTE: This function assumes that the token has been advanced to the beginning of the expression to be parsed

    AST_Node left = parse_term();
    AST_Node right;
    string   op;
    
    if( match("=") ){
        right = parse_expression();  advance();
        return make_ast_assignment( left, right );
    }
    
    while( match("+") || match("-") || // WARNING: THIS DOES NOT RESPECT OPERATOR PRECEDENCE!
           match("*") || match("/") || match("**") ){
        op    = curTkn; /*--*/ advance();
        right = parse_term();  advance();
        left  = make_ast_binary_op( op, left, right ); // WARNING: THIS MAKES A LISP-STYLE LIST!
    }
    
    if( match(";") ){  cout << endl << "<Statement End>" << endl;  }
    
    return left;
}


AST_Node AST_Parser::parse( const string& name ){
    // Convert the collection of tokens to an Abstract Source Tree
    AST_Node program = make_ast_program( name );
    while( input.size() ){  program.edges.push_back(  parse_expression()  );  }
    return program;
}




