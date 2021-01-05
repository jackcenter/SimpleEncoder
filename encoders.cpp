#include <Arduino.h>
#include <encoders.h>

encoderInterrupts encInts;

Encoder::Encoder(char a, char b, int c, char d){
  chanA_pin = a;
  chanB_pin = b;
  counts_per_rev = c;
  dir = d;

  flag = 0;
}

void Encoder::init(){

  pinMode(chanA_pin, INPUT_PULLUP);
  pinMode(chanB_pin, INPUT_PULLUP);

  if (dir >= 0)
    lookup = enc_lookup_table_pos;
  else
    lookup = enc_lookup_table_neg;

  // TODO: make the intPins in the struct an array so it can be searched

  for (char i = 0; i < encInts.available; ++i){
    // run through each interrupt pin on the board

    if (chanA_pin == encInts.pins[i]){
      encInts.chans[i] = 'A';
      encInts.encs[i] = this;
      attachInterrupt(digitalPinToInterrupt(chanA_pin), encInts.calls[i], CHANGE);
    }

    else if (chanB_pin == encInts.pins[i])
      encInts.chans[i] = 'B';
      encInts.encs[i] = this;
      attachInterrupt(digitalPinToInterrupt(chanB_pin), encInts.calls[i], CHANGE);
    }

  chanA_val = digitalRead(chanA_pin);
  chanB_val = digitalRead(chanB_pin);
  val_k0 = (chanA_val << 1) + chanB_val;
}

int Encoder::get_count(){
  return cnt;
}

void Encoder::set_flag(){
  flag = 1;
}

void Encoder::clear_flag(){
  flag = 0;
}

void Encoder::process_change(char chan){

  if (chan == 'A'){
    chanA_val = digitalRead(chanA_pin);
    update_encoder_count();
  }

  else if (chan == 'B'){
    chanB_val = digitalRead(chanB_pin);
    update_encoder_count();
  }
}

void Encoder::update_encoder_count(){
  char val_k1 = (chanA_val << 1) + chanB_val;

  if (val_k1 != val_k0){

    char reading = (val_k1 << 2) + val_k0;
    cnt += lookup[reading];

    val_k0 = val_k1;
  }
}

// One interrupt handler is needed for each external interrupt
void encInt_0(){
  char i = 0;
  encInts.encs[i]->process_change(encInts.chans[i]);
  encInts.encs[i]->set_flag();
}

void encInt_1(){
  char i = 1;
  encInts.encs[i]->process_change(encInts.chans[i]);
  encInts.encs[i]->set_flag();
}

