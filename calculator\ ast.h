#pragma once
#include <set>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

enum class TokenType {
    NUMBER, IDENT,
    PLUS, MINUS, MUL, DIV,
    LPAREN, RPAREN,
    ASSIGN, END
};

struct Token {
    TokenType type;
    int value = 0;
    char name = '\0';
};

class Lexer {
private:
    std::string text;
    int pos;
    char current;

    void advance();
    void skip_whitespace();
    Token number();

public:
    Lexer(std::string input_text);
    std::vector<Token> tokenize();
};



enum class NodeType { NUMBER, VARIABLE, OPERATOR, ASSIGN };
enum class OpType { NONE, ADD, SUB, MUL, DIV };

struct Node {
    NodeType type;
    OpType op;
    char var;
    int value;
    Node* left;
    Node* right;

    Node(int v);
    Node(char c);
    Node(OpType o, Node* l, Node* r);
};

class Parser {
private:
    std::vector<Token> tokens;
    int pos;
    Token current;
    Node* root;

    void advance();
    Node* expr();
    Node* term();
    Node* factor();
    Node* statement();
    void deleteTree(Node* node);

public:
    Parser();
    ~Parser();

    Node* parse(std::vector<Token> t);
    void collectVariables(Node* node, std::set<char>& vars);
};



class SymbolTable {
private:
    std::map<char, int> table;

public:
    void set(const char& name, int value);
    int get(const char& name) const;
    bool exists(const char& name) const;
};
