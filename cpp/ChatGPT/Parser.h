#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include <iostream>
#include <memory>
#include <cctype>
#include <map>

class Parser {
    std::string Input;
    size_t Index;
    char CurTok;
    std::map<std::string, std::unique_ptr<FunctionAST>> Functions;

    char getNextChar() { return (Index < Input.size()) ? Input[Index++] : '\0'; }
    char getNextToken() { return CurTok = getNextChar(); }

    std::unique_ptr<ExprAST> parseNumber() {
        double Val = CurTok - '0';
        getNextToken();
        return std::make_unique<NumberExprAST>(Val);
    }

    std::unique_ptr<ExprAST> parsePrimary() {
        if (isdigit(CurTok)) return parseNumber();
        if (isalpha(CurTok)) {
            std::string Name;
            while (isalnum(CurTok)) { Name += CurTok; getNextToken(); }
            return std::make_unique<VariableExprAST>(Name);
        }
        return nullptr;
    }

    std::unique_ptr<ExprAST> parseExpression() {
        auto LHS = parsePrimary();
        if (!LHS) return nullptr;

        while (CurTok == '+' || CurTok == '-' || CurTok == '*' || CurTok == '/') {
            char Op = CurTok;
            getNextToken();
            auto RHS = parsePrimary();
            if (!RHS) return nullptr;
            LHS = std::make_unique<BinaryExprAST>(Op, std::move(LHS), std::move(RHS));
        }
        return LHS;
    }

    std::unique_ptr<ExprAST> parseLetStatement() {
        getNextToken(); // consume 'let'
        std::string VarName;
        while (isalnum(CurTok)) { VarName += CurTok; getNextToken(); }
        if (CurTok != '=') return nullptr;
        getNextToken(); // consume '='
        auto Value = parseExpression();
        return std::make_unique<VariableAssignAST>(VarName, std::move(Value));
    }

    std::unique_ptr<FunctionAST> parseFunction() {
        getNextToken(); // consume 'def'
        std::string Name;
        while (isalnum(CurTok)) { Name += CurTok; getNextToken(); }
        if (CurTok != '(') return nullptr;
        getNextToken(); // consume '('

        std::vector<std::string> Args;
        while (isalnum(CurTok)) { Args.push_back(std::string(1, CurTok)); getNextToken(); }

        if (CurTok != ')') return nullptr;
        getNextToken(); // consume ')'

        auto Body = parseExpression();
        return std::make_unique<FunctionAST>(Name, Args, std::move(Body));
    }

public:
    Parser(const std::string& Input) : Input(Input), Index(0) { getNextToken(); }
    std::unique_ptr<ExprAST> parse() {
        if (CurTok == 'l') return parseLetStatement();
        if (CurTok == 'd') return parseFunction();
        return parseExpression();
    }
};

#endif


std::unique_ptr<ExprAST> parseIfStatement() {
    getNextToken(); // consume 'if'
    auto Cond = parseExpression();
    if (!Cond) return nullptr;
    auto Then = parseExpression();
    auto Else = parseExpression();
    return std::make_unique<IfExprAST>(std::move(Cond), std::move(Then), std::move(Else));
}

std::unique_ptr<ExprAST> parseWhileStatement() {
    getNextToken(); // consume 'while'
    auto Cond = parseExpression();
    auto Body = parseExpression();
    return std::make_unique<WhileExprAST>(std::move(Cond), std::move(Body));
}

std::unique_ptr<ExprAST> parseForStatement() {
    getNextToken(); // consume 'for'
    std::string VarName = getIdentifier();
    auto Start = parseExpression();
    auto End = parseExpression();
    auto Step = parseExpression();
    auto Body = parseExpression();
    return std::make_unique<ForExprAST>(VarName, std::move(Start), std::move(End), std::move(Step), std::move(Body));
}
