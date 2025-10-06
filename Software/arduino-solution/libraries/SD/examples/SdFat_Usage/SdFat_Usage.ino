/*
  SdFat usage from SD library

  Starting with Teensyduino 1.54, the SD library is a thin wrapper for SdFat.

  You can access the main SdFat filesystem with "SD.sdfs".  You may wish to
  use SD.sdfs.begin() to cause SdFat to access the SD card using faster
  drivers than the default.  You may also wish to open files as FsFile to
  gain access to SdFat's special file functions which are not available using
  the simpler SD File.

  This example shows some of the ways to select optimized SdFat drivers and
  how to use special file truncation and pre-allocation for optimized data
  logging.

  This example code is in the public domain.
*/
#include <SD.h>

// On Teensy 2.0, SdFat's files are "File32" rather than "FsFat"
#ifdef __AVR__
#define FsFile File32
#endif

void setup()
{
  //Uncomment these lines for Teensy 3.x Audio Shield (Rev C)
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14

  Serial.begin(9600);
  while (!Serial); // wait for Arduino Serial Monitor

  Serial.print("Initializing SD card...");
  bool ok;
  const int chipSelect = 10; // only for SPI, can't use BUILTIN_SDCARD here

  // Instead of the usual SD.begin(pin), you can access the underlying
  // SdFat library for much more control over how the SD card is
  // accessed.  Uncomment one of these, or craft your own if you wish
  // to use SdFat's many special features.

  // Faster SPI frequency.  16 MHz is default for longer / messy wiring.
  ok = SD.sdfs.begin(SdSpiConfig(chipSelect, SHARED_SPI, SD_SCK_MHZ(24)));

  // Very slow SPI frequency.  May be useful for hardware with slow buffers.
  //ok = SD.sdfs.begin(SdSpiConfig(chipSelect, SHARED_SPI, SD_SCK_MHZ(4)));

  // SdFat offers DEDICATED_SPI optimation when no other SPI chips are
  // connected.  More CPU time is used and results may vary depending on
  // interrupts, but for many cases speed is much faster.
  //ok = SD.sdfs.begin(SdSpiConfig(chipSelect, DEDICATED_SPI, SD_SCK_MHZ(16)));

  // Different SPI port (Teensy 4.1 SPI1 is MOSI1:pin26, MISO1:pin1, SCK1:pin27)
  // SPI1 photo: https://forum.pjrc.com/threads/69254?p=297875&viewfull=1#post297875
  // SPI2 photo: https://forum.pjrc.com/threads/60954?p=303782&viewfull=1#post303782
  //ok = SD.sdfs.begin(SdSpiConfig(chipSelect, SHARED_SPI, SD_SCK_MHZ(16), &SPI1));

  // Access the built in SD card on Teensy 3.5, 3.6, 4.1 using FIFO
  //ok = SD.sdfs.begin(SdioConfig(FIFO_SDIO));

  // Access the built in SD card on Teensy 3.5, 3.6, 4.1 using DMA (maybe faster)
  //ok = SD.sdfs.begin(SdioConfig(DMA_SDIO));

  if (!ok) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Serial.println();

  // After the SD card is initialized, you can access it using the ordinary
  // SD library functions, regardless of whether it was initialized by
  // SD library SD.begin() or SdFat library SD.sdfs.begin().
  //
  Serial.println("Print directory using SD functions");
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break; // no more files
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
    } else {
      printSpaces(40 - strlen(entry.name()));
      Serial.print("  ");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }

  // You can also access the SD card with SdFat's functions
  //
  Serial.println();
  Serial.println("Print directory using SdFat ls() function");
  SD.sdfs.ls();

  // You can access files using SdFat which uses "FsFile" for open files
  // FsFile offers more capability than regular SD "File".  As shown in this
  // example, you can truncate tiles.  You can also pre-allocate a file on
  // the SD card (if it does not yet have any data, the reason we truncate
  // first).  Pre-allocation impoves the speed of writes within the already
  // allocated space while data logging or performing other small writes.
  //
  Serial.println();
  Serial.println("Writing to datalog.bin using SdFat functions");
  FsFile myfile = SD.sdfs.open("datalog.bin", O_WRITE | O_CREAT);
  unsigned int len = myfile.fileSize();
  Serial.print("datalog.bin started with ");
  Serial.print(len);
  Serial.println(" bytes");
  if (len > 0) {
    // reduce the file to zero if it already had data
    myfile.truncate();
  }
  if (myfile.preAllocate(40*1024*1024)) {
    Serial.print("  Allocate 40 megabytes for datalog.bin");
  } else {
    Serial.print("  unable to preallocate this file");
  }
  myfile.print("Just some test data written to the file (by SdFat functions)");
  myfile.write((uint8_t)'\0'); // add a null byte to mark end of string
  myfile.close();

  // You can also use regular SD functions, even to access the same file.  Just
  // remember to close the SdFat FsFile before opening as a regular SD File.
  //
  Serial.println();
  Serial.println("Reading to datalog.bin using SD functions");
  File f = SD.open("datalog.bin");
  if (f) {
    char mybuffer[100];
    int index = 0;
    while (f.available()) {
      char c = f.read();
      mybuffer[index] = c;
      if (c == 0) break;  // end of string
      index = index + 1;
      if (index == 99) break; // buffer full
    }
    mybuffer[index] = 0;
    Serial.print("  Read from file: ");
    Serial.println(mybuffer);
  } else {
    Serial.println("unable to open datalog.bin :(");
  }
  f.close();

  // When mixing SD and SdFat file access, remember for writing that
  // SD defaults to appending if you open with FILE_WRITE.  You must
  // use FILE_WRITE_BEGIN if you wish to overwrite the file from the
  // start.  With SdFat, O_WRITE or O_RDWR starts overwriting from the
  // beginning.  You must add O_AT_END if you wish to appead.
}

void loop()
{
  // nothing happens after setup finishes.
}


void printSpaces(int num) {
  for (int i = 0; i < num; i++) {
    Serial.print(" ");
  }
}

