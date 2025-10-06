# espeak-ng_T4

An port of the [eSpeak NG](https://github.com/espeak-ng/espeak-ng) text-to-speech engine for **Teensy 4.0 / 4.1** boards.

This library is based on the [Arduino eSpeak NG port for ESP32](https://github.com/pschatzmann/arduino-espeak-ng) by Phil Schatzmann, which in turn builds upon the original [eSpeak NG](https://github.com/espeak-ng/espeak-ng) project.

---

## What is eSpeak NG?

eSpeak NG is a compact, open-source text-to-speech engine that uses **formant synthesis**. It supports:
- Over **100 languages and accents**,
- A **lightweight and fast** speech engine suitable for embedded systems,
- Partial support of **SSML tags**. 
- Optional support for **MBROLA voices** (but not yet implemented in this port...)

---

## About this Teensy port

This version is specifically adapted and optimized for **Teensy 4.0 / 4.1** (Cortex-M7):

- Runs entirely from **FLASH or RAM**, no filesystem required.
- Voices and dictionaries are **compiled directly into the binary**.
- Runtime memory usage is about **150 kB**, configurable:
  - In `DMAMEM` (default),
  - Or in `EXTMEM` (PSRAM) if available.
- FLASH usage is around **1 MB** with the English voice included. Adding additional languages require a few hundred kilobytes more. 
- Compatible with the Teensy Audio library.

---

## Usage

> 💡 Check the [`examples/`](examples/) directory for usage examples.

It shows how to:
- Initialize the engine,
- Load a voice,
- Generate speech from a a text string,
- Combine the library to be used with PJRC's Audio lib. 

---

## Included data

eSpeak NG supports more than 100 languages and accents, including:
- 🌍 English, French, German, Spanish, Italian, Russian, Japanese, etc.

The english voice is loaded as the default voice but the library includes precompiled voice and language data in header form, under `src/espeak-ng-data/`.


---

## Credits

- Original project: [eSpeak NG](https://github.com/espeak-ng/espeak-ng)
- ESP32 port: [pschatzmann/arduino-espeak-ng](https://github.com/pschatzmann/arduino-espeak-ng)
