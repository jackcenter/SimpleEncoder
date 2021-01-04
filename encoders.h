#ifndef ENCODERS_H
#define ENCODERS_H

#include <Arduino.h>
#include <encoders.h>


const signed char enc_lookup_table_pos[] = {
    0, 1, -1, 0,
    -1, 0 ,0, 1,
    1, 0, 0, -1,
    0, -1, 1, 0
};

const signed char enc_lookup_table_neg[] = {
    0, -1, 1, 0,
    1, 0 ,0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0
};

class Encoder {
  public:
    char chanA_pin;
    char chanB_pin;
    int counts_per_rev;
    const signed char *lookup;        // pointer to the first element in the array
    volatile char chanA_val;
    volatile char chanB_val;
    char val_k0;
    volatile int cnt;
    char flag;

    Encoder(char, char, int, char);
    void init();
    int get_count();
    void set_flag();
    void clear_flag();
    void process_change(char);
    void update_encoder_count();
};

struct encoderInterrupts {
  // This is board dependent
  const char int0Pin = 2;
  const char int1Pin = 3;

  char chan0;
  char chan1;

  Encoder *enc0;
  Encoder *enc1;
};

void encInt_0();
void encInt_1();

#endif
