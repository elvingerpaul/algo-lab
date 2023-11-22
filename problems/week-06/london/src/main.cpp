#include <iostream>
#include <string>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

typedef std::vector<int> vint;
typedef std::vector<vint> mint;
typedef std::vector<std::string> vstr;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
      
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;
  
  public:
    explicit edge_adder(graph &G) : G(G) {}
    
    void add_edge(int from, int to, long capacity){
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      const auto e = boost::add_edge(from, to, G).first;
      const auto rev_e = boost::add_edge(to, from, G).first;
      c_map[e] = capacity;
      c_map[rev_e] = 0;
      r_map[e] = rev_e;
      r_map[rev_e] = e;
    }
};

void testcase(){
  int h; std::cin >> h;
  int w; std::cin >> w;
  std::string note; std::cin >> note;
  
  // count occurences of each character in note
  vint occ(26, 0);
  for(char& c : note){
    occ[c - 'A']++;
  }
  
  // calculate occurence of (frontpage_char, backpage_char) pairs
  vstr lines(h);
  mint pairs(26, vint(26, 0));
  
  for (int i = 0; i < h; i++){
    std::cin >> lines[i];
  }
  
  for (int i = 0; i < h; i++){
    std::string backpage_line; std::cin >> backpage_line;
    for (int j = 0; j < w; j++){
      int char_front = lines[i][j] - 'A';
      int char_back = backpage_line[w-j-1] - 'A';
      pairs[char_front][char_back]++;
    }
  }
  
  // define graph and add edges below:
  //                             src -> (packpage_char, frontpage_char)
  // (backpage_char, frontpage_char) -> backpage_char
  // (backpage_char, frontpage_char) -> backpage_char
  //                            char -> sink
  int num_pairs = 26 * 26;
  int src = num_pairs + 26;
  int sink = num_pairs + 27;
  graph G(num_pairs + 26 + 2);
  edge_adder adder(G);
  
  for (int i = 0; i < 26; i++){
    adder.add_edge(i, sink, occ[i]);
  }
  
  for (int i = 0; i < 26; i++){
    for (int j = 0; j < 26; j++){
      int node_id = i*26+j;
      adder.add_edge(src, node_id, pairs[i][j]);
      adder.add_edge(node_id, i, pairs[i][j]);
      adder.add_edge(node_id, j, pairs[i][j]);
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, src, sink);
  // long flow = boost::edmonds_karp_max_flow(G, src, sink); slower
  
  if (flow == (long)note.size())
    std::cout << "Yes" << std::endl;
  else
    std::cout << "No" << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
  return 0;
}