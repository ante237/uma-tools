#include <stdexcept>
#include <array>
#include <cctype>

#define CAREER_PAD 400

#define FRONT_HPDRAIN_COEFF 0.95
#define FRONT_SPEED_EARLY_COEFF 1
#define FRONT_SPEED_MID_COEFF 0.98
#define FRONT_SPEED_LATE_COEFF 0.962
#define FRONT_ACCEL_EARLY_COEFF 1
#define FRONT_ACCEL_MID_COEFF 1
#define FRONT_ACCEL_LATE_COEFF 0.996

#define PACE_HPDRAIN_COEFF 0.89
#define PACE_SPEED_EARLY_COEFF 0.978
#define PACE_SPEED_MID_COEFF 0.991
#define PACE_SPEED_LATE_COEFF 0.975
#define PACE_ACCEL_EARLY_COEFF 0.985
#define PACE_ACCEL_MID_COEFF 1
#define PACE_ACCEL_LATE_COEFF 0.996

#define LATE_HPDRAIN_COEFF 1.0
#define LATE_SPEED_EARLY_COEFF 0.938
#define LATE_SPEED_MID_COEFF 0.998
#define LATE_SPEED_LATE_COEFF 0.994
#define LATE_ACCEL_EARLY_COEFF 0.975
#define LATE_ACCEL_MID_COEFF 1
#define LATE_ACCEL_LATE_COEFF 1

#define END_HPDRAIN_COEFF 0.995
#define END_SPEED_EARLY_COEFF 0.931
#define END_SPEED_MID_COEFF 1
#define END_SPEED_LATE_COEFF 1
#define END_ACCEL_EARLY_COEFF 0.945
#define END_ACCEL_MID_COEFF 1 
#define END_ACCEL_LATE_COEFF 0.997

#define STYLE_S_APT 1.1
#define STYLE_A_APT 1
#define STYLE_B_APT 0.85
#define STYLE_C_APT 0.75
#define STYLE_D_APT 0.6
#define STYLE_E_APT 0.4
#define STYLE_F_APT 0.2
#define STYLE_G_APT 0.1

#define DIST_S_APT_SPEED 1.05
#define DIST_A_APT_SPEED 1
#define DIST_B_APT_SPEED 0.9
#define DIST_C_APT_SPEED 0.8
#define DIST_D_APT_SPEED 0.6
#define DIST_E_APT_SPEED 0.4
#define DIST_F_APT_SPEED 0.2
#define DIST_G_APT_SPEED 0.1

#define DIST_S_APT_ACCEL 1
#define DIST_A_APT_ACCEL 1
#define DIST_B_APT_ACCEL 1
#define DIST_C_APT_ACCEL 1
#define DIST_D_APT_ACCEL 1
#define DIST_E_APT_ACCEL 0.6
#define DIST_F_APT_ACCEL 0.5
#define DIST_G_APT_ACCEL 0.4

#define TURF_S_APT 1.05
#define TURF_A_APT 1
#define TURF_B_APT 0.9
#define TURF_C_APT 0.8
#define TURF_D_APT 0.7
#define TURF_E_APT 0.5
#define TURF_F_APT 0.3
#define TURF_G_APT 0.1

//The lower limit of each length type
#define SPRINT_LEN 1000
#define MILE_LEN 1500
#define MEDUIM_LEN 2000
#define LONG_LEN 2500

#define TURF_FIRM_SPEED_MOD 0
#define TURF_GOOD_SPEED_MOD 0
#define TURF_SOFT_SPEED_MOD 0
#define TURF_HEAVY_SPEED_MOD -50

#define TURF_FIRM_POWER_MOD 0
#define TURF_GOOD_POWER_MOD -50
#define TURF_SOFT_POWER_MOD -50
#define TURF_HEAVY_POWER_MOD -50

#define TURF_FIRM_HPDRAIN_MOD 1
#define TURF_GOOD_HPDRAIN_MOD 1
#define TURF_SOFT_HPDRAIN_MOD 1.02
#define TURF_HEAVY_HPDRAIN_MOD 1.02

#define DIRT_FIRM_SPEED_MOD 0
#define DIRT_GOOD_SPEED_MOD 0
#define DIRT_SOFT_SPEED_MOD 0
#define DIRT_HEAVY_SPEED_MOD -50

#define DIRT_FIRM_POWER_MOD -100
#define DIRT_GOOD_POWER_MOD -50
#define DIRT_SOFT_POWER_MOD -100
#define DIRT_HEAVY_POWER_MOD -100

#define DIRT_FIRM_HPDRAIN_MOD 1
#define DIRT_GOOD_HPDRAIN_MOD 1
#define DIRT_SOFT_HPDRAIN_MOD 1.01
#define DIRT_HEAVY_HPDRAIN_MOD 1.02

double getHPDrainCoeff(int style);
double getStyleSpeedCoeff(int style, int phase);
double getStyleAcccelCoeff(int style, int phase);
double getStyleCoeff(char c);
double getDistSpeedCoeff(char c);
double getDistAcccelCoeff(char c);
double getTurfCoeff(char c);
int getSurfaceSpeedMod(int surface, int condition);
int getSurfacePowerMod(int surface, int condition);
double getSurfaceHPDrainMod(int surface, int condition);
int getLenClass(int trackLen);
int aptTranslation(char c);