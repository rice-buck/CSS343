#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include "ExpTree.h"

using namespace std;

/**
 * Loads the variable mapping from variable_map.txt.
 * Reads pairs of char and double until the end of the file.
 */
unordered_map<char, double> load_variable_map(const string& filename) {
    unordered_map<char, double> values;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Could not open " << filename << endl;
        return values;
    }
    char var;
    double val;
    while (infile >> var >> val) { // Loads all variables: a, b, c, d, e, f 
        values[var] = val;
    }
    infile.close();
    return values;
}

/**
 * Tests all required public methods for a given expression.
 */
void run_full_test(const string& expression, const unordered_map<char, double>& values) {
    cout << "-------------------------------------------------------" << endl;
    cout << "Testing Expression: " << expression << endl;
    
    // Test Parameterized Constructor
    ExpTree tree(expression);

    // Test isEmpty and getHeight
    cout << "isEmpty(): " << (tree.isEmpty() ? "True" : "False") << endl;
    cout << "getHeight(): " << tree.getHeight() << endl;

    // Test Traversals (Inorder, Preorder, Postorder)
    cout << "Inorder:   " << tree.inorder() << endl;
    cout << "Preorder:  " << tree.preorder() << endl;
    cout << "Postorder: " << tree.postorder() << endl;

    // Test Copy Constructor (Deep Copy Verification)
    cout << "\n[Testing Copy Constructor]" << endl;
    ExpTree copiedTree(tree);
    cout << "Copied Tree Postorder: " << copiedTree.postorder() << endl;

    // Test displaySideways
    cout << "\n[Display Sideways]:" << endl;
    tree.displaySideways();

    // Test operator<< (Overloaded ostream)
    cout << "\n[Operator<< Visualization]:\n" << tree << endl;

    // Test evaluate_postfix
    try {
        double result = tree.evaluate_postfix(tree.postorder(), values);
        cout << "Evaluation Result: " << result << endl;
    } catch (const exception& e) {
        cout << "Evaluation Error: " << e.what() << endl;
    }
    cout << "-------------------------------------------------------" << endl;
}

int main() {
    // 1. Test Edge Case: Empty Tree (Default Constructor)
    cout << "=== Edge Case: Default Constructor ===" << endl;
    ExpTree emptyTree;
    cout << "isEmpty(): " << (emptyTree.isEmpty() ? "True" : "False") << endl;
    cout << "getHeight(): " << emptyTree.getHeight() << endl;
    cout << "Inorder (should be blank): " << emptyTree.inorder() << endl;

    // 2. Load Variable Map from variable_map.txt 
    unordered_map<char, double> variable_map = load_variable_map("variable_map.txt");
    if (variable_map.empty()) {
        cerr << "Variable map is empty. Check variable_map.txt." << endl;
        return -1;
    }

    // 3. Process Expressions from expressions.txt 
    ifstream expfile("expressions.txt");
    if (!expfile) {
        cerr << "Error: Could not open expressions.txt" << endl;
        return -1;
    }

    string expression;
    while (getline(expfile, expression)) {
        if (expression.empty() || expression == "exit") continue;
        run_full_test(expression, variable_map);
    }
    expfile.close();

    // 4. Test Edge Case: Division by Zero
    // Note: In variable_map.txt, b = -4.0. Thus (b + b) = 0.
    cout << "\n=== Edge Case: Division by Zero (a / (b - b)) ===" << endl;
    run_full_test("a / (b - b)", variable_map);

    return 0;
}