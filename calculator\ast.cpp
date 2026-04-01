
#include "ast.h"
#include <cctype>
#include <stdexcept>

using namespace std;

Node::Node(int v) {
    type = NodeType::NUMBER;
    op = OpType::NONE;
    value = v;
    var = '\0';
    left = right = nullptr;
}

Node::Node(char c) {
    type = NodeType::VARIABLE;
    op = OpType::NONE;
    value = 0;
    var = c;
    left = right = nullptr;
}

Node::Node(OpType o, Node* l, Node* r) {
    type = NodeType::OPERATOR; 
    op = o;
    value = 0;
    var = '\0';
    left = l;
    right = r;
}

void SymbolTable::set(const char& name, int value) {
    table[name] = value;
}

int SymbolTable::get(const char& name) const {
    auto it = table.find(name);
    if (it != table.end()) return it->second;
    throw runtime_error("Unknown variable: " + string(1, name));
}

bool SymbolTable::exists(const char& name) const {
    return table.find(name) != table.end();
}

Lexer::Lexer(string input_text) {
    text = input_text;
    pos = 0;
    current = text.empty() ? '\0' : text[0];
}

void Lexer::advance() {
    pos++;
    current = (pos < text.size()) ? text[pos] : '\0';
}

void Lexer::skip_whitespace() {
    while (current == ' ')
        advance();
}

Token Lexer::number() {
    string result;
    while (current != '\0' && isdigit(current)) {
        result += current;
        advance();
    }
    return Token{TokenType::NUMBER, stoi(result), '\0'};
}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;

    while (current != '\0') {

        if (current == ' ') {
            skip_whitespace();
            continue;
        }

        if (isdigit(current)) {
            tokens.push_back(number());
            continue;
        }

        if (isalpha(current)) {
            char name = current;
            advance();
            tokens.push_back({TokenType::IDENT, 0, name});
            continue;
        }

        if (current == '+') { tokens.push_back({TokenType::PLUS}); advance(); continue; }
        if (current == '-') { tokens.push_back({TokenType::MINUS}); advance(); continue; }
        if (current == '*') { tokens.push_back({TokenType::MUL}); advance(); continue; }
        if (current == '/') { tokens.push_back({TokenType::DIV}); advance(); continue; }
        if (current == '(') { tokens.push_back({TokenType::LPAREN}); advance(); continue; }
        if (current == ')') { tokens.push_back({TokenType::RPAREN}); advance(); continue; }
        if (current == '=') { tokens.push_back({TokenType::ASSIGN}); advance(); continue; }

        throw runtime_error("Invalid character");
    }

    tokens.push_back({TokenType::END});
    return tokens;
}

Parser::Parser() {
    root = nullptr;
}

Parser::~Parser() {
    deleteTree(root);
}

void Parser::advance() {
    pos++;
    current = (pos < tokens.size()) ? tokens[pos] : Token{TokenType::END, 0, '\0'};
}

Node* Parser::expr() {
    Node* node = term();

    while (current.type == TokenType::PLUS ||
           current.type == TokenType::MINUS) {

        OpType op = (current.type == TokenType::PLUS)
                    ? OpType::ADD
                    : OpType::SUB;

        advance();
        node = new Node(op, node, term());
    }

    return node;
}

Node* Parser::term() {
    Node* node = factor();

    while (current.type == TokenType::MUL ||
           current.type == TokenType::DIV) {

        OpType op = (current.type == TokenType::MUL)
                    ? OpType::MUL
                    : OpType::DIV;

        advance();
        node = new Node(op, node, factor());
    }

    return node;
}

Node* Parser::factor() {
    if (current.type == TokenType::NUMBER) {
        Node* node = new Node(current.value);
        advance();
        return node;
    }

    if (current.type == TokenType::IDENT) {
        Node* node = new Node(current.name);
        advance();
        return node;
    }

    if (current.type == TokenType::LPAREN) {
        advance();
        Node* node = expr();

        if (current.type != TokenType::RPAREN)
            throw runtime_error("Missing ')'");

        advance();
        return node;
    }

    throw runtime_error("Invalid factor");
}

Node* Parser::statement() {
    if (current.type == TokenType::IDENT &&
        tokens[pos + 1].type == TokenType::ASSIGN) {

        char varName = current.name;
        advance(); 
        advance(); 

        Node* right = expr();
        Node* left = new Node(varName);

        Node* assign = new Node(OpType::NONE, left, right);
        assign->type = NodeType::ASSIGN; 

        return assign;
    }

    return expr();
}

Node* Parser::parse(vector<Token> t) {
    tokens = t;
    pos = 0;
    current = tokens[0];

    if (root)
        deleteTree(root);

    root = statement();

    if (current.type != TokenType::END)
        throw runtime_error("Unexpected input");

    return root;
}

void Parser::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void Parser::collectVariables(Node* node, set<char>& vars) {
    if (!node) return;

    if (node->type == NodeType::VARIABLE)
        vars.insert(node->var);

    collectVariables(node->left, vars);
    collectVariables(node->right, vars);
}
