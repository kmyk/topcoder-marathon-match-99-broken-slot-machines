#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0; (i) < int(n); ++ (i))
#define REP3(i, m, n) for (int i = (m); (i) < int(n); ++ (i))
#define REP_R(i, n) for (int i = int(n) - 1; (i) >= 0; -- (i))
#define REP3R(i, m, n) for (int i = int(n) - 1; (i) >= int(m); -- (i))
#define ALL(x) begin(x), end(x)
#define dump(x) cerr << #x " = " << x << endl
#define unittest_name_helper(counter) unittest_ ## counter
#define unittest_name(counter) unittest_name_helper(counter)
#define unittest __attribute__((constructor)) void unittest_name(__COUNTER__) ()
using ll = long long;
using namespace std;
template <class T> using reversed_priority_queue = priority_queue<T, vector<T>, greater<T> >;
template <class T> inline void chmax(T & a, T const & b) { a = max(a, b); }
template <class T> inline void chmin(T & a, T const & b) { a = min(a, b); }
template <typename T> ostream & operator << (ostream & out, vector<T> const & xs) { REP (i, int(xs.size()) - 1) out << xs[i] << ' '; if (not xs.empty()) out << xs.back(); return out; }

#ifdef LOCAL
class PlaySlots {
public:
    static int quickPlay(int machineNumber, int times);
    static vector<string> notePlay(int machineNumber, int times);
};
#endif

class Solver {
public:
    int coins;
    int maxTime;
    int noteTime;
    int numMachines;
    Solver(int coins, int maxTime, int noteTime, int numMachines)
            : coins(coins)
            , maxTime(maxTime)
            , noteTime(noteTime)
            , numMachines(numMachines) {
    }

    int quickPlay(int machineNumber, int times) {
        coins -= times;
        maxTime -= times;
        assert (coins >= 0);
        assert (maxTime >= 0);
        int win = PlaySlots::quickPlay(machineNumber, times);
        coins += win;
        return win;
    }
    pair<int, vector<string> > notePlay(int machineNumber, int times) {
        coins -= times;
        maxTime -= noteTime * times;
        assert (coins >= 0);
        assert (maxTime >= 0);
        auto res = PlaySlots::notePlay(machineNumber, times);
        int win = stoi(res[0]);
        coins += win;
        res[0] = res.back();
        res.pop_back();
        return make_pair(win, res);
    }

    void playSlots() {
        while (coins and maxTime) {
            quickPlay(0, 1);
        }
    }
};


class BrokenSlotMachines {
public:
    int playSlots(int coins, int maxTime, int noteTime, int numMachines);
};
int BrokenSlotMachines::playSlots(int coins, int maxTime, int noteTime, int numMachines) {
    Solver(coins, maxTime, noteTime, numMachines).playSlots();
    return 0;
}
