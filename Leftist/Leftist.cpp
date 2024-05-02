#include <iostream>
using namespace std;

class Node
{
public:
    int element;
    Node* left;
    Node* right;
    int d;
    Node(int& element, Node* l = NULL,
        Node* r = NULL, int np = 0)
    {
        this->element = element;
        right = r;
        left = l,
            d = np;
    }
};

class LeftistHeap
{
public:
    // Constructors and Destructor
    LeftistHeap();
    LeftistHeap(LeftistHeap& x);
    ~LeftistHeap();

    // Heap operations
    bool isEmpty();
    bool isFull();
    int& findMin();
    void Insert(int& x);
    void deleteMin();
    void deleteMin(int& mini);
    void makeEmpty();
    void Merge(LeftistHeap& x);
    LeftistHeap& operator =(LeftistHeap& x);

private:
    Node* root;

    Node* Merge(Node* h1, Node* h2);
    Node* Merge1(Node* h1, Node* h2);
    void swapChildren(Node* x);
    void getMemory(Node* x);
    Node* clone(Node* x);
};

LeftistHeap::LeftistHeap()
{
    root = NULL;
}

LeftistHeap::LeftistHeap(LeftistHeap& x)
{
    root = NULL;
    *this = x;
}

LeftistHeap::~LeftistHeap()
{
    makeEmpty();
}

// Merges the current heap with another heap
void LeftistHeap::Merge(LeftistHeap& x)
{
    if (this == &x)
        return;
    root = Merge(root, x.root);
    x.root = NULL;
}

// Merges two nodes maintaining the Leftist Heap property
Node* LeftistHeap::Merge(Node* h1, Node* h2)
{
    if (h1 == NULL)
        return h2;
    if (h2 == NULL)
        return h1;
    if (h1->element < h2->element)
        return Merge1(h1, h2);
    else
        return Merge1(h2, h1);
}

// Merges two nodes ensuring the Leftist Heap property
Node* LeftistHeap::Merge1(Node* h1, Node* h2)
{
    if (h1->left == NULL)
        h1->left = h2;
    else
    {
        h1->right = Merge(h1->right, h2);
        if (h1->left->d < h1->right->d)
            swapChildren(h1);
        h1->d = h1->right->d + 1;
    }
    return h1;
}

// Swaps the left and right children of a node
void LeftistHeap::swapChildren(Node* x)
{
    Node* tmp = x->left;
    x->left = x->right;
    x->right = tmp;
}

// Inserts an element into the heap
void LeftistHeap::Insert(int& x)
{
    root = Merge(new Node(x), root);
}

// Finds the minimum element in the heap
int& LeftistHeap::findMin()
{
    return root->element;
}

// Deletes the minimum element from the heap
void LeftistHeap::deleteMin()
{
    Node* oldRoot = root;
    root = Merge(root->left, root->right);
    delete oldRoot;
}

// Deletes the minimum element and stores it in mini
void LeftistHeap::deleteMin(int& mini)
{
    if (isEmpty())
    {
        cout << "Heap is Empty" << endl;
        return;
    }
    mini = findMin();
    deleteMin();
}

// Checks if the heap is empty
bool LeftistHeap::isEmpty()
{
    return root == NULL;
}

// Checks if the heap is full (always returns false for leftist heaps)
bool LeftistHeap::isFull()
{
    return false;
}

// Clears the memory allocated for the heap
void LeftistHeap::makeEmpty()
{
    getMemory(root);
    root = NULL;
}

// Assignment operator overload
LeftistHeap& LeftistHeap::operator =(LeftistHeap& x)
{
    if (this != &x)
    {
        makeEmpty();
        root = clone(x.root);
    }
    return *this;
}

// Recursively deallocates memory for the heap
void LeftistHeap::getMemory(Node* x)
{
    if (x != NULL)
    {
        getMemory(x->left);
        getMemory(x->right);
        delete x;
    }
}

// Recursively clones a node and its children
Node* LeftistHeap::clone(Node* x)
{
    if (x == NULL)
        return NULL;
    else
        return new Node(x->element, clone(x->left),
            clone(x->right), x->d);
}

int main()
{
    LeftistHeap h;
    LeftistHeap h1;
    LeftistHeap h2;
    int x;
    int v[] = { 3, 23, 26, 46 };
    int v1[] = { 11, 34,65 };

    h.Insert(v[0]);
    h.Insert(v[1]);
    h.Insert(v[2]);
    h.Insert(v[3]);
    h1.Insert(v1[0]);
    h1.Insert(v1[1]);
    h1.Insert(v1[2]);

    h.deleteMin(x);
    cout << x << endl;

    h1.deleteMin(x);
    cout << x << endl;

    h.Merge(h1);
    h2 = h;

    h2.deleteMin(x);
    cout << x << endl;

    return 0;
}