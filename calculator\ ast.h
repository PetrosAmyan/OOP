
#pragma once
#include <string>

enum class OpType { NONE, ADD, SUB, MUL, DIV };
enum class NodeType { NUMBER, VARIABLE, OPERATOR };

struct Node {
    NodeType type;
    OpType op;
    int value;
    std::string name;

    Node* left;
    Node* right;

    Node(int v);
    Node(const std::string& n);
    Node(OpType o, Node* l, Node* r);
};

class Calculator_AST {
public:
    Node* parse(const std::string& s);

private:
    std::string expr;
    int pos;

    void skip_spaces();
    void check_bounds();

    Node* parse_identifier();
    Node* parse_number();
    Node* parse_sub_add();
    Node* parse_mul_div();
    Node* parse_val();
};

void free_tree_recursive(Node* node);
