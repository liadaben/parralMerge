#include "pch.h"
#include <iostream>
#include "ParrlelMerge.h"

ParrlelMerge::ParrlelMerge(std::vector<size_t> in) : m_in(std::move(in)) {
}

void ParrlelMerge::merge(std::vector<size_t>& in, size_t start, size_t end)
{
	if (end <= start + 1) { return; }
	int m = start + (end - start) / 2;
	merge(in, start, m);
	merge(in, m, end);
	std::vector<size_t> temp = in;
	std::merge(in.begin() + start, in.begin() + m, in.begin() + m, in.begin() + end, temp.begin() + start);
	std::copy(temp.begin(), temp.end(), in.begin());
	std::cout << "start: " << start << " end: " << end << std::endl;
	print_merged(in);
}

void ParrlelMerge::print_merged(std::vector<size_t> in) const
{
	std::cout << "[";
	for (const auto& a : in) {
		std::cout << a << ",";
	}
	std::cout << "]" << std::endl;
}
