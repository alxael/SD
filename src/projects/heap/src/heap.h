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
            LeftistHeap(KeyType key, int distance = 0) : root(new LeftistNode(key, distance)) {}
            LeftistHeap() { root = NULL; }
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

};