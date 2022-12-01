#include <SD.h>
#include <ArduinoSound.h>
#define I2S_DEVICE 1  // this enables the I2C bus

int fsrAnalogPinN = 0;
int fsrAnalogPinS = 1;
int fsrAnalogPinW = 2;
int fsrReadingN;
int fsrReadingS;
int fsrReadingW;
bool soundOn = false;
bool pressure = false;

// filename of wave file to play
// file name must be 8 chars (max) .3 chars
const char filename[] = "w.wav";

// variable representing the Wave File
SDWaveFile waveFile;
// timestamp for printing the current time:
long lastPrintTime = 0;
void setup() {
  // put your setup code here, to run once:
  // Open serial communications:
  Serial.begin(9600);
  // wait for serial  monitor to open:
  while (!Serial)
    ;

  // setup the SD card.
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("SD card initialization failed!");
    while (true)
      ;  // do nothing
  }
  Serial.println("SD card is valid.");

  // create a SDWaveFile
  waveFile = SDWaveFile(filename);

  // check if the WaveFile is valid
  if (!waveFile) {
    Serial.print("There is no .wav file called ");
    Serial.println(filename);
    while (true)
      ;  // do nothing
  }
  // print the file's duration:
  long duration = waveFile.duration();
  Serial.print("Duration = ");
  Serial.print(duration);
  Serial.println(" seconds");

  // check if the I2S output can play the wave file
  if (!AudioOutI2S.canPlay(waveFile)) {
    Serial.println("unable to play wave file using I2S");
    while (true)
      ;  // do nothing
  }

  // set the playback volume:
  AudioOutI2S.volume(100);
  // start playback

  //  AudioOutI2S.play(waveFile);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (AudioOutI2S.isPlaying()) {
    soundOn = true;
  } else if (!AudioOutI2S.isPlaying() || AudioOutI2S.isPaused()) {
    soundOn = false;
  }

  fsrReadingN = analogRead(fsrAnalogPinN);
  fsrReadingS = analogRead(fsrAnalogPinS);
  fsrReadingW = analogRead(fsrAnalogPinW);

  if (fsrReadingN > 100 || fsrReadingS > 100 || fsrReadingW > 100) {
    pressure = true;
  } else {
    pressure = false;
  }

  Serial.println(pressure);
  Serial.println(fsrReadingW);


  if (AudioOutI2S.isPaused() && pressure) {
    Serial.println(1);
    AudioOutI2S.resume();
  }

  if (!AudioOutI2S.isPlaying() && pressure) {
    AudioOutI2S.play(waveFile);
  } else if (AudioOutI2S.isPlaying() && !pressure) {
    AudioOutI2S.pause();
  }
}