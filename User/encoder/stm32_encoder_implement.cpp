#include "stm32_encoder_implement.h"
#include "board.h"

EncoderImp::EncoderImp(unsigned char _num, bool _reverse) : num(_num), reverse(_reverse)
{
    clear();
}

void EncoderImp::init(){
    Board::get()->encoder_init(num);
}

void EncoderImp::clear(){
    total_count = 0;
    pid_pos = odom_pos = 0;
}

long EncoderImp::get_total_count(){
    if (!reverse)
        total_count += Board::get()->get_encoder_count(num);
    else
        total_count -= Board::get()->get_encoder_count(num);

    return total_count;
}

long EncoderImp::get_increment_count_for_dopid(){
    if (!reverse)
        total_count += Board::get()->get_encoder_count(num);
    else
        total_count -= Board::get()->get_encoder_count(num);

    long l = total_count-pid_pos;
    pid_pos = total_count;
    return l;
}

long EncoderImp::get_increment_count_for_odom(){
    total_count += Board::get()->get_encoder_count(num);
    
    long l = total_count-odom_pos;
    odom_pos = total_count;
    return l;
}