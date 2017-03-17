#include <iostream>
#include <map>

using namespace std;
using ClusterMap = map<int, int>;
using WinnerMap = map<ClusterMap, bool>;

bool is_win_after_removing(int i, ClusterMap map, WinnerMap* winner_map);
string map_to_string(const ClusterMap& map);

bool is_win(const ClusterMap& map, WinnerMap* winner_map) {
  const auto& cached = winner_map->find(map);
  if (cached != winner_map->end()) {
    // cout << "Found cached result for " << map_to_string(map)
    //   << (cached->second ? ": WIN" : ": LOSE") << endl;
    return cached->second;
  }
  for (const auto& iter : map) {
    // If you can force any loss, you win.
    if (!is_win_after_removing(iter.first, map, winner_map)) {
      // cout << "Found loss after removing " << to_string(iter.first)
      //   << " from " << map_to_string(map) << endl;
      return true;
    }
  }
  // Otherwise, every move results in your opponent winning: you lose.
  return false;
}

bool is_win_after_removing(int i, ClusterMap map, WinnerMap* winner_map) {
  if (map[i] == 1) {
    map.erase(i);
  } else {
    map[i]--;
  }
  // cout << "Evaluating case " << map_to_string(map) << endl;
  return is_win(map, winner_map);
}

string map_to_string(const ClusterMap& map) {
  string str = "";
  for (const auto& iter : map) {
    for (int i = 0; i < iter.second; i++) {
      str += ',' + to_string(iter.first);
    }
  }
  if (str.length() == 0) {
    return "<empty>";
  }
  return str.substr(1);
}

void push_cluster(int size, ClusterMap* map) {
  if (size == 0) {
    return;
  }
  if (map->find(size) == map->end()) {
    (*map)[size] = 1;
  } else {
    (*map)[size]++;
  }
}

int main() {
  int T;
  cin >> T;
  // cout << "Evaluating " << T << " cases." << endl;
  WinnerMap winner_map;
  winner_map[ClusterMap{}] = false;
  for (int t = 0; t < T; t++) {
    int N;
    cin >> N;
    // cout << "Case " << t << " with " << N << " pins: ";
    char c;
    ClusterMap map;
    int size = 0;
    for (int n = 0; n < N; n++) {
      cin >> c;
      if (c == 'X') {
        push_cluster(size, &map);
        size = 0;
      } else { // c == 'I'
        size++;
      }
    }
    push_cluster(size, &map);
    // cout << map_to_string(map) << endl;
    cout << (is_win(map, &winner_map) ? "WIN" : "LOSE") << endl;
  }
  return 0;
}
