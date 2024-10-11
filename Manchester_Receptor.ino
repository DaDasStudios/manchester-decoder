#include <LiquidCrystal.h>

// * Constants
#define BAUD_RATE 9600
// The half of a bit duration (period/2)
#define HALF_TE 417
// Number of bits expected to be decoded
#define NUM_BITS 8
// Input pin for the input manchester signal
#define MANCHESTER_PIN 7
// Time in milliseconds to wait after a signal decodification
#define TIME_BETWEEN_SIGNALS 800

// LCD configuration
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// This is where the decoded output will be stored
unsigned long decodedOutput;

// The signal is analyzed in two parts: the first half and the second half, based on the first half value and the second half value it can be inferred the signal binary value.
int firstHalf, secondHalf;

// Variables to compensate the delay between instructions
unsigned long prevMicros, currentMicros;
int offset;

void setup()
{
  // If you want to use Serial communication
  Serial.begin(BAUD_RATE);
  pinMode(MANCHESTER_PIN, INPUT);

  // Welcome message
  lcd.begin(16, 2);
  lcd.print("Decoder");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
}

void loop()
{
  // Reset the output before processing a new signal
  decodedOutput = 0;

  // Reset the compensation variables
  prevMicros = 0;
  currentMicros = 0;
  offset = 0;

  // Bloquear el funcionamiento hasta que no haya un cambio en el pin de entrada
  // ! It's mandatory to use `digitalRead(MANCHESTER_PIN)` in order to reat the value on the fly
  while (digitalRead(MANCHESTER_PIN) == HIGH);

  // Once the initial change is detected, wait until the next change
  delayMicroseconds(HALF_TE + 50); // Delay to avoid problems
  delayMicroseconds(HALF_TE);
  
  // ? Start to decode data
  for (int i = 0; i < NUM_BITS; i++){
    // Read the first half
    firstHalf = digitalRead(MANCHESTER_PIN);

    // Shift to the left the binary output to create new space for the next value
    decodedOutput <<= 1;

    // Wait until the next change
    delayMicroseconds(HALF_TE);

    // Save the time
    prevMicros = micros();

    // Read the second half
    secondHalf = digitalRead(MANCHESTER_PIN);

    // Determine whether the signal is a 0, 1 or an error
    if (firstHalf == HIGH && secondHalf == LOW)
    {
      decodedOutput |= 0;
    } else if (firstHalf == LOW && secondHalf == HIGH)
    {
      decodedOutput |= 1;
    } else {
      lcd.clear();
      lcd.print("Invalid input");
      delay(1000);
      return;
    }
    
    // Save the time passed since the last save operation and compensate it in the delayMicroseconds() function, so that the program never overlaps with a unwanted signal half
    currentMicros = micros();
    offset = currentMicros - prevMicros;
    delayMicroseconds(HALF_TE - offset);

    // Now the prevMicros is the currentMicros
    prevMicros = currentMicros;
  }

  // Mostrar el byte decodificado en la pantalla LCD
  showInLCD(decodedOutput);
  // Espera un poco antes de decodificar otro byte
  delay(TIME_BETWEEN_SIGNALS);
}

void showInLCD(unsigned long data)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Binary: ");
  lcd.setCursor(0, 1);
  lcd.print(data, BIN);
}
