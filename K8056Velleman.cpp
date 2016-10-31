/*	K8056Velleman.ccp

	Programmer		:	A.S. Tilanus | reptile-addict.nl (alban.tilanus[at]gmail.com)
					:	Jure Mav | Trilane.si (jure.mav[at]gmail.com)

	Version			:	20130421 First release
						20130422 Made some changes as discussed in: http://arduino.cc/forum/index.php/topic,161877.msg1211201.html#msg1211201
						20130423 byteWrite() Allows to control the status of all relays in one instruction, by sending a
						byte containing the relay status for each relay. (MSB : relay1, LSB : relay8)

	Description		:	Arduino library to control the Velleman K8056 8-channel relay card trough RS232
						http://www.reptile-addict.nl/arduino/?page_id=209

	Keywords		:	Arduino; RS232; Velleman; K8056;

	Copyright		:	http://opensource.org/licenses/mit-license.php

	Disclaimer		:	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "K8056Velleman.h"
#include "Arduino.h"

/// Public functions
K8056Velleman::K8056Velleman() {
}

byte SerialID; // used to identify the necessary Serial connection

/// Open the Serial connection for the RS232 commands between the Arduino and the K8056.
/// Optionally a SerialAddress could be supplied if a Serial connection other than the default is desired.
void K8056Velleman::begin() {
	Serial.begin(2400);
	SerialID=0;
}


/// Overload of previous function containing the optionally SerialAddress
void K8056Velleman::begin(byte SerialAddress) {
	// the Arduino MEGA2560 offers 4 Serial connections
	// if this routine is ran from an Arduino other than the ones described below the default Serial0 or Serial is issued,
	// same applies for SerialAddresses that are illegal such as 8 (=Serial8) on a MEGA for example which only has 4 Serials.

	int BaudRate = 2400;

	// TODO: Check for other boards that have more than 1 serial connection.
	#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
		switch (SerialAddress) {
		case 0:
			Serial.begin(BaudRate);
			SerialID=0;
			break;
		case 1:
			Serial1.begin(BaudRate);
			SerialID=1;
			break;
		case 2:
			Serial2.begin(BaudRate);
			SerialID=2;
			break;
		case 3:
			Serial3.begin(BaudRate);
			SerialID=3;
			break;
		default:
			Serial.begin(BaudRate);
			SerialID=0;
			break;
		}
	#elif defined(__AVR_ATmega32U4__)	// support for the Arduino micro and Leonardo
		Serial1.begin(BaudRate);
		SerialID=1;
	#else
		Serial.begin(2400);
		SerialID=0;
	#endif
}



void K8056Velleman::ON(byte CardAddress, byte Relay) {
	RS232Instruction(CardAddress,0x53,Relay,false); // Instruction S
}

void K8056Velleman::OFF(byte CardAddress, byte Relay) {
	RS232Instruction(CardAddress,0x43,Relay,false); // Instruction C
}

void K8056Velleman::EmergencyStop() {
	RS232Instruction(2,0x45,1,false); // any cardaddress will do
}

void K8056Velleman::Toggle(byte CardAddress, byte Relay) {
	RS232Instruction(CardAddress,0x54,Relay,false); // Instruction T
}

void K8056Velleman::digitalWrite(byte CardAddress, byte Relay, byte Status) {
	if (Status == HIGH) {
		ON(CardAddress, Relay);
	}
	else if (Status == LOW) {
		OFF(CardAddress, Relay);
	}
}

void K8056Velleman::byteWrite(byte CardAddress, byte Relay) {
	RS232Instruction(CardAddress,0x42,Relay,true); // Instruction B
}

/// Private functions

byte K8056Velleman::CheckSum(byte Address, byte Instruction, byte Relay) {
	byte CR = 0x0D; // Carriage return
	// Calculate the checksum for the RS232 transmission:
	//	2-complement of the sum of the 4 previous bytes + 1
	return 255 - ((CR + Address + Instruction + Relay)%255) + 1;
}


void K8056Velleman::RS232Instruction(byte Address ,byte Instruction, byte Relay, byte isBinary) {

	byte CR = 0x0D; // Carrtiage Return

	if (isBinary == false) {
		Relay = 48 + Relay; // '1' = dec 48 + 1 & '2' = dec 48 + 2 etc... ASCII ;)
	}

	byte _CheckSum = CheckSum(Address,Instruction,Relay);
	byte NrSends = 4; // number of times the RS232 instruction is sent to the card. Needs to be at least 2 according to the manual!!!
	for (byte i=1; i <= NrSends; i++){
		// TODO: Check for other boards that have more than 1 serial connection.
		#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
			switch (SerialID) {
			case 0:
				Serial.write(CR);
				Serial.write(Address);
				Serial.write(Instruction);
				Serial.write(Relay);
				Serial.write(_CheckSum);
				break;
			case 1:
				Serial1.write(CR);
				Serial1.write(Address);
				Serial1.write(Instruction);
				Serial1.write(Relay);
				Serial1.write(_CheckSum);
				break;
			case 2:
				Serial2.write(CR);
				Serial2.write(Address);
				Serial2.write(Instruction);
				Serial2.write(Relay);
				Serial2.write(_CheckSum);
				break;
			case 3:
				Serial3.write(CR);
				Serial3.write(Address);
				Serial3.write(Instruction);
				Serial3.write(Relay);
				Serial3.write(_CheckSum);
				break;
			}
		#elif defined(__AVR_ATmega32U4__)	// support for the Arduino micro and Leonardo
			Serial1.write(CR);
			Serial1.write(Address);
			Serial1.write(Instruction);
			Serial1.write(Relay);
			Serial1.write(_CheckSum);
		#else
			Serial.write(CR);
			Serial.write(Address);
			Serial.write(Instruction);
			Serial.write(Relay);
			Serial.write(_CheckSum);
		#endif
	}
}
