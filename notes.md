I2C Stuff
https://manpages.debian.org/jessie-backports/linux-manual-4.8/I2C_BOARD_INFO.9.en.html
https://manpages.debian.org/jessie/linux-manual-3.16/i2c_new_device.9.en.html
section 5.1.1... I2C address is 001100xb where x is either 0 or 1 depending on whether SD0/SA0 pad is grounded

Accel Info:
address = 001100x
	where x is 1 if SA0 is volt, 0 if SA0 is ground
	in this board, SA0 is ground
	address = 0011000 (0x18)

Needed commands:
 Write Byte/Word
 Read Byte/Word

check operation:
 Read WHO_AM_I (0Fh) should be: 0b00110010 (0x32)

enable operation:
 CTRL_REG1 (20h) write 0b00100111 (0x27)

