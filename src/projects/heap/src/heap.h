#include <limits>
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

        KeyType getKey() { return key; }
        void setKey(KeyType key) { key = key; }

        int getDistance() { return distance; }
        void setDistance(int distance) { distance = distance; }

        LeftistNode *getLeft() { return left; }
        void setLeft(LeftistNode *left) { left = left; }

        LeftistNode *getRight() { return right; }
        void setRight(LeftistNode *right) { right = right; }

        template <typename ValueType>
        friend class LeftistHeap;
    };

    template <typename KeyType>
    class LeftistHeap
    {
    private:
        LeftistNode<KeyType> *root;

        LeftistNode<KeyType> *mergeHeap(LeftistNode<KeyType> *a, LeftistNode<KeyType> *b)
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
        void freeMemory(LeftistNode<KeyType> *node)
        {
            if (node != nullptr)
            {
                freeMemory(node->left);
                freeMemory(node->right);
                delete node;
            }
        }
        void print(ostream &out, LeftistNode<KeyType> *node)
        {
            out << *node;
            if (node->left != nullptr)
                print(out, node->left);
            if (node->right != nullptr)
                print(out, node->right);
        }

    public:
        LeftistHeap() { root = nullptr; }
        LeftistHeap(KeyType key, int distance = 0) : root(new LeftistNode<KeyType>(key, distance)) {}
        LeftistHeap(LeftistNode<KeyType> *root) : root(root) {}
        ~LeftistHeap()
        {
            freeMemory(root);
            root = nullptr;
        }

        friend ostream &operator<<(ostream &out, LeftistNode<KeyType> &heap)
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
        void push(KeyType key) { root = mergeHeap(root, new LeftistNode<KeyType>(key)); }
        void pop()
        {
            if (this->empty())
                throw(runtime_error("Can't pop! Heap is empty!"));
            LeftistNode<KeyType> *tmp = root;
            root = mergeHeap(root->left, root->right);
            delete tmp;
        }
        bool empty() { return (root == nullptr); }
        void modifyKey(LeftistNode<KeyType> *modifiedNode, KeyType newKey)
        {
            LeftistNode<KeyType> *tmp = modifiedNode;
            modifiedNode = mergeHeap(modifiedNode->left, modifiedNode->right);
            delete tmp;
            push(newKey);
        }
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

        template <typename ValueType>
        friend class PairingHeap;
    };

    template <typename KeyType>
    class PairingHeap
    {
    private:
        PairingNode<KeyType> *root;

        PairingNode<KeyType> *mergeHeap(PairingNode<KeyType> *a, PairingNode<KeyType> *b)
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
        PairingNode<KeyType> *twoPassMerge(PairingNode<KeyType> *node)
        {
            if (node == nullptr || node->sibling == nullptr)
                return node;
            PairingNode<KeyType> *nodeOne = node;
            PairingNode<KeyType> *nodeTwo = node->sibling;
            PairingNode<KeyType> *nextNode = node->sibling->sibling;
            nodeOne->sibling = nodeTwo->sibling = nullptr;
            return mergeHeap(mergeHeap(nodeOne, nodeTwo), twoPassMerge(nextNode));
        }
        void freeMemory(PairingNode<KeyType> *node)
        {
            if (node != nullptr)
            {
                freeMemory(node->child);
                freeMemory(node->sibling);
                delete node;
            }
        }
        void print(ostream &out, PairingNode<KeyType> *node)
        {
            out << *node;
            if (node->child != nullptr)
                print(out, node->child);
            if (node->sibling != nullptr)
                print(out, node->sibling);
        }

    public:
        PairingHeap() : root(nullptr) {}
        PairingHeap(KeyType key) : root(new PairingNode<KeyType>(key)) {}
        PairingHeap(PairingNode<KeyType> *root) : root(root) {}
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
        void push(KeyType key) { root = mergeHeap(root, new PairingNode<KeyType>(key)); }
        void pop()
        {
            if (this->empty())
                throw(runtime_error("Can't pop! Heap is empty!"));
            PairingNode<KeyType> *tmp = root;
            root = twoPassMerge(root->child);
            delete tmp;
        }
        bool empty() { return (root == nullptr); }
        void modifyKey(PairingNode<KeyType> *modifiedNode, KeyType newKey)
        {
            if (modifiedNode->child != nullptr)
            {
                PairingNode<KeyType> *tmp = modifiedNode;
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


    template <class KeyType>
    class FibonacciNode
    {
    private:
        FibonacciNode<KeyType> *left;
        FibonacciNode<KeyType> *right;
        FibonacciNode<KeyType> *child;
        FibonacciNode<KeyType> *parent;
        KeyType val;
        bool marked;
        int degree;

    public:
        FibonacciNode(KeyType value)
        {
            left = this;
            right = this;
            child = nullptr;
            parent = nullptr;
            degree = 0;
            val = value;
            marked = false;
        }

        template <typename ValueType>
        friend class FibonacciHeap;

        ~FibonacciNode() = default;
    };

    template <typename KeyType>
    class FibonacciHeap
    {
    private:
        FibonacciNode<KeyType> *maxNode;
        FibonacciNode<KeyType> *rootList;

        void mergeWithRoot(FibonacciNode<KeyType> *mergedNode)
        {
            mergedNode->parent = nullptr;
            if (rootList != nullptr)
            {
                mergedNode->right = rootList;
                mergedNode->left = rootList->left;
                rootList->left->right = mergedNode;
                rootList->left = mergedNode;
            }
            else
            {
                mergedNode->right = mergedNode;
                mergedNode->left = mergedNode;
                rootList = mergedNode;
            }
        }

        void removeFromRoot(FibonacciNode<KeyType> *removedNode)
        {
            if (removedNode == rootList)
                rootList = removedNode->right;
            if (removedNode->right != removedNode)
            {
                removedNode->left->right = removedNode->right;
                removedNode->right->left = removedNode->left;
            }
            if (removedNode->parent != nullptr)
            {
                if (removedNode->parent->degree == 1)
                    removedNode->parent->child = nullptr;
                else
                    removedNode->parent->child = removedNode->right;
                removedNode->parent->degree--;
            }
        }

        void removeFromChildren(FibonacciNode<KeyType> *removedChild, FibonacciNode<KeyType> *parent)
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

        void mergeWithChild(FibonacciNode<KeyType> *newChild, FibonacciNode<KeyType> *parent)
        {
            if (parent->degree == 0)
            {
                parent->child = newChild;
                newChild->right = newChild;
                newChild->left = newChild;
                newChild->parent = parent;
            }
            else
            {
                FibonacciNode<KeyType> *firstChild = parent->child;
                FibonacciNode<KeyType> *firstChildsLeft = firstChild->left;
                firstChild->left = newChild;
                newChild->right = firstChild;
                newChild->left = firstChildsLeft;
                firstChildsLeft->right = newChild;
            }
            newChild->parent = parent;
            parent->degree++;
        }

        void heapLink(FibonacciNode<KeyType> *child, FibonacciNode<KeyType> *parent)
        {
            removeFromRoot(child);
            mergeWithChild(child, parent);
            child->marked = false;
        }

        void cleanUp()
        {
            // magic number 128 = 64 bits x 2
            // 64 seems to be working just fine tho
            // increase to 128 for bigger values?
            vector<FibonacciNode<KeyType> *> degreeTable = {64, nullptr};
            vector<FibonacciNode<KeyType> *> rootNodes = {rootList};

            FibonacciNode<KeyType> *p = rootList->right;
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
                    FibonacciNode<KeyType> *degNode = degreeTable[deg];
                    if (rootNode->val < degNode->val)
                        std::swap(rootNode, degNode);
                    heapLink(degNode, rootNode);
                    degreeTable[deg] = nullptr;
                    deg++;
                }
                degreeTable[deg] = rootNode;
            }
            for (int index = 0; index < 64; index++)
                if (degreeTable[index] != nullptr)
                    if (degreeTable[index]->val > maxNode->val)
                        maxNode = degreeTable[index];
        }

        void cut(FibonacciNode<KeyType> *removedChild, FibonacciNode<KeyType> *parent)
        {
            removeFromChildren(removedChild, parent);
            parent->degree -= 1;
            mergeWithRoot(removedChild);
            removedChild->parent = nullptr;
            removedChild->marked = false;
        }

        void cascadingCut(FibonacciNode<KeyType> *currentNode)
        {
            FibonacciNode<KeyType> *currentParent = currentNode->parent;
            if (currentParent != nullptr)
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

        void freeMemory(FibonacciNode<KeyType> *x)
        {
            if (x != nullptr)
            {
                FibonacciNode<KeyType> *t1 = x;
                do
                {
                    FibonacciNode<KeyType> *t2 = t1;
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

        void push(KeyType insertedValue)
        {
            FibonacciNode<KeyType> *insertedNode = new FibonacciNode<KeyType>(insertedValue);
            mergeWithRoot(insertedNode);
            if (maxNode == nullptr || maxNode->val < insertedValue)
                maxNode = insertedNode;
        }

        void merge(FibonacciHeap<KeyType> *other)
        {

            if (rootList == nullptr)
            {
                rootList = other->rootList;
                maxNode = other->maxNode;
            }
            else if (other->rootList != nullptr)
            {

                FibonacciNode<KeyType> *last = other->rootList->left; // ultimul nod dupa merge
                other->rootList->left = rootList->left;               // rootList->left = ultimul din primul heap
                rootList->left->right = other->rootList;              // ult din primul heap ->left = other.rootList
                rootList->left = last;                                // rootList->left = ultimul nod dupa merge
                rootList->left->right = rootList;                     // ultimul nod dupam merge ->right = rootList

                // maxNode = max(minNode, other.minNode)
                if (maxNode->val < other->maxNode->val)
                    maxNode = other->maxNode;
            }
        }

        KeyType top()
        {
            if (maxNode == nullptr)
                throw(runtime_error("Can't get maximum! Heap is empty!"));
            return maxNode->val;
        };

        KeyType pop()
        {
            FibonacciNode<KeyType> *z = maxNode;
            if (z != nullptr)
            {
                KeyType maxVal;
                if (z->child != nullptr)
                {

                    FibonacciNode<KeyType> *currentChild = z->child;
                    do
                    {
                        auto temp = currentChild;
                        currentChild = currentChild->right;
                        mergeWithRoot(temp);
                    } while (currentChild != z->child);
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

        void increaseValue(FibonacciNode<KeyType> *incNode, KeyType incValue)
        {
            if (incValue < incNode->val)
                return;
            incNode->val = incValue;
            FibonacciNode<KeyType> *parentNode = incNode->parent;
            if (parentNode != NULL && incNode->val > parentNode->val)
            {
                cut(incNode, parentNode);
                cascadingCut(parentNode);
            }
            if (incValue > maxNode->val)
                maxNode = incNode;
        }

        FibonacciNode<KeyType> *deleteNode(FibonacciNode<KeyType> *delNode)
        {
            decreaseValue(delNode, numeric_limits<KeyType>::max());
            return pop();
        }
    };
};