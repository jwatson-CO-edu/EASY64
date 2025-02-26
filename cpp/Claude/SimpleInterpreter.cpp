// Source: https://claude.site/artifacts/cca400dc-69cd-439d-b389-52d95d2b37b7

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <cctype>
#include <stdexcept>

// Token types for lexical analysis
enum class TokenType {
    NUMBER,
    IDENTIFIER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    PRINT,
    IF,
    ELSE,
    WHILE,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    EQUAL,
    NOTEQUAL,
    LESS,
    GREATER,
    END
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    
    Token(TokenType t, const std::string& v = "") : type(t), value(v) {}
};

// Lexer class to convert source code into tokens
class Lexer {
public:
    Lexer(const std::string& input) : source(input), position(0), line(1) {}
    
    Token getNextToken() {
        skipWhitespace();
        
        if (position >= source.length()) {
            return Token(TokenType::END);
        }
        
        char current = source[position];
        
        // Handle numbers
        if (isdigit(current)) {
            return handleNumber();
        }
        
        // Handle identifiers and keywords
        if (isalpha(current) || current == '_') {
            return handleIdentifier();
        }
        
        // Handle operators and other characters
        switch (current) {
            case '+': position++; return Token(TokenType::PLUS);
            case '-': position++; return Token(TokenType::MINUS);
            case '*': position++; return Token(TokenType::MULTIPLY);
            case '/': position++; return Token(TokenType::DIVIDE);
            case '=': 
                position++;
                if (position < source.length() && source[position] == '=') {
                    position++;
                    return Token(TokenType::EQUAL);
                }
                return Token(TokenType::ASSIGN);
            case ';': position++; return Token(TokenType::SEMICOLON);
            case '(': position++; return Token(TokenType::LPAREN);
            case ')': position++; return Token(TokenType::RPAREN);
            case '{': position++; return Token(TokenType::LBRACE);
            case '}': position++; return Token(TokenType::RBRACE);
            case '<': position++; return Token(TokenType::LESS);
            case '>': position++; return Token(TokenType::GREATER);
            case '!':
                position++;
                if (position < source.length() && source[position] == '=') {
                    position++;
                    return Token(TokenType::NOTEQUAL);
                }
                throw std::runtime_error("Unexpected character: !");
            default:
                throw std::runtime_error("Unexpected character: " + std::string(1, current));
        }
    }
    
private:
    std::string source;
    size_t position;
    int line;
    
    void skipWhitespace() {
        while (position < source.length() && 
               (source[position] == ' ' || source[position] == '\t' || 
                source[position] == '\n' || source[position] == '\r')) {
            if (source[position] == '\n') {
                line++;
            }
            position++;
        }
    }
    
    Token handleNumber() {
        std::string number;
        while (position < source.length() && isdigit(source[position])) {
            number += source[position++];
        }
        return Token(TokenType::NUMBER, number);
    }
    
    Token handleIdentifier() {
        std::string identifier;
        while (position < source.length() && 
               (isalnum(source[position]) || source[position] == '_')) {
            identifier += source[position++];
        }
        
        // Check for keywords
        if (identifier == "print") {
            return Token(TokenType::PRINT);
        } else if (identifier == "if") {
            return Token(TokenType::IF);
        } else if (identifier == "else") {
            return Token(TokenType::ELSE);
        } else if (identifier == "while") {
            return Token(TokenType::WHILE);
        }
        
        return Token(TokenType::IDENTIFIER, identifier);
    }
};

// AST node base class
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual int evaluate(std::map<std::string, int>& variables) = 0;
};

// Number node
class NumberNode : public ASTNode {
public:
    NumberNode(int val) : value(val) {}
    
    int evaluate(std::map<std::string, int>& variables) override {
        return value;
    }
    
private:
    int value;
};

// Variable node
class VariableNode : public ASTNode {
public:
    VariableNode(const std::string& name) : varName(name) {}
    
    int evaluate(std::map<std::string, int>& variables) override {
        if (variables.find(varName) == variables.end()) {
            variables[varName] = 0; // Initialize undefined variables to 0
        }
        return variables[varName];
    }
    
    const std::string& getName() const {
        return varName;
    }
    
private:
    std::string varName;
};

// Binary operation node
class BinaryOpNode : public ASTNode {
public:
    BinaryOpNode(ASTNode* left, TokenType op, ASTNode* right)
        : left(left), operation(op), right(right) {}
    
    ~BinaryOpNode() override {
        delete left;
        delete right;
    }
    
    int evaluate(std::map<std::string, int>& variables) override {
        int leftValue = left->evaluate(variables);
        int rightValue = right->evaluate(variables);
        
        switch (operation) {
            case TokenType::PLUS: return leftValue + rightValue;
            case TokenType::MINUS: return leftValue - rightValue;
            case TokenType::MULTIPLY: return leftValue * rightValue;
            case TokenType::DIVIDE:
                if (rightValue == 0) {
                    throw std::runtime_error("Division by zero");
                }
                return leftValue / rightValue;
            case TokenType::EQUAL: return leftValue == rightValue;
            case TokenType::NOTEQUAL: return leftValue != rightValue;
            case TokenType::LESS: return leftValue < rightValue;
            case TokenType::GREATER: return leftValue > rightValue;
            default:
                throw std::runtime_error("Unknown binary operation");
        }
    }
    
private:
    ASTNode* left;
    TokenType operation;
    ASTNode* right;
};

// Assignment node
class AssignNode : public ASTNode {
public:
    AssignNode(const std::string& name, ASTNode* expr)
        : varName(name), expression(expr) {}
    
    ~AssignNode() override {
        delete expression;
    }
    
    int evaluate(std::map<std::string, int>& variables) override {
        int value = expression->evaluate(variables);
        variables[varName] = value;
        return value;
    }
    
private:
    std::string varName;
    ASTNode* expression;
};

// Print node
class PrintNode : public ASTNode {
public:
    PrintNode(ASTNode* expr) : expression(expr) {}
    
    ~PrintNode() override {
        delete expression;
    }
    
    int evaluate(std::map<std::string, int>& variables) override {
        int value = expression->evaluate(variables);
        std::cout << value << std::endl;
        return value;
    }
    
private:
    ASTNode* expression;
};

// Block node (for statement lists)
class BlockNode : public ASTNode {
public:
    ~BlockNode() override {
        for (ASTNode* statement : statements) {
            delete statement;
        }
    }
    
    void addStatement(ASTNode* statement) {
        statements.push_back(statement);
    }
    
    int evaluate(std::map<std::string, int>& variables) override {
        int lastValue = 0;
        for (ASTNode* statement : statements) {
            lastValue = statement->evaluate(variables);
        }
        return lastValue;
    }
    
private:
    std::vector<ASTNode*> statements;
};

// If-Else node
class IfNode : public ASTNode {
public:
    IfNode(ASTNode* cond, ASTNode* thenBranch, ASTNode* elseBranch = nullptr)
        : condition(cond), thenBlock(thenBranch), elseBlock(elseBranch) {}
    
    ~IfNode() override {
        delete condition;
        delete thenBlock;
        if (elseBlock) delete elseBlock;
    }
    
    int evaluate(std::map<std::string, int>& variables) override {
        if (condition->evaluate(variables)) {
            return thenBlock->evaluate(variables);
        } else if (elseBlock) {
            return elseBlock->evaluate(variables);
        }
        return 0;
    }
    
private:
    ASTNode* condition;
    ASTNode* thenBlock;
    ASTNode* elseBlock;
};

// While node
class WhileNode : public ASTNode {
public:
    WhileNode(ASTNode* cond, ASTNode* body)
        : condition(cond), loopBody(body) {}
    
    ~WhileNode() override {
        delete condition;
        delete loopBody;
    }
    
    int evaluate(std::map<std::string, int>& variables) override {
        int lastValue = 0;
        while (condition->evaluate(variables)) {
            lastValue = loopBody->evaluate(variables);
        }
        return lastValue;
    }
    
private:
    ASTNode* condition;
    ASTNode* loopBody;
};

// Parser class to convert tokens into an AST
class Parser {
public:
    Parser(Lexer& lexer) : lexer(lexer) {
        currentToken = lexer.getNextToken();
    }
    
    ASTNode* parse() {
        return parseProgram();
    }
    
private:
    Lexer& lexer;
    Token currentToken;
    
    void consume(TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        } else {
            throw std::runtime_error("Unexpected token: expected " + 
                                    std::to_string(static_cast<int>(type)) + 
                                    ", got " + 
                                    std::to_string(static_cast<int>(currentToken.type)));
        }
    }
    
    ASTNode* parseProgram() {
        BlockNode* program = new BlockNode();
        
        while (currentToken.type != TokenType::END) {
            program->addStatement(parseStatement());
        }
        
        return program;
    }
    
    ASTNode* parseStatement() {
        ASTNode* statement = nullptr;
        
        if (currentToken.type == TokenType::PRINT) {
            consume(TokenType::PRINT);
            consume(TokenType::LPAREN);
            ASTNode* expr = parseExpression();
            consume(TokenType::RPAREN);
            consume(TokenType::SEMICOLON);
            statement = new PrintNode(expr);
        } else if (currentToken.type == TokenType::IF) {
            consume(TokenType::IF);
            consume(TokenType::LPAREN);
            ASTNode* condition = parseExpression();
            consume(TokenType::RPAREN);
            ASTNode* thenBranch = parseBlock();
            
            ASTNode* elseBranch = nullptr;
            if (currentToken.type == TokenType::ELSE) {
                consume(TokenType::ELSE);
                elseBranch = parseBlock();
            }
            
            statement = new IfNode(condition, thenBranch, elseBranch);
        } else if (currentToken.type == TokenType::WHILE) {
            consume(TokenType::WHILE);
            consume(TokenType::LPAREN);
            ASTNode* condition = parseExpression();
            consume(TokenType::RPAREN);
            ASTNode* body = parseBlock();
            
            statement = new WhileNode(condition, body);
        } else if (currentToken.type == TokenType::LBRACE) {
            statement = parseBlock();
        } else if (currentToken.type == TokenType::IDENTIFIER) {
            std::string varName = currentToken.value;
            consume(TokenType::IDENTIFIER);
            
            if (currentToken.type == TokenType::ASSIGN) {
                consume(TokenType::ASSIGN);
                ASTNode* expr = parseExpression();
                consume(TokenType::SEMICOLON);
                statement = new AssignNode(varName, expr);
            } else {
                throw std::runtime_error("Expected assignment after identifier");
            }
        } else {
            throw std::runtime_error("Unexpected token in statement");
        }
        
        return statement;
    }
    
    ASTNode* parseBlock() {
        BlockNode* block = new BlockNode();
        
        consume(TokenType::LBRACE);
        while (currentToken.type != TokenType::RBRACE) {
            block->addStatement(parseStatement());
        }
        consume(TokenType::RBRACE);
        
        return block;
    }
    
    ASTNode* parseExpression() {
        return parseComparison();
    }
    
    ASTNode* parseComparison() {
        ASTNode* left = parseAdditive();
        
        while (currentToken.type == TokenType::EQUAL || 
               currentToken.type == TokenType::NOTEQUAL ||
               currentToken.type == TokenType::LESS ||
               currentToken.type == TokenType::GREATER) {
            TokenType op = currentToken.type;
            consume(op);
            ASTNode* right = parseAdditive();
            left = new BinaryOpNode(left, op, right);
        }
        
        return left;
    }
    
    ASTNode* parseAdditive() {
        ASTNode* left = parseMultiplicative();
        
        while (currentToken.type == TokenType::PLUS || 
               currentToken.type == TokenType::MINUS) {
            TokenType op = currentToken.type;
            consume(op);
            ASTNode* right = parseMultiplicative();
            left = new BinaryOpNode(left, op, right);
        }
        
        return left;
    }
    
    ASTNode* parseMultiplicative() {
        ASTNode* left = parsePrimary();
        
        while (currentToken.type == TokenType::MULTIPLY || 
               currentToken.type == TokenType::DIVIDE) {
            TokenType op = currentToken.type;
            consume(op);
            ASTNode* right = parsePrimary();
            left = new BinaryOpNode(left, op, right);
        }
        
        return left;
    }
    
    ASTNode* parsePrimary() {
        if (currentToken.type == TokenType::NUMBER) {
            int value = std::stoi(currentToken.value);
            consume(TokenType::NUMBER);
            return new NumberNode(value);
        } else if (currentToken.type == TokenType::IDENTIFIER) {
            std::string varName = currentToken.value;
            consume(TokenType::IDENTIFIER);
            return new VariableNode(varName);
        } else if (currentToken.type == TokenType::LPAREN) {
            consume(TokenType::LPAREN);
            ASTNode* expr = parseExpression();
            consume(TokenType::RPAREN);
            return expr;
        } else {
            throw std::runtime_error("Unexpected token in expression");
        }
    }
};

// Interpreter class to execute the program
class Interpreter {
public:
    void interpret(const std::string& source) {
        try {
            Lexer lexer(source);
            Parser parser(lexer);
            
            ASTNode* program = parser.parse();
            program->evaluate(variables);
            
            delete program;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
    void interpretFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        interpret(buffer.str());
    }
    
private:
    std::map<std::string, int> variables;
};

int main(int argc, char* argv[]) {
    Interpreter interpreter;
    
    if (argc > 1) {
        // Run from file
        interpreter.interpretFile(argv[1]);
    } else {
        // Interactive mode
        std::cout << "Simple Interpreter (Enter 'exit' to quit)" << std::endl;
        
        std::string line;
        std::string program;
        
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, line);
            
            if (line == "exit") {
                break;
            }
            
            program += line + "\n";
            
            // Check for balanced braces to determine if a complete statement has been entered
            int balance = 0;
            bool complete = true;
            
            for (char c : program) {
                if (c == '{') balance++;
                else if (c == '}') balance--;
                
                if (balance < 0) {
                    complete = false;
                    break;
                }
            }
            
            if (complete && balance == 0 && !program.empty()) {
                interpreter.interpret(program);
                program.clear();
            }
        }
    }
    
    return 0;
}