/*
 * SPI Flash memory library for anarduino.
 * This works with 256byte/page SPI flash memory (typically SPANSION S25FL127S)
 * For instance a 128MBit (16Mbyte) flash chip will have 65536 pages: 256*65536 = 16777216 bytes (16MKbytes)
 * This library uses the same function ones as the Moteino SPIFlash and is interned to be used for wireless programming
 * using the Moteino terminology for Anarduino
 * See http://lowpowerlab.com/blog/category/moteino/wireless-programming/ and https://github.com/LowPowerLab/WirelessProgramming 
 * Therefore only a limited set of SPANSION programming functions are implemented to erase, read and write the Flash memory
 *
 * NOTES:
 *		The Anarduino SPANSION Flash memory (S25FL127S) uses a SPI command interface that is slightly different than the one used by the
 *		Moteino WINBOND (W25X40CL).
 *		1. Deep Power mode (Down/Sleep 0xB9 and Release/Wakeup 0xAB) mode is not implemented, the equivalent Moteino SPIFlash functions are therefore programmed as a NOOP
 *		   for compatibility reasons
 *		2. blockErase34K(); (0x52) command is not exactly implemented as for the WINBOND, instead it generates a 8 * blockErase4K() fore compatibility
 *		3. The chipErase(); (0x60) which in the case of the WINBOND is equivalent to a 512K Block erase is simulated by a 8 * blockErase64K(), the actual Chip Erase 
 *		   which takes quite a long time (typically 45 seconds for 16 MBytes) is implemented as a new function (bulkErase()) in case of need;
 * 		4. The WINBOND Unique Identifier 8 Bytes value is replaced by a 12 last Bytes of the fisrt 16 Bytes OTP (Manufacturer One Time Program) which is obtained using 
 *		   the readUniqueId () command
 *			Note from the specs:
 *				The OTP 16 lowest address bytes are programmed by Spansion with a 128-bit random number. Only Spansion is able to program these bytes.
 *		5. The JEDEC identifier for the S25FL127S is 0x12018
 *			Note:
 *				1st Byte:  0x01 Manufacturer ID for Spansion
 *				2nd Byte:  0x20 (128 Mb) Device ID Most Significant Byte - Memory Interface Type *				3rd Byte:  0x18 (128 Mb) Device ID Least Significant Byte - Density 
 *		6. A new command printRDID (), is implemented to dump the Manufacturer and Device ID 320Bytes table
 *		7. A new command printStatus(), is implemented to print the status registers 1 and 2 
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
#ifndef _SPIFLASHA_H_
#define _SPIFLASHA_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <wiring.h>
#include "pins_arduino.h"
#endif

#include <SPI.h>

/// IMPORTANT: NAND FLASH memory requires erase before write, because
///            it can only transition from 1s to 0s and only the erase command can reset all 0s to 1s
/// See http://en.wikipedia.org/wiki/Flash_memory
/// The smallest range that can be erased is a sector (4K, (32K), 64K); there is also a chip erase command

/// Standard SPI flash commands
/// Assuming the W pin is pulled up (to disable hardware write protection)
/// To use any write commands the WEL bit in the status register must be set to 1 using the Write Enable command (WREN-0x06)./// The WREN command sets the Write Enable Latch (WEL) bit. The WEL bit is cleared to 0 (disables writes)/// during power-up, hardware reset, or after the device completes the following commands:///	– Reset///	– Page Program (PP-0x02)///	– Sector Erase (SE-0xD8)///	– Bulk Erase (BE-0x60)///	– Write Disable (WRDI-0x04)///	– Write Registers (WRR-0x01)///	– Quad-input Page Programming (QPP-0x32 or 0x38)///	– OTP Byte Programming (OTPP-0x42)

#define SPIFLASH_STATUSWRITE      0x01        // write status register - WRR
#define SPIFLASH_BYTEPAGEPROGRAM  0x02        // Page Program or Write (1 to 256bytes) - PP
#define SPIFLASH_ARRAYREADLOWFREQ 0x03        // Slow read array (low frequency) - READ
#define SPIFLASH_WRITEDISABLE     0x04        // write disable - WRDI
#define SPIFLASH_STATUSREAD       0x05        // read status register 1 - RDSR1
#define SPIFLASH_WRITEENABLE      0x06        // write enable - WREN
#define SPIFLASH_STATUSREAD2      0x07        // read status register 2 - RDSR2
#define SPIFLASH_ARRAYREAD        0x0B        // Fast read array (Need to add 1 dummy byte after 3 address bytes) - FAST_READ
#define SPIFLASH_BLOCKERASE_4K    0x20        // erase one 4K block of flash memory - P4E
#define SPIFLASH_CHIPERASE        0x60        // Bulk Erase (may take several seconds depending on size) - BE
//#define SPIFLASH_BLOCKERASE_32K   0x52        // Erase one 32K block of flash memory Not implemenetd for SPANION
#define SPIFLASH_MACREAD          0x4B        // One Time Program read (OTP)
#define SPIFLASH_IDREAD           0x9f        // read JEDEC manufacturer and device ID (3 bytes, specific bytes for each manufacturer and device)
//#define SPIFLASH_WAKE             0xAB      	// As another meaning for SPANSION than WINBOND deep power wake up
//#define SPIFLASH_SLEEP            0xB9        // As another meaning for SPANSION than WINBOND deep power down
#define SPIFLASH_BLOCKERASE_64K   0xD8        // erase one 64K block of flash memory
                                              
class SPIFlashA {
public:
  static byte UNIQUEID[12];						// Extended to 12 for SPANSION
  SPIFlashA(byte slaveSelectPin, uint32_t jedecID=0);
  boolean initialize();
  void command(byte cmd, boolean isWrite=false);
  byte readStatus();
  void printStatus();
  void printRDID();
  byte readByte(long addr);
  void readBytes(long addr, void* buf, word len);
  void writeByte(long addr, byte byt);
  void writeBytes(long addr, const void* buf, uint16_t len);
  boolean busy();
  void chipErase();
  void bulkErase();
  void blockErase4K(long address);
  void blockErase32K(long address);
  void blockErase64K(long address);
  void blockErase512K(long address);		// New for SPANSION
  long readDeviceId();
  byte* readUniqueId();
  
  void sleep();
  void wakeup();
  void end();
protected:
  void select();
  void unselect();
  byte _slaveSelectPin;
  long _jedecID;					// Changed form uint16_t to uint32_t for SPANSION
  byte _SPCR;
  byte _SPSR;
};

#endif