# Teodor Synth 3001

This is a VST Synthesizer developed mainly for Linux as the project for 2024 C++ course

## Components

Currently the synthesizer has two oscillators with selectable waveforms, a noise oscillator as well as an Attack/Decay envelope modulating a lowpass filter.

## Prerequisites

`gcc`, `cmake`, `make`, `vst3sdk`

## Building

Edit `CMakeLists.txt` to set the vst3 sdk source directory. Then, build the TeodorSynth3001 target using cmake.

## Usage

Compatible with any DAW supporting Vst plugins. Tested primarily with Reaper on Linux.
