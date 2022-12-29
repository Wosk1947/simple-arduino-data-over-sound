# Simple Arduino Data Over Sound
## Introduction
  This project shows how one can perform data transfer from any device capable of producing sound and having a 3.5 mm mini jack for headphones to Arduino. 
The data is being transferred via modulating the signal produced by device's audio system. The project is purely educational, its goal is to provide reader 
with some basics of data over sound transferring that is commonly used since 60s in modems. The project can hardly be applied in practice, 
because maximum speed of data transferring achieved is ~80 bits per second.

## Setup

<img src="https://user-images.githubusercontent.com/66104180/209701223-4b9deaf5-bf26-456b-a9e2-286be31ba553.jpg" width="300" height="400" />

For this project you will need Arduino(I used Arduino Nano) and a cable with 3.5 mm mini jack connected to headphones port. You can use broken headphones for this purpose. 
You should connect one of signal lines to some pin of Arduino (A1 is used in project). Note that if you play mono sound then it doesn't matter which 
signal line to use (Left or Right). When reading data from pin Arduino connects this pin to ground, so you don't need to worry about ground lines of headphones.
Also you will need Arduino IDE for editing and running Arduino code and any IDE for Java (I used Android Studio). The Java part is responsible for converting stream of data to sounds. Arduino
code is responsible for reading this signal and reconstructing the initial data. The Java code uses LibGDX framework, because it is easy to use with 
different media(sounds, pictures, etc.) and is multiplatform - it can be run on desktop, android, MacOS, Web. 

## Details

The basics of data transferring via analogue signal (sound, radio, light, anything...) is modulating the shape of the wave so different shapes of different regions of
the wave correspond to some data. There are many ways of wave modulation - [amplitude modulation](https://en.wikipedia.org/wiki/Amplitude_modulation), 
[frequency modulation](https://en.wikipedia.org/wiki/Frequency_modulation), [phase modulation](https://en.wikipedia.org/wiki/Phase_modulation) and more. In this project currently amplitude and frequency modulation are implemented, but not in a way of modulating the amplitude and frequency of sequences of waves but rather modulating each spike individual amplitude and wavelength.  

Java code produces spikes of sound waves at 100 Hz and 50 Hz, modulates the amplitude (volume) of each spike so the amplitude corresponds to an integer number. So both 
sound frequency and amplitude being used for encoding numbers. Arduino code recieves the analog signals and demodulates it, retrieving the numbers.

Sample of signal being read by Arduino:

<img src="https://user-images.githubusercontent.com/66104180/209829329-9a9f7e51-fdc0-4a9d-9d75-36d80e1c1a52.png" width="500" height="300" />

It was found that the maximum amount of distinct values that can be transferred via
one spike in this setup is 7. Further descritization is hard to perform because spike amplitude errors start to overlap which makes it impossible to
distinct adjacent values. A spike is being produced 20 times a second. More frequent spikes result in increasing the error in spikes's amplitude. Likely because of inertia 
in anlog components of audio system. Also increasing the frequency of sound leads to increasing the error of spike's amplitude. Because the wavelength becomes smaller,
Arduino reads fewer values of each spike, which leads to bigger error. 

Inside Java code you will find samples of sound spikes of different frequency if you want to experiment and come up with better algorithms, or you can just generate your own through additional software (I used Audacity) or at runtime using some framework.

Because different devices have different audio systems, the resulting physical amplitude and wavelength of a wave for each number will be specific for your setup. In order to successfully demodulate this signal you will have to adjust Arduino script. 
First you need to change the *amplitudeIntervals* array that contains values of amplitudes dividing ranges of amplitudes for each numeric value:
```
int amplitudeIntervals[] = {20,30,40,50,65,75,85,100};
```
This can be done by sending sequence 0-6 several times, printing the resulting amplitudes and picking up appropriate dividing amplitudes. Note that printing to Serial
is computational heavy procedure for Arduino, and if you try to output too much info, it will decrease the speed of Arduino which will lead to fewer points of each spike registered and as a result to errors in recognition of spikes.
Second, you need to change *wavelengthIntervals* array that contains ranges of width of spike (amount of points containing the spike):
```
int wavelengthIntervals[] = {8,12,20};
```
Finally you will need to adjust 2 values. *minVolume* in Java code and *rawValueThreshold* in Arduino script. *minVolume* corresponds to minimum volume of signal produced. This value is needed because on low volumes signals are noised and distorted. *rawValueThreshold* is needed to cut the noise that is being produced during signal travelling through cable and through Arduino. Lower values of these two variables make the range of amplitudes wider and provide more points for each spike but at the same time increase the errors. 
