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
        LeftistNode(KeyType key, int distance = 0, LeftistNode *left = nullptr, LeftistNode *right = nullptr) : key(key), distance(distance), left(left), right(right) {}

        friend ostream &operator<<(ostream &out, LeftistNode &node)
        {
            out << node.key << " " << node.distance;
            if (node.left != nullptr || node.right != nullptr)
                out << " -> " << (node.left != nullptr ? to_string(node.left->key) : "|")
                    << " " << (node.right != nullptr ? to_string(node.right->key) : "|");
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
                if (a == nullptr)
                    return b;
                if (b == nullptr)
                    return a;
                if (a->key < b->key)
                    swap(a, b);
                if (a->left == nullptr)
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
                if (node != nullptr)
                {
                    freeMemory(node->left);
                    freeMemory(node->right);
                    delete node;
                }
            }
            void print(ostream &out, LeftistNode *node)
            {
                out << *node;
                if (node->left != nullptr)
                    print(out, node->left);
                if (node->right != nullptr)
                    print(out, node->right);
            }

        public:
            LeftistHeap() { root = nullptr; }
            LeftistHeap(KeyType key, int distance = 0) : root(new LeftistNode(key, distance)) {}
            LeftistHeap(LeftistNode *root) : root(root) {}
            ~LeftistHeap()
            {
                freeMemory(root);
                root = nullptr;
            }

            friend ostream &operator<<(ostream &out, LeftistHeap &heap)
            {
                heap.print(out, heap.root);
                return out;
            }

            KeyType top()
            {
                if (root == nullptr)
                    throw(runtime_error("Can't get maximum! Heap is empty!"));
                return root->key;
            }
            void push(KeyType key) { root = mergeHeap(root, new LeftistNode(key)); }
            void pop()
            {
                if (this->empty())
                    throw(runtime_error("Can't pop! Heap is empty!"));
                LeftistNode *tmp = root;
                root = mergeHeap(root->left, root->right);
                delete tmp;
            }
            bool empty() { return (root == nullptr); }
            void modifyKey(LeftistNode *modifiedNode, KeyType newKey)
            {
                LeftistNode *tmp = modifiedNode;
                modifiedNode = mergeHeap(modifiedNode->left, modifiedNode->right);
                delete tmp;
                push(newKey);
            }
        };
    };

    template <typename KeyType>
    class PairingNode
    {
    private:
        KeyType key;
        PairingNode *child, *sibling;

    public:
        PairingNode(KeyType key, PairingNode *child = nullptr, PairingNode *sibling = nullptr) : key(key), child(child), sibling(sibling) {}

        friend ostream &operator<<(ostream &out, PairingNode &node)
        {
            out << node.key;
            if (node.child != nullptr || node.sibling != nullptr)
                out << " -> " << (node.child != nullptr ? to_string(node.child->key) : "|")
                    << " " << (node.sibling != nullptr ? to_string(node.sibling->key) : "|");
            out << endl;
            return out;
        }

        KeyType getKey() { return key; }
        void setKey(KeyType key) { key = key; }

        PairingNode *getChild() { return child; }
        void setChild(PairingNode *child) { child = child; }

        PairingNode *getSibling() { return sibling; }
        void setSibling(PairingNode *sibling) { sibling = sibling; }

        class PairingHeap
        {
        private:
            PairingNode *root;

            PairingNode *mergeHeap(PairingNode *a, PairingNode *b)
            {
                if (a == nullptr)
                    return b;
                if (b == nullptr)
                    return a;
                if (a->key < b->key)
                    swap(a, b);
                if (a->child != nullptr)
                    b->sibling = a->child;
                a->child = b;
                return a;
            }
            PairingNode *twoPassMerge(PairingNode *node)
            {
                if (node == nullptr || node->sibling == nullptr)
                    return node;
                PairingNode *nodeOne = node;
                PairingNode *nodeTwo = node->sibling;
                PairingNode *nextNode = node->sibling->sibling;
                nodeOne->sibling = nodeTwo->sibling = nullptr;
                return mergeHeap(mergeHeap(nodeOne, nodeTwo), twoPassMerge(nextNode));
            }
            void freeMemory(PairingNode *node)
            {
                if (node != nullptr)
                {
                    freeMemory(node->child);
                    freeMemory(node->sibling);
                    delete node;
                }
            }
            void print(ostream &out, PairingNode *node)
            {
                out << *node;
                if (node->child != nullptr)
                    print(out, node->child);
                if (node->sibling != nullptr)
                    print(out, node->sibling);
            }

        public:
            PairingHeap() : root(nullptr) {}
            PairingHeap(KeyType key) : root(new PairingNode(key)) {}
            PairingHeap(PairingNode *root) : root(root) {}
            ~PairingHeap()
            {
                freeMemory(root);
                root = nullptr;
            }

            friend ostream &operator<<(ostream &out, PairingHeap &heap)
            {
                heap.print(out, heap.root);
                return out;
            }

            KeyType top()
            {
                if (root == nullptr)
                    throw(runtime_error("Can't get maximum! Heap is empty!"));
                return root->key;
            }
            void push(KeyType key) { root = mergeHeap(root, new PairingNode(key)); }
            void pop()
            {
                if (this->empty())
                    throw(runtime_error("Can't pop! Heap is empty!"));
                PairingNode *tmp = root;
                root = twoPassMerge(root->child);
                delete tmp;
            }
            bool empty() { return (root == nullptr); }
            void modifyKey(PairingNode<KeyType> *modifiedNode, KeyType newKey)
            {
                if (modifiedNode->child != nullptr)
                {
                    PairingNode *tmp = modifiedNode;
                    modifiedNode = twoPassMerge(modifiedNode->child);
                    delete tmp;
                }
                else
                {
                    if (modifiedNode->leftSibling != nullptr)
                        modifiedNode->leftSibling->rightSibling = modifiedNode->rightSibling;
                    if (modifiedNode->rightSibling != nullptr)
                        modifiedNode->rightSibling->leftSibling = modifiedNode->leftSibling;

                    delete modifiedNode;
                    modifiedNode = nullptr;
                }
                push(newKey);
            }
        };
    };

    template <class V>
    class FibonacciHeap;

    const double INF_fibonacci_heap = 2000000001;

    template <class V>
    struct node
    {
        node<V> *left;
        node<V> *right;
        node<V> *child;
        node<V> *parent;
        V val;
        bool marked;
        int degree;
    };

    template <class V>
    class FibonacciHeap
    {
    private:
        node<V> *maxNode;
        node<V> *rootList;

        node<V> *constructNode(V value)
        {
            auto *newNode = new node<V>;
            newNode->left = newNode;
            newNode->right = newNode;
            newNode->child = nullptr;
            newNode->parent = nullptr;
            newNode->degree = 0;
            newNode->val = value;
            newNode->marked = false;
            return newNode;
        }
        void mergeWithRoot(node<V> *mergedNode)
        {
            if (rootList == nullptr)
                rootList = mergedNode;
            else
            {
                mergedNode->right = rootList;
                mergedNode->left = rootList->left;
                rootList->left->right = mergedNode;
                rootList->left = mergedNode;
            }
        }

        void removeFromRoot(node<V> *removedNode)
        {
            if (removedNode == rootList)
                rootList = removedNode->right;
            removedNode->left->right = removedNode->right;
            removedNode->right->left = removedNode->left;
        }

        void removeFromChildren(node<V> *removedChild, node<V> *parent)
        {
            if (parent->child == parent->child->right)
                parent->child = nullptr;
            else if (parent->child == removedChild)
            {
                parent->child = removedChild->right;
                removedChild->right->parent = parent;
            }
            removedChild->left->right = removedChild->right;
            removedChild->right->left = removedChild->left;
        }

        void mergeWithChild(node<V> *newChild, node<V> *parent)
        {

            if (parent->child == nullptr)
                parent->child = newChild;
            else
            {
                // Inserez mereu la dreapta primului copil
                newChild->right = parent->child->right;
                newChild->left = parent->child;
                parent->child->right->left = newChild;
                parent->child->right = newChild;
            }
        }

        void heapLink(node<V> *child, node<V> *parent)
        {
            removeFromRoot(child);
            child->left = child->right = child;
            parent->degree++;
            mergeWithChild(child, parent);
            child->parent = parent;
        }

        void cleanUp()
        {
            // magic number 128 = 64 bits x 2
            // 64 seems to be working just fine tho
            // increase to 128 for bigger values?
            std::vector<node<V> *> degreeTable = {64, nullptr};
            std::vector<node<V> *> rootNodes = {rootList};

            node<V> *p = rootList->right;
            while (p != rootList)
            {
                rootNodes.push_back(p);
                p = p->right;
            }

            for (auto rootNode : rootNodes)
            {
                int deg = rootNode->degree;
                while (degreeTable[deg] != nullptr)
                {
                    node<V> *degNode = degreeTable[deg];
                    if (rootNode->val < degNode->val)
                        std::swap(rootNode, degNode);
                    heapLink(degNode, rootNode);
                    degreeTable[deg] = nullptr;
                    deg++;
                }
                degreeTable[deg] = rootNode;
            }
            for (int i = 0; i < 64; i++)
                if (degreeTable[i] != nullptr)
                    if (degreeTable[i]->val > maxNode->val)
                        maxNode = degreeTable[i];
        }

        void cut(node<V> *removedChild, node<V> *parent)
        {
            removeFromChildren(removedChild, parent);
            parent->degree -= 1;
            mergeWithRoot(removedChild);
            removedChild->parent = NULL;
            removedChild->marked = false;
        }

        void cascadingCut(node<V> *currentNode)
        {
            node<V> *currentParent = currentNode->parent;
            if (currentParent != NULL)
            {
                if (!currentNode->marked)
                    currentNode->marked = true;
                else
                {
                    cut(currentNode, currentParent);
                    cascadingCut(currentParent);
                }
            }
        }
        void freeMemory(node<V> *x)
        {
            if (x != nullptr)
            {
                node<V> *t1 = x;
                do
                {
                    node<V> *t2 = t1;
                    t1 = t1->right;
                    freeMemory(t2->child);
                    delete t2;
                } while (t1 != x);
            }
        }

    public:
        FibonacciHeap()
        {
            maxNode = nullptr;
            rootList = nullptr;
        }
        ~FibonacciHeap()
        {
            freeMemory(rootList);
            rootList = nullptr;
            maxNode = nullptr;
        }

        void insert(V insertedValue)
        {
            node<V> *insertedNode = constructNode(insertedValue);

            mergeWithRoot(insertedNode);

            if (maxNode == nullptr || maxNode->val < insertedValue)
                maxNode = insertedNode;
        }

        void merge(FibonacciHeap<V> *other)
        {

            if (rootList == nullptr)
            {
                rootList = other->rootList;
                maxNode = other->maxNode;
            }
            else if (other->rootList != nullptr)
            {

                node<V> *last = other->rootList->left;   // ultimul nod dupa merge
                other->rootList->left = rootList->left;  // rootList->left = ultimul din primul heap
                rootList->left->right = other->rootList; // ult din primul heap ->left = other.rootList
                rootList->left = last;                   // rootList->left = ultimul nod dupa merge
                rootList->left->right = rootList;        // ultimul nod dupam merge ->right = rootList

                // maxNode = max(minNode, other.minNode)
                if (maxNode->val < other->maxNode->val)
                    maxNode = other->maxNode;
            }
        }

        V getMaximum()
        {
            if (maxNode == nullptr)
                throw(runtime_error("Can't get maximum! Heap is empty!"));
            return maxNode->val;
        };

        V extractMax()
        {
            node<V> *z = maxNode;
            if (z != nullptr)
            {
                V maxVal = 0;
                if (z->child != nullptr)
                {
                    node<V> *p = rootList->right;

                    // AICI PT UN CAZ AM Z->DEGREE MAI MARE CU 1 DECAT AR TREBUI SA FIE
                    // NU-MI DAU SEAMA DE CE
                    std::vector<node<V> *> children = {};
                    node<V> *currentChild = z->child;
                    do
                    {
                        auto temp = currentChild;
                        // std::cout << currentChild->val << "-> child. \n";
                        children.push_back(temp);
                        currentChild = currentChild->right;
                    } while (currentChild != z->child);

                    for (auto child : children)
                    {
                        mergeWithRoot(child);
                        child->parent = nullptr;
                    }
                }

                removeFromRoot(z);

                if (z == z->right)
                {
                    // Am extras dintr-un heap cu un singur element (care era si minim)
                    maxNode = nullptr;
                    rootList = nullptr;
                }
                else
                {
                    maxNode = z->right;
                    cleanUp();
                }

                maxVal = z->val;
                delete z;

                return maxVal;
            }
            else
                throw(runtime_error("Can't pop! Heap is empty!"));
        }

        void increaseValue(node<V> *incNode, V incValue)
        {
            if (incValue < incNode->val)
                return;
            incNode->val = incValue;
            node<V> *parentNode = incNode->parent;
            if (parentNode != NULL && incNode->val > parentNode->val)
            {
                cut(incNode, parentNode);
                cascadingCut(parentNode);
            }
            if (incValue > maxNode->val)
                maxNode = incNode;
        }

        node<V> *deleteNode(node<V> *delNode)
        {
            decreaseValue(delNode, INF_fibonacci_heap);
            return extractMax();
        }
    };
};