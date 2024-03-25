#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <queue>

using namespace std;

namespace ds {
    template<typename T>
    class Vector {
    private:
        int dim;
        T *v;

        ///functie care imi interclaseaza subvectorii:
        ///v1: subvectorul de la pozitia st la mij al vectorului tata
        ///v2: subvectorul de la pozitia mij + 1 la dr al vectorului tata
        ///in acelasi vector tata, pe intervalul [st, dr]
        void combina(int st, int mij, int dr) {
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

        void mergeSortUtil(int st, int dr) {
            if (st >= dr) {
                return;
            }

            int mij = st + (dr - st) / 2;
            mergeSortUtil(st, mij);
            mergeSortUtil(mij + 1, dr);
            combina(st, mij, dr);
        }

    public:
        Vector() { dim = 0; }

        Vector(int x) {
            dim = x;
            v = new T[dim + 1];
            v[0] = 0;

            for (int i = 1; i <= dim; i++) {
                v[i] = 0;
            }
        }

        Vector(int N, T w[]) {
            dim = N;
            v = new T[dim + 1];
            v[0] = 0;

            for (int i = 1; i <= dim; i++) {
                v[i] = w[i];
            }
        }

        ~Vector() {
            if (dim != 0) {
                delete v;
            }
        }

        void updateaza(int N, T w[]) {
            if (dim != 0) {
                delete v;
            }

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

        int getMax() {
            T mx = v[dim];
            for (int i = 1; i <= dim; i++) {
                if (v[i] > mx) {
                    mx = v[i];
                }
            }
            return mx;
        }

        void countSort(int exp) {
            int output[dim + 1];
            int ct[10] = {0};

            for (int i = 1; i <= dim; i++) {
                int cifra = ((int) (v[i] / exp)) % 10;
                ct[cifra]++;
            }

            for (int i = 1; i <= 9; i++) {
                ct[i] = ct[i] + ct[i - 1];
            }

            for (int i = dim; i >= 1; i--) {
                int cifra = ((int) (v[i] / exp)) % 10;
                output[ct[cifra]] = v[i];
                ct[cifra]--;
            }

            for (int i = 1; i <= dim; i++) {
                v[i] = output[i];
            }

        }

        int radixSort() {
            ///doar daca T = int!
            ///altfel nu am cum
            if (!(std::is_same<T, int>::value)) {
                cout << "Nu se poate RadixSort pt ca nu am numere naturale!\n";
                return 0;
            }
            int mx = getMax();

            for (int exp = 1; exp <= mx; exp = exp * 10) {
                countSort(exp);
                ///cout << *(this);

            }
        }

        void mergeSort() {
            mergeSortUtil(1, dim);
        }


        int shellSort() {

            auto start_cronometru = std::chrono::high_resolution_clock::now();

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


            auto finish_cronometru = std::chrono::high_resolution_clock::now();
            auto duration_cronometru = duration_cast<std::chrono::microseconds>(finish_cronometru - start_cronometru);

            return duration_cronometru.count();
        }

        void heapSortUtil(int st, int dr) {
            int N = dr - st + 1;
            priority_queue <T, vector<T>, greater<T>> X;
            for (int i = st; i <= dr; i++) {
                X.push(v[i]);
            }

            for (int i = st; i <= dr; i++) {
                v[i] = X.top();
                X.pop();
            }
        }

        int heapSort() {
            auto start_cronometru = std::chrono::high_resolution_clock::now();

            heapSortUtil(1, dim);

            auto finish_cronometru = std::chrono::high_resolution_clock::now();
            auto duration_cronometru = duration_cast<std::chrono::microseconds>(finish_cronometru - start_cronometru);

            return duration_cronometru.count();
        }

        int partitie(int st, int dr) {
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

        void quickSortUtil(int st, int dr) {
            if (!(st < dr)) {
                return;
            }

            int pi = partitie(st, dr);

            quickSortUtil(st, pi - 1);
            quickSortUtil(pi + 1, dr);
        }

        int quickSort() {
            auto start_cronometru = std::chrono::high_resolution_clock::now();


            quickSortUtil(1, dim);

            auto finish_cronometru = std::chrono::high_resolution_clock::now();
            auto duration_cronometru = duration_cast<std::chrono::microseconds>(finish_cronometru - start_cronometru);

            return duration_cronometru.count();
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

        int insertionSort() {
            auto start_cronometru = std::chrono::high_resolution_clock::now();

            insertionSortUtil(1, dim);


            auto finish_cronometru = std::chrono::high_resolution_clock::now();
            auto duration_cronometru = duration_cast<std::chrono::microseconds>(finish_cronometru - start_cronometru);

            return duration_cronometru.count();
        }

        void introSortUtil(int st, int dr, int depthLimit) {
            if (!(st < dr)) {
                return;
            }

            int N = dr - st + 1;

            if (N <= 16) {
                insertionSortUtil(st, dr);
            }
            if (depthLimit == 0) {
                heapSortUtil(st, dr);
            } else {
                int pi = partitie(st, dr);
                introSortUtil(st, pi - 1, depthLimit - 1);
                introSortUtil(pi + 1, dr, depthLimit - 1);
            }
        }

        int introSort() {
            auto start_cronometru = std::chrono::high_resolution_clock::now();

            int depthLimit = 2 * floor(log(dim));
            introSortUtil(1, dim, depthLimit);

            auto finish_cronometru = std::chrono::high_resolution_clock::now();
            auto duration_cronometru = duration_cast<std::chrono::microseconds>(finish_cronometru - start_cronometru);

            return duration_cronometru.count();
        }
    };
}
