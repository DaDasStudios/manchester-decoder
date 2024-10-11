# DALI Receptor
Code to see your DALI forward frame in a LCD screen 2x16

## Prerequisites
* LCD Screen 2x16 and the LiquidCrystal library
* Push button
* 10K resistors
* Manchester transmitter device (can use another Arduino)
* Wires
* Having understand the simple _Manchester receptor_ in the __main__ branch

## Usage
Now the `NUM_BITS = 16` because a DALI forward frame contains 16 bits. The time between flanks must be 416.67 microseconds (`HALF_TE`). Finally, in reality, the time between consecutive forward frames must be at least 9.17ms (`TIME_BETWEEN_SIGNALS`) but this wasn't changed in because the transmitter was setup to send signals each 1000 ms. 
```C
#define BAUD_RATE 9600
// The half of a bit duration (period/2)
#define HALF_TE 417
// Number of bits expected to be decoded
#define NUM_BITS 16
// Input pin for the input manchester signal
#define MANCHESTER_PIN 7
// Time in milliseconds to wait after a signal decodification
#define TIME_BETWEEN_SIGNALS 800
```
### Push button for toggling LCD format
A push button was introduced to see the signal decoded either in binary or hexadecimal format. By default (LOW state) the controller will assume a BIN format. If you want to change to hexa, just hold pressed the push button the time you want to see it. 

Introducing toggling by just a press is kind of hard in this code, since you need to guarantee the current timings but in this way you'll to prevent bouncing with delays. That's the reason because button detection is inside the following part of code:

```ino
while (digitalRead(MANCHESTER_PIN) == HIGH)
{
  // Button to allow to toggle to display between binary and hex format
  showHex = digitalRead(HEX_BIN_PIN) == HIGH;
}
```


## Example
The DALI documentation specifies that the signal must follow this format:

![DALI forward frame](/images/dali-forward-frame.png)

![Forward frame description](/images/forward-frame-description.png)

Let's say we want to send `Y-AAAAAA-S-CCCCCCCC = 0-011010-0-00010000`(the same signal shown in the main branch). In hexa the parts become:
* `Y = 0x00`
* `AA = 0x01A`
* `S = 0x00`
* `CC = 0x010`

![Input signal](/images/input-signal.png)

Since the left-zeros aren't printed out in the simple version, there's a code portion that includes those zeros in order to keep the format. 

![alt text](/images/simulation-binary.png)

In the other hand, by pressing the push button the signal is now displayed in hexadecimal format.

![alt text](/images/simulation-hex.png)
