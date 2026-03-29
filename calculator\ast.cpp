
#include "ast.h"
#include <cctype>
#include <stdexcept>

using namespace std;

Node::Node(int v)
    : type(NodeType::NUMBER), op(OpType::NONE), value(v),
      left(nullptr), right(nullptr) {}

Node::Node(const string& n)
    : type(NodeType::VARIABLE), op(OpType::NONE), value(0), name(n),
      left(nullptr), right(nullptr) {}

Node::Node(OpType o, Node* l, Node* r)
    : type(NodeType::OPERATOR), op(o), value(0),
      left(l), right(r) {}

void Calculator_AST::skip_spaces() {
    while (pos < expr.size() && isspace(expr[pos]))
        pos++;
}

void Calculator_AST::check_bounds() {
    if (pos >= expr.size())
        throw runtime_error("Unexpected end of input");
}


Node* Calculator_AST::parse_identifier() {
    skip_spaces();

    if (!isalpha(expr[pos]))
        throw runtime_error("Expected variable");

    string name;
    while (pos < expr.size() && isalnum(expr[pos])) {
        name += expr[pos];
        pos++;
    }

    return new Node(name);
}

Node* Calculator_AST::parse_number() {
    skip_spaces();

    int num = 0;
    while (pos < expr.size() && isdigit(expr[pos])) {
        num = num * 10 + (expr[pos] - '0');
        pos++;
    }

    return new Node(num);
}

Node* Calculator_AST::parse_sub_add() {
    Node* left = parse_mul_div();

    while (true) {
        skip_spaces();
        if (pos >= expr.size()) break;

        if (expr[pos] == '+' || expr[pos] == '-') {
            OpType op = (expr[pos] == '+') ? OpType::ADD : OpType::SUB;
            pos++;
            Node* right = parse_mul_div();
            left = new Node(op, left, right);
        } else break;
    }

    return left;
}

Node* Calculator_AST::parse_mul_div() {
    Node* left = parse_val();

    while (true) {
        skip_spaces();
        if (pos >= expr.size()) break;

        if (expr[pos] == '*' || expr[pos] == '/') {
            OpType op = (expr[pos] == '*') ? OpType::MUL : OpType::DIV;
            pos++;
            Node* right = parse_val();
            left = new Node(op, left, right);
        } else break;
    }

    return left;
}

Node* Calculator_AST::parse_val() {
    skip_spaces();
    check_bounds();

    if (expr[pos] == '(') {
        pos++;
        Node* node = parse_sub_add();

        skip_spaces();
        if (pos >= expr.size() || expr[pos] != ')')
            throw runtime_error("Mismatched parentheses");

        pos++;
        return node;
    }

    if (isdigit(expr[pos]))
        return parse_number();

    if (isalpha(expr[pos]))
        return parse_identifier();

    throw runtime_error("Unexpected token");
}

Node* Calculator_AST::parse(const string& s) {
    expr = s;
    pos = 0;

    Node* root = parse_sub_add();

    skip_spaces();
    if (pos != expr.size())
        throw runtime_error("Unexpected characters at end");

    return root;
}

void free_tree_recursive(Node* node) {
    if (!node) return;
    free_tree_recursive(node->left);
    free_tree_recursive(node->right);
    delete node;
}
