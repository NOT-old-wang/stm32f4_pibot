#ifndef PIBOT_ENCODER_IMP_H_
#define PIBOT_ENCODER_IMP_H_

#include "encoder.h"

#define ENCODER_USE_INTERRUPTS
#define ENCODER_OPTIMIZE_INTERRUPTS

class EncoderImp:public Encoder{
    public:
	    EncoderImp(unsigned char num, bool reverse=false);
        void init();
        void clear();
        long get_total_count();
        long get_increment_count_for_dopid();  
        long get_increment_count_for_odom();  
    private:
        unsigned char num;
        long pid_pos, odom_pos;
        float total_count;
        bool reverse;
};

#endif