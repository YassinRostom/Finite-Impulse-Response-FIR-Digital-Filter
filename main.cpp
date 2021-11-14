#include "mbed.h"
#include "filter.h"

#define ON      1
#define OFF     0

float x[N]={0};                         //sample array of size N, initialised with 0's
float yn;                               //Output variable

void sampler(void);                     //Ticker routine PROTOTYPE

DigitalOut SampLED(PB_0);               //Digital Output for measuring sampling frequency and calculation time

AnalogIn  Ain(PA_3);                    //Analog Input (Signal Input 0 to +3 Volts)
AnalogOut Aout(PA_4);                   //Analog Output (Signal Input 0 to +3 Volts)

Ticker sample_timer;                    //Ticker class instance called sample_timer. Used to interrupt and sample ADC.


int main() {
  
    float sample_rate=(1.0/Fs)*1000000.0; //Calculate the number of uS required for a Frequency Sampling Rate. Fs held in filter.h
  
    sample_timer.attach_us(&sampler,(int)sample_rate);  //Ticker Instance serviced by routine at a repeat rate in microseconds
                                          
  
    while(1) {
        sleep(); //Sleep while not sampling
    }
}

void sampler(void){                     //Ticker routine
    
    SampLED = ON;                       //LED Indicates start of sampling
    
    int i;                              //Initialise local variable i
     
    x[0]=Ain;                           //Input ADC. N.B. ADC in MBED is 0.0 to 1.0 float   

    yn=0.0;                             //output accumulation, start as zero
    
    for(i=0; i<N; i++) yn+=x[i]*b[i];   //Sum the multiplication of the sample array and coefficient array
    for(i=N-1; i!=0; i--) x[i]=x[i-1];  //shift data in sample array down
    
    Aout=yn;                            //Output resultant summation and multiplications to DAC    
    
    //<comment>
    //Uncomment this to show aliasing
    //Aout = Ain;                           
    //</comment>
    
    SampLED = OFF;                      //LED Indicates end of sampling
}


