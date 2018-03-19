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
    static int quickPlay(int machineNumber, int times) {
        cout << "quick" << endl;
        cout << machineNumber << endl;
        cout << times << endl;
        cout.flush();
        int ret; cin >> ret;
        assert (cin);
        return ret;
    }
    static vector<string> notePlay(int machineNumber, int times) {
        cout << "note" << endl;
        cout << machineNumber << endl;
        cout << times << endl;
        cout.flush();
        vector<string> ret(times + 1);
        REP (i, times + 1) {
            cin >> ret[i];
        }
        assert (cin);
        return ret;
    }
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
        quick_earned.resize(numMachines);
        quick_count.resize(numMachines);
        note_result.resize(numMachines);
        note_count.resize(numMachines);
        note_expected.resize(numMachines, - INFINITY);
    }

    vector<int> quick_earned;
    vector<int> quick_count;
    int quickPlay(int machineNumber, int times) {
        coins -= times;
        maxTime -= times;
        if (coins < 0) throw stop("bankrupted");
        if (maxTime < 0) throw stop("time up!");
        int win = PlaySlots::quickPlay(machineNumber, times);
        coins += win;
        quick_earned[machineNumber] += win;
        quick_count[machineNumber] += times;
        return win;
    }

    vector<vector<string> > note_result;
    vector<int> note_count;
    vector<double> note_expected;
    pair<int, vector<string> > notePlay(int machineNumber, int times) {
        coins -= times;
        maxTime -= noteTime * times;
        if (coins < 0) throw stop("bankrupted");
        if (maxTime < 0) throw stop("time up");
        auto res = PlaySlots::notePlay(machineNumber, times);
        int win = stoi(res[0]);
        res.erase(res.begin());
        coins += win;
        copy(ALL(res), back_inserter(note_result[machineNumber]));
        note_count[machineNumber] += times;
        note_expected[machineNumber] = calc_expected(get_freq(note_result[machineNumber]));
        return make_pair(win, res);
    }

    array<array<int, 7>, 3> get_freq(vector<string> const & result) {
        array<array<int, 7>, 3> freq = {};
        for (string s : result) {
            REP (j, 3 * 3) {
                freq[j % 3][s[j] - 'A'] += 1;
            }
        }
        return freq;
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

    array<vector<string>, 3> split_to_chunks(vector<string> const & result) {
        array<vector<string>, 3> chunks;
        for (string s : result) {
            REP (i, 3) {
                chunks[i].emplace_back();
            }
            REP (j, 3 * 3) {
                chunks[j % 3].back() += s[j];
            }
        }
        return chunks;
    }
    string reconstruct_wheel1(vector<string> chunks) {
        default_random_engine gen;
        sort(ALL(chunks));
        chunks.erase(unique(ALL(chunks)), chunks.end());
        shuffle(ALL(chunks), gen);
        string t = chunks.back();
        chunks.pop_back();
        while (not chunks.empty()) {
            shuffle(ALL(chunks), gen);
            REP (j, chunks.size()) {
                if (t.substr(t.length() - 2) == chunks[j].substr(1)) {
                    t += chunks[j][2];
                    chunks.erase(chunks.begin() + j);
                    goto next;
                }
            }
            REP (j, chunks.size()) {
                if (t[t.length() - 1] == chunks[j][0]) {
                    t += chunks[j].substr(1);
                    chunks.erase(chunks.begin() + j);
                    goto next;
                }
            }
            t += chunks.back();
            chunks.pop_back();
next: ;
        }
        return t;
    }
    vector<string> reconstruct_wheel(vector<string> const & result) {
        auto chunks = split_to_chunks(result);
        array<string, 3> wheel;
        REP (i, 3) {
            wheel[i] = reconstruct_wheel1(chunks[i]);
        }
        return vector<string>(ALL(wheel));
    }

    void playSlots() {
        cerr << "Coins: " << coins << endl;
        cerr << "Remainig Time: " << maxTime << endl;

        cerr << "Explore..." << endl;
        { // explore
            constexpr int first_depth = 10;
            constexpr int second_depth = 5;
            const int first_k = min(numMachines, maxTime / noteTime / first_depth);
            const int second_k = min(3, first_k);
            if (maxTime - first_k * first_depth * noteTime <= 100) {
                return;
            }
            REP (i, first_k) {
                notePlay(i, first_depth);
            }
            vector<int> indices(numMachines);
            iota(ALL(indices), 0);
            sort(ALL(indices), [&](int i, int j) { return note_expected[i] > note_expected[j]; });
            REP (j, second_k) {
                int i = indices[j];
                notePlay(i, second_depth);
            }
            REP (i, numMachines) {
                cerr << "Expected payout rate: " << note_expected[i] << endl;
            }
        }

        cerr << "Coins: " << coins << endl;
        cerr << "Remainig Time: " << maxTime << endl;

        auto modified_expected = [&](int i) {
            int k = pow(3 * note_count[i], 3);
            return (quick_earned[i] + note_expected[i] * k) / (quick_count[i] + k);
        };

        cerr << "Exploit..." << endl;
        { // exploit
            int i = max_element(ALL(note_expected)) - note_expected.begin();
            if (i == numMachines or note_expected[i] < 1.1) {
                return;
            }
            cerr << "Selected Machine: " << i << endl;
            vector<int> indices(numMachines);
            iota(ALL(indices), 0);
#ifdef LOCAL
            while (coins and maxTime) {
                quickPlay(i, min(coins, maxTime));  // for speed
            }
#endif
            while (coins and maxTime) {
                int j = *max_element(ALL(indices), [&](int i, int j) { return modified_expected(i) < modified_expected(j); });
                if (j != i) {
                    cerr << "Selected Machine: " << j << endl;
                    i = j;
                }
                quickPlay(i, 1);
            }
            REP (i, numMachines) {
                cerr << "Modified Expected payout rate: " << modified_expected(i) << endl;
            }
        }

        cerr << "Coins: " << coins << endl;
        cerr << "Remainig Time: " << maxTime << endl;
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

#ifdef LOCAL
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    int coins; cin >> coins;
    int maxTime; cin >> maxTime;
    int noteTime; cin >> noteTime;
    int numMachines; cin >> numMachines;
    int ret = BrokenSlotMachines().playSlots(coins, maxTime, noteTime, numMachines);
    cout << ret << endl;
    cout.flush();
    return 0;
}
#endif
