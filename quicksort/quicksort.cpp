// quicksort.cpp : Defines the entry point for the application.
//

#include "quicksort.h"
#include<vector>
#include<string>
#include<random>
#include<ppl.h>
#include<ctime>
#include <parlay/parallel.h>
#include <parlay/primitives.h>
#include <parlay/sequence.h>

using namespace std;

const int maxn = 100'000'000;
void quicksort_seq(int* a, int l, int r) {
	if (r - l <= 1) {
		return;
	}
	int element = a[l];
	int i = l;
	int j = r - 1;
	while (i < j) {
		while (a[i] < element && i < j) {
			++i;
		}
		while (a[j] > element && i < j) {
			--j;
		}
		if (i >= j) {
			break;
		}
		swap(a[i], a[j]);
		++i;
		--j;
	}
	if (i == j) {
		if (a[i] <= element) {
			++i;
		}
	}
	quicksort_seq(a, l, i);
	quicksort_seq(a, i, r);
}
void quicksort_seq(int* a) {
	quicksort_seq(a, 0, maxn);
}

const int block_size = 1000;
const bool do_validation = false;
void quicksort_par(int* a, int l, int r) {
	if (r - l <= block_size) {
		quicksort_seq(a, l, r);
		return;
	}
	int element = a[l];
	int i = l;
	int j = r - 1;
	while (i < j) {
		while (a[i] < element && i < j) {
			++i;
		}
		while (a[j] > element && i < j) {
			--j;
		}
		if (i >= j) {
			break;
		}
		swap(a[i], a[j]);
		++i;
		--j;
	}
	if (i == j) {
		if (a[i] <= element) {
			++i;
		}
	}
	concurrency::parallel_invoke([&] {
		quicksort_par(a, l, i);
		}, [&] {
			quicksort_par(a, i, r);
		});
}
void quicksort_par(int* a) {
	quicksort_par(a, 0, maxn);
}

int main() {
	mt19937 rnd(71711);
	long long total_t_seq = 0;
	long long total_t_par = 0;
	int* v = new int[maxn];
	for (int i = 0; i < 5; ++i) {
		for (int i = 0; i < maxn; ++i) {
			v[i] = rnd();
		}
		auto t = clock();
		quicksort_seq(v);
		total_t_seq += clock() - t;
		if (do_validation) {
			for (int i = 1; i < maxn; ++i) {
				if (v[i] < v[i - 1]) {
					cout << "Invalid sequential sort result";
					return -1;
				}
			}
		}
	}
	auto p = Concurrency::SchedulerPolicy();
	p.SetConcurrencyLimits(1, 4);
	concurrency::Scheduler::SetDefaultSchedulerPolicy(p);
	for (int i = 0; i < 5; ++i) {
		for (int i = 0; i < maxn; ++i) {
			v[i] = rnd();
		}
		auto t = clock();
		quicksort_par(v);
		total_t_par += clock() - t;
		if (do_validation) {
			for (int i = 1; i < maxn; ++i) {
				if (v[i] < v[i - 1]) {
					cout << "Invalid parallel sort result";
					return -1;
				}
			}
		}
	}
	delete[] v;
	cout << "Sequential: " << total_t_seq / 5.0 / CLOCKS_PER_SEC << '\n';
	cout << "Parallel: " << total_t_par / 5.0 / CLOCKS_PER_SEC << '\n';
	return 0;
}
