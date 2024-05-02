#ifndef HEAP_23_HPP
#include <iostream>
#include <cmath>

template<typename T>
class Node23;

template<typename T>
int merge_nodes(Node23<T>** a, Node23<T>** b);

template<typename T>
class Heap23
{
public:
    Heap23(int max_nodes = 100) : nnodes(0), value(0)
    {
        max_trees_ = (int)(0.5 + std::log(max_nodes + 1) / std::log(2.0));
        trees = new Node23<T> *[max_trees_];

        for (int i = 0; i < max_trees_; ++i) {
            trees[i] = nullptr;
        }
    }

    ~Heap23()
    {
        delete[] trees;
    }

    Node23<T>* insert(int key, T value);
    Node23<T>* min();
    Node23<T>* extractMin();
    bool merge(Heap23<T>& heap);
    void remove(Node23<T>* node);
    void decreaseKey(Node23<T>* node, int new_key);
    void print(std::ostream& out);
    bool isEmpty();

protected:
    void meld(Node23<T>* list);

private:
    Node23<T>** trees;
    int max_trees_;
    int nnodes;
    int value;
};

template<typename T>
class Node23
{
    template<typename Y> friend class Heap23;
    template<typename Y> friend int merge_nodes(Node23<Y>** a, Node23<Y>** b);

public:
    Node23(int key, T value) :
        child(nullptr),
        left(nullptr),
        right(nullptr),
        parent(nullptr),
        partner(nullptr),
        extra(false),
        dim(0),
        key_(key),
        value_(value)
    {
    }

    int priority() { return key_; }
    T value() { return value_; }

    void addChild(Node23<T>* node);
    void removeChild(Node23<T>* child);
    void replaceChild(Node23<T>* new_node);

    void print(std::ostream& out, unsigned level);

protected:
    void printPartners(std::ostream& out, unsigned level);

private:
    Node23* parent;
    Node23* child;
    Node23* left;
    Node23* right;
    Node23* partner;

    bool extra;
    int dim;
    int key_;
    T value_;
};

template<typename T>
int merge_nodes(Node23<T>** a, Node23<T>** b)
{
    Node23<T>* tree, * next_tree, * other, * next_other;
    int c = 0;

    if ((*a)->priority() <= (*b)->priority()) {
        tree = (*a);
        other = (*b);
    }
    else {
        tree = (*b);
        other = (*a);
    }
    ++c;

    next_tree = tree->partner;
    next_other = other->partner;

    if (!next_tree) {
        if (next_other) {
            tree->addChild(other);
            tree->dim++;
            *a = nullptr;
            *b = tree;
        }
        else {
            tree->partner = other;
            other->partner = tree;
            other->extra = true;

            *a = tree;
            *b = nullptr;
        }
    }
    else if (!next_other) {
        tree->partner = nullptr;
        other->partner = next_tree;
        next_tree->partner = other;

        if (other->priority() < next_tree->priority()) {
            tree->addChild(other);
        }
        else {
            next_tree->extra = false;
            other->extra = true;
            tree->addChild(next_tree);
        }

        ++(tree->dim);

        ++c;
        *a = nullptr;
        *b = tree;
    }
    else {
        tree->partner = nullptr;
        next_tree->partner = nullptr;
        next_tree->extra = false;
        next_tree->left = next_tree->right = next_tree;
        next_tree->parent = nullptr;

        tree->addChild(other);
        ++(tree->dim);

        *a = next_tree;
        *b = tree;
    }
    return c;
}

template<typename T>
void Node23<T>::addChild(Node23<T>* node)
{
    Node23<T>* left, * right;
    if ((left = child)) {
        right = left->right;
        node->left = left;
        node->right = right;
        right->left = node;
        left->right = node;
    }
    else {
        node->left = node->right = node;
    }
    child = node;
    node->parent = this;
}

template<typename T>
void Node23<T>::removeChild(Node23<T>* node)
{
    Node23<T>* left, * right;
    if (node->dim) {
        left = node->left;
        right = node->right;
        left->right = right;
        right->left = left;
        child = left;
    }
    else {
        child = nullptr;
    }
}

template<typename T>
void Node23<T>::replaceChild(Node23<T>* new_node)
{
    Node23<T>* l, * r = right;

    if (r == this) {
        new_node->right = new_node->left = new_node;
    }
    else {
        l = left;
        l->right = new_node;
        r->left = new_node;
        new_node->left = l;
        new_node->right = r;
    }

    new_node->parent = parent;
    if (parent->child == this)
        parent->child = new_node;
}

template<typename T>
void Node23<T>::print(std::ostream& out, unsigned level)
{
    Node23<T>* x;
    bool is_started = false;
    for (x = this; !is_started || x != this; x = x->right) {
        if (!is_started)
            is_started = true;
        for (int i = 0; i < level; ++i) {
            out << "  ";
        }
        out << x->key_ << '[' << x->dim << ']' << '[' << x->extra << ']';
        if (x->partner != nullptr && x->partner != x) {
            out << " | " << x->partner->key_ << '[' << x->partner->dim << ']' << '[' << x->partner->extra << ']';
        }
        out << std::endl;
        if (x->child != nullptr) {
            x->child->print(out, level + 1);
        }
        if (x->partner != nullptr && x->partner != x && x->partner->child != nullptr) {
            x->partner->child->print(out, level + 1);
        }
    }
}

template<typename T>
void Node23<T>::printPartners(std::ostream& out, unsigned level)
{
    Node23<T>* x = partner;
    for (int i = 0; i < level; ++i) {
        out << "  ";
    }
    out << x->key_ << '[' << x->dim << ']' << '[' << x->extra << ']' << std::endl;
    if (x->child != nullptr) {
        x->child->print(out, level + 1);
    }
}

template<typename T>
Node23<T>* Heap23<T>::insert(int key, T value)
{
    Node23<T>* node = new Node23<T>(key, value);
    meld(node);
    ++nnodes;
    return node;
}

template<typename T>
Node23<T>* Heap23<T>::min()
{
    Node23<T>* min_node, * next;
    long k, k2;
    int r, v;

    v = value;
    r = -1;
    while (v) {
        v = v >> 1;
        r++;
    }

    min_node = trees[r];
    k = min_node->priority();
    while (r > 0) {
        r--;
        next = trees[r];
        if (next) {
            if ((k2 = next->priority()) < k) {
                k = k2;
                min_node = next;
            }
        }
    }
    return min_node;
}

template<typename T>
Node23<T>* Heap23<T>::extractMin()
{
    Node23<T>* child, * next, * partner, * min_node = min();

    int r = min_node->dim;
    if ((partner = min_node->partner)) {
        partner->partner = nullptr;
        partner->extra = false;
        partner->parent = nullptr;
        partner->left = partner->right = partner;
        trees[r] = partner;
    }
    else {
        trees[r] = nullptr;
        value -= (1 << r);
    }
    --nnodes;

    child = min_node->child;
    if (child) {
        next = child->right;
        next->left = child->right = nullptr;
        meld(next);
    }

    return min_node;
}

template<typename T>
bool Heap23<T>::merge(Heap23<T>& heap)
{
    for (int i = 0; i < heap.max_trees_; ++i) {
        if (heap.trees[i] != nullptr) {
            heap.trees[i]->right = heap.trees[i]->left = nullptr;
            meld(heap.trees[i]);
            heap.trees[i] = nullptr;
        }
    }
    nnodes += heap.nnodes;
    heap.nnodes = 0;
    return true;
}

template<typename T>
void Heap23<T>::remove(Node23<T>* node)
{
    Node23<T>* p, * m, * ax, * bx, * ap, * bp, * b1, * a1, * a2, * l, * r, * partner = node->partner;
    int d;

    if (node->extra) {
        node->partner->partner = nullptr;
        node->partner = nullptr;
        node->extra = false;
    }
    else if (partner) {
        partner->partner = nullptr;
        node->partner = nullptr;
        partner->extra = false;

        node->replaceChild(partner);
    }
    else {
        m = node->right;
        if (m->dim) {
            ax = ap = nullptr;
            bx = m->child->partner;
            bp = (Node23<T> *) (bx ? nullptr : m->child);
        }
        else {
            m = node->parent;
            if (m->extra) {
                bx = bp = nullptr;
                m = m->partner;
                ax = m->child->partner;
                ap = (Node23<T> *) (ax ? nullptr : m->child);
            }
            else {
                if (m->parent) {
                    ax = m->left->partner;
                    ap = (Node23<T> *) (ax ? nullptr : m->left);
                }
                else {
                    ax = ap = nullptr;
                }

                if ((m = m->partner)) {
                    bx = m->child->partner;
                    bp = (Node23<T> *) (bx ? nullptr : m->child);
                }
                else {
                    bx = bp = nullptr;
                }
            }
        }

        if (bx) {
            bx->partner->partner = nullptr;
            bx->partner = nullptr;
            bx->extra = false;

            node->replaceChild(bx);
        }
        else if (bp) {
            b1 = bp->parent;

            remove(b1);
            b1->removeChild(bp);
            bp->partner = b1;
            b1->partner = bp;
            bp->extra = true;
            b1->dim = node->dim;

            node->replaceChild(b1);
        }
        else if (ax) {
            p = node->parent;
            a2 = ax->partner;
            a1 = a2->parent;

            p->removeChild(node);

            ax->partner = nullptr;
            ax->extra = false;

            d = a2->dim;
            a2->dim = p->dim;
            p->dim = d;

            a2->addChild(ax);

            if (p->extra) {
                p->partner = nullptr;
                p->extra = false;

                a2->partner = a1;
                a1->partner = a2;
                a2->extra = true;

                a2->replaceChild(p);
            }
            else {
                a2->partner = nullptr;
                if ((l = a2->left) != p) {
                    r = p->right;
                    p->left = l;
                    a2->right = r;
                    p->right = a2;
                    a2->left = p;
                    l->right = p;
                    r->left = a2;

                    if (a1->child == p) {
                        a1->child = a2;
                    }
                }
                else {
                    a1->child = a2;
                }
            }
        }
        else if (ap) {
            p = node->parent;
            p->removeChild(node);
            remove(p);
            p->dim = node->dim;

            p->partner = ap;
            ap->partner = p;

            if (p->priority() < ap->priority()) {
                p->extra = false;
                ap->replaceChild(p);
                ap->extra = true;
            }
            else {
                p->extra = true;
            }
        }
        else {
            d = node->dim;
            p = node->parent;

            p->removeChild(node);

            trees[d + 1] = nullptr;
            value -= (1 << (d + 1));
            p->dim = d;

            p->left = p->right = nullptr;
            meld(p);
        }
    }
}

template<typename T>
void Heap23<T>::decreaseKey(Node23<T>* node, int new_key)
{
    node->key_ = new_key;

    if (!(node->parent || node->extra)) {
        return;
    }

    remove(node);
    node->right = node->left = nullptr;

    meld(node);
}

template<typename T>
void Heap23<T>::print(std::ostream& out)
{
    for (int i = 0; i < max_trees_; ++i) {
        if (trees[i] != nullptr) {
            out << "Tree #" << i << std::endl;
            trees[i]->print(out, 0);
        }
    }
}

template<typename T>
bool Heap23<T>::isEmpty()
{
    return nnodes == 0;
}

template<typename T>
void Heap23<T>::meld(Node23<T>* list)
{
    Node23<T>* next, * add_tree, * carry_tree;
    int d;

    add_tree = list;
    carry_tree = nullptr;

    do {
        if (add_tree) {
            next = add_tree->right;
            add_tree->right = add_tree->left = add_tree;
            add_tree->parent = nullptr;
        }
        else {
            add_tree = carry_tree;
            carry_tree = nullptr;
        }

        if (carry_tree) {
            merge_nodes(&add_tree, &carry_tree);
        }

        if (add_tree) {
            d = add_tree->dim;
            if (trees[d]) {
                merge_nodes(&trees[d], &add_tree);
                if (!trees[d]) value -= (1 << d);
                carry_tree = add_tree;
            }
            else {
                trees[d] = add_tree;
                value += (1 << d);
            }
        }

        add_tree = next;
    } while (add_tree || carry_tree);
}

#endif

int main() {
    // Crearea unui obiect de tip Heap23 cu un număr maxim de noduri specificat
    Heap23<int> heap1(100);

    // Inserarea unor noduri în heap1
    heap1.insert(5, 50);
    heap1.insert(10, 100);
    heap1.insert(8, 300);
    heap1.insert(7, 60);
    heap1.insert(13, 90);


    std::cout << "Heap23 1 initial:\n";
    heap1.print(std::cout);


    Node23<int>* min_node1 = heap1.extractMin();
    std::cout << "\nNodul minim extras din heap1: " << min_node1->priority() << std::endl;


    std::cout << "\nHeap23 1 dupa extragerea minimului:\n";
    heap1.print(std::cout);

    // Crearea unui al doilea obiect de tip Heap23 cu un număr maxim de noduri specificat
    Heap23<int> heap2(100);

    // Inserarea unor noduri în heap2
    heap2.insert(4, 40);
    heap2.insert(12, 120);
    heap2.insert(6, 70);
    heap2.insert(9, 80);

    // Adăugăm un nod cu 3 copii: 15 va fi adăugat ca fiu al lui 12
    Node23<int>* parent = heap2.insert(12, 120);
    heap2.insert(15, 150);
    heap2.insert(16, 160);

    std::cout << "\nHeap23 2 initial:\n";
    heap2.print(std::cout);


    Node23<int>* min_node2 = heap2.extractMin();
    std::cout << "\nNodul minim extras din heap2: " << min_node2->priority() << std::endl;


    std::cout << "\nHeap23 2 dupa extragerea minimului:\n";
    heap2.print(std::cout);


    std::cout << "\nOperatia de merge intre heap-uri:\n";
    heap1.merge(heap2);
    heap1.print(std::cout);


    return 0;
}



