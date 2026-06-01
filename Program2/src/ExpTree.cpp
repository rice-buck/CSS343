//Author: Rhys Buckeye, CSS 343, Program 2, ExpTree.cpp
// Implementation of the ExpTree class and its member functions 

#include "ExpTree.h"
#include <stack> 
#include <algorithm> // Required for std::remove_if
#include <cctype>    // Required for std::isalnum
#include <unordered_map>
#include <cmath>


//Node constructor
Node::Node(const char& val) : value(val), left(nullptr), right(nullptr) {}


//Default constructor
ExpTree::ExpTree() : infix(""), root(nullptr) {}


//Paramaterized Constructor
ExpTree::ExpTree(const string& exp) : infix(exp), root(nullptr) {
    cleanExpression(infix);             //clean
    postfix = convertToPostfix(infix);  //convert
    root = constructTree(postfix);      //to tree
}


//Copy constructor
ExpTree::ExpTree (const ExpTree& other) {
    this->infix = other.infix;
    this->postfix = other.postfix;
    this->prefix = other.prefix;
    this->root = copyHelper(other.root);
}


// Helper function for deep copying the tree
Node *ExpTree::copyHelper(Node *otherNode){
    if(otherNode == nullptr) return nullptr;

    Node *newNode = new Node(otherNode->value);
    newNode->left = copyHelper(otherNode->left);
    newNode->right = copyHelper(otherNode->right);
    return newNode; 
}


// Traversal functions
string ExpTree::inorder(){
    return infix;
}


string ExpTree::preorder(){
    prefix = "";
    preorderHelper(root, prefix);
    return prefix;
}


string ExpTree::postorder(){
    return postfix;
}


//Print Tree in human-readable format
std::ostream& operator<<(std::ostream& os, const ExpTree& tree){
    if (tree.root == nullptr) {
        os << "Tree is empty.";
    } else {
        tree.printHelper(os, tree.root, 0);
    }
    return os; // Crucial: return the stream to allow chaining (e.g., cout << t1 << t2;)
}


// Helper function to print the tree in a human-readable format
void ExpTree::printHelper(std::ostream& os, Node *node, int level) const {
    if (node == nullptr) return;

    // Create a string of spaces based on the current level
    // (4 spaces per level)
    string indent = "";
    for (int i = 0; i < level; i++) {
        indent += "    "; 
    }

    // Only print if it's an operator (has children)
    if (node->left != nullptr || node->right != nullptr) {
        os << indent + "Root: " + node->value + "\n";
        
        os << indent << "L --- ";
        os << (node->left ? node->left->value : ' ');
        os << "\n";
        

        os << indent << "R --- ";
        os << (node->right ? node->right->value : ' ');
        os << "\n\n";
    }

    // Pass level + 1 to the next subtree to increase indentation
    printHelper(os, node->left, level + 1);
    printHelper(os, node->right, level + 1);
}


// Display the tree sideways
void ExpTree::displaySideways(std::ostream&os) const{
    if (root == nullptr) {
        os << "Empty tree" << std::endl;
        return;
    }
    printSidewaysHelper(os, root, 0, 5);
}


// Helper function to print the tree sideways
void ExpTree::printSidewaysHelper(std::ostream& os, Node* root, int space, int count) const {
    if (root == nullptr) return;

    // Increase distance between levels
    space += count;

    // Process right child first (appears at the top when sideways)
    printSidewaysHelper(os, root->right, space);

    // Print current node after indentation
    os << "\n";
    for (int i = count; i < space; i++) {
        os << " ";
    }
    os << root->value << "\n";

    // Process left child
    printSidewaysHelper(os, root->left, space);
}


// Evaluate the postfix expression using the provided variable map
double ExpTree::evaluate_postfix(const string& postfix, const unordered_map<char, double>& exp_map){
    stack<double> s;
    
    for(char c : postfix) {
        if(isalnum(c)) {
            s.push(exp_map.at(c)); //push operands
        } else {
            if (s.size() < 2) throw runtime_error("Invalid postfix evaluation!");

            //when operator, pop two operands
            double op2 = s.top(); 
            s.pop();
            double op1 = s.top();
            s.pop();
            switch (c) {
                //do the calculation and push back onto stack
                case '+': s.push(op1 + op2); break;
                case '-': s.push(op1 - op2); break;
                case '*': s.push(op1 * op2); break;
                case '/': 
                // Check for division by zero
                    if (op2 == 0) throw runtime_error("Division by zero!");
                    s.push(op1 / op2);
                break;
                case '^': s.push(pow(op1, op2)); break;
            }
        }
    }
    return s.empty() ? 0 : s.top();
}


// Function to get the height of the tree.
int ExpTree::getHeight(){
    return heightHelper(root);
}


// Helper function to calculate the height of the tree
int ExpTree::heightHelper(Node *node){
    if(node == nullptr) return 0;

    //recursively go down each subtree
    int leftHeight = heightHelper(node->left);
    int rightHeight = heightHelper(node->right);

    return 1 + max(leftHeight, rightHeight); //return the max height of the two subtrees + 1 for the current node
}


// Check if the tree is empty
bool ExpTree::isEmpty(){
    if(root == nullptr) return true;
    else return false;
}
 
// Function to print the original infix expression.
void ExpTree::printExpression(){
    cout << "Expression:" << endl;
    cout << "infix: " << infix << endl;
    cout << "postfix: " << postfix << endl;
    cout << "prefix: " << prefix << endl;
}


// Helper to define operator priority
int ExpTree::precedence(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}


//Funciton to read an infix expression and convert it to a postfix expression
string ExpTree::convertToPostfix(const string infix){
    string output = "";
    stack<char> expStack;
    for (int i = 0; i < infix.size(); ++i) {
        char c = infix[i];

        // If operand, add to output
        if (isalnum(c)) {
            output += c;
        }
        // If '(', push to stack
        else if (c == '(') {
            expStack.push('(');
        }
        // If ')', pop until '(' is found
        else if (c == ')') {
            while (!expStack.empty() && expStack.top() != '(') {
                output += expStack.top();
                expStack.pop();
            }
            if (!expStack.empty()) expStack.pop(); // Remove '('
        }
        // If operator
        else {
            while (!expStack.empty() && precedence(expStack.top()) >= precedence(c)) {
                // Special case for '^' 
                if (c == '^' && expStack.top() == '^') break; 
                
                output += expStack.top();
                expStack.pop();
            }
            expStack.push(c);
        }
    }

    //Pop remaining operators
    while (!expStack.empty()) {
        output += expStack.top();
        expStack.pop();
    }

    return output;
}

// Helper function for preorder traversal
void ExpTree::preorderHelper(Node *node, string& result){
    if(node == nullptr) return;

    result += node->value;

    //Traverse left subtree
    preorderHelper(node->left, result);

    //Traverse right subtree
    preorderHelper(node->right, result);
}


// Check if a character is an operator
bool ExpTree::isOperator(char c){
    if(c == '*' || c == '/' || c == '+' || c == '-' || c == '^') return true;
    else return false;
}


// Check if the parentheses in the expression are valid
bool ExpTree::validParenthese(std::string s) {
    //use stack data structure to check for valid parenthese
    std::stack<char> st;
    for (char c : s) {
        if (c == '(') {
            st.push(c);
        } else if (c == ')') {
            if (st.empty()) return false;
            char top = st.top();
            if (c == ')' && top == '(') {
                st.pop();
            } else {
                return false;
            }
        }
    }
    return st.empty();
}


//clean a expression by removing whitespace and checking for invalid characters
void ExpTree::cleanExpression(string& exp){
    //remove all whitespace
    exp.erase(std::remove_if(exp.begin(), exp.end(), [](unsigned char x) {
    return std::isspace(x); }), exp.end()); 

    for(int i = 0; i < exp.size() - 1; ++i){
        //check for invalid expression

        if(isalnum(exp[i]) && isalnum(exp[i + 1])){
            throw std::runtime_error("Invalid : Invalid Operands!");
            return;
        }

        if(isOperator(exp[exp.size() - 1]) || isOperator(exp[i]) && isOperator(exp[i + 1])){
            throw std::runtime_error("Invalid : Invalid Operator!");
            return;
        }

        if(!validParenthese(exp)){
            throw std::runtime_error("Invalid : Invalid Parenthese!");
            return;
        }

    }
}


// Construct the expression tree from the postfix expression
Node* ExpTree::constructTree(const string postExp) { 
    stack<Node*> s;
    for (char c : postExp) {
        if (!isOperator(c)) {
            // Push operand as a leaf node
            s.push(new Node(c));
        } else {
            // Operator: Pop two operands and make them children
            Node* newNode = new Node(c);
            newNode->right = s.top(); s.pop();
            newNode->left = s.top(); s.pop();
            s.push(newNode);
        }
    }
    return s.top(); // Final root
}