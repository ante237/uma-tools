#ifndef TRACK
#define TRACK

using namespace std;

class Track
{
    public:
    Track(int surf, int cond, int len, 
        double early, double mid, double late, double spurt)
        :
        surface(surf), condition(cond), length(len),
        early(early), mid(mid), late(late), spurt(spurt)
        {};

    int surface;
    int condition;

    int length;
    double early;
    double mid;
    double late;
    double spurt;
};



#endif