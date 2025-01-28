/*
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
#define G_LED           7
#define R_LED           4
#define Y_LED           2
#define BUZZER          3
#define BUTTON          5

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

String chipuid = "";
String newuid = "";

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  pinMode(G_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(Y_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
    if(chipuid == "")
    {
      digitalWrite(Y_LED, HIGH);
    }
    else
    {
    digitalWrite(G_LED, LOW);
    digitalWrite(R_LED, HIGH);
    }
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
  
  if (chipuid== ""){
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        chipuid += String(mfrc522.uid.uidByte[i], HEX);
      }

    // Print the UID to the serial monitor
    Serial.print("UID Value: ");
    Serial.println(chipuid);
      
      // Optional: Do something with the UID value
      // You can now use rfidUID in your program

      // Halt the RFID tag
    

    digitalWrite(Y_LED, LOW);
  }
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    newuid += String(mfrc522.uid.uidByte[i], HEX);
  }
  

  if (newuid == chipuid)
  {
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, HIGH);
    while(mfrc522.PICC_IsNewCardPresent())
    {
      digitalWrite(BUZZER, HIGH);
      delay(1);
      digitalWrite(BUZZER, LOW);
      delay(1);
    }
  }
  
  newuid = "";

  if (BUTTON == LOW)
  {
    chipuid = "";
  }
	// Dump debug info about the card; PICC_HaltA() is automatically called
	
}
