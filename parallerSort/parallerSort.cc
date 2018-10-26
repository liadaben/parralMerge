#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include "ParrlelMerge.h"
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

std::vector<size_t> rand_vec(size_t size) {
	// First create an instance of an engine.
	std::random_device rnd_device;
	// Specify the engine and distribution.
	std::mt19937 mersenne_engine{ rnd_device() };  // Generates random integers
	std::uniform_int_distribution<size_t> dist{ 1, size };

	auto gen = [&dist, &mersenne_engine]() {
		return dist(mersenne_engine);
	};

	std::vector<size_t> vec(size);
	std::generate(vec.begin(), vec.end(), gen);
	return vec;
}

void print_vector_lines(const std::vector<size_t>& in) {
	for (auto a : in) {
		std::cout << a << std::endl;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "need exactly two arguments" << std::endl;
		exit(1);
	}
	size_t p = std::atoi(argv[1]);
	std::string input(argv[2]);
	//size_t p = 6;
	//std::string input("C:\\Users\\liad\\source\\repos\\parallerSort\\parallerSort\\Debug\\input.txt");

	std::ifstream f;
	f.open(input);
	if (!f.is_open()) {
		std::cout << " Failed to open file" << std::endl;
		exit(1);
	}
	std::vector<size_t> a;
	size_t elem;
	while (f >> elem) {
		a.push_back(elem);
	}
	//a = rand_vec(1000000);
	f.close();

	for (size_t p = 1; p < 60; p++) {
		std::vector<size_t> in_vec = a;
		tbb::task_scheduler_init init(p);
		ParrlelMerge sorter(p);
		std::cout << "Hello World! p is " << p << " input: " << input << std::endl;

		tbb::tick_count t0 = tbb::tick_count::now();
		// Execute parallel algorithm using task or
		sorter.par_sort(in_vec);
		// template algorithm here
		tbb::tick_count t1 = tbb::tick_count::now();
		double t = (t1 - t0).seconds();
		//std::cout << "time = " << t << " with " << p << " threads" << std::endl;
		std::cout << "MergeSort: " << t * 1e6 << std::endl;
		//print_vector_lines(in_vec);
	}
}
