#include <iostream>
#include <math.h>

#include "calc.hpp"
#include "modifiers.hpp"

using namespace std;


int calcStamNeeded(Uma& uma, int trackLen, vector<double> recovery)
{
    array<int, 5> stats = uma.getStats();
    int sta = stats[1];

    double hp = calcHP(uma, trackLen);
    double ehp = calcEHP(uma, hp, recovery);
    double styleCoeff = getHPDrainCoeff(uma.getStyle());

    return 0;
}

/**
 * @brief Caculates the effective stamina of the uma. This is the stamina value that
 * the skill recovery would amount to.
 * 
 * @param uma The uma object
 * @param trackLen The length of the track in meters
 * @param recovery A vector of the recovery skills
 * @return The calculated effective stamina of the uma
 */
int calcEffectiveStamina(Uma& uma, int trackLen, vector<double> recovery)
{
    array<int, 5> stats = uma.getStats();
    int sta = stats[1];
    int style = uma.getStyle();

    double stratCoeff = getHPDrainCoeff(style);
    double hp = calcHP(uma, trackLen);
    double ehp = calcEHP(uma, hp, recovery);
    return sta + round((ehp - hp) / (0.8 * stratCoeff));
}

/**
 * @brief Calculates the effective HP of the uma. This is the hp it would have
 * considering the probabalistic hp recovered by recovery skills
 * 
 * @param uma The uma object
 * @param hp The HP of the uma on the given track
 * @param recovery A vector of the recovery skills
 * @return Total effective HP of the uma
 */
double calcEHP(Uma& uma, double hp, vector<double> recovery)
{
    array<int, 5> stats = uma.getStats();
    int sta = stats[1];

    double procProb = calcSkillProb(uma);
    double totalRecovery = 0;

    for(auto rec: recovery)
    {
        totalRecovery += rec * procProb;
    }
    double ehp = hp + hp * totalRecovery;
    cout << "EHP: " << ehp << "\n";
    return  ehp;
}

/**
 * @brief Calculates the HP of the uma based on the track
 * 
 * @param uma The uma object
 * @param trackLen The length of the track in meters
 * @return Total HP of the uma 
 */
double calcHP(Uma& uma, int trackLen)
{
    double stratCoeff = getHPDrainCoeff(uma.getStyle());
    double hp = (0.8 * stratCoeff * uma.sta * uma.getMoodMult()) + trackLen;
    cout << "HP: " << hp << "\n";
    return hp;
}


array<int, 2> calcReqStamina(Uma& uma, Track& track, vector<double> recovery)
{
    int style = uma.getStyle();
    double styleMod = getHPDrainCoeff(style);

    array<double, 2> hpDrain = calcHPDrain(uma, track);
    double procProb = calcSkillProb(uma);
    double totalRecovery = 0;

    for(auto rec: recovery)
    {
        totalRecovery += rec * procProb;
    }

    double worstDrain = hpDrain[0] * (1 - totalRecovery);
    double optimalDrain = hpDrain[1] * (1 - totalRecovery);

    return {
        (int)((worstDrain - track.length) / (0.8 * styleMod)),
        (int)((optimalDrain - track.length) / (0.8 * styleMod))
    };
}

array<double, 2> calcHPDrain(Uma& uma, Track& track)
{
    array<int, 5> stats = uma.getStats();
    double spurtMod = 1 + (200.0 / sqrt(600.0 * stats[3]));
    double surfaceDrainMod = getSurfaceHPDrainMod(track.surface, track.condition);
    array<double, 5> targetSpeeds = calcTargetSpeed(uma, track);
    array<double, 5> accels = calcAccel(uma, track);
    array<array<double, 2>, 6> durations = calcDurations(uma, track, targetSpeeds, accels);

    double baseSpeed = calcBaseSpeed(track.length);
    double baseConsumption = 20 * baseSpeed  * durations[0][0];
    double startSpeed = min(targetSpeeds[0], 3 + accels[0] * durations[0][0]);
    double drain = 0;

    
    for(int i = 1; i < 3; i++)
    {
        //Quadradic motion (accel)
        if(accels[i] != 0)
        {
            drain = 20 * (pow(accels[i] * durations[i][0] + startSpeed - 
                baseSpeed + 12, 3) - pow(startSpeed - baseSpeed + 12, 3)) / (3 * accels[i]) / 144;
            baseConsumption += drain;
        }
        else
        {
            drain = 20 * pow(targetSpeeds[i] - baseSpeed  + 12, 2) / 144 * durations[i][0];
            baseConsumption += drain;   
        }
        //Linear motion
        drain = 20 * pow(targetSpeeds[i] - baseSpeed  + 12, 2) / 144 * durations[i][1];
        baseConsumption += drain;

        //Next iteration starting speed
        startSpeed = min(targetSpeeds[i+1], startSpeed + accels[i+1] * (durations[i+1][0] + durations[i+1][1]));
    }

    double startSpeed2 = startSpeed;
    double worstConsumption = baseConsumption;
    double bestConsumption = baseConsumption;

    //Calculate worst acceptable case
    drain = 20 * (pow(accels[3] * durations[3][0] + startSpeed - baseSpeed + 12, 3) - 
            pow(startSpeed - baseSpeed + 12, 3)) / (3 * accels[3]) / 144;
    worstConsumption += drain;

    drain = 20 * pow(targetSpeeds[3] - baseSpeed  + 12, 2) / 144 * durations[3][1];
    worstConsumption += drain;

    startSpeed = min(targetSpeeds[4], startSpeed + accels[4] * (durations[4][0] + durations[4][1]));

    drain = spurtMod * 20 * (pow(accels[4] * durations[4][0] + startSpeed - 
        baseSpeed + 12, 3) - pow(startSpeed - baseSpeed + 12, 3)) / (3 * accels[4]) / 144;
    worstConsumption += drain;

    drain = spurtMod * 20 * pow(targetSpeeds[4] - baseSpeed  + 12, 2) / 
        144 * durations[4][1];
    worstConsumption += drain;

    //Calculate optimal case
    drain = spurtMod * 20 * (pow(accels[4] * durations[5][0] + startSpeed2 - 
        baseSpeed + 12, 3) - pow(startSpeed2 - baseSpeed + 12, 3)) / (3 * accels[4]) / 144;
    bestConsumption += drain;

    drain = spurtMod * 20 * pow(targetSpeeds[4] - baseSpeed  + 12, 2) / 
        144 * durations[5][1];
    bestConsumption += drain;
    
    return {
        worstConsumption * surfaceDrainMod, 
        bestConsumption * surfaceDrainMod
    };
}

/**
 * @brief Calculate the duration spent in acceleration and steady speed during each phase
 * 
 * @param uma 
 * @param track 
 * @return array<array<double, 2>, 6> 
 * 0: start dash
 * 1: early race
 * 2: mid race
 * 3: late race (worst acceptable)
 * 4: final spurt (worst acceptable)
 * 5: final spurt (optimal)
 */
array<array<double, 2>, 6> calcDurations(Uma& uma, Track& track)
{
    array<double, 5> targetSpeeds = calcTargetSpeed(uma, track);
    array<double, 5> accels = calcAccel(uma, track);

    array<int, 5> stats = uma.getStats();

    double currentPhaseLen = track.early;
    //Assume start speed for phase is target speed of prev.
    array<double, 5> startSpeed = {3.0, targetSpeeds[0], targetSpeeds[1], targetSpeeds[2], targetSpeeds[3]};
    
    //Early race
    array<double, 2> startDash;
    array<double, 2> early;
    startDash[0] = (targetSpeeds[0] - 3.0) / accels[0];
    startDash[1] = 0;
    double dashAccelDist = (startSpeed[0] + targetSpeeds[0]) / 2 * startDash[0];
    currentPhaseLen -= dashAccelDist;
    
    //Calculate time needed to reach speed and distance, then use the shorter of the two
    double reachAccel = (targetSpeeds[1] - targetSpeeds[0]) / accels[1];
    double reachDist = (-targetSpeeds[0] + 
        sqrt(pow(targetSpeeds[0], 2) + 2 * accels[1] * currentPhaseLen)) / accels[1];
    early[0] = min(reachAccel, reachDist);
    double earlyAccelDist = (startSpeed[1] + accels[1] * early[0] / 2) * early[0];
    currentPhaseLen -= earlyAccelDist;
    //Duration for rest of early phase
    early[1] = currentPhaseLen / targetSpeeds[1];
    //If target speed is not met before distance, change start speed for next phase
    if(currentPhaseLen < 0)
        startSpeed[2] = startSpeed[1] + (accels[1] * early[0]);

    //Mid race
    array<double, 2> mid;
    currentPhaseLen = track.mid - track.early;
    double currentAccel = accels[2];
    //If start speed is bigger than target speed, set accel to negative.
    if(startSpeed[2] > targetSpeeds[2])
        currentAccel = -0.8;
    mid[0] = (targetSpeeds[2] - startSpeed[2]) / currentAccel;
    double midAccelDist = (startSpeed[2] + targetSpeeds[2]) / 2 * mid[0];
    currentPhaseLen -= midAccelDist;
    mid[1] = currentPhaseLen / targetSpeeds[2];
    //If target speed is not met before distance, change start speed for next phase
    if(currentPhaseLen < 0)
        startSpeed[3] = startSpeed[2] + (accels[2] * mid[0]);

    //Late race (worst)
    array<double, 2> late;
    currentPhaseLen = track.late - track.mid;
    currentAccel = accels[3];
    //If start speed is bigger than target speed, set accel to negative.
    if(startSpeed[3] > targetSpeeds[3])
        currentAccel = -0.8;
    
    late[0] = (targetSpeeds[3] - startSpeed[3]) / currentAccel;
    double LateAccelDist = (startSpeed[3] + targetSpeeds[3]) / 2 * late[0];
    currentPhaseLen -= LateAccelDist;
    
    late[1] = currentPhaseLen / targetSpeeds[3];
    if(currentPhaseLen < 0)
        startSpeed[4] = startSpeed[3] + (accels[3] * late[0]);

    //Final spurt (worst)
    array<double, 2> spurtWorst;
    currentPhaseLen = track.spurt - track.late;
    currentAccel = accels[4];
    if(startSpeed[4] > targetSpeeds[4])
        currentAccel = -0.8;
    spurtWorst[0] = (targetSpeeds[4] - startSpeed[4]) / currentAccel;
    double spurtWorstAccelDist = (startSpeed[4] + targetSpeeds[4]) / 2 * spurtWorst[0];
    currentPhaseLen -= spurtWorstAccelDist;

    spurtWorst[1] = currentPhaseLen / targetSpeeds[4];

    //Final spurt (optimal)
    array<double, 2> spurtOpt;
    currentPhaseLen = track.spurt - track.mid;
    currentAccel = accels[4];
    if(startSpeed[3] > targetSpeeds[4])
        currentAccel = -0.8;
    spurtOpt[0] = (targetSpeeds[4] - startSpeed[3]) / currentAccel;
    double spurtOptAccelDist = (startSpeed[3] + targetSpeeds[4]) / 2 * spurtOpt[0];
    currentPhaseLen -= spurtOptAccelDist;
    spurtOpt[1] = currentPhaseLen / targetSpeeds[4];

    return {startDash, early, mid, late, spurtWorst, spurtOpt};
}

/** 
 * @brief Overload to avoid re-calculations
 * @overload
 * @param uma 
 * @param track 
 * @param targetSpeeds 
 * @param accels 
 * @return array<array<double, 2>, 6> 
 */
array<array<double, 2>, 6> calcDurations(Uma& uma, Track& track, std::array<double, 5> targetSpeeds, std::array<double, 5> accels)
{
    array<int, 5> stats = uma.getStats();

    double currentPhaseLen = track.early;
    //Assume start speed for phase is target speed of prev.
    array<double, 5> startSpeed = {3.0, targetSpeeds[0], targetSpeeds[1], targetSpeeds[2], targetSpeeds[3]};
    
    //Early race
    array<double, 2> startDash;
    array<double, 2> early;
    startDash[0] = (targetSpeeds[0] - 3.0) / accels[0];
    startDash[1] = 0;
    double dashAccelDist = (startSpeed[0] + targetSpeeds[0]) / 2 * startDash[0];
    currentPhaseLen -= dashAccelDist;
    
    //Calculate time needed to reach speed and distance, then use the shorter of the two
    double reachAccel = (targetSpeeds[1] - targetSpeeds[0]) / accels[1];
    double reachDist = (-targetSpeeds[0] + 
        sqrt(pow(targetSpeeds[0], 2) + 2 * accels[1] * currentPhaseLen)) / accels[1];
    early[0] = min(reachAccel, reachDist);
    double earlyAccelDist = (startSpeed[1] + accels[1] * early[0] / 2) * early[0];
    currentPhaseLen -= earlyAccelDist;
    //Duration for rest of early phase
    early[1] = currentPhaseLen / targetSpeeds[1];
    //If target speed is not met before distance, change start speed for next phase
    if(currentPhaseLen < 0)
        startSpeed[2] = startSpeed[1] + (accels[1] * early[0]);

    //Mid race
    array<double, 2> mid;
    currentPhaseLen = track.mid - track.early;
    double currentAccel = accels[2];
    //If start speed is bigger than target speed, set accel to negative.
    if(startSpeed[2] > targetSpeeds[2])
        currentAccel = -0.8;
    mid[0] = (targetSpeeds[2] - startSpeed[2]) / currentAccel;
    double midAccelDist = (startSpeed[2] + targetSpeeds[2]) / 2 * mid[0];
    currentPhaseLen -= midAccelDist;
    mid[1] = currentPhaseLen / targetSpeeds[2];
    //If target speed is not met before distance, change start speed for next phase
    if(currentPhaseLen < 0)
        startSpeed[3] = startSpeed[2] + (accels[2] * mid[0]);

    //Late race (worst)
    array<double, 2> late;
    currentPhaseLen = track.late - track.mid;
    currentAccel = accels[3];
    //If start speed is bigger than target speed, set accel to negative.
    if(startSpeed[3] > targetSpeeds[3])
        currentAccel = -0.8;
    
    late[0] = (targetSpeeds[3] - startSpeed[3]) / currentAccel;
    double LateAccelDist = (startSpeed[3] + targetSpeeds[3]) / 2 * late[0];
    currentPhaseLen -= LateAccelDist;
    
    late[1] = currentPhaseLen / targetSpeeds[3];
    if(currentPhaseLen < 0)
        startSpeed[4] = startSpeed[3] + (accels[3] * late[0]);

    //Final spurt (worst)
    array<double, 2> spurtWorst;
    currentPhaseLen = track.spurt - track.late;
    currentAccel = accels[4];
    if(startSpeed[4] > targetSpeeds[4])
        currentAccel = -0.8;
    spurtWorst[0] = (targetSpeeds[4] - startSpeed[4]) / currentAccel;
    double spurtWorstAccelDist = (startSpeed[4] + targetSpeeds[4]) / 2 * spurtWorst[0];
    currentPhaseLen -= spurtWorstAccelDist;

    spurtWorst[1] = currentPhaseLen / targetSpeeds[4];
    //Final spurt (optimal)
    array<double, 2> spurtOpt;
    currentPhaseLen = track.spurt - track.mid;
    currentAccel = accels[4];
    if(startSpeed[3] > targetSpeeds[4])
        currentAccel = -0.8;
    spurtOpt[0] = (targetSpeeds[4] - startSpeed[3]) / currentAccel;
    double spurtOptAccelDist = (startSpeed[3] + targetSpeeds[4]) / 2 * spurtOpt[0];
    currentPhaseLen -= spurtOptAccelDist;
    spurtOpt[1] = currentPhaseLen / targetSpeeds[4];

    return {startDash, early, mid, late, spurtWorst, spurtOpt};
}

array<double, 5> calcTargetSpeed(Uma& uma, Track& track)
{
    array<double, 5> ret;

    array<int, 5> stats = uma.getStats();
    int spd = stats[0] + getSurfaceSpeedMod(track.surface, track.condition);
    int wit = stats[4];
    int style = uma.getStyle();
    int lenClass = getLenClass(track.length);

    char distApt = uma.getDistApt()[lenClass];

    double baseSpeed = calcBaseSpeed(track.length);

    double a, b, c;

    a = baseSpeed * getStyleSpeedCoeff(style, 1); // Base speed mid
    b = ((wit / 5500.0) * log10(wit * 0.1) - 0.65 / 2) * 0.01 * baseSpeed; //Wit mod

    //Abnormal state modifiers
    double abnTopSpeed = a + b;
    double abnUptime = (track.length/24.0/abnTopSpeed) / (track.length/24.0/abnTopSpeed+3.0);
    double abnStateMod = 1 + abnUptime * (style != 0 ? -0.055*0.05 : 0.04*0.2*log10(wit*0.1));

    //Mid phase target
    ret[2] = (a + b) * abnStateMod;

    //Start target
    ret[0] = baseSpeed * 0.85;

    //Early phase target
    a = baseSpeed * getStyleSpeedCoeff(style, 0); //Base speed early
    ret[1] = a + b;

    //Late phase target
    a = baseSpeed * getStyleSpeedCoeff(style, 2); //Base speed late
    c = sqrt(500 * spd) * getDistSpeedCoeff(distApt) * 0.002; //Spd mod
    ret[3] = a + b + c;

    //Final spurt target
    a = baseSpeed * (getStyleSpeedCoeff(style, 2) + 0.01); //Base speed spurt
    b = (a + c) * 1.05; // Spurt speed mod
    ret[4] = b + c;
    return ret;
}

/**
 * @brief Calculates the acceleration for each phase of the race
 * 
 * @param uma The uma object
 * @param trackLen The length of the track in meters
 * @param trackSurface The surface type of the track
 * @param trackCondition The condition of the track
 * @return array<double, 4> An array with all the acceleration calcs in the format 
 * [Starting dash, early, mid, late/final spurt]
 */
array<double, 5> calcAccel(Uma& uma, Track& track)
{
    array<double, 5> ret;

    array<int, 5> stats = uma.getStats();
    int pow = stats[2] + getSurfacePowerMod(track.surface, track.condition);
    int style = uma.getStyle();
    int lenClass = getLenClass(track.length);

    char distApt = uma.getDistApt()[lenClass];
    char turfApt = uma.getSurfaceApt()[track.surface];

    double distAccelMod = getDistAcccelCoeff(distApt);
    double turfAptMod = getTurfCoeff(turfApt);

    for(int i = 0; i < 3; i++)
    {
        double styleAccelMod = getStyleAcccelCoeff(style, i);
        double accel = 0.0006 * sqrt(500 * pow) * styleAccelMod * distAccelMod * turfAptMod;
        //cout << "Debug: " << pow << "\n";
        if(i == 0)
            ret[0] = 24 + accel;
        ret[i + 1] = accel;
    }
    ret[4] = ret[3];
    return ret;
}

double calcBaseSpeed(int trackLen)
{
    return (20 - (trackLen - 2000) / 1000.0);
}


/**
 * @brief Calculate the probability of a skill being used
 * 
 * @param uma The uma object
 * @return Percentage of a skill being used in decimal format
 */
double calcSkillProb(Uma& uma)
{
    array<int, 5> stats = uma.getStats();
    double effWit = stats[4];
    char styleApt = uma.getCurrentStyleApt();

    double coeff = getStyleCoeff(styleApt);
    effWit = effWit * coeff;
    double prob = 1 - (90 / effWit);
    return max(prob, 0.2);
}
