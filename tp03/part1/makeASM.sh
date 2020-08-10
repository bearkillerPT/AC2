
pcompile $1".s";
hex2asm $1".hex"; 
ldpic32 $1".hex" -p /dev/ttyS3; 
rm $1".hex";
rm $1".elf"; 
rm $1".map"; 
rm $1".o"; 
pterm -p /dev/ttyS3;
