# Simple Arduino Data Over Sound
## Introduction
  This project shows how one can perform data transfer from any device capable of producing sound and having a 3.5 mm mini jack for headphones to arduino. 
The data is being transferred via modulating the signal produced by device's audio system. The project is purely educational, its goal is to provide reader 
with some basics of data over sound transferring that is commonly used since 60s in modems. The project can hardly be applied in practice, 
because maximum speed of data transferring achieved is ~60 bits per second.

## Setup

<img src="https://user-images.githubusercontent.com/66104180/209701223-4b9deaf5-bf26-456b-a9e2-286be31ba553.jpg" width="300" height="400" />

For this project you will need Arduino and a cable with 3.5 mm mini jack connected to headphones port. You can use broken headphones for this purpose. 
You should connect one of signal lines to some pin of Arduino (A1 is used in project). Note that if you play mono sound then it doesn't matter which 
signal line to use (Left or Right). When reading data from pin Arduino connects this pin to ground, so you don't need to worry about ground lines of headphones.
Also you will need Arduino IDE for editing and running Arduino code and any IDE for Java (I used Android Studio). The Java part is responsible for converting stream of data to sounds. Arduino
code is responsible for reading this signal and reconstructing the initial data. The Java code uses LibGDX framework, because it is easy to use with 
different media(sounds, pictures, etc.) and is multiplatform - it can be run on desktop, android, MacOS, Web. 

## Details

The basics of data transferring via analogue signal (sound, radio, light, anything...) is modulating the shape of the wave so different shapes of different regions of
the wave correspond to some data. There are many ways of wave modulation - [amplitude modulation](https://en.wikipedia.org/wiki/Amplitude_modulation), 
[frequency modulation](https://en.wikipedia.org/wiki/Frequency_modulation), [phase modulation](https://en.wikipedia.org/wiki/Phase_modulation) and more. In this project
the simplest one, amplitude modulation, was made.

Java code produces spikes of sound waves at 100 Hz, and modulates the amplitude (volume) of each spike so the amplitude corresponds to an integer number.
Arduino code recieves the analog signals and demodulates it, retrieving the numbers. It was found that the maximum amount of distinct values that can be transferred via
one spike in this setup is 7 (integer numbers 0-6). Further descritization is hard to perform because spike amplitude errors start to overlap which makes it impossible to
distinct adjacent values. A spike is being produced 20 times a second. More frequent spikes result in increasing the error in spikes's amplitude. Likely because of inertia 
in anlog components of audio system. Also increasing the frequency of sound leads to increasing the error of spike's amplitude. Because the wavelength becomes smaller,
Arduino reads fewer values of each spike, which leads to bigger error. Inside Java code you will find samples of sound spikes of different frequency if you want to experiment and come up with better algorithms, or you can just generate your own through additional software (I used Audacity) or at runtime using some framework.
Because different devices have different audiosystems, the resulting physical amplitude of a wave for each number will be specific for your device. In order to successfully demodulate this signal you will have to adjust Arduino script, mainly the *intervals* array that contains values of amplitudes dividing ranges of amplitudes for each numeric value:
```
float intervals[] = {20,30,40,50,65,75,85,100};
```
This can be done by sending sequence 0-6 several times, printing the resulting amplitudes and picking up appropriate dividing amplitudes. 
