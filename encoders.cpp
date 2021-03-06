#include <Arduino.h>
#include <SimpleEncoder.h>

encoderInterrupts encInts;      // creates the structure which holds board
                                // specific information used by encoders

/**
 * Constructor for Encoder objects
 *
 * @param a The pin associated with channel A
 * @param b The pin associated with channel B
 * @param c The counts per revolution for the encoder
 * @param d The direction for the count, 1 or -1.
 * @return none
 */
Encoder::Encoder(char a, char b, int c, char d){
  chanA_pin = a;
  chanB_pin = b;
  counts_per_rev = c;
  dir = d;

  flag = 0;
}

/**
 * Initialize the encoder during setup. Sets the pin modes, attaches interrupts (as
 * appropriate), selects the appropriate lookup table, and gets the first reading
 *
 * @param none
 * @return none
 */
void Encoder::init(){

  pinMode(chanA_pin, INPUT_PULLUP);
  pinMode(chanB_pin, INPUT_PULLUP);

  // Attach Interrupts

  for (char i = 0; i < encInts.available; ++i){

    if (chanA_pin == encInts.pins[i]){
      chanA_int = 1;
      encInts.encs[i] = this;
      attachInterrupt(digitalPinToInterrupt(chanA_pin), encInts.calls[i], CHANGE);
    }

    else if (chanB_pin == encInts.pins[i]){
      chanB_int = 1;
      encInts.encs[i] = this;
      attachInterrupt(digitalPinToInterrupt(chanB_pin), encInts.calls[i], CHANGE);
    }
  }

  // Assign lookup table

  if (chanA_int & chanB_int){
      // Dual interrupt
      Serial.println("Dual");
      if (dir >= 0)
        lookup = enc_lookup_table_dual_pos;
      else
        lookup = enc_lookup_table_dual_neg;
  }

  else if (chanA_int ^ chanB_int){
      // Single interrupt
      Serial.println("Single");
      if (dir >= 0)
        lookup = enc_lookup_table_single_pos;
      else
        lookup = enc_lookup_table_single_neg;
  }

  chanA_val = digitalRead(chanA_pin);
  chanB_val = digitalRead(chanB_pin);
  val_k0 = (chanA_val << 1) + chanB_val;
}

/**
 * Get the current encoder tick count.
 *
 * @param none
 * @return the curent value stored in cnt
 */
int Encoder::get_count(){

  return cnt;
}

/**
 * Indicate that an interrupt has occured on one of the channels by setting the
 * flag attribute. May be polled in the main loop.
 *
 * @param none
 * @return none
 */
void Encoder::set_flag(){

  flag = 1;
}

/**
 * Set the flag attribute to zero.
 *
 * @param none
 * @return none
 */
void Encoder::clear_flag(){

  flag = 0;
}

/**
 * Reads the encoder channels and updates the associated values to be used in
 * the 'update_encoder_count()' function.
 *
 * @param none
 * @return none
 */
void Encoder::process_change(){

  chanA_val = digitalRead(chanA_pin);
  chanB_val = digitalRead(chanB_pin);
  update_encoder_count();
}

/**
 * Lookup the step magnitude and direction of the last reading and update the
 * tick count.
 *
 * @param none
 * @return none
 */
void Encoder::update_encoder_count(){

  char val_k1 = (chanA_val << 1) + chanB_val;

  if (val_k1 != val_k0){

    char reading = (val_k1 << 2) + val_k0;
    cnt += lookup[reading];

    val_k0 = val_k1;
  }
}

// Interrupt service routines
// One interrupt handler is needed for each external interrupt
void encInt_0(){
  char i = 0;
  encInts.encs[i]->process_change();
  encInts.encs[i]->set_flag();
}

void encInt_1(){
  char i = 1;
  encInts.encs[i]->process_change();
  encInts.encs[i]->set_flag();
}

