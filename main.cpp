#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Item
{
public:
    string itemName;
    string category;
    int price;

    Item(string name, string cat, int pr) : itemName(name), category(cat), price(pr) {}

    bool operator<(const Item &other) const
    {
        return itemName < other.itemName;
    }

    bool operator>(const Item &other) const
    {
        return itemName > other.itemName;
    }

    void print() const
    {
        cout << "Item Name: " << itemName << ", Category: " << category << ", Price: " << price << endl;
    }
};

class Heap
{
private:
    vector<Item> heap;

    int parent(int i) {
        return (i - 1) / 2;
    }
    int left(int i) {
        return (2 * i + 1);
    }
    int right(int i) {
        return (2 * i + 2);
    }

    bool compare(const Item &item1, const Item &item2, bool sortByName, bool ascending) {
        if (sortByName) {
            return ascending ? item1.itemName < item2.itemName : item1.itemName > item2.itemName;
        } else {
            return ascending ? item1.price < item2.price : item1.price > item2.price;
        }
    }

    void heapifyUp(int index, bool sortByName = true, bool ascending = true) {
        if (index && compare(heap[index], heap[parent(index)], sortByName, ascending)) {
            swap(heap[index], heap[parent(index)]);
            heapifyUp(parent(index), sortByName, ascending);
        }
    }

    void heapifyDown(int index, bool sortByName = true, bool ascending = true) {
        int leftChild = left(index);
        int rightChild = right(index);
        int smallestOrLargest = index;

        if (leftChild < size() && compare(heap[leftChild], heap[index], sortByName, ascending))
            smallestOrLargest = leftChild;

        if (rightChild < size() && compare(heap[rightChild], heap[smallestOrLargest], sortByName, ascending))
            smallestOrLargest = rightChild;

        if (smallestOrLargest != index) {
            swap(heap[index], heap[smallestOrLargest]);
            heapifyDown(smallestOrLargest, sortByName, ascending);
        }
    }



public:
    bool isMinHeap;
    Heap(bool minHeap = true) : isMinHeap(minHeap) {}

    void add(Item item)
    {
        heap.push_back(item);
        heapifyUp(size() - 1);
    }

    void remove()
    {
        if (size())
        {
            heap[0] = heap.back();
            heap.pop_back();
            heapifyDown(0);
        }
    }

    void display() const
    {
        for (const auto &item : heap)
        {
            item.print();
        }
    }

    int size() const
    {
        return heap.size();
    }

    void heapSortBy(bool sortByName = true, bool ascending = true)
    {
        vector<Item> originalHeap = heap;
        vector<Item> sorted;

        while (size()) {
            sorted.push_back(heap[0]);
            heap[0] = heap.back();
            heap.pop_back();
            heapifyDown(0, sortByName, ascending);
        }

        heap = originalHeap;
        display();
    }
};

class AVLNode
{
public:
    Item data;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(Item item) : data(item), left(nullptr), right(nullptr), height(1) {}
};

class AVL
{
private:
    AVLNode *root;

    int height(AVLNode *node)
    {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode *node)
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    AVLNode *rightRotate(AVLNode *y)
    {
        AVLNode *x = y->left;
        AVLNode *T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode *leftRotate(AVLNode *x)
    {
        AVLNode *y = x->right;
        AVLNode *T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    AVLNode *addHelper(AVLNode *node, Item item)
    {
        if (!node)
            return new AVLNode(item);

        if (item < node->data)
            node->left = addHelper(node->left, item);
        else if (item > node->data)
            node->right = addHelper(node->right, item);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && item < node->left->data)
            return rightRotate(node);

        if (balance < -1 && item > node->right->data)
            return leftRotate(node);

        if (balance > 1 && item > node->left->data)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && item < node->right->data)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode *minValueNode(AVLNode *node)
    {
        AVLNode *current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    AVLNode *removeHelper(AVLNode *root, Item item)
    {
        if (!root)
            return root;

        if (item < root->data)
            root->left = removeHelper(root->left, item);
        else if (item > root->data)
            root->right = removeHelper(root->right, item);
        else
        {
            if ((!root->left) || (!root->right))
            {
                AVLNode *temp = root->left ? root->left : root->right;
                if (!temp)
                {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;
                delete temp;
            }
            else
            {
                AVLNode *temp = minValueNode(root->right);
                root->data = temp->data;
                root->right = removeHelper(root->right, temp->data);
            }
        }

        if (!root)
            return root;

        root->height = 1 + max(height(root->left), height(root->right));

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && getBalance(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && getBalance(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void inOrderHelper(AVLNode *node, vector<Item> &items) const
    {
        if (node)
        {
            inOrderHelper(node->left, items);
            items.push_back(node->data);
            inOrderHelper(node->right, items);
        }
    }

public:
    AVL() : root(nullptr) {}

    void add(Item item)
    {
        root = addHelper(root, item);
    }

    void remove(Item item)
    {
        root = removeHelper(root, item);
    }

    void display() const
    {
        vector<Item> items;
        inOrderHelper(root, items);
        for (const auto &item : items)
        {
            item.print();
        }
    }

    void displayInOrder(bool ascending = true) const
    {
        vector<Item> items;
        inOrderHelper(root, items);
        if (!ascending)
        {
            sort(items.rbegin(), items.rend());
        }
        for (const auto &item : items)
        {
            item.print();
        }
    }

    void displayByPrice(bool ascending = true) const
    {
        vector<Item> items;
        inOrderHelper(root, items);
        if (ascending)
        {
            sort(items.begin(), items.end(), [](const Item &a, const Item &b)
                 { return a.price < b.price; });
        }
        else
        {
            sort(items.begin(), items.end(), [](const Item &a, const Item &b)
                 { return a.price > b.price; });
        }
        for (const auto &item : items)
        {
            item.print();
        }
    }
};

class BSTNode
{
public:
    Item data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(Item item) : data(item), left(nullptr), right(nullptr) {}
};

class BST
{
private:
    BSTNode *root;

    void addHelper(BSTNode *&node, Item item)
    {
        if (!node)
        {
            node = new BSTNode(item);
        }
        else if (item < node->data)
        {
            addHelper(node->left, item);
        }
        else
        {
            addHelper(node->right, item);
        }
    }

    BSTNode *removeHelper(BSTNode *node, Item item)
    {
        if (!node)
            return node;
        if (item < node->data)
        {
            node->left = removeHelper(node->left, item);
        }
        else if (item > node->data)
        {
            node->right = removeHelper(node->right, item);
        }
        else
        {
            if (!node->left)
            {
                BSTNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                BSTNode *temp = node->left;
                delete node;
                return temp;
            }
            BSTNode *temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = removeHelper(node->right, temp->data);
        }
        return node;
    }

    BSTNode *minValueNode(BSTNode *node)
    {
        BSTNode *current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    void inOrderHelper(BSTNode *node, vector<Item> &items) const
    {
        if (node)
        {
            inOrderHelper(node->left, items);
            items.push_back(node->data);
            inOrderHelper(node->right, items);
        }
    }

public:
    BST() : root(nullptr) {}

    void addItem(Item item)
    {
        addHelper(root, item);
    }

    void remove(Item item)
    {
        root = removeHelper(root, item);
    }

    void display() const
    {
        vector<Item> items;
        inOrderHelper(root, items);
        for (const auto &item : items)
        {
            item.print();
        }
    }

    void displayInOrder(bool ascending = true) const
    {
        vector<Item> items;
        inOrderHelper(root, items);
        if (!ascending)
        {
            sort(items.rbegin(), items.rend());
        }
        for (const auto &item : items)
        {
            item.print();
        }
    }

    void displayByPrice(bool ascending = true) const
    {
        vector<Item> items;
        inOrderHelper(root, items);
        if (ascending)
        {
            sort(items.begin(), items.end(), [](const Item &a, const Item &b)
                 { return a.price < b.price; });
        }
        else
        {
            sort(items.begin(), items.end(), [](const Item &a, const Item &b)
                 { return a.price > b.price; });
        }
        for (const auto &item : items)
        {
            item.print();
        }
    }
};

void readItems(istream &input, BST &tree)
{
    int numItems;
    input >> numItems;
    input.ignore();

    for (int i = 0; i < numItems; ++i)
    {
        string itemName, category;
        int price;

        getline(input, itemName);
        getline(input, category);
        input >> price;
        input.ignore();

        tree.addItem(Item(itemName, category, price));
    }
}

void readItems(istream &input, Heap &heap)
{
    int numItems;
    input >> numItems;
    input.ignore();

    for (int i = 0; i < numItems; ++i)
    {
        string itemName, category;
        int price;

        getline(input, itemName);
        getline(input, category);
        input >> price;
        input.ignore();

        heap.add(Item(itemName, category, price));
    }
}

void readItems(istream &input, AVL &tree)
{
    int numItems;
    input >> numItems;
    input.ignore();

    for (int i = 0; i < numItems; ++i)
    {
        string itemName, category;
        int price;

        getline(input, itemName);
        getline(input, category);
        input >> price;
        input.ignore();

        tree.add(Item(itemName, category, price));
    }
}

void Menu()
{
    cout << "1- ==> Binary Search Trees (BST)" << endl;
    cout << "2- ==> Heaps" << endl;
    cout << "3- ==> AVL Trees" << endl;
    cout << "0- ==> Exit" << endl;
    cout << "Your choice: ";
}

void TreeMenu()
{
    cout << "1- ==> Add item" << endl;
    cout << "2- ==> Remove item" << endl;
    cout << "3- ==> Display items normally" << endl;
    cout << "4- ==> Display items sorted by name ascending" << endl;
    cout << "5- ==> Display items sorted by name descending" << endl;
    cout << "6- ==> Display items sorted by price ascending" << endl;
    cout << "7- ==> Display items sorted by price descending" << endl;
    cout << "8- ==> Read Items from File" << endl;
    cout << "9- ==> Back to Main Menu" << endl;
    cout << "Your choice: ";
}

int main()
{
    BST bst;
    Heap minHeap(true);
    Heap maxHeap(false);
    AVL avl;
    int mainChoice, treeChoice;
    string itemName, category;
    int price;

    ifstream inFile("E:\\Amr\\EDUCATION\\FCAI\\Second-Year\\Second-semester\\DataStructures\\Assignments\\Assignment-2\\data.txt");
    if (!inFile)
    {
        cerr << "Unable to open file data.txt";
        return 1;
    }

    do
    {
        Menu();
        cin >> mainChoice;

        switch (mainChoice)
        {
        case 1:
            do
            {
                TreeMenu();
                cin >> treeChoice;
                cin.ignore();
                switch (treeChoice)
                {
                case 1:
                    cout << "Enter item name: ";
                    getline(cin, itemName);
                    cout << "Enter category: ";
                    getline(cin, category);
                    cout << "Enter price: ";
                    cin >> price;
                    bst.addItem(Item(itemName, category, price));
                    break;
                case 2:
                    cout << "Enter item name to remove: ";
                    getline(cin, itemName);
                    bst.remove(Item(itemName, "", 0));
                    break;
                case 3:
                    bst.display();
                    break;
                case 4:
                    bst.displayInOrder(true);
                    break;
                case 5:
                    bst.displayInOrder(false);
                    break;
                case 6:
                    bst.displayByPrice(true);
                    break;
                case 7:
                    bst.displayByPrice(false);
                    break;
                case 8:
                    readItems(inFile, bst);
                    break;
                }
            } while (treeChoice != 9);
            break;
        case 2:
            do
            {
                TreeMenu();
                cin >> treeChoice;
                cin.ignore();
                switch (treeChoice)
                {
                case 1:
                    cout << "Enter item name: ";
                    getline(cin, itemName);
                    cout << "Enter category: ";
                    getline(cin, category);
                    cout << "Enter price: ";
                    cin >> price;
                    if (minHeap.isMinHeap)
                        minHeap.add(Item(itemName, category, price));
                    else
                        maxHeap.add(Item(itemName, category, price));
                    break;
                case 2:
                    if (minHeap.isMinHeap)
                        minHeap.remove();
                    else
                        maxHeap.remove();
                    break;
                case 3:
                    if (minHeap.isMinHeap)
                        minHeap.display();
                    else
                        maxHeap.display();
                    break;
                case 4:
                    if (minHeap.isMinHeap)
                        minHeap.heapSortBy();
                    else
                        maxHeap.heapSortBy();
                    break;
                    case 5:
                        minHeap.heapSortBy();
                        break;
                    case 6:
                    break;
                    case 7:
                        break;
                case 8:
                    if (minHeap.isMinHeap)
                        readItems(inFile, minHeap);
                    else
                        readItems(inFile, maxHeap);
                    break;
                }
            } while (treeChoice != 9);
            break;
        case 3:
            do
            {
                TreeMenu();
                cin >> treeChoice;
                cin.ignore();
                switch (treeChoice)
                {
                case 1:
                    cout << "Enter item name: ";
                    getline(cin, itemName);
                    cout << "Enter category: ";
                    getline(cin, category);
                    cout << "Enter price: ";
                    cin >> price;
                    avl.add(Item(itemName, category, price));
                    break;
                case 2:
                    cout << "Enter item name to remove: ";
                    getline(cin, itemName);
                    avl.remove(Item(itemName, "", 0));
                    break;
                case 3:
                    avl.display();
                    break;
                case 4:
                    avl.displayInOrder(true);
                    break;
                case 5:
                    avl.displayInOrder(false);
                    break;
                case 6:
                    avl.displayByPrice(true);
                    break;
                case 7:
                    avl.displayByPrice(false);
                    break;
                case 8:
                    readItems(inFile, avl);
                    break;
                }
            } while (treeChoice != 9);
            break;
        }
    } while (mainChoice != 0);

    return 0;
}
