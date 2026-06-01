#include "ExpTree.h"
#include <string>
#include <stack>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;



int main(){
    const unordered_map<char, double> map = {{'a', 10}, {'b', 10}, {'c', 5}, {'d', 8}, {'e', 2}, {'f', 5}};

    ExpTree tree("a + b * (c + d) / e ^ f");
    tree.printExpression();
    int value = tree.getHeight();
    cout << value << endl;
    ExpTree tree2;

    string preorder = tree.preorder();
    cout << preorder << endl;

    // tree.displaySideways();

    // cout << tree << endl;
    double result;
    result = tree.evaluate_postfix(tree.postorder(), map);
    cout << result << endl;


}