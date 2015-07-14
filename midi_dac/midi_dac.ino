#include <SPI.h>
#include <MIDI.h>

#define CS_DAC 6
#define GATE_PIN 7
#define LED 13

byte MIDI_COMMAND;
byte MIDI_NOTE;
byte MIDI_VEL;
byte ActiveNote;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
    MIDI.begin();
    SPI.begin();  
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    pinMode(CS_DAC, OUTPUT);
    pinMode(GATE_PIN, OUTPUT);
    pinMode(LED, OUTPUT);
    digitalWrite(GATE_PIN, LOW);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
}


void loop()
{
    MIDI_READ();
}


int DAC_OUTPUT(int value)
{
    int outputValue = value;
    digitalWrite(CS_DAC, LOW);
    int data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(CS_DAC, HIGH);
}

int MIDI2DAC(byte note)
{
      int dac_val = (note-36)*70;//sample
      return dac_val;
}

void MIDI_READ()
{
    if(MIDI.read())
    {
        if(MIDI.getType()==144)  //Note On
        {
            digitalWrite(GATE_PIN,LOW);
            MIDI_COMMAND = MIDI.getType(); 
            MIDI_NOTE = MIDI.getData1();
            MIDI_VEL = MIDI.getData2();
            ActiveNote = MIDI_NOTE;
            DAC_OUTPUT(MIDI2DAC(MIDI_NOTE));
            digitalWrite(GATE_PIN,HIGH);
        }
    }
     
    if(MIDI.getType()==128) //Note Off
    {
        if(MIDI.getData1()==ActiveNote)
        {
            digitalWrite(GATE_PIN,LOW);                
        }
    }        
}

void MIDI_TEST()
{
    if(MIDI.read())
    {
        if(MIDI.getType()==144)  //Note On
        {
            digitalWrite(13, HIGH);
        }
        if(MIDI.getType()==128)
        {
            digitalWrite(13, LOW);   
        }
    }
}
	
