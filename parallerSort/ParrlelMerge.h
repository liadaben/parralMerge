#pragma once

#include "tbb/flow_graph.h"

class ParrlelMerge
{
public:
	ParrlelMerge(std::vector<size_t> in);

	void merge(std::vector<size_t>& in, size_t start, size_t end);

	void print_merged(std::vector<size_t> in) const;
private:
	tbb::flow::graph m_g;
	std::vector<size_t> m_in;
};

