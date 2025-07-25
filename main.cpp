#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "calc.hpp"

using namespace std;

int main()
{
    Uma uma(2, 0, 100, 100, 100, 100, 100,
    's', 's', 
    's', 's', 's', 's', 
    'a', 'a', 'a', 'a');

    Track track(1, 0, 1000);

    //vector<double> vec = {};
    vector<double> vec = {0.015, 0.055, 0.015};
    /*
    int effStam = calcEffectiveStamina(uma, 3000, vec);

    cout << "Effective Stamina: " << effStam << "\n";
    
    */
    array<double, 5> accel = calcAccel(uma, track);
    cout << "Accel: [";
    for(auto a: accel)
    {
        cout << a << ",";
    }
    cout << "]\n";

    array<double, 5> tspeed = calcTargetSpeed(uma, track);
    cout << "TSpeed: [";
    for(auto a: tspeed)
    {
        cout << a << ",";
    }
    cout << "]\n";

    array<array<double, 2>, 6> ret =  calcDurations(uma, track);

    cout << "Start:\nAccel time: " << ret[0][0] << "s\nRun time: " << ret[0][1] << "s\n";
    cout << "Early:\nAccel time: " << ret[1][0] << "s\nRun time: " << ret[1][1] << "s\n";
    cout << "Mid:\nAccel time: " << ret[2][0] << "s\nRun time: " << ret[2][1] << "s\n";
    cout << "Late:\nAccel time: " << ret[3][0] << "s\nRun time: " << ret[3][1] << "s\n";
    cout << "Spurt(w):\nAccel time: " << ret[4][0] << "s\nRun time: " << ret[4][1] << "s\n";
    cout << "Spurt(o):\nAccel time: " << ret[5][0] << "s\nRun time: " << ret[5][1] << "s\n";

    array<double, 2> hpDrain = calcHPDrain(uma, track);
    cout << "HP Drain\nWorst: " << hpDrain[0] << "\nOptimal: " << hpDrain[1] << "\n";
   
    array<int, 2> reqSta = calcReqStamina(uma, track, vec);
    cout << "Reqired Stamina\nWorst: " << reqSta[0] << "\nOptimal: " << reqSta[1] << "\n";
}