// Input the data from joysticks and send it to seial.
// This has two threads. Thread-1 reads joystick commands
// Thread-2 sends data to serial.

#include <iostream>
#include "joystick.h"
#include <unistd.h>
#include "serialstream.h"
#include <thread>

using namespace std;
using namespace boost::posix_time;

atomic<bool> is_ok;
atomic<int> m1_val;
atomic<int> m2_val;
atomic<int> m3_val;
atomic<int> m4_val;
atomic<bool> has_changed;

// #define JOYSTICK_AXIS_MIN -32767
#define JOYSTICK_AXIS_MIN 0
#define JOYSTICK_AXIS_MAX 32767

void joystick_reader( Joystick* joystick )
{
  while( is_ok ) {
    // Attempt to sample an event from the joystick
    JoystickEvent event;
    if (joystick->sample(&event))
    {
        std::cout << "[isInitialState?" << event.isInitialState() << "] ";
        std:: cout << " t=" << event.time;
        if (event.isButton())
        {
            std::cout << " Button#" << (int)event.number ;
            std::cout << " value=" << ((event.value == 0) ? "up" : "down") ;
            if( (int)event.number==0 && event.value==0 && event.isInitialState() ==false ) {
              is_ok = false;
              cout << "[joystick_reader] quit button is pressed\n";
            }

            if( (int)event.number==1 && event.value==0 && event.isInitialState() ==false ) {
              m1_val = 1000;
              m2_val = 1000;
              m3_val = 1000;
              m4_val = 1000;
              has_changed = true;
              cout << "[joystick_reader] quit button is pressed\n";
            }
            std::cout << std::endl;
        }
        else if (event.isAxis())
        {
            std::cout << "Axis#" << (int)event.number;
            std::cout << " value=" << (int)event.value;
            std::cout << std::endl;

            if( event.value < 0 )
              continue;

            // convert [-32000,32000] --> [1000,2000]
            if( (int)event.number == 0 ) {
              int m1_val_new =  1000 + 1000.0 * ( (float)event.value - JOYSTICK_AXIS_MIN ) / ( JOYSTICK_AXIS_MAX - JOYSTICK_AXIS_MIN ) ;
              if( m1_val_new != m1_val ) {
                m1_val = m1_val_new;
                has_changed = true;
                cout << "m1 has changed, ie. axis0 to " << m1_val << endl;
              }

            }

            if( (int)event.number == 1 ) {
              int m2_val_new =  1000 + 1000.0 * ( (float)event.value - JOYSTICK_AXIS_MIN ) / ( JOYSTICK_AXIS_MAX - JOYSTICK_AXIS_MIN ) ;
              if( m2_val_new != m2_val ) {
                m2_val = m2_val_new;
                has_changed = true;
                cout << "m2 has changed, ie. axis1 to " << m2_val << endl;
              }
            }

            if( (int)event.number == 2 ) {
              int m3_val_new =  1000 + 1000.0 * ( (float)event.value - JOYSTICK_AXIS_MIN ) / ( JOYSTICK_AXIS_MAX - JOYSTICK_AXIS_MIN ) ;
              if( m3_val_new != m3_val ) {
                m3_val = m3_val_new;
                has_changed = true;
                cout << "m3 has changed, ie. axis2 to " << m3_val << endl;
              }
            }

            if( (int)event.number == 3 ) {
              int m4_val_new =  1000 + 1000.0 * ( (float)event.value - JOYSTICK_AXIS_MIN ) / ( JOYSTICK_AXIS_MAX - JOYSTICK_AXIS_MIN ) ;
              if( m4_val_new != m4_val ) {
                m4_val = m4_val_new;
                has_changed = true;
                cout << "m4 has changed, ie. axis3 to " << m4_val << endl;
              }
            }
        }
    }
  usleep(100);

  }

  cout << "Finished thread joystick_reader\n";
}


int main( )
{
  // Setup joysticks
  Joystick * joystick = new Joystick("/dev/input/js0");
  if (!(*joystick).isFound())
  {
    cout << "cannot open joysticks\n";
    exit(1);
  }

  // Setup Serial
  SerialOptions options;
  options.setDevice("/dev/ttyUSB0");
  options.setBaudrate(9600);
  options.setTimeout(milliseconds(20));

  SerialStream serial(options);
  serial.exceptions(ios::badbit | ios::failbit); //Important!

  cout << "Setup of joysticks and serial is successful\n";

  std::thread th1( joystick_reader, joystick );

  is_ok = true;
  m1_val = 1000; m2_val=1200; m2_val=1200; m2_val=1200;
  has_changed = true;
  while( is_ok ) {
    if( has_changed) {
    cout<<"#"<<m1_val<<":" << m2_val << ":" << m3_val << ":" << m4_val << "$" << endl;
    serial<<"#"<<m1_val<<":" << m2_val << ":" << m3_val << ":" << m4_val << "$" << endl;
    has_changed = false;
    }

    try {
        string s;
        //serial>>s;
        getline(serial,s);
        cout<<s<<endl;
    } catch(TimeoutException&) {
        serial.clear(); //Don't forget to clear error flags after a timeout
        // cerr<<"Nothing received for now..."<<endl;
    }

    // usleep(1000);
  }

  th1.join();
  cout << "Finished main\n";

}
