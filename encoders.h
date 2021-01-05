#ifndef ENCODERS_H
#define ENCODERS_H

#include <Arduino.h>
#include <encoders.h>

// TODO: what if there is only one interrupt pin!

const signed char enc_lookup_table_dual_pos[] = {
    0, 1, -1, 0,
    -1, 0 ,0, 1,
    1, 0, 0, -1,
    0, -1, 1, 0
};

const signed char enc_lookup_table_dual_neg[] = {
    0, -1, 1, 0,
    1, 0 ,0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0
};

class Encoder {
  public:
    char chanA_pin;             // port pin assinged to channel A
    char chanB_pin;             // port pin assigned to channel B
    char chanA_int;             // is chanA an interrupt pin?
    char chanB_int;             // is chanB an interrupt pin?
    int counts_per_rev;         // encoder counts per revolution
    char dir;                   // encoder directions
    const signed char *lookup;  // pointer to the first element in the array
    volatile char chanA_val;    // reading on chanA: 0 or 1
    volatile char chanB_val;    // reading on chanB: 0 or 1
    char val_k0;                // combined reading for chanA, chanB: ie 00, 01, ..
    volatile int cnt;           // current tick count
    char flag;                  // flag to indicated an interrupt was processed

    Encoder(char, char, int, char);
    void init();
    int get_count();
    void set_flag();
    void clear_flag();
    void process_change(char);
    void update_encoder_count();
};

void encInt_0();
void encInt_1();

struct encoderInterrupts {
  // This is board dependent, should check board and assign accordingly
  const char available = 2;
  char pins[2] = {2, 3};
  char chans[2];
  Encoder *encs[2];
  void (*calls[2])() = {encInt_0, encInt_1};
};

#endif


