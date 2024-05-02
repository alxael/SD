#include <iostream>

using namespace std;

namespace heap
{
    template <typename KeyType>
    class LeftistNode
    {
    private:
        KeyType key;
        int distance;
        LeftistNode *left, *right;

    public:
        LeftistNode(KeyType key, int distance = 0, LeftistNode *left = NULL, LeftistNode *right = NULL) : key(key), distance(distance), left(left), right(right) {}

        friend ostream &operator<<(ostream &out, LeftistNode &node)
        {
            out << node.key << " " << node.distance;
            if (node.left != NULL || node.right != NULL)
                out << " -> " << (node.left != NULL ? to_string(node.left->key) : "|")
                    << " " << (node.right != NULL ? to_string(node.right->key) : "|");
            out << endl;
            return out;
        }

        KeyType getKey() { return key; }
        void setKey(KeyType key) { key = key; }

        int getDistance() { return distance; }
        void setDistance(int distance) { distance = distance; }

        LeftistNode *getLeft() { return left; }
        void setLeft(LeftistNode *left) { left = left; }

        LeftistNode *getRight() { return right; }
        void setRight(LeftistNode *right) { right = right; }

        class LeftistHeap
        {
        private:
            LeftistNode *root;

            LeftistNode *mergeHeap(LeftistNode *a, LeftistNode *b)
            {
                if (a == NULL)
                    return b;
                if (b == NULL)
                    return a;
                if (a->key > b->key)
                    swap(a, b);
                if (a->left == NULL)
                    a->left = b;
                else
                {
                    a->right = mergeHeap(a->right, b);
                    if (a->right->distance > a->left->distance)
                        swap(a->left, a->right);
                    a->distance = 1 + a->right->distance;
                }
                return a;
            }
            void freeMemory(LeftistNode *node)
            {
                if (node != NULL)
                {
                    freeMemory(node->left);
                    freeMemory(node->right);
                    delete node;
                }
            }
            void print(ostream &out, LeftistNode *node)
            {
                out << *node;
                if (node->left != NULL)
                    print(out, node->left);
                if (node->right != NULL)
                    print(out, node->right);
            }

        public:
            LeftistHeap() { root = NULL; }
            LeftistHeap(KeyType key, int distance = 0) : root(new LeftistNode(key, distance)) {}
            LeftistHeap(LeftistNode *root) : root(root) {}
            ~LeftistHeap()
            {
                freeMemory(root);
                root = NULL;
            }

            friend ostream &operator<<(ostream &out, LeftistHeap &heap)
            {
                heap.print(out, heap.root);
                return out;
            }

            KeyType top() { return root->key; }
            void push(KeyType key) { root = mergeHeap(root, new LeftistNode(key)); }
            void pop()
            {
                LeftistNode *tmp = root;
                root = mergeHeap(root->left, root->right);
                delete tmp;
            }
            bool empty() { return (root == NULL); }
        };
    };

    template <typename KeyType>
    class PairingNode
    {
    private:
        KeyType key;
        PairingNode *child, *sibling;

    public:
        PairingNode(KeyType key, PairingNode *child = NULL, PairingNode *sibling = NULL) : key(key), child(child), sibling(sibling) {}

        friend ostream &operator<<(ostream &out, PairingNode &node)
        {
            out << node.key;
            if (node.child != NULL || node.sibling != NULL)
                out << " -> " << (node.child != NULL ? to_string(node.child->key) : "|")
                    << " " << (node.sibling != NULL ? to_string(node.sibling->key) : "|");
            out << endl;
            return out;
        }

        KeyType getKey() { return key; }
        void setKey(KeyType key) { key = key; }

        PairingNode *getLeft() { return left; }
        void setLeft(PairingNode *left) { left = left; }

        PairingNode *getRight() { return right; }
        void setRight(PairingNode *right) { right = right; }

        class PairingHeap
        {
        private:
            PairingNode *root;

            PairingNode *mergeHeap(PairingNode *a, PairingNode *b)
            {
                if (a == NULL)
                    return b;
                if (b == NULL)
                    return a;
                if (a->key > b->key)
                    swap(a, b);
                if (a->child != NULL)
                    b->sibling = a->child;
                a->child = b;
                return a;
            }
            PairingNode *twoPassMerge(PairingNode *node)
            {
                if (node == NULL || node->sibling == NULL)
                    return node;
                PairingNode *nodeOne = node;
                PairingNode *nodeTwo = node->sibling;
                PairingNode *nextNode = node->sibling->sibling;
                nodeOne->sibling = nodeTwo->sibling = NULL;
                return mergeHeap(mergeHeap(nodeOne, nodeTwo), twoPassMerge(nextNode));
            }
            void freeMemory(PairingNode *node)
            {
                if (node != NULL)
                {
                    freeMemory(node->child);
                    freeMemory(node->sibling);
                    delete node;
                }
            }
            void print(ostream &out, PairingNode *node)
            {
                out << *node;
                if (node->child != NULL)
                    print(out, node->child);
                if (node->sibling != NULL)
                    print(out, node->sibling);
            }

        public:
            PairingHeap() : root(NULL) {}
            PairingHeap(KeyType key) : root(new PairingNode(key)) {}
            PairingHeap(PairingNode *root) : root(root) {}
            ~PairingHeap()
            {
                freeMemory(root);
                root = NULL;
            }

            friend ostream &operator<<(ostream &out, PairingHeap &heap)
            {
                heap.print(out, heap.root);
                return out;
            }

            KeyType top() { return root->key; }
            void push(KeyType key) { root = mergeHeap(root, new PairingNode(key)); }
            void pop()
            {
                PairingNode *tmp = root;
                root = twoPassMerge(root->child);
                delete tmp;
            }
            bool empty() { return (root == NULL); }
        };
    };
};