#include<iostream>
#include<cstdio>
#include<map>
#include<vector>
#include<list>
#include<string>
#include<sstream> 
#include<algorithm>

using namespace std;

struct Candidate {
    string name;
    list<unsigned> votes;
};

vector<Candidate> candidates;
vector< list<unsigned> > voters; 
vector<unsigned> eliminated_candidates;

unsigned num_candidates = 0; 
unsigned num_voters = 0; 

void sort() {

    vector<unsigned> top, bottom;
    unsigned num_top = ((num_voters+1)/2), num_bottom = 2000;

    for(unsigned i = 1; i <= num_candidates; ++i) {

        if (candidates[i].votes.empty()) {
            continue;
        }

        if (candidates[i].votes.size() > num_top) { 
            top.clear(); 
            top.push_back(i);
            num_top = candidates[i].votes.size();
        } else if (candidates[i].votes.size() == num_top) {
            top.push_back(i);
        }

        if (candidates[i].votes.size() < num_bottom) {
            bottom.clear();
            bottom.push_back(i);
            num_bottom = candidates[i].votes.size(); 
        } else if (candidates[i].votes.size() == num_bottom) {
            bottom.push_back(i);
        }
    }

    if (!top.empty()) {
        for (auto c: top) {
            cout << candidates[c].name << endl;
        }
        return;
    }

    for (auto b: bottom) {

        for (auto v: candidates[b].votes) {
            while(find(eliminated_candidates.begin(), eliminated_candidates.end(), 
                        voters[v].front()) != eliminated_candidates.end()) { 
                voters[v].pop_front();
            }

            unsigned vote = voters[v].front();
            voters[v].pop_front();
            candidates[vote].votes.push_back(v);
        }

        candidates[b].votes.clear();
        eliminated_candidates.push_back(b);
    }

    /*
    for (unsigned j = 1; j <= num_candidates; ++j) {
        cout << candidates[j].name << " " << candidates[j].votes.size() << endl;
    }*/

    sort();
}

int main(void) {

    voters.resize(1000); 
    candidates.resize(21);

    string line;
    unsigned cases;
    cin >> cases;

    for (unsigned i = 0; i < cases; ++i) {

        cin >> num_candidates;

        for (unsigned j = 1; j <= num_candidates; ++j) {
            do {
                getline(cin, line);
            } while (line.empty());
            candidates[j].name = line;
            candidates[j].votes.clear();
        }

        num_voters = 0;
        getline(cin, line);
        while (!line.empty()) {
            // parse each string from the line
            voters[num_voters].resize(num_candidates); 
            istringstream stream(line);
            list<unsigned> l;
            for (unsigned k=0; stream.good(); k++) {
                unsigned tmp;
                stream >> tmp;
                l.push_back(tmp);
                //cout << num_voters << " " << k << " " << l.back() << endl;
            } 
            voters[num_voters] = move(l);
            ++num_voters;   
            getline(cin, line);
        }

        //cout << "num_voters = " << num_voters << ", num_candidates " << num_candidates <<"\n";

        for (unsigned j = 0; j < num_voters; ++j) {
            unsigned vote = *(voters[j].begin());
            //cout << vote << endl;
            voters[j].pop_front();
            candidates[vote].votes.push_back(j);
        }

        /* 
        for (unsigned j = 1; j <= num_candidates; ++j) {
            cout << candidates[j].name << " " << candidates[j].votes.size() << endl;
        }*/

        sort();
    }

    return 0;
}
