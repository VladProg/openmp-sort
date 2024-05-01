#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>
#include <ctime>
#include <omp.h>

using namespace std;

struct HeapItem {
    vector<unsigned>::iterator cur, last;
    void next() { cur++; }
    unsigned get() const { return *cur; }
    bool has() const { return cur != last; }
    friend bool operator<(const HeapItem& l, const HeapItem& r) { return l.get() > r.get(); }
};

int main() {
    unsigned n;
    cin >> n;

    vector<unsigned> v(n);
    unsigned mul = 1664525, add = 1013904223, cur = 123456789;
    for (unsigned i = 0; i < n; i++) {
        v[i] = cur = cur * mul + add;
    }
    cerr << clock() * 1.0 / CLOCKS_PER_SEC << " s - finished generation\n";

    vector<HeapItem> heap;
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int start_idx = (unsigned long long)thread_id * n / num_threads;
        int end_idx = (unsigned long long)(thread_id + 1) * n / num_threads;
        sort(v.begin() + start_idx, v.begin() + end_idx);
        HeapItem item{v.begin() + start_idx, v.begin() + end_idx};
        if (item.has()) {
            #pragma omp critical
            heap.push_back(item);
        }
    }
    cerr << clock() * 1.0 / CLOCKS_PER_SEC << " s - finished sort of parts\n";

    make_heap(heap.begin(), heap.end());
    vector<unsigned> res(n);
    for (unsigned i = 0; i < n; i++) {
        pop_heap(heap.begin(), heap.end());
        HeapItem& item = heap.back();
        res[i] = item.get();
        item.next();
        if (item.has())
            push_heap(heap.begin(), heap.end());
        else
            heap.pop_back();
    }
    cerr << clock() * 1.0 / CLOCKS_PER_SEC << " s - finished merge\n";

    for (unsigned i = 0; i <= 99; i++)
        cout << res[(unsigned long long)i * n / 99] << " ";
    cerr << clock() * 1.0 / CLOCKS_PER_SEC << " s - finished output\n";

    return 0;
}
