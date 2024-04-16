#include "timer.h"

class Utils
{
public:
    Utils();
    ~Utils();

    void init(int timeslot);


public:
    int m_TIMESLOT; //最小时间间隙
};