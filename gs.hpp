#ifndef _GS_H
#define _GS_H
#include <bits/stdc++.h>

using namespace std;

struct GS{
    int r, n; // residents
    int h, m; // hospitals
    vector<int> max_cap;
    vector< vector<vector<int>> > r_prefs;
    vector< vector<vector<int>> > h_prefs;

    vector<vector<int>> prefc;
    vector<vector<int>> prefr;
    vector<unordered_map<int, int>> prefr_inv;

    vector< unordered_map<int, int> > r_prefs_inv; // resident, hospital -> index
    vector< unordered_map<int, int> > h_prefs_inv; // hospital, resident -> index

    vector<int> curr_match_r;
    vector<int> curr_err_r;
    vector<priority_queue<pair<int, int>>> curr_match_h;
    vector<priority_queue<pair<int, int>>> curr_match_h_del;
    vector<int> curr_cap;
    long long curr_obj_f;

    vector<int> best_match_r;
    long long best_obj_f;

    GS(){
        cin >> r >> h;
        m = h; n = r;
        max_cap = vector<int>(m);
        curr_cap = vector<int>(h);
        r_prefs = vector< vector<vector<int>> >(r);
        h_prefs = vector< vector<vector<int>> >(h);
        r_prefs_inv = vector< unordered_map<int, int> >(r);
        h_prefs_inv = vector< unordered_map<int, int> >(h);
        curr_match_r = vector<int>(r);
        curr_err_r = vector<int>(r);
        curr_match_h = vector<priority_queue<pair<int, int>>>(h);
        curr_match_h_del = vector<priority_queue<pair<int, int>>>(h);

        for(int i=0; i<m; i++){
            cin >> max_cap[i];
        }

        for(int i=0; i<n; i++){
            int l; cin >> l;
            r_prefs[i] = vector<vector<int>>(l);
            string s; getline(cin, s);
            for(int j=0; j<l; j++){
                getline(cin, s);
                stringstream in(s);
                int tmp;
                while(in >> tmp){
                    r_prefs[i][j].push_back(tmp);
                    r_prefs_inv[i][tmp] = j;
                }
            }
        }

        for(int i=0; i<m; i++){
            int l; cin >> l;
            h_prefs[i] = vector<vector<int>>(l);
            string s; getline(cin, s);
            for(int j=0; j<l; j++){
                getline(cin, s);
                stringstream in(s);
                int tmp;
                while(in >> tmp){
                    h_prefs[i][j].push_back(tmp);
                    h_prefs_inv[i][tmp] = j;
                }
            }
        }

        // break ties
        prefc = vector<vector<int>>(r);
        prefr = vector<vector<int>>(h);
        prefr_inv = vector<unordered_map<int, int>>(h);
        for(int i=0; i<r; i++){
            for(vector<int> tmp : r_prefs[i]){
                for(int tmpi : tmp){
                    prefc[i].push_back(tmpi);
                }
            }
        }
        for(int i=0; i<h; i++){
            for(vector<int> tmp : h_prefs[i]){
                for(int tmpi : tmp){
                    prefr[i].push_back(tmpi);
                }
            }
            for(int j=0; j<prefr[i].size(); j++){
                prefr_inv[i][prefr[i][j]] = j;
            }
        }
    }



    void solve(int max_itrs){
        vector<priority_queue<pair<int, int>>> custAtRest(m);
        vector<int> occupCust(n, 0);
        vector<int> matching(n, -1);
        int freeCnt = n;

        queue<int> notOcc;
        for(int i=0; i<n; i++) notOcc.push(i);

        while(!notOcc.empty()){
            int id= notOcc.front(); notOcc.pop();

            for(int i=0; i<prefc[id].size() && !occupCust[id]; i++){

                int rest = prefc[id][i];
                if(custAtRest[rest].size() < max_cap[rest]){  // the resturant has free seats
                    custAtRest[rest].push({prefr_inv[rest][id], id});
                    freeCnt--;
                    occupCust[id] = 1;
                    matching[id] = rest;
                    
                }
                else{

                    pair<int, int> worstCust = custAtRest[rest].top();
                    if(worstCust.first > prefr_inv[rest][id]){
                        custAtRest[rest].pop();
                        occupCust[worstCust.second] = 0;
                        notOcc.push(worstCust.second);
                        occupCust[id] = 1;
                        custAtRest[rest].push({prefr_inv[rest][id], id});
                        matching[worstCust.second] = -1;
                        matching[id] = rest;
                    }


                }

            }
            if(!occupCust[id]) occupCust[id] = 2;

        }
		
		cout << "Singles: " << freeCnt << " bl. pairs: " << 0 << endl;
		cerr << "Singles: " << freeCnt << " bl. pairs: " << 0 << endl;
        for(int i=0; i<n; i++){
            cerr << i << " -> " << matching[i] << endl;
        }




    }



};

#endif
