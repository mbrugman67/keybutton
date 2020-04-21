/*****************************************
 * keyboard buttons
 * 
 * Oh Christ, I said I'd never use Arduino...
 * 
 * This sketch will be used by a Teensy 2.0
 * (Because that's what I had laying around)
 * 
 * It is a USB composite device that acts as
 * a keyboard.  The first 7 inputs will, when
 * pulled low, fire a keyboard press of
 * buttons 1 to 7, repsectively.
 * 
 * There is, of course, debounce and one-
 * shotting, so it doesn't spam the target
 * with multiple inputs.
*****************************************/

#include <Keyboard.h>

#define BUTTON_CNT     7    // how many buttons
#define DEBOUNCE_TIME 75    // 75ms debounce time

/***********************************************
 * This class is for one individual input button.
 * 
 * Instantiate it with input number and the 
 * character with which it should spit out when
 * that input is pulled low.
 ***********************************************/
class keybutton
{
 public:

  // constructor - 'i' is the input number,
  // 'k' is the char to push out
  keybutton(int i, char k): input(i), key(k) 
  {
    // init the I/O pin to be an input with pullup
    // enabled
    pinMode(input, INPUT_PULLUP);
  }

  // generic destructor
  virtual ~keybutton() {}

  // update method, call periodically
  void update()
  {
    // button is pressed when input goes low
    if (digitalRead(input) == LOW)
    {
      // make sure debounce time has expired and that 
      // we haven't already passed this button's 
      // keyboard cahracter
      if ((millis() - debounce > DEBOUNCE_TIME) && !once)
      {
        // write the char 
        Keyboard.write(key);

        // enforce one-shot
        once = true; 
      }
    }
    else
    {
      // button is not pressed, clear debounce timer
      // and reset one-shot flag
      once = false;
      debounce = millis();
    }
  }

private:
  bool once;                  // one-shot flag
  int input;                  // input pin number
  char key;                   // character to send
  unsigned long debounce;     // debounce timer elapsed value
};

// static button instance counter
static unsigned char thisButton = 0;

// allocate an array of buttons offa the heap.  The first 7 
// I/O pins will be used to send numeric values 1 to 7
static keybutton* keys[BUTTON_CNT] = { new keybutton(0, '1'), 
                                       new keybutton(1, '2'),
                                       new keybutton(2, '3'),
                                       new keybutton(3, '4'),
                                       new keybutton(4, '5'),
                                       new keybutton(5, '6'),
                                       new keybutton(6, '7') };



/***********************************************
 * About the only thing to init is the keyboard
 * library
 ***********************************************/
void setup() 
{
  // put your setup code here, to run once:
  Keyboard.begin();
}

/***********************************************
 * WTF, Arduino?  Is the main()????
 ***********************************************/
void loop() 
{
  // each loop pass checks one button
  keys[thisButton]->update();

  // check the next one next pass through
  ++thisButton;
  thisButton %= BUTTON_CNT;
}
