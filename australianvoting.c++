#include<iostream>
#include<cstdio>
#include<map>
#include<vector>
#include<list>
#include<string>
#include<sstream> 
#include<algorithm>
#include<iomanip>

using namespace std;

// class for candidate
struct Candidate {
    string name;
    list<unsigned> votes;
};

vector<Candidate> candidates;
vector< list<unsigned> > voters; // voters with their ballot selection list
vector<unsigned> eliminated_candidates; // to keep track 

unsigned num_candidates = 0; 
unsigned num_voters = 0; 

void sort() {
    vector<unsigned> top, bottom;
    unsigned num_top = ((num_voters+1)/2); // to account for odd # of voters
    unsigned num_bottom = 2000; 

    // get the top and bottom candidates
    for(unsigned i = 1; i <= num_candidates; ++i) {
        // ignore candidates that are eliminated 
        if (candidates[i].votes.empty()) {
            continue;
        }
        // if found a candidate whose votes > current max
        // or votes == current max, 
        // update the top vector 
        if (candidates[i].votes.size() > num_top) { 
            top.clear(); 
            top.push_back(i);
            num_top = candidates[i].votes.size();
        } else if (candidates[i].votes.size() == num_top) {
            top.push_back(i);
        }

        // if found a candidate whose votes < current min
        // or votes == current min, 
        // update the bottom vector 
        if (candidates[i].votes.size() < num_bottom) {
            bottom.clear();
            bottom.push_back(i);
            num_bottom = candidates[i].votes.size(); 
        } else if (candidates[i].votes.size() == num_bottom) {
            bottom.push_back(i);
        }
    }

    // now print all the winners, if any, and exit  
    if (!top.empty()) {
        for (auto c: top) {
            cout << candidates[c].name << endl;
        }
        return;
    }
    
    // go thru all the candidates at the bottom 
    // and redistribute their votes 
    for (auto b: bottom) {
        
        for (auto v: candidates[b].votes) {
            // first get rid of a ballot's top candidate 
            // if they are already eliminated 
            while(find(eliminated_candidates.begin(), eliminated_candidates.end(), 
                        voters[v].front()) != eliminated_candidates.end()) { 
                voters[v].pop_front();
            }
            
            if (!voters[v].empty()) {
                unsigned vote = voters[v].front();
                //cout << vote << endl;
                voters[v].pop_front();
                candidates[vote].votes.push_back(v);
            }
        }

        // keep track of eliminated candidates 
        candidates[b].votes.clear();
        eliminated_candidates.push_back(b);
    }

    /*
    for (unsigned j = 1; j <= num_candidates; ++j) {
        cout << candidates[j].name << " " << candidates[j].votes.size() << endl;
    }*/

    // we still haven't found a winner, sort again 
    sort();
}

int main(void) {

    voters.resize(1000); 
    candidates.resize(21);

    string line;
    unsigned cases;
    cin >> cases; 
    
    // go thru each case
    for (unsigned i = 0; i < cases; ++i) {

        cin >> num_candidates;
        // accept candidate names
        for (unsigned j = 1; j <= num_candidates; ++j) {
            do {
                getline(cin, line);
            } while (line.empty());
            candidates[j].name = line;
            candidates[j].votes.clear();
        }
        // accept voter ballots
        num_voters = 0;
        getline(cin, line);
        while (!line.empty()) {
            voters[num_voters].resize(num_candidates); 
            // parse each string from the line
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
        
        // for each candidate, update number of 
        // first choice voters
        for (unsigned j = 0; j < num_voters; ++j) {
            unsigned vote = *(voters[j].begin());
            //cout << vote << endl;
            voters[j].pop_front();
            candidates[vote].votes.push_back(j);
        }

        /* 
        for (unsigned j = 1; j <= num_candidates; ++j) {
            cout << candidates[j].name << " " << candidates[j].votes.size() << endl;
        }
        */
        
        // do the sort and elimination
        sort();
    }

    return 0;
}
