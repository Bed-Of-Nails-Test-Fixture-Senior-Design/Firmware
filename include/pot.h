#include <Arduino.h>
#ifndef POT_H
#define POT_H

class POT{
    private:
        String state;
        int CCW;
        int MID;
        int CW;
    public:
        void set_CCW();
        void set_MID();
        void set_CW();
};

#endif