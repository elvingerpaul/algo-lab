#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::pair<int, int>> vpint;

int compare(std::pair<int, int> p1, std::pair<int, int> p2){
    return p1.second < p2.second;
}

void testcase(){
    int n; std::cin >> n;
    vpint intervals(n);

    for (int i = 0; i < n; i++){
        int l; std::cin >> l;
        int p; std::cin >> p;
        intervals[i] = std::pair<int, int>(l, p);
    }
    std::sort(intervals.begin(), intervals.end(), compare);

    if (n == 1){
      std::cout << 1 << std::endl;
    } else {
      int num_boats = 2;
      int previous_right = intervals[0].second;
      int current_right = std::max(previous_right + intervals[1].first, intervals[1].second);
      for (int i = 2; i < n; i++){
        int l = intervals[i].first;
        int p = intervals[i].second;
        if (p-l >= current_right){
          previous_right = current_right;
          current_right = p;
          num_boats++;
        } else if (p >= current_right) {
          // directly append to last chosen boat
          previous_right = current_right;
          current_right += l;
          num_boats++;
        } else {
          // might be able to swap last chosen boat with current one to obtain better right boundary
          int new_pot_right = std::max(previous_right + l, p);
          if (new_pot_right < current_right)
            current_right = new_pot_right;
        }
      }
      std::cout << num_boats << std::endl;
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}