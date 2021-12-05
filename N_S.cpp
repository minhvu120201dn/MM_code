#include <iostream>
#include <map>
#include <vector>
#include <set>

using namespace std;

int m0[] = {1, 0, 0};
pair< int, string > place[] = {make_pair(m0[0], "free"), make_pair(m0[1], "busy"), make_pair(m0[2], "docu")};
#define FREE 0
#define BUSY 1
#define DOCU 2

class Transition {
    set<int> in;
    set<int> out;
public:
    Transition() {}
    Transition(const Transition&& copy) {
        this->in = move(copy.in);
        this->out = move(copy.out);
    }
    Transition(set<int>& in, set<int>& out) : in(move(in)), out(move(out)) {}
    Transition& operator=(const Transition&& copy) {
        this->in = move(copy.in);
        this->out = move(copy.out);
        return *this;
    }
    inline bool available() { return !in.empty() || !out.empty(); }
    inline void fire();
};
map< string, Transition > transition;

void Transition::fire() {
    for (int p : in)
        if (!place[p].first)
            return;
    for (int p : in)
        place[p].first -= 1;
    for (int p : out)
        place[p].first += 1;
}

void make_transition() {
    {
        set<int> in;
        set<int> out;
        in.insert(FREE);
        out.insert(BUSY);
        transition["start"] = Transition(in, out);
    }

    {
        set<int> in;
        set<int> out;
        in.insert(BUSY);
        out.insert(DOCU);
        transition["change"] = Transition(in, out);
    }

    {
        set<int> in;
        set<int> out;
        in.insert(DOCU);
        out.insert(FREE);
        transition["end"] = Transition(in, out);
    }
}

void get_initial_marking() {
    int ind = 0;
    for (auto &p : place) {
        cout << "Initial marking of state " << p.second << ": ";
        cin >> p.first;
        m0[ind++] = p.first;
    }
}

void displayP() {
    cout << "P  = {";
    bool begin = true;
    for (auto &x : transition) {
        if (!begin) cout << ", ";
        begin = false;

        cout << x.first;
    }
    cout << '}' << endl;
}

void displayT() {
    cout << "T  = {";
    bool begin = true;
    for (auto &x : place) {
        if (!begin) cout << ", ";
        begin = false;

        cout << x.second;
    }
    cout << '}' << endl;
}

void displayM0() {
    cout << "M0 = [";
    bool begin = true;
    int ind = 0;
    for (auto &x : place) {
        if (!begin) cout << ", ";
        begin = false;

        cout << m0[ind++] << '.' << x.second;
    }
    cout << ']' << endl;
}

void displayM() {
    cout << "M  = [";
    bool begin = true;
    for (auto &x : place) {
        if (!begin) cout << ", ";
        begin = false;

        cout << x.first << '.' << x.second;
    }
    cout << ']' << endl;
}

int main() {
    make_transition();
    get_initial_marking();
    while (true) {
        system("cls");
        cout << "N  = {P, T, F, M0}" << endl;
        displayP();
        displayT();
        displayM0();
        displayM();

        cout << "Type here the transition you want to fire: ";
        string x; cin >> x;
        Transition &temp = transition[x];
        if (!temp.available()) {
            cout << "Transition not available" << endl;
            system("pause");
            return 0;
        }
        temp.fire();
    }
}