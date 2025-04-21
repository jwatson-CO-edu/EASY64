#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <unordered_map>

// AST Node Types
enum class NodeType {
    Program,
    BinaryExpression,
    NumericLiteral,
    Identifier,
    AssignmentExpression,
    VariableDeclaration
};

// Base AST Node
class ASTNode {
public:
    explicit ASTNode(NodeType type) : type(type) {}
    virtual ~ASTNode() = default;
    
    NodeType getType() const { return type; }
    virtual void print(int indent = 0) const = 0;

protected:
    NodeType type;
    
    void printIndent(int indent) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }
};

// Numeric Literal Node
class NumericLiteralNode : public ASTNode {
public:
    explicit NumericLiteralNode(double value) 
        : ASTNode(NodeType::NumericLiteral), value(value) {}
    
    double getValue() const { return value; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "NumericLiteral: " << value << std::endl;
    }
    
private:
    double value;
};

// Identifier Node
class IdentifierNode : public ASTNode {
public:
    explicit IdentifierNode(const std::string& name) 
        : ASTNode(NodeType::Identifier), name(name) {}
    
    const std::string& getName() const { return name; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "Identifier: " << name << std::endl;
    }
    
private:
    std::string name;
};

// Binary Expression Node
class BinaryExpressionNode : public ASTNode {
public:
    BinaryExpressionNode(const std::string& op, 
                        std::unique_ptr<ASTNode> left, 
                        std::unique_ptr<ASTNode> right)
        : ASTNode(NodeType::BinaryExpression), 
          op(op), 
          left(std::move(left)), 
          right(std::move(right)) {}
    
    const std::string& getOperator() const { return op; }
    const ASTNode* getLeft() const { return left.get(); }
    const ASTNode* getRight() const { return right.get(); }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "BinaryExpression: " << op << std::endl;
        left->print(indent + 1);
        right->print(indent + 1);
    }
    
private:
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
};

// Assignment Expression Node
class AssignmentExpressionNode : public ASTNode {
public:
    AssignmentExpressionNode(std::unique_ptr<ASTNode> left, 
                           std::unique_ptr<ASTNode> right)
        : ASTNode(NodeType::AssignmentExpression), 
          left(std::move(left)), 
          right(std::move(right)) {}
    
    const ASTNode* getLeft() const { return left.get(); }
    const ASTNode* getRight() const { return right.get(); }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "AssignmentExpression: =" << std::endl;
        left->print(indent + 1);
        right->print(indent + 1);
    }
    
private:
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
};

// Variable Declaration Node
class VariableDeclarationNode : public ASTNode {
public:
    VariableDeclarationNode(const std::string& varName, 
                          std::unique_ptr<ASTNode> initializer = nullptr)
        : ASTNode(NodeType::VariableDeclaration), 
          varName(varName), 
          initializer(std::move(initializer)) {}
    
    const std::string& getVarName() const { return varName; }
    const ASTNode* getInitializer() const { return initializer.get(); }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "VariableDeclaration: " << varName << std::endl;
        if (initializer) {
            initializer->print(indent + 1);
        }
    }
    
private:
    std::string varName;
    std::unique_ptr<ASTNode> initializer;
};

// Program Node (Root)
class ProgramNode : public ASTNode {
public:
    ProgramNode() : ASTNode(NodeType::Program) {}
    
    void addStatement(std::unique_ptr<ASTNode> statement) {
        statements.push_back(std::move(statement));
    }
    
    const std::vector<std::unique_ptr<ASTNode>>& getStatements() const {
        return statements;
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "Program:" << std::endl;
        for (const auto& statement : statements) {
            statement->print(indent + 1);
        }
    }
    
private:
    std::vector<std::unique_ptr<ASTNode>> statements;
};

// Parser class
class Parser {
public:
    explicit Parser(const std::vector<std::string>& tokens) 
        : tokens(tokens), currentTokenIndex(0) {}
    
    std::unique_ptr<ProgramNode> parse() {
        auto program = std::make_unique<ProgramNode>();
        
        while (currentTokenIndex < tokens.size()) {
            program->addStatement(parseStatement());
        }
        
        return program;
    }
    
private:
    const std::vector<std::string>& tokens;
    size_t currentTokenIndex;
    
    std::string currentToken() const {
        if (currentTokenIndex < tokens.size()) {
            return tokens[currentTokenIndex];
        }
        return "";
    }
    
    void advance() {
        if (currentTokenIndex < tokens.size()) {
            currentTokenIndex++;
        }
    }
    
    bool match(const std::string& expected) {
        if (currentToken() == expected) {
            advance();
            return true;
        }
        return false;
    }
    
    void expect(const std::string& expected) {
        if (!match(expected)) {
            throw std::runtime_error("Expected '" + expected + "', got '" + currentToken() + "'");
        }
    }
    
    std::unique_ptr<ASTNode> parseStatement() {
        if (match("let")) {
            return parseVariableDeclaration();
        }
        return parseExpression();
    }
    
    std::unique_ptr<ASTNode> parseVariableDeclaration() {
        if (currentToken().empty() || !isalpha(currentToken()[0])) {
            throw std::runtime_error("Expected identifier, got '" + currentToken() + "'");
        }
        
        std::string varName = currentToken();
        advance();
        
        std::unique_ptr<ASTNode> initializer = nullptr;
        if (match("=")) {
            initializer = parseExpression();
        }
        
        expect(";");
        
        return std::make_unique<VariableDeclarationNode>(varName, std::move(initializer));
    }
    
    std::unique_ptr<ASTNode> parseExpression() {
        auto left = parseTerm();
        
        if (match("=")) {
            auto right = parseExpression();
            return std::make_unique<AssignmentExpressionNode>(std::move(left), std::move(right));
        }
        
        while (match("+") || currentToken() == "-" || 
               currentToken() == "*" || currentToken() == "/") {
            std::string op = tokens[currentTokenIndex - 1];
            auto right = parseTerm();
            left = std::make_unique<BinaryExpressionNode>(op, std::move(left), std::move(right));
        }
        
        if (match(";")) {
            // End of statement
        }
        
        return left;
    }
    
    std::unique_ptr<ASTNode> parseTerm() {
        if (currentToken().empty()) {
            throw std::runtime_error("Unexpected end of input");
        }
        
        // Handle numbers
        if (isdigit(currentToken()[0])) {
            double value = std::stod(currentToken());
            advance();
            return std::make_unique<NumericLiteralNode>(value);
        }
        
        // Handle identifiers
        if (isalpha(currentToken()[0])) {
            std::string name = currentToken();
            advance();
            return std::make_unique<IdentifierNode>(name);
        }
        
        // Handle parenthesized expressions
        if (match("(")) {
            auto expr = parseExpression();
            expect(")");
            return expr;
        }
        
        throw std::runtime_error("Unexpected token: " + currentToken());
    }
};

int main() {
    // Example usage
    std::vector<std::string> tokens = {
        "let", "x", "=", "5", ";",
        "let", "y", "=", "x", "+", "10", ";",
        "x", "=", "y", "*", "2", ";"
    };
    
    Parser parser(tokens);
    try {
        auto ast = parser.parse();
        std::cout << "Successfully built AST:" << std::endl;
        ast->print();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
