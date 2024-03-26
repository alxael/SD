#include <queue>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

namespace ds {
    template<typename T>
    class Vector {
    private:
        int dim;
        T *v;

        void freeArray() {
            if(dim)
                delete v;
        }

        ///functie care imi interclaseaza subvectorii:
        ///v1: subvectorul de la pozitia st la mij al vectorului tata
        ///v2: subvectorul de la pozitia mij + 1 la dr al vectorului tata
        ///in acelasi vector tata, pe intervalul [st, dr]
        void combine(int st, int mij, int dr) {
            ///voi crea doi vectori temporari (indexati de la 1)
            int dimSt = mij - st + 1;
            int dimDr = dr - (mij + 1) + 1;
            auto *stArr = new T[dimSt + 1];
            auto *drArr = new T[dimDr + 1];

            for (int i = 1; i <= dimSt; i++) {
                stArr[i] = v[st + i - 1];
            }
            for (int i = 1; i <= dimDr; i++) {
                drArr[i] = v[(mij + 1) + i - 1];
            }

            int indexSt = 1;
            int indexDr = 1;
            int indexV = st;
            while (indexSt <= dimSt && indexDr <= dimDr) {
                if (stArr[indexSt] <= drArr[indexDr]) {
                    v[indexV] = stArr[indexSt];
                    indexSt++;
                } else {
                    v[indexV] = drArr[indexDr];
                    indexDr++;
                }
                indexV++;
            }

            for (int i = indexSt; i <= dimSt; i++) {
                v[indexV] = stArr[i];
                indexV++;
            }
            for (int i = indexDr; i <= dimDr; i++) {
                v[indexV] = drArr[i];
                indexV++;
            }

            delete[] stArr;
            delete[] drArr;
        }

        void countSort(int exp, int base) {
            int output[dim + 1];
            int ct[base] = {0};

            for (int i = 1; i <= dim; i++) {
                int cifra = ((int) (v[i] / exp)) % base;
                ct[cifra]++;
            }

            for (int i = 1; i <= base - 1; i++) {
                ct[i] = ct[i] + ct[i - 1];
            }

            for (int i = dim; i >= 1; i--) {
                int cifra = ((int) (v[i] / exp)) % base;
                output[ct[cifra]] = v[i];
                ct[cifra]--;
            }


            for (int i = 1; i <= dim; i++) {
                v[i] = output[i];
            }

        }

        void mergeSortUtil(int st, int dr) {
            if (st >= dr) {
                return;
            }

            int mij = st + (dr - st) / 2;
            mergeSortUtil(st, mij);
            mergeSortUtil(mij + 1, dr);
            combine(st, mij, dr);
        }

        void heapSortUtil(int st, int dr) {
            int N = dr - st + 1;
            priority_queue<T, vector<T>, greater<T>> X;
            for (int i = st; i <= dr; i++) {
                X.push(v[i]);
            }

            for (int i = st; i <= dr; i++) {
                v[i] = X.top();
                X.pop();
            }
        }

        void quickSortUtil(int st, int dr) {
            if (st >= dr) {
                return;
            }

            int pi = partition(st, dr);

            quickSortUtil(st, pi - 1);
            quickSortUtil(pi + 1, dr);
        }

        void insertionSortUtil(int st, int dr) {
            for (int i = st; i <= dr; i++) {
                T temp = v[i];
                int k = i - 1;
                while (temp < v[k] && k >= st) {
                    v[k + 1] = v[k];
                    k--;
                }
                v[k + 1] = temp;
            }
        }

        void introSortUtil(int st, int dr, int depthLimit) {
            if (st >= dr) {
                return;
            }

            int N = dr - st + 1;

            if (N <= 16) {
                insertionSortUtil(st, dr);
            }
            if (depthLimit == 0) {
                heapSortUtil(st, dr);
            } else {
                int pi = partition(st, dr);
                introSortUtil(st, pi - 1, depthLimit - 1);
                introSortUtil(pi + 1, dr, depthLimit - 1);
            }
        }

        int partition(int st, int dr) {
            T pivot = v[dr];
            int lastBun = st - 1;
            for (int i = st; i <= dr; i++) {
                if (v[i] < pivot) {
                    lastBun++;
                    swap(v[i], v[lastBun]);
                }
            }
            swap(v[lastBun + 1], v[dr]);
            return lastBun + 1;
        }

    public:
        Vector() { dim = 0; }

        Vector(int x) {
            freeArray();
            dim = x;
            v = new T[dim + 1];
            v[0] = 0;

            for (int i = 1; i <= dim; i++) {
                v[i] = 0;
            }
        }

        Vector(int N, T w[]) {
            freeArray();
            dim = N;
            v = new T[dim + 1];
            v[0] = 0;

            for (int i = 1; i <= dim; i++) {
                v[i] = w[i];
            }
        }

        ~Vector() {
            freeArray();
        }

        void update(int N, T w[]) {
            freeArray();
            dim = N;
            v = new T[dim + 1];
            v[0] = 0;

            for (int i = 1; i <= N; i++) {
                v[i] = w[i];
            }
        }

        friend istream &operator>>(istream &in, Vector &X) {
            in >> X.dim;
            X.v = new T[X.dim + 1];
            X.v[0] = 0;


            for (int i = 1; i <= X.dim; i++) {
                in >> X.v[i];
            }

            return in;
        }

        friend ostream &operator<<(ostream &out, Vector const &X) {
            out << "Vectorul are " << X.dim << " elemente:" << "\n";
            for (int i = 1; i <= X.dim; i++) {
                out << X.v[i] << " ";
            }
            out << "\n";
            return out;
        }

        Vector &operator=(Vector<T> const &X) {
            freeArray();
            dim = X.dim;
            v = new T[dim + 1];
            v[0] = 0;
            for (int i = 1; i <= dim; i++) {
                v[i] = X.v[i];
            }
            return *this;
        }

        T getMax() {
            T mx = v[dim];
            for (int i = 1; i <= dim; i++) {
                if (v[i] > mx) {
                    mx = v[i];
                }
            }
            return mx;
        }

        void radixSort(int base) {
            ///doar daca T = positive int!
            ///altfel nu am cum
            if (!(std::is_same<T, int>::value)) {
                //throw (runtime_error("You may only perform radix sort on natural numbers."));
                return;
            }

            for(int i = 1; i <= dim; i++){
                if(v[i] < 0){
                    //throw (runtime_error("You may only perform radix sort on natural numbers."));
                    return;
                }
            }

            int mx = getMax();

            for (int exp = 1; mx / exp > 0; exp = exp * base)
                countSort(exp, base);
        }

        void mergeSort() {
            mergeSortUtil(1, dim);
        }

        void shellSort() {
            for (int gap = dim / 2; gap >= 1; gap = gap / 2) {
                for (int i = gap + 1; i <= dim; i++) {
                    T temp = v[i];

                    int j;
                    for (j = i; j - gap >= 1 && v[j - gap] > temp; j = j - gap) {
                        v[j] = v[j - gap];
                    }
                    v[j] = temp;
                }
            }
        }

        void heapSort() {
            heapSortUtil(1, dim);
        }

        void quickSort() {
            quickSortUtil(1, dim);
        }

        void insertionSort() {
            insertionSortUtil(1, dim);
        }

        void introSort() {
            int depthLimit = 2 * floor(log(dim));
            introSortUtil(1, dim, depthLimit);
        }

        void stlSort() {
            sort(v + 1, v + dim + 1);
        }
    };
}
