# Manchester receptor
A simple implementation of a manchester receptor with Arduino and a LCD screen.

## Prerequisites
* LCD Screen 2x16 and the LiquidCrystal library
* Manchester transmitter device (can use another Arduino)
* Wires

## Usage
Set the initial constants according to your needs. By default, the values are the shown below.
```C
#define BAUD_RATE 9600
// The half of a bit duration (period/2)
#define HALF_TE 417
// Number of bits expected to be decoded
#define NUM_BITS 8
// Input pin for the input manchester signal
#define MANCHESTER_PIN 7
// Time in milliseconds to wait after a signal decodification
#define TIME_BETWEEN_SIGNALS 800
```
### LCD
The code is for educational purposes, so one of the best ways to test this code is using a circuits simulator like __Proteus__ or others. In that case, watching the serial monitor isn't possible, so I decided to include a LCD screen 2x16. 


## Example
Let's say we want to detect the 16 bits signal shown in the figure below

![Input signal](/images/input-signal.png)

Decoding the signal manually we get `0011-0100-0001-0000` without including the start bit.

In the simulator we can see the signal decoded successfully. The zeros at the left are not printed:

![Simulation](/images/simulation.png)



## Common problems
### VSCode intellisense errors 
If you're using VSCode and the Arduino community extension, you'll probably will have problems with the intelissense and the `LiquidCrystal` library. My solution was including manually the LiquidCrystal library in the `c_cpp_properties.json` file:

```
C:\\Users\\$YOUR_``USER\\Documents\\Arduino\\libraries\\LiquidCrystal\\src
```

I didn't use the path at Appdata/Arduino15/... because it didn't work, how ever you can try it.

### "Invalid input" or unexpected outputs

During the development of this code I had several problems with the decoding code section, the program printed out unexpected binaries. The main reason was the timings between the intructions, the delays reached a point where the program overlaped the the second half of the period signal, so it decoded the binary wrong. 

To solve that, I had to include the time compensation with the `micros()` function as you'll see in the code. So, in theory, you shouldn't get any misconvertions in your signals. However, if you keep getting a different signal from you're sending, __try to change the initial time compensation at `Manchester_Receptor.ino` which is the following line:__

```C
  delayMicroseconds(HALF_TE + 50); // Delay to avoid problems
```