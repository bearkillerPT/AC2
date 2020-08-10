
pcompile $1".s";
hex2asm $1".hex"; 
ldpic32 $1".hex" -p /dev/ttyS3; 
rm *.hex;
rm *.elf; 
rm *.map; 
rm *.o; 
pterm -p /dev/ttyS3 
