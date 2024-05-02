#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <queue>
using namespace std;

const int NMAX = 105;

struct Node {
    int key;
    Node* left = nullptr;
    Node* right = nullptr;
    int rank;
};

class RankPairingHeap {
    Node* root = nullptr;

public:
    RankPairingHeap() {}
    ~RankPairingHeap() {}

    Node* getRoot() const {
        return root;
    }

    void setRoot() {
        root = nullptr;
    }

    void setRoot(Node* nroot) {
        root = nroot;
    }

    static Node* mergeWith(Node* a, Node* b) {
        if (a == nullptr)
            return b;
        if (b == nullptr)
            return a;
        if (a->key > b->key) {
            b->right = a->left;
            a->left = b;
            a->rank++;
            return a;
        }
        else {
            a->right = b->left;
            b->left = a;
            b->rank++;
            return b;
        }
    }

    void merge(Node* b) {
        if (root == nullptr) {
            root = b;
            return;
        }
        if (b == nullptr)
            return;
        if (root->key > b->key) {
            b->right = root->left;
            root->left = b;
            root->rank++;
        }
        else {
            root->right = b->left;
            b->left = root;
            b->rank++;
            root = b;
        }
    }

    int extractMin() {
        int minVal = root->key;
        queue<Node*> q;
        Node* aux = root->left;
        q.push(aux);
        if (aux == nullptr) {
            root = q.front();
            q.pop();
            return minVal;
        }
        aux = aux->right;
        while (aux != nullptr) {
            q.push(aux);
            aux = aux->right;
        }
        Node* a1, * a2;
        while (q.size() > 1) {
            a1 = q.front();
            q.pop();
            a2 = q.front();
            q.pop();
            aux = mergeWith(a1, a2);
            q.push(aux);
        }
        root = q.front();
        return minVal;
    }

    void push(int val) {
        Node* aux = new Node;
        aux->key = val;
        aux->rank = 0;
        merge(aux);
    }
};

int main() {
    ifstream f("mergeheap.in");
    ofstream g("mergeheap.out");
    int n, q, op;
    RankPairingHeap R[NMAX];
    f >> n >> q;
    while (q--) {
        f >> op;
        switch (op) {
        case 1:
            f >> n >> op;
            R[n].push(op);
            break;
        case 2:
            f >> n;
            g << R[n].extractMin() << "\n";
            break;
        case 3:
            f >> n >> op;
            R[n].merge(R[op].getRoot());
            R[op].setRoot();
            break;
        }
    }
    f.close();
    g.close();
    return 0;
}
