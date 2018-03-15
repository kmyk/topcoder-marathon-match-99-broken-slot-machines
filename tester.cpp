#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#define REP(i, n) for (int i = 0; (i) < int(n); ++ (i))
#define REP3(i, m, n) for (int i = (m); (i) < int(n); ++ (i))
#define ALL(x) begin(x), end(x)
using namespace std;

class PlaySlots {
public:
    static int quickPlay(int machineNumber, int times);
    static vector<string> notePlay(int machineNumber, int times);
};

class BrokenSlotMachines {
public:
    int playSlots(int coins, int maxTime, int noteTime, int numMachines);
};

class PlaySlotsImpl {
public:
    vector<array<string, 3> > wheels;
    int coins;
    int maxTime;
    int noteTime;
    int numMachines;
    vector<int> wheelSize;
    mt19937_64 gen;

    int nextInt(int r) {
        return uniform_int_distribution<int>(0, r - 1)(gen);
    }
    PlaySlotsImpl() = default;
    PlaySlotsImpl(long seed) {
        gen = mt19937_64(seed);
        REP (iteration, 100) gen();  // shuffle
        coins = 100 + nextInt(9901);
        maxTime = 100 + nextInt(9901);
        noteTime = 2 + nextInt(9);
        numMachines = nextInt(8) + 3;
        wheels.resize(numMachines);
        wheelSize.resize(numMachines);
        REP (i, numMachines) {
            if (seed == 0 and i == 0)  {
                wheels[i][0] = wheels[i][1] = wheels[i][2] = "AABBBBCCCCCDDDDDDEEEEEEFFFFFFFGGGGGGGG";
                wheelSize[i] = wheels[i][0].length();
                continue;
            }
            wheelSize[i] = 10 + nextInt(21);
            REP (j, 3) {
                wheels[i][j] = "";
                REP (k, wheelSize[i]) {
                    wheels[i][j] += "AABBBBCCCCCDDDDDDEEEEEEFFFFFFFGGGGGGGG"[nextInt(38)];
                }
            }
        }
    }


    vector<string> doPlay(int machineNumber, int times) {
        int win = 0;
        vector<string> ret(times + 1);
        REP (i, times) {
            if (coins <= 0) {
                throw runtime_error("Attempted to play after coins ran out.\n");
            }
            coins --;
            int w = wheelSize[machineNumber];
            int res[3] = { nextInt(w), nextInt(w), nextInt(w) };
            ret[i + 1] = "";
            REP3 (j, -1, 1 + 1) {
                REP (k, 3) {
                    ret[i + 1] += wheels[machineNumber][k][(res[k] + w + j) % w];
                }
            }
            if (ret[i + 1][3] != ret[i + 1][4] or ret[i + 1][3] != ret[i + 1][5]) {
                continue;  // when lose
            }
            if (ret[i + 1][3] == 'A') win += 1000;
            if (ret[i + 1][3] == 'B') win +=  200;
            if (ret[i + 1][3] == 'C') win +=  100;
            if (ret[i + 1][3] == 'D') win +=   50;
            if (ret[i + 1][3] == 'E') win +=   20;
            if (ret[i + 1][3] == 'F') win +=   10;
            if (ret[i + 1][3] == 'G') win +=    5;
        }
        ret[0] = to_string(win);
        coins += win;
        return ret;
    }

public:
    int quickPlay(int machineNumber, int times) {
        if (times > maxTime) {
            throw runtime_error("Attempted to play after time ran out.\n");
        }
        maxTime -= times;
        return stoi(doPlay(machineNumber, times)[0]);
    }

    vector<string> notePlay(int machineNumber, int times) {
        if (times * noteTime > maxTime) {
            throw runtime_error("Attempted to play after time ran out.\n");
        }
        maxTime -= times * noteTime;
        return doPlay(machineNumber, times);
    }
};

PlaySlotsImpl g_impl;
int PlaySlots::quickPlay(int machineNumber, int times) {
    return g_impl.quickPlay(machineNumber, times);
}
vector<string> PlaySlots::notePlay(int machineNumber, int times) {
    return g_impl.notePlay(machineNumber, times);
}

int main(int argc, char **argv) {
    int seed = -1;
    bool get_json = false;
    REP3 (i, 1, argc) {
        if (argv[i] == string("-seed")) {
            seed = stoi(argv[++ i]);
        } else if (argv[i] == string("-json")) {
            get_json = true;
        } else {
            assert (false);
        }
    }
    assert (seed != -1);
    cout << "Seed = " << seed << endl;
    cout << endl;

    g_impl = PlaySlotsImpl(seed);
    int coins       = g_impl.coins;
    int maxTime     = g_impl.maxTime;
    int noteTime    = g_impl.noteTime;
    int numMachines = g_impl.numMachines;
    cout << "Coins: " << coins << endl;
    cout << "Max Time: " << maxTime << endl;
    cout << "Note Time: " << noteTime << endl;
    cout << "Num Machines: " << numMachines << endl;
    cout << endl;
    vector<double> expected(numMachines);
    REP (i, numMachines) {
        cout << "Machine " << i << "..."  << endl;
        auto const & wheels = g_impl.wheels[i];
        int wheelSize = g_impl.wheelSize[i];
        REP (j, 3) {
            cout << "wheel " << j << ": " << wheels[j] << endl;
        }
        int payout = 0;
        payout += count(ALL(wheels[0]), 'A') * count(ALL(wheels[1]), 'A') * count(ALL(wheels[2]), 'A') * 1000;
        payout += count(ALL(wheels[0]), 'B') * count(ALL(wheels[1]), 'B') * count(ALL(wheels[2]), 'B') * 200;
        payout += count(ALL(wheels[0]), 'C') * count(ALL(wheels[1]), 'C') * count(ALL(wheels[2]), 'C') * 100;
        payout += count(ALL(wheels[0]), 'D') * count(ALL(wheels[1]), 'D') * count(ALL(wheels[2]), 'D') * 50;
        payout += count(ALL(wheels[0]), 'E') * count(ALL(wheels[1]), 'E') * count(ALL(wheels[2]), 'E') * 20;
        payout += count(ALL(wheels[0]), 'F') * count(ALL(wheels[1]), 'F') * count(ALL(wheels[2]), 'F') * 10;
        payout += count(ALL(wheels[0]), 'G') * count(ALL(wheels[1]), 'G') * count(ALL(wheels[2]), 'G') * 5;
        expected[i] = payout / pow(wheelSize, 3);
        cout << "Expected payout rate: " << expected[i] << endl;
        cout << endl;
    }

    BrokenSlotMachines().playSlots(coins, maxTime, noteTime, numMachines);
    cout << "Result: " << g_impl.coins << endl;
    cout << "Delta: " << (g_impl.coins - coins) << endl;

    if (get_json) {
        cout << "{";
        cout << "\"seed\":" << seed << ",";
        cout << "\"coins\":" << coins << ",";
        cout << "\"maxTime\":" << maxTime << ",";
        cout << "\"noteTime\":" << noteTime << ",";
        cout << "\"numMachines\":" << numMachines << ",";
        cout << "\"machines\":[";
        REP (i, numMachines) {
            if (i) cout << ",";
            cout << "{";
            cout << "\"expected\":" << expected[i] << ",";
            cout << "\"wheelSize\":" << g_impl.wheelSize[i] << ",";
            cout << "\"wheels\":[";
            REP (j, 3) {
                if (j) cout << ",";
                cout << "\"" << g_impl.wheels[i][j] << "\"";
            }
            cout << "]";
            cout << "}";
        }
        cout << "],";
        cout << "\"result\":" << g_impl.coins << "}" << endl;
    }
    return 0;
}
