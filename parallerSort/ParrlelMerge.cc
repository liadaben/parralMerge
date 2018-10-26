#include "pch.h"
#include <math.h> 
#include <iostream>
#include <memory>
#include <algorithm>
#include "ParrlelMerge.h"

ParrlelMerge::ParrlelMerge(size_t threads)
	:
	m_root_node(m_g, [](msg_t) {
	//std::cout << "root start" << std::endl;
	}),
	m_threads(threads) {
}

void ParrlelMerge::merge(std::vector<size_t>& in, size_t start, size_t end)
{
	if (end <= start + 1) { return; }
	int m = start + (end - start) / 2;
	merge(in, start, m);
	merge(in, m, end);
	std::vector<size_t> temp = in;
	std::merge(in.begin() + start, in.begin() + m, in.begin() + m, in.begin() + end, temp.begin() + start);
	std::copy(temp.begin() + start, temp.begin() + end, in.begin() + start);
	std::cout << "start: " << start << " end: " << end << std::endl;
	print_merged(in);
}

auto ParrlelMerge::add_task(std::vector<size_t>& in, size_t start, size_t end, size_t level) -> ParrlelMerge::node_t&
{
	if (end <= start + 1) {
		std::unique_ptr<node_t>rv(new node_t(m_g, [](msg_t) {
		}));
		tbb::flow::make_edge(m_root_node, *rv);
		m_nodes.emplace_back(std::move(rv));
		return  *m_nodes.back();
	}
	
	if (level > log2(m_threads)*2) {
		std::unique_ptr<node_t>rv(new node_t(m_g, [&in, start, end](msg_t) {
			std::sort(in.begin() + start, in.begin() + end);
		}));
		tbb::flow::make_edge(m_root_node, *rv);
		m_nodes.emplace_back(std::move(rv));
		return  *m_nodes.back();
	}
	
	int m = start + (end - start) / 2;
	auto& l = add_task(in, start, m, level + 1);
	auto& r = add_task(in, m, end, level + 1);

	std::unique_ptr<node_t>rv(new node_t(m_g, [&in, start, end, m](msg_t) {
		//std::cout << "merger task start" << std::endl;
		std::vector<size_t> temp(end - start);
		std::merge(in.begin() + start, in.begin() + m, in.begin() + m, in.begin() + end, temp.begin());
		std::copy(temp.begin(), temp.begin() + end - start, in.begin() + start);
	}));

	tbb::flow::make_edge(l, *rv);
	tbb::flow::make_edge(r, *rv);
	m_nodes.emplace_back(std::move(rv));
	return  *m_nodes.back();
}

void ParrlelMerge::par_sort(std::vector<size_t>& in)
{
	//std::cout << "#########################################start#########################################" << std::endl;
	//print_merged(in);
	auto fin = add_task(in, 0, in.size(), 0);
	m_root_node.try_put(tbb::flow::continue_msg());
	m_g.wait_for_all();
	//print_merged(in);
	//std::cout << "#########################################end#########################################" << std::endl;
}

void ParrlelMerge::print_merged(std::vector<size_t> in) const
{
	std::cout << "[";
	for (const auto& a : in) {
		std::cout << a << ",";
	}
	std::cout << "]" << std::endl;
}
