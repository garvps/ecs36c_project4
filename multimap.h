#ifndef MULTIMAP_H_
#define MULTIMAP_H_

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

template <typename K, typename V>
class Multimap {
    public:
    // Return size of tree
    unsigned int Size();
    // Return value associated to @key
    const V& Get(const K& key);
    // Return whether @key is found in tree
    bool Contains(const K& key);
    // Return max key in tree
    const K& Max();
    // Return min key in tree
    const K& Min();
    // Insert @key in tree
    void Insert(const K &key, const V &value);
    // Remove @key from tree
    void Remove(const K &key);
    // Print tree in-order
    void Print();

    private:
    enum Color { RED, BLACK };
    struct Node{
        K key;
        std::vector <V> values;     // added a vector to store multiple values in a key
        bool color;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(K Key, V value, bool col)
            : key(Key), color(col), left(nullptr), right(nullptr) {
            values.push_back(value);
        }
    };

    std::unique_ptr<Node> root;
    unsigned int cur_size = 0;

    // Iterative helper methods
    Node* Get(Node *n, const K &key);

    // Recursive helper methods
    Node* Min(Node *n);
    void Insert(std::unique_ptr<Node> &n, const K &key, const V &value);
    void Remove(std::unique_ptr<Node> &n, const K &key);
    void Print(Node *n);

    // Helper methods for the self-balancing
    bool IsRed(Node *n);
    void FlipColors(Node *n);
    void RotateRight(std::unique_ptr<Node> &prt);
    void RotateLeft(std::unique_ptr<Node> &prt);
    void FixUp(std::unique_ptr<Node> &n);
    void MoveRedRight(std::unique_ptr<Node> &n);
    void MoveRedLeft(std::unique_ptr<Node> &n);
    void DeleteMin(std::unique_ptr<Node> &n);
};

template <typename K, typename V>
unsigned int Multimap<K, V>::Size() {
    return cur_size;
}

template <typename K, typename V>
typename Multimap<K, V>::Node* Multimap<K, V>::Get(Node *n, const K &key) {
    while (n) {
        if (key == n->key)
            return n;
        if (key < n->key)
            n = n->left.get();
        else
            n = n->right.get();
    }
    return nullptr;
}

// method has been updated to return the first value stored in the key's list
template <typename K, typename V>
const V& Multimap<K, V>::Get(const K &key) {
    Node *n = Get(root.get(), key);
    if (!n)
        throw std::runtime_error("Error: cannot find key");
    return n->values.front();
}

template <typename K, typename V>
bool Multimap<K, V>::Contains(const K &key) {
    return Get(root.get(), key) != nullptr;
}

template <typename K, typename V>
const K& Multimap<K, V>::Max(void) {
    if (root == nullptr)
        throw std::runtime_error ("Empty Multimap!");
    Node *n = root.get();
    while (n->right) n = n->right.get();
    return n->key;
}

template <typename K, typename V>
const K& Multimap<K, V>::Min(void) {
    if (root == nullptr)
        throw std::runtime_error ("Empty Multimap!");
    return Min(root.get())->key;
}

template <typename K, typename V>
typename Multimap<K, V>::Node* Multimap<K, V>::Min(Node *n) {
    if (n->left)
        return Min(n->left.get());
    else
        return n;
}

template <typename K, typename V>
bool Multimap<K, V>::IsRed(Node *n) {
    if (!n) return false;
    return (n->color == RED);
}

template <typename K, typename V>
void Multimap<K, V>::FlipColors(Node *n) {
    n->color = !n->color;
    n->left->color = !n->left->color;
    n->right->color = !n->right->color;
}

template <typename K, typename V>
void Multimap<K, V>::RotateRight(std::unique_ptr<Node> &prt) {
    std::unique_ptr<Node> chd = std::move(prt->left);
    prt->left = std::move(chd->right);
    chd->color = prt->color;
    prt->color = RED;
    chd->right = std::move(prt);
    prt = std::move(chd);
}

template <typename K, typename V>
void Multimap<K, V>::RotateLeft(std::unique_ptr<Node> &prt) {
    std::unique_ptr<Node> chd = std::move(prt->right);
    prt->right = std::move(chd->left);
    chd->color = prt->color;
    prt->color = RED;
    chd->left = std::move(prt);
    prt = std::move(chd);
}

template <typename K, typename V>
void Multimap<K, V>::FixUp(std::unique_ptr<Node> &n) {
    // Rotate left if there is a right-leaning red node
    if (IsRed(n->right.get()) && !IsRed(n->left.get()))
        RotateLeft(n);
    // Rotate right if red-red pair of nodes on left
    if (IsRed(n->left.get()) && IsRed(n->left->left.get()))
        RotateRight(n);
    // Recoloring if both children are red
    if (IsRed(n->left.get()) && IsRed(n->right.get()))
        FlipColors(n.get());
}

template <typename K, typename V>
void Multimap<K, V>::MoveRedRight(std::unique_ptr<Node> &n) {
    FlipColors(n.get());

    if (IsRed(n->left->left.get())) {
        RotateRight(n);
        FlipColors(n.get());
  }
}

template <typename K, typename V>
void Multimap<K, V>::MoveRedLeft(std::unique_ptr<Node> &n) {
    FlipColors(n.get());
    if (IsRed(n->right->left.get())) {
        RotateRight(n->right);
        RotateLeft(n);
        FlipColors(n.get());
    }
}

template <typename K, typename V>
void Multimap<K, V>::DeleteMin(std::unique_ptr<Node> &n) {
  // No left child, min is 'n'
    if (!n->left) {
        // Remove n
        n = nullptr;
        return;
    }

    if (!IsRed(n->left.get()) && !IsRed(n->left->left.get()))
        MoveRedLeft(n);

    DeleteMin(n->left);

    FixUp(n);
}

template <typename K, typename V>
void Multimap<K, V>::Remove(const K &key) {
    if (!Contains(key))
        return;

    Node* n = Get(root.get(), key);

    if (n -> values.size() > 1) {
        n -> values.erase(n->values.begin());
        cur_size--;
    }
    else {
        Remove(root, key);
        cur_size--;
    }

    if (root)
        root->color = BLACK;
}

// method has been updated to delete the node only when there is one value associated with it
// otherwise, it only deletes the first element in the list of values associated with the key
template <typename K, typename V>
void Multimap<K, V>::Remove(std::unique_ptr<Node> &n, const K &key) {
  // Key not found
    if (!n) return;

    if (key < n->key) {
        if (!IsRed(n->left.get()) && !IsRed(n->left->left.get()))
            MoveRedLeft(n);
        Remove(n->left, key);
    }  else {
            if (IsRed(n->left.get()))
                RotateRight(n);

            if (key == n->key && !n->right) {
                // Remove n
                n = nullptr;
                return;
            }

            if (!IsRed(n->right.get()) && !IsRed(n->right->left.get()))
                MoveRedRight(n);

            if (key == n->key) {
                // Find min node in the right subtree
                Node *n_min = Min(n->right.get());
                // Copy content from min node
                n->key = n_min->key;
                n->values = n_min->values;
                // Delete min node recursively
                 n->right = std::move(n_min->right);
                ////DeleteMin(n->right);
                //cur_size--;
            } else {
                Remove(n->right, key);
            }
        }

    FixUp(n);
}

template <typename K, typename V>
void Multimap<K, V>::Insert(const K &key, const V &value) {
    Insert(root, key, value);
    cur_size++;
    root->color = BLACK;
}

// insert function updated: pushes value into the list associated to the key instead of throwing a runtime error.
template <typename K, typename V>
void Multimap<K, V>::Insert(std::unique_ptr<Node> &n, const K &key, const V &value) {
    if (!n) {
        n = std::unique_ptr<Node>(new Node(key, value, RED));
    }
    else if (key < n->key)
        Insert(n->left, key, value);
    else if (key > n->key)
        Insert(n->right, key, value);
    else    // changes made here
        n -> values.push_back (value);

  FixUp(n);
}

template <typename K, typename V>
void Multimap<K, V>::Print() {
    Print(root.get());
    std::cout << std::endl;
}

template <typename K, typename V>
void Multimap<K, V>::Print(Node *n) {
    if (!n) return;
    Print(n->left.get());
    std::cout << "<" << n->key << ": ";
    for (int i = 0; i < n->values.size() - 1; i++)
        std::cout << n -> values [i] << ", ";
    std::cout << n -> values[n->values.size() - 1];
    std::cout << "> ";
    Print(n->right.get());
}

#endif  // MAP_H_
