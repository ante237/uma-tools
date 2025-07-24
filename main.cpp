#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "calc.hpp"

using namespace std;

int main()
{
    Uma uma(2, 2, 400, 400, 400, 400, 400,
    'a', 'a', 
    'a', 'a', 'a', 'a', 
    'a', 'a', 'a', 'a');

    //vector<double> vec = {};
    vector<double> vec = {0.015, 0.055, 0.015};
    int effStam = calcEffectiveStamina(uma, 3000, vec);

    cout << "Effective Stamina: " << effStam << "\n";
}