#include "Canal.h"
#include "Debug.h"
#include "Timer.h"

Canal::Canal () {

}

bool Canal::attente_reponse (Stream &serialPort, char strwait[], char strinfo[], unsigned int timeout, unsigned int attente) {
    debugln (F("Canal::attente_reponse - BEGIN"));
    Timer tOut ( timeout );
    tOut.restart ();
    while ( !tOut.isExpired() ) {
        debug   (F("Canal::attente_reponse - Waiting "));
        debugln (strinfo);
        if (serialPort.find(strwait)) {
            debug   (F("Canal::attente_reponse - "));
            debug   (strwait);
            debugln (F(" found"));
            delay(attente);
            debugln (F("Canal::attente_reponse - END"));
            return true;
        }
    }
    debugln (F("Canal::attente_reponse - !!! Timeout !!!"));
    flush_serial(serialPort);
    debugln (F("Canal::attente_reponse - END [false]"));
    return false;
}

void Canal::flush_serial(Stream &serialPort) {
    while (serialPort.available() > 0) {
        char t = serialPort.read();
    }
}

String Canal::intToHex( int _input, int _nbChar ) {
  char tempChar[7] = "";

  switch (_nbChar) {
    case 2:
      sprintf(tempChar, "%02X", _input);
      break;

    case 4:
      sprintf(tempChar, "%04X", _input);
      break;
  }
  return String(tempChar);
}

boolean Canal::readSerial ( Stream &serial, String &buffer, char delimiteur, unsigned long timeout ) {
    unsigned long startTime = millis ();
    char curChar;
    boolean delimiterFound = false;

    while ( !delimiterFound && serial.available() > 0 && ( startTime - millis() ) < timeout ) {
        curChar = serial.read ();
        if ( curChar == delimiteur ) {
            delimiterFound = true;
        } else {
            buffer += curChar;
        }
    }
    
    return delimiterFound;
}