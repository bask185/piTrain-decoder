#!/bin/bash
echo "COMPILING"
arduino-cli compile -b arduino:avr:nano ~/Documents/software/piTrain-decoder
#echo "COMPILING DONE"

echo "UPLOADING"
arduino-cli upload -b arduino:avr:nano:cpu=atmega328old -p COM4 -i ~/Documents/software/piTrain-decoder/piTrain-decoder.arduino.avr.nano.hex
#echo "UPLOADING COMPLETE"
rm *.hex *.elf
exit
