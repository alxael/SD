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
        long long dim;
        T *v;

        void freeArray() {
            if (dim)
                delete v;
        }

        ///functie care imi interclaseaza subvectorii:
        ///v1: subvectorul de la pozitia st la mij al vectorului tata
        ///v2: subvectorul de la pozitia mij + 1 la dr al vectorului tata
        ///in acelasi vector tata, pe intervalul [st, dr]
        void combine(long long st, long long mij, long long dr) {
            ///voi crea doi vectori temporari (indexati de la 1)
            long long dimSt = mij - st + 1;
            long long dimDr = dr - (mij + 1) + 1;
            auto *stArr = new T[dimSt + 1];
            auto *drArr = new T[dimDr + 1];

            for (long long i = 1; i <= dimSt; i++) {
                stArr[i] = v[st + i - 1];
            }
            for (long long i = 1; i <= dimDr; i++) {
                drArr[i] = v[(mij + 1) + i - 1];
            }

            long long indexSt = 1;
            long long indexDr = 1;
            long long indexV = st;
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

            for (long long i = indexSt; i <= dimSt; i++) {
                v[indexV] = stArr[i];
                indexV++;
            }
            for (long long i = indexDr; i <= dimDr; i++) {
                v[indexV] = drArr[i];
                indexV++;
            }

            delete[] stArr;
            delete[] drArr;
        }

        void countSort(long long exp, unsigned long long base) {
            auto *output = new long long[dim + 1];
            auto *ct = new long long[base + 1];

            for (long long i = 0; i <= base; i++)
                ct[i] = 0;

            for (long long i = 1; i <= dim; i++) {
                long long digit = ((long long) (v[i] / exp)) % base;
                ct[digit]++;
            }

            for (long long i = 1; i <= base - 1; i++)
                ct[i] += ct[i - 1];

            for (long long i = dim; i >= 1; i--) {
                long long digit = ((long long) (v[i] / exp)) % base;
                output[ct[digit]] = v[i];
                ct[digit]--;
            }

            for (long long i = 1; i <= dim; i++)
                v[i] = output[i];

            delete[] ct;
            delete[] output;
        }

        void mergeSortUtil(long long st, long long dr) {
            if (st >= dr) {
                return;
            }

            long long mij = st + (dr - st) / 2;
            mergeSortUtil(st, mij);
            mergeSortUtil(mij + 1, dr);
            combine(st, mij, dr);
        }

        void heapSortUtil(long long st, long long dr) {
            priority_queue<T, vector<T>, greater<T>> X;
            for (long long i = st; i <= dr; i++) {
                X.push(v[i]);
            }

            for (long long i = st; i <= dr; i++) {
                v[i] = X.top();
                X.pop();
            }
        }

        void quickSortUtil(long long st, long long dr) {
            if (st >= dr)
                return;

            long long pi = partition(st, dr);

            quickSortUtil(st, pi - 1);
            quickSortUtil(pi + 1, dr);
        }

        void quickSortDoublePivotUtil(long long st, long long dr) {
            if (st >= dr)
                return;

            if (v[st] > v[dr])
                swap(v[st], v[dr]);

            T pst = v[st], pdr = v[dr];
            long long j = st + 1, k = st + 1, g = dr - 1;
            while (k <= g) {
                if (v[k] < pst) {
                    swap(v[k], v[j]);
                    j++;
                } else if (v[k] >= pdr) {
                    while (v[g] > pdr && k < g)
                        g--;
                    swap(v[k], v[g]);
                    g--;
                    if (v[k] < pst) {
                        swap(v[k], v[j]);
                        j++;
                    }
                }
                k++;
            }
            j--;
            g++;

            swap(v[st], v[j]);
            swap(v[dr], v[g]);

            pst = j;
            pdr = g;
            quickSortDoublePivotUtil(st, pst - 1);
            quickSortDoublePivotUtil(pst + 1, pdr - 1);
            quickSortDoublePivotUtil(pdr + 1, dr);
        }


        void insertionSortUtil(long long st, long long dr) {
            for (long long i = st; i <= dr; i++) {
                T temp = v[i];
                long long k = i - 1;
                while (temp < v[k] && k >= st) {
                    v[k + 1] = v[k];
                    k--;
                }
                v[k + 1] = temp;
            }
        }

        void introSortUtil(long long st, long long dr, long long depthLimit) {
            if (st >= dr) {
                return;
            }

            long long N = dr - st + 1;

            if (N <= 16) {
                insertionSortUtil(st, dr);
            }
            if (depthLimit == 0) {
                heapSortUtil(st, dr);
            } else {
                long long pi = partition(st, dr);
                introSortUtil(st, pi - 1, depthLimit - 1);
                introSortUtil(pi + 1, dr, depthLimit - 1);
            }
        }

        long long partition(long long st, long long dr) {
            T pivot = v[dr];
            long long lastBun = st - 1;
            for (long long i = st; i <= dr; i++) {
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

        explicit Vector(long long x) {
            freeArray();
            dim = x;
            v = new T[dim + 1];
            v[0] = 0;

            for (long long i = 1; i <= dim; i++) {
                v[i] = 0;
            }
        }

        Vector(long long N, T w[]) {
            freeArray();
            dim = N;
            v = new T[dim + 1];
            v[0] = 0;

            for (long long i = 1; i <= dim; i++) {
                v[i] = w[i];
            }
        }

        ~Vector() {
            freeArray();
        }

        void update(long long N, T w[]) {
            freeArray();
            dim = N;
            v = new T[dim + 1];
            v[0] = 0;

            for (long long i = 1; i <= N; i++) {
                v[i] = w[i];
            }
        }

        friend istream &operator>>(istream &in, Vector &X) {
            in >> X.dim;
            X.v = new T[X.dim + 1];
            X.v[0] = 0;

            for (long long i = 1; i <= X.dim; i++)
                in >> X.v[i];

            return in;
        }

        friend ostream &operator<<(ostream &out, Vector const &X) {
            for (long long i = 1; i <= X.dim; i++) {
                out << X.v[i];
                if (i != X.dim)
                    out << ",";
            }
            return out;
        }

        Vector &operator=(Vector<T> const &X) {
            freeArray();
            dim = X.dim;
            v = new T[dim + 1];
            v[0] = 0;
            for (long long i = 1; i <= dim; i++) {
                v[i] = X.v[i];
            }
            return *this;
        }

        T getMax() {
            T mx = v[dim];
            for (long long i = 1; i <= dim; i++)
                if (v[i] > mx)
                    mx = v[i];

            return mx;
        }

        bool isSorted() const {
            for (long long i = 1; i < dim; i++)
                if (v[i] > v[i + 1])
                    return false;
            return true;
        }

        T getItemAtIndex(unsigned long long index) {
            if (index >= dim)
                throw (runtime_error("Index is larger than array size."));
            return v[index];
        }

        void radixSort(unsigned long long base) {
            ///doar daca T = positive long long!
            ///altfel nu am cum
            if (!(std::is_same<T, long long>::value) || !(std::is_same<T, long long>::value))
                throw (runtime_error("You may only perform radix sort on natural numbers."));

            for (long long i = 1; i <= dim; i++)
                if (v[i] < 0)
                    v[i] = abs(v[i]); // temporary

            long long mx = getMax();

            for (long long exp = 1; mx / exp > 0; exp = exp * base)
                countSort(exp, base);
        }

        void mergeSort() {
            mergeSortUtil(1, dim);
        }

        void shellSort() {
            for (long long gap = dim / 2; gap >= 1; gap = gap / 2) {
                for (long long i = gap + 1; i <= dim; i++) {
                    T temp = v[i];

                    long long j;
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

        void quickSortDoublePivot() {
            quickSortDoublePivotUtil(1, dim);
        }

        void insertionSort() {
            insertionSortUtil(1, dim);
        }

        void introSort() {
            long long depthLimit = 2 * floor(log(dim));
            introSortUtil(1, dim, depthLimit);
        }

        void stlSort() {
            sort(v + 1, v + dim + 1);
        }
    };
}
