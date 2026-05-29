#include <iostream>
#include <string>
#include "llist.h"   // adjust path if needed

using namespace std;

// Helper function to print list contents
template <class T>
void printList(const LList<T>& list) {
    cout << "[ ";
    for (int i = 0; i < list.size(); ++i) {
        cout << list.retrieve(i) << " ";
    }
    cout << "]" << endl;
}

int main() {
    cout << "===== LList<string> DRIVER TEST =====\n\n";

    // 1. Test default constructor and isEmpty
    cout << "1. Construct empty list\n";
    LList<string> list;
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << endl;
    cout << "size(): " << list.size() << endl;
    printList(list);
    cout << endl;

    // 2. Test insert at front, middle, end
    cout << "2. Insert elements (0-based indexing)\n";
    list.insert("apple", 0);    // front
    list.insert("banana", 1);   // end
    list.insert("pear", 1);     // middle
    list.insert("orange", list.size()); // append

    printList(list);
    cout << "size(): " << list.size() << endl;
    cout << endl;

    // 3. Test retrieve
    cout << "3. Retrieve elements by index\n";
    for (int i = 0; i < list.size(); ++i) {
        cout << "retrieve(" << i << ") = " << list.retrieve(i) << endl;
    }
    cout << "retrieve(100) (out of bounds): " << list.retrieve(100) << endl;
    cout << endl;

    // 4. Test find
    cout << "4. Find elements\n";
    cout << "find(\"apple\") = " << list.find("apple") << endl;
    cout << "find(\"pear\")  = " << list.find("pear") << endl;
    cout << "find(\"kiwi\")  = " << list.find("kiwi") << endl;
    cout << endl;

    // 5. Test remove (front, middle, end)
    cout << "5. Remove elements\n";
    list.remove("apple");   // front
    printList(list);

    list.remove("pear");    // middle
    printList(list);

    list.remove("orange");  // end
    printList(list);

    list.remove("not-there"); // not found
    printList(list);
    cout << endl;

    // 6. Test copy constructor
    cout << "6. Copy constructor\n";
    LList<string> copyList(list);
    cout << "Original: ";
    printList(list);
    cout << "Copy:     ";
    printList(copyList);
    cout << endl;

    // 7. Test assignment operator
    cout << "7. Assignment operator\n";
    LList<string> assigned;
    assigned.insert("temp", 0);
    assigned = list;

    cout << "Assigned: ";
    printList(assigned);
    cout << endl;

    // 8. Test clear
    cout << "8. Clear list\n";
    list.clear();
    cout << "isEmpty(): " << (list.isEmpty() ? "true" : "false") << endl;
    cout << "size(): " << list.size() << endl;
    printList(list);
    cout << endl;

    cout << "===== END OF TEST =====\n";
    return 0;
}