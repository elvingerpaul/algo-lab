#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor			vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

typedef std::vector<int> vint;
typedef std::vector<std::map<int, int>> vmint;

struct Booking {
	int src;
	int dest;
	int start;
	int end;
	int profit;
};

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

void testcase() {
	int N, S; std::cin >> N >> S;

	int tot_num_cars = 0;
	vint num_cars(S);
	for (int i = 0; i < S; i++){
		int cars; std::cin >> cars;
		tot_num_cars += cars;
		num_cars[i] = cars;
	}

	int max_profit = 0;
	vmint timestamps(S);
	std::vector<Booking> bookings(N);
	for (int i = 0; i < N; i++){
		int s, t, d, a, p; std::cin >> s >> t >> d >> a >> p;
		max_profit = std::max(max_profit, p);

		// keep track of the important timestamps for each station
		timestamps[s-1].insert({d, 0});
		timestamps[t-1].insert({a, 0});

		bookings[i] = {s-1, t-1, d, a, p};
	}

	// run over all timestamps in each station and assign them a unique node id
	int node_id = 0;
	for (int i = 0; i < S; i++)
		for (auto &[time, index]: timestamps[i])
			index = node_id++;

	graph G(node_id);
	edge_adder adder(G);
	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_sink = boost::add_vertex(G);

	// add edge for each booking
	for (int i = 0; i < N; i++){
		Booking b = bookings[i];
		int width = b.end - b.start;
		adder.add_edge(timestamps[b.src][b.start], timestamps[b.dest][b.end], 1, max_profit * width - b.profit);
	}

	for (int i = 0; i < S; i++){

		// add edge to provide each station with initial number of cars
		std::map<int, int>::iterator itr = timestamps[i].begin();
		adder.add_edge(v_source, itr->second, num_cars[i], (itr->first) * max_profit);

		// add edge between each timestamp for a given station
		for (itr = timestamps[i].begin(); itr != std::prev(timestamps[i].end()); ++itr){
			auto successor = std::next(itr);
			int start = itr->first;
			int end = successor->first;
			int width = end - start;
			adder.add_edge(itr->second, successor->second, tot_num_cars, max_profit * width);
		}

		// add edge from last timestamp of station to sink
		std::map<int, int>::reverse_iterator rev_itr = timestamps[i].rbegin();
		int width = 100'000 - rev_itr->first;
		adder.add_edge(rev_itr->second, v_sink, tot_num_cars, width * max_profit);
	}

	boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);

	std::cout << (100'000 * max_profit * tot_num_cars) - cost << "\n";
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
