#ifndef TRACK
#define TRACK

using namespace std;

struct trackStruct
    {
        int surface;
        int condition;
        int length;
    };

class Track
{
    public:
    Track(int surf, int cond, int len)
    :
    surface(surf), condition(cond), length(len)
    {
        this->early = len / 6.0;
        this->mid = len / 2.0 + this->early;
        this->late = len / 6.0 + this->mid;
        this->spurt = len / 6.0 + this->late;
    }

    Track(trackStruct& trackS)
    :
    surface(trackS.surface), condition(trackS.condition), length(trackS.length)
    {
        this->early = trackS.length / 6.0;
        this->mid = trackS.length / 2.0 + this->early;
        this->late = trackS.length / 6.0 + this->mid;
        this->spurt = trackS.length / 6.0 + this->late;
    }

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