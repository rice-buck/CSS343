//Author: Rhys Buckeye, CSS 343, Program 2, ExpTree.h
// Header file for the ExpTree class, which represents an expression tree and provides various 
// functionalities such as construction, traversal, evaluation, and display.

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

struct Node {
    char value;
        Node* left;
        Node* right;
    // Constructor with parameter to initialize the node value
    Node(const char& val);
};

class ExpTree {
public:
    ExpTree(); //default constructor
    ExpTree(const string& exp); //Paramaterized Constructor
    ExpTree (const ExpTree& other); //copy constructor

    // Traversal functions
    string inorder();
    string preorder();
    string postorder();

    //Both functions are printing the tree in a human-readable format
    friend std::ostream& operator<<(std::ostream& os, const ExpTree& tree);

    // Display the tree sideways
    void displaySideways(std::ostream&os = std::cout) const;
    
    // evaluate postfix expression
    double evaluate_postfix(const string& postfix, const unordered_map<char, double>& exp_map);

    // Function to get the height of the tree. 
    int getHeight();

    // Function to check if the tree is empty.
    bool isEmpty();

    // Function to print the original infix expression.
    void printExpression();

    


    
private:
    // Root node of the expression tree
    Node* root;

    string infix;
    string prefix; 
    string postfix;
    
    // Function to determine the precedence of operators
    int precedence(char c);

    //Convert  infix expression to postfix expression
    string convertToPostfix(const string infix);

    //Helper fucntion for preorder traversal
    void preorderHelper(Node *node, string& result);

    // Check if a character is an operator
    bool isOperator(char c);

    // Check if the parentheses in the expression are valid
    bool validParenthese(std::string s);

    //Clean whitespace from the expression
    void cleanExpression(string& exp);

    // Construct the expression tree from the postfix expression
    Node *constructTree(const string postExp);

    // Helper function for deep copying the tree
    Node *copyHelper(Node * otherNode);

    // Helper function to calculate the height of the tree
    int heightHelper(Node *node);

    // Helper function to print the tree in a human-readable format
    void printHelper(std::ostream& os, Node* node, int level) const;

    // Helper function to display the tree sideways
    void printSidewaysHelper(std::ostream& os, Node* root, int space = 0, int count = 5) const;

};