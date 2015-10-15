#include<iostream>
#include<cstdio>
#include<map>
#include<vector>
#include<list>
#include<string>
#include<cstring>
#include<sstream> 
#include<algorithm>
#include<iomanip>
#include<set>

using namespace std;

// class for candidate
struct Candidate {
    char name[81];
    list<unsigned> votes;
};

vector<Candidate> candidates;
vector< list<unsigned> > voters; // voters with their ballot selection list
set<unsigned> remain_candidates; // to keep track 

unsigned num_candidates = 0; 
unsigned num_voters = 0; 

void sort() {
    vector<unsigned> top, bottom;
    unsigned num_top = ((num_voters+1)/2); // to account for odd # of voters
    unsigned num_bottom = 2000; 

    // get the top and bottom candidates
    for(unsigned i: remain_candidates) {
        // if found a candidate whose votes > current max
        // or votes == current max, 
        // update the top vector 
        if (candidates[i].votes.size() > num_top) { 
            cout << candidates[i].name << endl;
            return;
            /*
            top.clear(); 
            top.push_back(i);
            num_top = candidates[i].votes.size();
            */
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

    //cout << "bottom " << bottom.size() << " remain " << remain_candidates.size() << endl;
    unsigned left = remain_candidates.size() - bottom.size(); 
    if (left == 0) {
        // all tie 
        for (auto c: bottom) {
            cout << candidates[c].name << endl;
        }
        return;
    } else if (left == 1) {
        // only one left, must be the winner, so no need to recount the votes
        for (unsigned c: remain_candidates) {
            if (candidates[c].votes.size() > num_bottom) {
                cout << candidates[c].name << endl;
                return;
            }
        }
    }

    // go thru all the candidates at the bottom 
    // and redistribute their votes 
    for (auto b: bottom) {
        
        for (auto v: candidates[b].votes) {
            // first get rid of a ballot's top candidate 
            // if they are already eliminated 
            while (!voters[v].empty() && 
                    remain_candidates.find(voters[v].front()) == remain_candidates.end()) { 
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
        remain_candidates.erase(b);
    }

    /*
    for (unsigned j = 1; j <= num_candidates; ++j) {
        cout << candidates[j].votes.size() << " " << candidates[j].name << endl;
    }*/

    // we still haven't found a winner, sort again 
    sort();
}

int main(void) {

    voters.resize(1000); 
    candidates.resize(21);
    for (auto &v: voters){
        v.resize(21);
    }

    string line;
    unsigned cases;
    scanf("%d\n\n", &cases);
    //cout << cases << endl;

    // go thru each case
    for (unsigned i = 0; i < cases; ++i) {

        if (i > 0) {
            cout << endl;
        }

        remain_candidates.clear();

        scanf("%d\n", &num_candidates);
        //cout << num_candidates << endl;
        // accept candidate names
        for (unsigned j = 1; j <= num_candidates; ++j) {
            gets(candidates[j].name);
            candidates[j].votes.clear();
            //cout << j << " " << candidates[j].name << endl;
        }

        // accept voter ballots
        num_voters = 0;
        while(getline(cin, line) && !line.empty() && (line[0] != '\n') && (line[0] != ' ')) {
            //cout << "size " << line.size() << " '" << line << "'" << endl;;
            //voters[num_voters].resize(num_candidates); 
            // parse each string from the line
            istringstream stream(line);
            list<unsigned> l;
            for (unsigned k = 0; k < num_candidates; ++k) {
                unsigned tmp;
                stream >> tmp;
                l.push_back(tmp);
                //cout << num_voters << " " << k << " " << l.back() << endl;
            } 
            voters[num_voters] = move(l);
            ++num_voters;   
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

        for (unsigned j = 1; j <= num_candidates; ++j) {
            if (!candidates[j].votes.empty()) {
                remain_candidates.insert(j);
            }
            //cout << candidates[j].votes.size() << " " << candidates[j].name << endl;
        }
        
        if (remain_candidates.size() == 1) {
            cout << candidates[*remain_candidates.begin()].name << endl;
        } else {
            // do the sort and elimination
            sort();
        }
    }

    return 0;
}
