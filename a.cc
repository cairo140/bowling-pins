#include <iostream>
#include <map>

using namespace std;
using ClusterMap = map<int, int>;
using WinnerMap = map<ClusterMap, bool>;

bool can_force_loss(int from_cluster, const ClusterMap& map,
                    WinnerMap* winner_map);
string map_to_string(const ClusterMap& map);
void push_cluster(int size, ClusterMap* map);

bool is_win(const ClusterMap& map, WinnerMap* winner_map) {
  const auto& cached = winner_map->find(map);
  if (cached != winner_map->end()) {
#ifdef DEBUG
    cerr << "Found cached result for " << map_to_string(map)
      << (cached->second ? ": WIN" : ": LOSE") << endl;
#endif
    return cached->second;
  }
  for (const auto& iter : map) {
    // If you can force any loss, you win.
    if (can_force_loss(iter.first, map, winner_map)) {
      return true;
    }
  }
  // Otherwise, every move results in your opponent winning: you lose.
  return false;
}

bool can_force_loss(int from_cluster, const ClusterMap& map,
                    WinnerMap* winner_map) {
  ClusterMap map_with_from_cluster_removed;
  map_with_from_cluster_removed.insert(map.begin(), map.end());
  if (map_with_from_cluster_removed[from_cluster] == 1) {
    map_with_from_cluster_removed.erase(from_cluster);
  } else {
    map_with_from_cluster_removed[from_cluster]--;
  }
  // Remove 1
  for (int start_index = 0; start_index < from_cluster; start_index++) {
    ClusterMap new_map;
    new_map.insert(map_with_from_cluster_removed.begin(),
                   map_with_from_cluster_removed.end());
    if (start_index != 0) {
      push_cluster(start_index, &new_map);
    }
    if (start_index < from_cluster - 1) {
      push_cluster(from_cluster - 1 - start_index, &new_map);
    }
#ifdef DEBUG
    cerr << "Evaluating case " << map_to_string(new_map) << endl;
#endif
    bool result = is_win(new_map, winner_map);
    if (!result) {
#ifdef DEBUG
      cerr << "Found loss after removing 1 from cluster of "
        << from_cluster << " in " << map_to_string(map) << endl;
#endif
      return true;
    }
  }
  // Remove 2
  for (int start_index = 0; start_index < from_cluster - 1; start_index++) {
    ClusterMap new_map;
    new_map.insert(map_with_from_cluster_removed.begin(),
                   map_with_from_cluster_removed.end());
    if (start_index != 0) {
      push_cluster(start_index, &new_map);
    }
    if (start_index < from_cluster - 2) {
      push_cluster(from_cluster - 2 - start_index, &new_map);
    }
#ifdef DEBUG
    cerr << "Evaluating case " << map_to_string(new_map) << endl;
#endif
    bool result = is_win(new_map, winner_map);
    if (!result) {
#ifdef DEBUG
      cerr << "Found loss after removing 2 from cluster of "
        << from_cluster << " in " << map_to_string(map) << endl;
#endif
      return true;
    }
  }
  return false;
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
#ifdef DEBUG
  cerr << "Evaluating " << T << " cases." << endl;
#endif
  WinnerMap winner_map;
  winner_map[ClusterMap{}] = false;
  for (int t = 0; t < T; t++) {
    int N;
    cin >> N;
#ifdef DEBUG
    cerr << "Case " << t << " with " << N << " pins: ";
#endif
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
#ifdef DEBUG
    cerr << map_to_string(map) << endl;
#endif
    cout << (is_win(map, &winner_map) ? "WIN" : "LOSE") << endl;
  }
  return 0;
}
