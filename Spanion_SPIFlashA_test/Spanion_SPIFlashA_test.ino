/* Sample SPIFlashA for Anarduino miniWireless test script f.
 * For compatibility purposes with multiple SPI devices it is associated to a RFM69 transceiver
 * (Typical case when running on a miniWireless platform)
 * Most of the function can be tested by removing the comments delimiters
 * NOTE:  one write constraint is that the bits of the location to write must be set to 1
 *        Therefore the user should insure that any write attempt occurs on a location that is 
 *        erased (containing 0xFF), typically by staring with a bulkEarse() function.
 *        WARNING: on a 16 MBytes chip this function takes approximately 45s)
*/
#include <RFM69.h>          //get it here: https://www.github.com/lowpowerlab/rfm69 
#include <SPI.h>        
#include <SPIFlashA.h> 
#define FLASH_SS      5     // IMPORTANT: on Anarduino miniWireless the Flash SPI salve select is D5 (vs D8 on Moteino)
#define NODEID        2     // Unique for each node on same network (
#define NETWORKID     100   // the same on all nodes that talk to each other
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_915MHZ 

byte flashBuffer[90];                // Define a read buffer for readBytes() tests
byte x = 0;                          // Used to store incremental write pattern (test 9)
RFM69 radio;                         // Create a dummy RFM69 radio instance
SPIFlashA flash(FLASH_SS, 0x12018);  // Create a SPANION SPI Flash instance 

void setup() {
  Serial.begin (115200);
    radio.initialize(FREQUENCY,NODEID,NETWORKID);
  if (flash.initialize())
    Serial.println("SPI Flash Init OK!");
  else
    Serial.println("SPI Flash Init FAIL!");
}

void loop() {
Serial.println ("\r\nLoop");

/* 1. Print the JEDEC ID 
   ======================= */

Serial.print("Test 1: Get the JedecID: ");
Serial.println((long)flash.readDeviceId(), HEX); 

/* 2. Print the Device Unique ID 
   =============================*/
   
Serial.print("Test 2: Get the Device Unique ID: ");
flash.readUniqueId ();
for (int i = 0; i<12; i++)
{
  Serial.print(flash.UNIQUEID[i],HEX);
  if (i <11) Serial.print('-');
}
Serial.println();


/* 3. Bulk Erase of the chip 
 *    ====================== */
/*
Serial.println ("Test 3: Bulk Erase of the chip - WARNING this may take about 45s");
Serial.print ("Read address 16777215 (HEX): ");         
Serial.println (flash.readByte(16777215),HEX);          // Read last location of the Flash Memory         
Serial.println ("Write address 16777215 (HEX) AA ");
flash.writeByte(16777215,0xAA);                         // Write to this location (should work if this location was previously erased)
Serial.print ("Read address 16777215 (HEX): ");
Serial.println (flash.readByte(16777215),HEX);          // Verify that the location was correctly written
Serial.println ("Erasing 16MBytes ");
long start = millis();                                  // Start time before starting the Erase
flash.bulkErase();                                      // Initiate a the erase
      while(flash.busy());                              // Wait until finished
      Serial.print("DONE after (ms): ");Serial.println (millis()-start);  // Print the erasure time
Serial.print ("Read address 16777215 (HEX): ");
Serial.println (flash.readByte(16777215),HEX);          // Verify that the location is well erased
*/
/* 4. 64KBytes Erase  
 *    ============== */
 /*
Serial.println ("Test 4: 64KBytes Erase");
Serial.print ("Read address 65535 (HEX): ");
Serial.println (flash.readByte(65535),HEX);
Serial.println ("Write address 65535 (HEX) BB ");
flash.writeByte(65535,0xBB);
Serial.print ("Read address 65535 (HEX): ");
Serial.println (flash.readByte(65535),HEX);
Serial.println ("Erasing 64KBytes: ");
long start = millis();
flash.blockErase64K(0);
      while(flash.busy());
      Serial.print("DONE after (ms): ");Serial.println (millis()-start);   
Serial.print ("Read address 65535 (HEX): ");
Serial.println (flash.readByte(65535),HEX);
*/
/* 5. 32KBytes Erase  
 *    ============== */
 /*
Serial.println ("Test 5: 32KBytes Erase");
Serial.print ("Read address 32767 (HEX): ");
Serial.println (flash.readByte(32767),HEX);
Serial.println ("Write address 32767 (HEX) CC");
flash.writeByte(32767,0xCC);
Serial.print ("Read address 32767 (HEX): ");
Serial.println (flash.readByte(32767),HEX);
Serial.println ("Erasing 32KBytes: ");
long start = millis();
flash.blockErase32K(0);
      while(flash.busy());
      Serial.print("DONE after (ms): ");Serial.println (millis()-start);  
Serial.print ("Read address 32767 (HEX): ");
Serial.println (flash.readByte(32767),HEX);
*/
/* 6. 4KBytes Erase  
 *    ============== */
/*
Serial.println ("Test 6: 4KBytes Erase");
Serial.print ("Read address 4095 (HEX): ");
Serial.println (flash.readByte(4095),HEX);
Serial.println ("Write address 4095 (HEX) DD: ");
flash.writeByte(4095,0xDD);
Serial.print ("Read address 4095 (HEX): ");
Serial.println (flash.readByte(4095),HEX);
Serial.println ("Erasing 4KBytes: ");
long start = millis();
flash.blockErase4K(0);
      while(flash.busy());
      Serial.print("DONE after (ms): ");Serial.println (millis()-start);  
Serial.print ("Read address 4095 (HEX): ");
Serial.println (flash.readByte(4095),HEX);
*/
/*
/* 7. 512KBytes Erase  
 *    ============== */
 /*
Serial.println ("Test 7: 512KBytes Erase");
Serial.print ("Read address 524287 (HEX): ");
Serial.println (flash.readByte(524287),HEX);
Serial.println ("Write address 524287 (HEX) EE: ");
flash.writeByte(524287,0xEE);
Serial.print ("Read address 524287 (HEX): ");
Serial.println (flash.readByte(524287),HEX);
Serial.println ("Erasing 512KBytes: ");
flash.writeByte(524287,0xEE);
long start = millis();
flash.blockErase512K(0);
      while(flash.busy());
      Serial.print("DONE after (ms): ");Serial.println (millis()-start);  
Serial.print ("Read address 524287 (HEX): ");
Serial.println (flash.readByte(524287),HEX);
*/
/* 8. Simulate Moteino Chip Erase (512KBytes Erase ) 
 *    ============================================== */
/* 
Serial.println ("Test 8: Moteino Chip Erase (512KBytes)");
Serial.print ("Read address 524287 (HEX): ");
Serial.println (flash.readByte(524287),HEX);
Serial.println ("Write address 524287 (HEX) 11: ");
flash.writeByte(524287,0x11);
Serial.print ("Read address 524287 (HEX): ");
Serial.println (flash.readByte(524287),HEX);
Serial.println ("Chip Erase (512K): ");
long start = millis();
flash.chipErase();
      while(flash.busy());
      Serial.print("DONE after (ms): ");Serial.println (millis()-start); 
Serial.print ("Read address 524287 (HEX): ");
Serial.println (flash.readByte(524287),HEX); 
*/
/* 9. Read Byte test 
 *    ============== */
/*
Serial.println ("Test 9: Read 40 Bytes (Byte per Byte)");
for (int i = 0; i < 40 ; i++)
{ 
  byte b =flash.readByte(i);
  if(b<0x10) Serial.print("0");
  Serial.print(b,HEX), Serial.print (" ");
}
Serial.println ();
Serial.println ("Erasing 4K of the area to write:");
flash.blockErase4K(0);
Serial.println ("Writing 40 incremental HEX Bytes");
for (int i = 0; i < 40 ; i++)
{
  flash.writeByte(i,x);
  x++;
}
while (flash.busy());
Serial.println ("Read 40 Bytes (Byte per Byte):");
for (int i = 0; i < 40 ; i++)
{ 
  byte b =flash.readByte(i);
  if(b<0x10) Serial.print("0");
  Serial.print(b,HEX), Serial.print (" ");
}
Serial.println();
/* Test 10. Write / Read bulk test 
 * ============================== */
/*
Serial.println ("Test 10: Read - Write - Read 44 Bytes into buffer");
Serial.print ("Read Bulk: "); 
flash.readBytes (0,flashBuffer,44);
for (int i = 0; i <44; i++)
{
 Serial.print((char) flashBuffer[i]);
}
Serial.println ();
Serial.println ("Erasing 4K to write:");
flash.blockErase4K(0);
Serial.print ("Verify Erasure: ");
flash.readBytes (0,flashBuffer,44);
for (int i = 0; i <44; i++)
{
 Serial.print((char) flashBuffer[i]);
}
Serial.println ();
Serial.println ("Write Bulk: The quick brown fox jumps over the lazy dog");
flash.writeBytes (0,"The quick brown fox jumps over the lazy dog",44);
while (flash.busy());
Serial.print ("Read Bulk:  ");
flash.readBytes (0,flashBuffer,44);
for (int i = 0; i <44; i++)
{
 Serial.print((char) flashBuffer[i]);
}
Serial.println ();
*/
/* Test 11. Print Status Registers 
 * =============================== */
/*
Serial.println ("Test 11: Print the Status Registers");
flash.printStatus();
*/
/* Test 12. Print the Manufacturer and Device ID area
   ================================================== */
/*
 * Serial.println ("Test 12: Manufacturer ID and Device ID area (320 Bytes)");
flash.printRDID();
*/
delay (2000);

}
