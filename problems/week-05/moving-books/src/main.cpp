#include <iostream>
#include <vector>

typedef std::vector<int> vint;

void testcase(){
    int n; std::cin >> n;
    int m; std::cin >> m;
    
    vint friends(n);
    for (int i = 0; i < n; i++)
        std::cin >> friends[i];
    
    vint weights(m);
    for (int i = 0; i < m; i++)
        std::cin >> weights[i];

    std::sort(friends.begin(), friends.end(), std::greater<int>());
    std::sort(weights.begin(), weights.end(), std::greater<int>());

    // there is a box that is to heavy for anyone
    if (friends[0] < weights[0]){
        std::cout << "impossible" << std::endl;
        return;
    }

    int time = 0;
    int boxes_remaining = m;
    vint last_pointer(n, 0);
    while (boxes_remaining){
        for (int i = 0; i < n; i++){
            int ptr = (i > 0) ? std::max(last_pointer[i], last_pointer[i-1]) : last_pointer[i];
            while ((ptr < m) && (weights[ptr] > friends[i] || weights[ptr] == -1)){
                ptr++;
            }
            
            if (ptr < m){
                weights[ptr] = -1;
                boxes_remaining--;
            }

            last_pointer[i] = ptr+1;
        }
        time += 3;
    }

    // subtract 1 because last carry only takes 2 seconds
    std::cout << time - 1 << std::endl;

}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}

// 2 2 2 2 1 1
// 2 2 1
// 2 2 2 2 3 3
// 1 1 2 2