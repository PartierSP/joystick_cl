// Reads from HC-12 with Hardware Serial aka Serial1.
// This data is expected to be like `#1223:1400:1700:1300$`.
// Should start with an # and end with $ anything else is
// discarded. The motors are driven to those pwm.
//    This works well with Teensy2++, however does not work
//    with Arduino Nano.
//
//    Author  : Manohar Kuse <mpkuse@connect.ust.hk>
//    Created : 4th Feb, 2019


//#include <AltSoftSerial.h>
//AltSoftSerial altserial; --> makes some pins unusable. see documentation

#define led 6

#include <Servo.h>
Servo M1, M2, M3, M4;
#define M1_pin 16
#define M2_pin 15
#define M3_pin 14
#define M4_pin 1



void blink( int led_pin, int mili)
{
  digitalWrite( led_pin, HIGH );
  delay( mili );
  digitalWrite( led_pin, LOW );
  delay( mili );
}

void setup()
{
  Serial.begin( 9600 );
  Serial1.begin( 9600 );
  delay(40);

  // Setup Servos
  M1.attach( M1_pin, 1000, 2000 );
  M1.writeMicroseconds( 1200 );

  M2.attach( M2_pin, 1000, 2000 );
  M2.writeMicroseconds( 1200 );

  M3.attach( M3_pin, 1000, 2000 );
  M3.writeMicroseconds( 1200 );

  M4.attach( M4_pin, 1000, 200 );
  M4.writeMicroseconds( 1200 );
  Serial.printf( "Setup of servos done" );
}

char str[50];
int counter=0;
bool flag = false;
bool serial_data_available = false;

void loop()
{
  while( Serial1.available() )
  {
    char ch = Serial1.read();
    if( ch == '#' ) {
      Serial.println( "Start Reading" );
      flag = true; // start reading
      break;
    }

    if( flag && ch == '$' ) {
      Serial.println( "End of reading indicator detected" );
      str[counter] = '\0';
      Serial.printf( "Final string `%s`\n", str );
      counter = 0;
      serial_data_available = true;
      break;
    }

    if( flag ) {
      str[counter] = ch;

      //Serial.print( String(counter) + String( ":" ) +  String( ch ) + String( ";" ) );
      counter++;
      break;
    }

    Serial.println( "Ignoring char " + String( ch ) );
  }

  if( serial_data_available )
  {
    Serial.printf( "serial_data_available=true. len=%d, `%s`\n", strlen(str), str );
    serial_data_available = false;

    // data is available now in `str`..consume the data
//    int M1_val=0, M2_val=0, M3_val=0, M4_val=0;
    int M1_val, M2_val, M3_val, M4_val;
    int n = sscanf( str, "%d:%d:%d:%d", &M1_val, &M2_val, &M3_val, &M4_val );
    Serial.printf( "n=%d M1_val=%d, M2_val=%d, M3_val=%d, M4_val=%d\n", n, M1_val, M2_val, M3_val, M4_val );


    if( n == 4 ) {
      // Data looks like 4 numbers
      //TODO: ensure they are in the correct range
      M1.writeMicroseconds( M1_val );
      M2.writeMicroseconds( M2_val );
      M3.writeMicroseconds( M3_val );
      M4.writeMicroseconds( M4_val );
      Serial.printf("Done setting motor PWMs\n" );
      Serial1.printf("Done setting motor PWMs\n" );

    }
    else {
      Serial.printf("Error Parsing" );
      Serial1.printf("Error Parsing" );
    }
  }
  blink(led, 1);
}
