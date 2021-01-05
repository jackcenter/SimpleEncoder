#ifndef ENCODERS_H
#define ENCODERS_H

#include <encoders.h>

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
    volatile long cnt;          // current tick count
    char flag;                  // flag to indicated an interrupt was processed

    Encoder(char, char, int, char);
    void init();
    int get_count();
    void set_flag();
    void clear_flag();
    void process_change();
    void update_encoder_count();
};

// One interrupt handler is needed for each external interrupt
void encInt_0();
void encInt_1();

// This is board dependent, should check board and assign accordingly
// Array positions are associated with the specific external interrupt
struct encoderInterrupts {

  const char available = 2;     // number of external interrupts available
  char pins[2] = {2, 3};        // array of pins
  Encoder *encs[2];             // array of encoders
  void (*calls[2])() = {encInt_0, encInt_1};    // array of function pointers
};

// Lookup Tables
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

const signed char enc_lookup_table_single_pos[] = {
    0, 0, 1, -2,
    0, 0, 2, -1,
    -1, 2, 0, 0,
    -2, 1, 0, 0
};

const signed char enc_lookup_table_single_neg[] = {
    0, 0, -1, 2,
    0, 0, -2, 1,
    1, -2, 0, 0,
    2, -1, 0, 0
};

#endif


