// parallerSort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "ParrlelMerge.h"
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

int main(int argc, char *argv[])
{
	// Construct task scheduler with p threads
	//size_t p = std::atoi(argv[1]);
	//std::string input(argv[2]);

	size_t p = 2;
	std::string input("input.txt");

	std::ifstream f;
	f.open(input);

	std::vector<size_t> a;
	size_t elem;
	while (f >> elem) {
		a.push_back(elem);
	}

	tbb::task_scheduler_init init(p);
	ParrlelMerge sorter(2);
	std::cout << "Hello World! p is " << p << " input: " << input << std::endl;

	tbb::tick_count t0 = tbb::tick_count::now();
	// Execute parallel algorithm using task or
	sorter.par_sort(a);
	// template algorithm here
	tbb::tick_count t1 = tbb::tick_count::now();
	double t = (t1 - t0).seconds();
	std::cout << "time = " << t << " with " << p << " threads" << std::endl;
	//p.merge(a, 0, a.size());
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
