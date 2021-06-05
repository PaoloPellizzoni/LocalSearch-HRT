#ifndef _AdaptiveLocalSearch_H
#define _AdaptiveLocalSearch_H

#include <bits/stdc++.h>

using namespace std;

struct AdaptiveLocalSearch{
    int r; // residents
    int h; // hospitals
    vector<int> max_cap;
    vector< vector<vector<int>> > r_prefs;
    vector< vector<vector<int>> > h_prefs;
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


    AdaptiveLocalSearch(){
        cin >> r >> h;
        int m = h, n = r;
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
    }

    int bl_pair_error(int res, int hos){
        int worst_res_in_h;
        if(curr_cap[hos] < max_cap[hos]) worst_res_in_h = -1;
        else    worst_res_in_h = curr_match_h[hos].top().second;

        int rank_w_r = (worst_res_in_h != -1) ? h_prefs_inv[hos][worst_res_in_h] : r;
        int rank_res;
        if(h_prefs_inv[hos].find(res) == h_prefs_inv[hos].end()){ // res finds hos not acceptable
            rank_res = r;
        }
        else{
            rank_res = h_prefs_inv[hos][res];
        }
        if(rank_res == r) return 0; // res not a valid partner for hos, not bp
        if(rank_w_r == r) return 1; // hos has still space, bp

        return max(0, rank_w_r - rank_res);
    }

    bool is_bl_pair(int res, int hos){
        if(r_prefs_inv[res].find(hos) == r_prefs_inv[res].end()){ // res finds hos not acceptable
            return false;
        }
        if(h_prefs_inv[hos].find(res) == h_prefs_inv[hos].end()){ // hos finds res not acceptable
            return false;
        }
        int rank;
        if(r_prefs_inv[res].find(curr_match_r[res]) != r_prefs_inv[res].end())
            rank = r_prefs_inv[res][curr_match_r[res]];
        else
            rank = h;
        if(curr_match_r[res] < 0 || r_prefs_inv[res][hos] < rank ){ // res prefers hos
            while(curr_match_h_del[hos].size() > 0 && curr_match_h[hos].top() == curr_match_h_del[hos].top()){
                curr_match_h[hos].pop();
                curr_match_h_del[hos].pop();
            }
            if(curr_cap[hos] < max_cap[hos] || h_prefs_inv[hos][res] < curr_match_h[hos].top().first ){ // hos prefers res
                return true;
            }
        }
        return false;
    }


    long long cost_of_matching(){
        int singles = 0;
        long long bp = 0;
        for(int i=0; i<r; i++){
            int hos = curr_match_r[i];
            int rankh;
            if(hos == -1){
                singles++;
                rankh = h;
            }
            else{
                if(r_prefs_inv[i].find(hos) != r_prefs_inv[i].end())
                    rankh = r_prefs_inv[i][hos];
                else
                    rankh = h;
            }
            for(int j=0; j<rankh && j < r_prefs[i].size(); j++){
                for(int hos1 : r_prefs[i][j]){
                    if(bl_pair_error(i, hos1) > 0){
                        bp++;
                    }

                }
            }

        }
        return bp*r + singles;
    }


    vector<pair<int, int>> undom_bl_pairs(){
        vector<int> ubp(h);
        for(int i=0; i<h; i++)
            ubp[i] = -1;
        for(int i = 0; i < r; i++){
            bool found = false;
            int id_curr_match_r;
            if(curr_match_r[i] == -1)
                id_curr_match_r = r_prefs[i].size();
            else
                id_curr_match_r = r_prefs_inv[i][curr_match_r[i]];
            for(int j = 0; !found && j < id_curr_match_r; j++){ // foreach hospital better than current
                for(int hosp : r_prefs[i][j]){
                    curr_err_r[i] = bl_pair_error(i, hosp);
                    if(curr_err_r[i] > 0){
                        if(ubp[hosp]==-1 || h_prefs_inv[hosp][i] < h_prefs_inv[hosp][ubp[hosp]]){
                            ubp[hosp] = i;
                            found = true;
                            break;
                        }
                    }
                }
            }
        }
        vector<pair<int, int>> ans;
        for(int i=0; i<h; i++){
            if(ubp[i] != -1){
                ans.push_back(make_pair(ubp[i], i));
            }
        }
        return ans;
    }

    void reset(vector<pair<int, int>>& ubp){
        if(curr_obj_f >= r){
            int rnd_r = rand()%r;
            vector<int> tmp = r_prefs[rnd_r][rand()%(r_prefs[rnd_r].size())];
            int rnd_h = tmp[rand()%tmp.size()];

            fix_pair(rnd_r, rnd_h);
        }

        vector<int> sings;
        for(int i=0; i<r; i++){
            if(curr_match_r[i] == -1)
                sings.push_back(i);
        }
        int rnd_r;
        if(sings.size()>0)
            rnd_r = sings[rand()%sings.size()];
        else
            rnd_r = rand()%r;
        vector<int> tmp = r_prefs[rnd_r][rand()%(r_prefs[rnd_r].size())];
        int rnd_h = tmp[rand()%tmp.size()];

        fix_pair(rnd_r, rnd_h);
        
        curr_obj_f = cost_of_matching();
        if(curr_obj_f < best_obj_f){
            best_match_r = curr_match_r;
            best_obj_f = curr_obj_f;
        }
    }

    void fix_pair(int re, int ho){
        int rank = h_prefs_inv[ho][re];
        int prv_ho = curr_match_r[re];

        if(prv_ho != -1){
            pair<int, int> pairdel = make_pair(h_prefs_inv[prv_ho][re], re);
            curr_match_h_del[prv_ho].push(pairdel);
            curr_cap[prv_ho]--;
        }

        if(curr_cap[ho] >= max_cap[ho]){
            while(curr_match_h_del[ho].size() > 0 && curr_match_h[ho].top() == curr_match_h_del[ho].top()){
                curr_match_h[ho].pop();
                curr_match_h_del[ho].pop();
            }
            int worst_res = curr_match_h[ho].top().second;
            curr_match_h[ho].pop();
            curr_match_r[worst_res] = -1;
            curr_cap[ho]--;
        }

        curr_match_r[re] = ho;
        curr_match_h[ho].push(make_pair(rank, re));
        curr_cap[ho]++;

    }

    void solve(int max_itrs){
        for(int i=0; i<r; i++){
            int lv = rand()%(r_prefs[i].size());
            int hos = r_prefs[i][lv][rand()%(int)(r_prefs[i][lv].size())];
            if(curr_cap[hos] == max_cap[hos]){
                curr_match_r[i] = -1;
                continue;
            }
            int rank;
            if(h_prefs_inv[hos].find(i) != h_prefs_inv[hos].end())
                rank = h_prefs_inv[hos][i];
            else
                rank = r;
            curr_match_r[i] = hos;
            curr_match_h[hos].push(make_pair(rank, i));
            curr_cap[hos]++;
        }
        curr_obj_f = cost_of_matching();
        best_obj_f = curr_obj_f;
        // main loop
        int iters = 0;
        while(iters < max_itrs){
            if(PRINT_DEBUG && iters%1 == 0) cerr <<"Iter: "<< iters <<" f:"<<curr_obj_f << " bf:" << best_obj_f << endl;

            if(curr_obj_f == 0) // perfect matching
                break;

            vector<pair<int, int>> ubp = undom_bl_pairs();
            if(ubp.size() == 0){
                if(PRINT_DEBUG) cerr << "Reset no bp" << endl;
                reset(ubp);


            }
            else{
                int mini = 0;
                int minf = (r+1)*(h+1);
                for(int i=0; i<ubp.size(); i++){
                    int re = ubp[i].first;
                    if(curr_err_r[re] > minf){
                        minf = curr_err_r[re];
                        mini = i;
                    }
                }
                pair<int, int> bp = ubp[mini];
                int re = bp.first;
                int ho = bp.second;
                // remove worst pair
                fix_pair(re, ho);

                int new_obj_f = cost_of_matching();

                if(new_obj_f < curr_obj_f){
                    curr_obj_f = new_obj_f;
                    if(curr_obj_f < best_obj_f){
                        best_match_r = curr_match_r;
                        best_obj_f = curr_obj_f;
                    }
                }
                else{
                    if(PRINT_DEBUG) cerr << "Reset local minima" << endl;
                    reset(ubp);
                }
            }

            iters++;

        }
        cerr << "Singles: " << best_obj_f%r << " bl. pairs: " << best_obj_f/r << endl;
        cout << "Singles: " << best_obj_f%r << " bl. pairs: " << best_obj_f/r << endl;
        for(int i=0; i<r; i++)
            //if(best_match_r[i] != -1)
                cout << i << " -> " << best_match_r[i] <<endl;

    }



};

#endif
