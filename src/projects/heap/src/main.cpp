#include <fstream>
#include <iostream>

#include "heap.h"
#include "tester.h"

using namespace std;
using namespace heap;

int main()
{
    ifstream in("tests/1.txt");
    LeftistNode<int>::LeftistHeap leftistHeap;
    PairingNode<int>::PairingHeap pairingHeap;
    int value, numberOfValues;

    in >> numberOfValues;
    for (int index = 1; index <= numberOfValues; index++)
    {
        in >> value;
        leftistHeap.push(value);
        pairingHeap.push(value);
    }

    cout << leftistHeap;
    cout << endl;
    cout << pairingHeap << endl;

    // cout << pairingHeap.top() << endl;
    // pairingHeap.pop();
    // cout << pairingHeap << endl;
    return 0;
}