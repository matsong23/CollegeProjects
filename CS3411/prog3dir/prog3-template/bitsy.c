#include "bitsy.h"
#include <stdio.h>
#include <unistd.h>

// Add any global variables or structures you need.
// Reading globals
char blocks[1024];	// Bytes being read
int curBit = -1;	// Bit to be read when read bit is called next
int byteCount = -1; // Bytes in the block array

// Writing globals
unsigned char byteBuf = 0;
int byteBufIdx = 0;
unsigned char buffer[1024];
int bufferIdx = 0;

/* read_byte()
 * Abstraction to read a byte.
 * Relys on read_bit().
 */
unsigned short read_byte(){ 
    /* This function should not call read() directly.
     * If we are buffering data in read_bit, we dont want to skip over
     * that data by calling read again. Instead, call read_bit and 
     * construct a byte one bit at a type. This also allows us to read
     * 8 bits that are not necessarily byte alligned
     */

    // Do NOT call read here. Instead rely on read_bit.

    // I suggest returning a unique value greater than the max byte value
    // to communicate end of file. We need this since 0 is a valid byte value
    // so we need another way to communicate eof. These functions are typed
    // as short to allow us to return a value greater than the max byte value.
 
	unsigned short byte = 0;
	unsigned short bit = 0;
	
	// Gather all 8 bits
	for (int i = 0; i < 8 && (bit = read_bit()) != 255; i++) {
		if (bit == 1) byte |= 1 << i;  
		else byte &= ~(1 << i); 
	}
	
	// EOF is 256 because the max value of a byte is 255 so 256 isn't a valid byte value
	if (bit == 255) return 256; 
	
	return byte; 
}

/* read_bit()
 * Abstraction to read a bit.
 */
unsigned short read_bit(){
    /* This function is responsible for reading the next bit on the
     * input stream from stdin (fd = 0). To accomplish this, keep a 
     * byte sized buffer. Each time read bit is called, use bitwise
     * operations to extract the next bit and return it to the caller.
     * Once the entire buffered byte has been read the next byte from 
     * the 1024 sized buffer. Once all 1024 bytes have been read from
     * the buffer read a new 1024 bytes and repeat. Once eof is reached,
     * return a unique value > 255.
     */	

    // You will need to call read here.

    // I suggest returning a unique value greater than the max byte value
    // to communicate end of file. We need this since 0 is a valid byte value
    // so we need another way to communicate eof. These functions are typed
    // as short to allow us to return a value greater than the max byte value.
    

	
	// Check if a read call is need
	// Base case is -1 
	// otherwise call it if the current bit is out of the range of bits from 1024 bytes
	if (curBit == -1 || curBit >= 1024 * 8) {
		byteCount = read(0, blocks, 1024);
		curBit = 0;
	}	
	
	// Check if nothing left to be read
	// So if the current bit to be read is outside of the range of bits from bytes read
	if (curBit >= byteCount * 8) {
		return 255;
	}
	
	// Read bit
	unsigned int curByte = curBit / 8; // Current byte to be read
	unsigned int bitNum = curBit - (curByte * 8); // Bit to be read from current byte
	unsigned short bit = (blocks[curByte] >> bitNum) & 1; // Current bit value 
	curBit++; // Increment for next read_bit() call
	return bit; 
}

/* write_byte()
 * Abstraction to write a byte.
 */
void write_byte(unsigned char byte) {
    /* Use write_bit() to write each bit of byt one at a time. Using write_bit()
     * abstracts away byte boundaries in the output.
     */

    // Do NOT call write, instead utilize write_bit().
	
	unsigned char bit;
	for (int i = 0; i < 8; i++) {
		bit = (byte >> i) & 1;
		write_bit(bit);
	}
}

/* write_bit()
 * Abstraction to write a single bit.
 */
void write_bit(unsigned char bit) {
    /* Keep a byte sized buffer. Each time this function is called, insert the 
     * new bit into the buffer. Once 8 bits are buffered, place this byte into a
     * 1024 sized buffer. Once the 1024 sized buffer is full write 1024 bytes to
     * stdout (fd 1).
     */

    // You will need to call write here eventually.
	
	// Add bit to byte
	if (bit == 1) byteBuf |= 1 << byteBufIdx;
	else byteBuf &= ~(1 << byteBufIdx);
	byteBufIdx++;
	// Check if byte should be added to buffer
	if (byteBufIdx >= 8) {
		byteBufIdx = 0;
		buffer[bufferIdx] = byteBuf;
		bufferIdx++;
	}
	// Check if the buffer should be written
	if (bufferIdx >= 1024) {
		write(1, buffer, 1024);
		bufferIdx = 0;
	}
}

/* flush_write_buffer()
 * Helper to write out remaining contents of a buffer after padding empty bits
 * with 1s.
 */
void flush_write_buffer() {
    /* This will be utilized when finishing your encoding. It may be that some bits
     * are still buffered and have not been written to stdout. Call this function 
     * which should do the following: Determine if any buffered bits have yet to be 
     * written. Pad remaining bits in the byte with 1s. Write byte to stdout.
     */
	
	// Pad byte if needed
	if (byteBufIdx > 0) {
		while (byteBufIdx <= 8) {
			byteBuf |= 1 << byteBufIdx;
			byteBufIdx++;
		}
		byteBufIdx = 0;
		buffer[bufferIdx] = byteBuf;
		bufferIdx++; 
	}

	// Write the rest of the Buffer
	if (bufferIdx > 0) {
		write(1, buffer, bufferIdx);
		bufferIdx = 0;
	}
}
