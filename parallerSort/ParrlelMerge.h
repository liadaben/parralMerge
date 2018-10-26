#pragma once

#include "tbb/flow_graph.h"

class ParrlelMerge
{
public:
	using node_t = tbb::flow::continue_node< tbb::flow::continue_msg > ;
	using msg_t = const tbb::flow::continue_msg&;
	ParrlelMerge(size_t threads);

	void merge(std::vector<size_t>& in, size_t start, size_t end);

	node_t& add_task(std::vector<size_t>& in, size_t start, size_t end, size_t level);

	void par_sort(std::vector<size_t>& in);

	void print_merged(std::vector<size_t> in) const;
private:
	tbb::flow::graph m_g;
	node_t m_root_node;
	size_t m_threads;
	std::vector < std::unique_ptr<node_t> > m_nodes;
};

