#include <bits/stdc++.h>
#define PRINT_DEBUG 0

#include "adaptive_ls.hpp"
#include "complete_ls.hpp"
#include "gs.hpp"


using namespace std;

int main(){
    srand (time(NULL));
    CompleteLocalSearch solver;
    //AdaptiveLocalSearch solver;
    //GS solver;
    solver.solve(10000);


}
