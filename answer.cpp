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

const int payout_table[7] = { 1000, 200, 100, 50, 20, 10, 5 };

struct stop : public runtime_error {
    stop(string const & msg) : runtime_error(msg) {}
};
class Solver {
public:
    int coins;
    int maxTime;
    const int noteTime;
    const int numMachines;
    Solver(int coins, int maxTime, int noteTime, int numMachines)
            : coins(coins)
            , maxTime(maxTime)
            , noteTime(noteTime)
            , numMachines(numMachines) {
    }

    int quickPlay(int machineNumber, int times) {
        coins -= times;
        maxTime -= times;
        if (coins < 0) throw stop("bankrupted");
        if (maxTime < 0) throw stop("time up!");
        int win = PlaySlots::quickPlay(machineNumber, times);
        coins += win;
        return win;
    }
    pair<int, vector<string> > notePlay(int machineNumber, int times) {
        coins -= times;
        maxTime -= noteTime * times;
        if (coins < 0) throw stop("bankrupted");
        if (maxTime < 0) throw stop("time up");
        auto res = PlaySlots::notePlay(machineNumber, times);
        int win = stoi(res[0]);
        coins += win;
        res[0] = res.back();
        res.pop_back();
        return make_pair(win, res);
    }

    double calc_expected(array<array<int, 7>, 3> const & freq) {
        int size[3] = {
            accumulate(ALL(freq[0]), 0),
            accumulate(ALL(freq[1]), 0),
            accumulate(ALL(freq[2]), 0),
        };
        int payout = 0;
        REP (j, 7) {
            payout += freq[0][j] * freq[1][j] * freq[2][j] * payout_table[j];
        }
        return payout /(double) (size[0] * size[1] * size[2]);
    }

    vector<array<array<int, 7>, 3> > freq;
    vector<double> expected;
    void playSlots() {
        freq.resize(numMachines, array<array<int, 7>, 3>());
        expected.resize(numMachines, - INFINITY);
        auto inspect = [&](int i, int depth) {
            auto result = notePlay(i, depth).second;
            for (string s : result) {
                REP (j, 3 * 3) {
                    freq[i][j % 3][s[j] - 'A'] += 1;
                }
            }
            expected[i] = calc_expected(freq[i]);
        };

        { // explore
            constexpr int first_depth = 10;
            constexpr int second_depth = 50;
            const int first_k = min(numMachines, maxTime / noteTime / first_depth);
            const int second_k = min(3, first_k);
            if (maxTime - (first_k * first_depth + second_k + second_depth) * noteTime <= 1000) {
                return;
            }
            REP (i, first_k) {
                inspect(i, first_depth);
            }
            vector<int> indices(numMachines);
            iota(ALL(indices), 0);
            sort(ALL(indices), [&](int i, int j) { return expected[i] > expected[j]; });
            REP (j, second_k) {
                int i = indices[j];
                inspect(i, second_depth);
            }
            REP (i, numMachines) {
                cerr << "Expected payout rate: " << expected[i] << endl;
            }
            cerr << "Coins: " << coins << endl;
            cerr << "Remainig Time: " << maxTime << endl;
        }

        { // exploit
            int i = max_element(ALL(expected)) - expected.begin();
            if (i == numMachines or expected[i] < 1.1) {
                return;
            }
            cerr << "Selected Machine: " << i << endl;
            while (coins and maxTime) {
                quickPlay(i, min(coins, maxTime));
                cerr << "Coins: " << coins << endl;
                cerr << "Remainig Time: " << maxTime << endl;
            }
        }
    }
};


class BrokenSlotMachines {
public:
    int playSlots(int coins, int maxTime, int noteTime, int numMachines);
};
int BrokenSlotMachines::playSlots(int coins, int maxTime, int noteTime, int numMachines) {
    try {
        Solver(coins, maxTime, noteTime, numMachines).playSlots();
    } catch (stop e) {
        cerr << e.what() << endl;
    }
    return 0;
}
