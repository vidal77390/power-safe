/* @file CustomKeypad.pde
  || @version 1.0
  || @author Alexander Brevig
  || @contact alexanderbrevig@gmail.com
  ||
  || @description
  || | Demonstrates changing the keypad size and key values.
  || #
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {D0, D1, D2, D3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {D4, D5, D6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
int indexMdp;
char mdp[4];
const byte PIN_BUZZER = D8;
const byte PIN_PIR = D7;

bool initialisationDone;

ESP8266WebServer server(80);
// WIFI config
const char* ssid = "ESGI";
const char* password = "Reseau-GES";
int ledState = 0;



void setup() {
  
  pinMode(PIN_BUZZER, OUTPUT);
  // PIR Detector
  pinMode(PIN_PIR, INPUT);
  // Led of ESP
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  initialisationDone = false;
  
  Serial.begin(115200);
  Serial.println("debut");
  indexMdp = 0;

  delay(10);

  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP to access the server

  server.on("/getAlarm", handleAlarmState);
  server.on("/desactivateAlarm", desactivateAlarm);
  server.on("/new", newPassword);

  server.begin();                                       //Start the server
  Serial.println("Server listening");

}

//Server Part

void handleAlarmState() {
  String message = "{\r \"alarmState\": \"";
  if(initialisationDone) message = message + "true";
  else message = message + "false";
  message = message + "\"\r}";
  server.send(200, "application/json", message);          //Returns the HTTP response
}



void desactivateAlarm() {
  String message = "";
  if (server.arg("password") == "") {   //Parameter not found
    message = "password Argument not found";

  } else {
    if(initialisationDone) {
      String password = server.arg("password");
      bool isGoodPassword = true;
      for(int i = 0; i<4; i++){
        if(password[i] != mdp[i]) isGoodPassword = false;
      }
      if(isGoodPassword) {
        initialisationDone = false;
      }else{
        playAlarm();
        initialisationDone = false;
      }
    }
  }
   digitalWrite(2, ledState);
  server.send(200, "text/plain", message);
}

void newPassword() {
  String message = "";
  int code;

  if (initialisationDone) {
    if (server.arg("password") == "") {   //Parameter not found
      message = "password Argument not found";
    } else {    //Parameter found
      String password = server.arg("password");
      for(int i = 0; i<4; i++) {
        mdp[i] = password[i];
      }
      message = "password changed";    //Gets the value of the query parameter
    }
  }else {
     message = "Alarme desactivé pas possible de changer le mdp";
     
  }
  server.send(200, "text/plain", message);
}




void loop() {

 server.handleClient();
  
  if(!initialisationDone) {
      if ( indexMdp < 4) {
        char customKey = customKeypad.getKey();
        if (customKey) {
          Serial.println(customKey);
          playTonalitie(customKey);
          mdp[indexMdp] = customKey;
          indexMdp++;
        }
      } else if (indexMdp == 4) {
        indexMdp++;
        //playSong();
        Serial.print("mot de passe:");
        for (int i = 0; i < 4; i++) {
          Serial.print(mdp[i]);
        }
        Serial.println("\n is initialized");
        initialisationDone = true;
      }
  }else {
    bool humanDetected = isHumanDetected();
    if(humanDetected) {
      Serial.println("human detected");
      //playAlarm();
      initialisationDone = false;
    }else {
      Serial.println("in progress");    
    }
  }


}

bool isHumanDetected() {
  bool detect = false;
    ////Test routine for PIR
  detect = digitalRead(PIN_PIR);
  if (detect) {
    Serial.print(F("----> Detectteeedddd"));
  }
  delay(1000);
  return detect;
}

void playAlarm() {
  for(int i=0; i<10; i++){
  tone(PIN_BUZZER, 1000, 200);
          delay(200);
  tone(PIN_BUZZER, 400, 200);
          delay(200);
  }
}

void playTonalitie(char key) {
  switch (key) {
        case '1':
          tone(PIN_BUZZER, 400, 200);
          break;
        case '2':
          tone(PIN_BUZZER, 600, 200);
          break;
        case '3':
          tone(PIN_BUZZER, 800, 200);
          break;
        case '4':
          tone(PIN_BUZZER, 1000, 200);
          break;
        case '5':
          tone(PIN_BUZZER, 1200, 200);
          break;
        case '6':
          tone(PIN_BUZZER, 1400, 200);
          break;
        case '7':
          tone(PIN_BUZZER, 1600, 200);
          break;
        case '8':
          tone(PIN_BUZZER, 1800, 200);
          break;
        case '9':
          tone(PIN_BUZZER, 2000, 200);
          break;
        case '0':
          tone(PIN_BUZZER, 2200, 200);
          break;
        default:
          break;
      }
}
 
void playSong() {
  tone(PIN_BUZZER, 2637, 200);
  delay(400);
  tone(PIN_BUZZER, 1975, 200);
  delay(200);
  tone(PIN_BUZZER, 2093, 200);
  delay(200);
  tone(PIN_BUZZER, 2349, 200);
  delay(400);
  tone(PIN_BUZZER, 2093, 200);
  delay(200);
  tone(PIN_BUZZER, 1975, 200);
  delay(200);
  tone(PIN_BUZZER, 1760, 200);
  delay(400);
  tone(PIN_BUZZER, 1760, 200);
  delay(200);
  tone(PIN_BUZZER, 2093, 200);
  delay(200);
  tone(PIN_BUZZER, 2637, 200);
  delay(400);
  tone(PIN_BUZZER, 2349, 200);
  delay(200);
  tone(PIN_BUZZER, 2093, 200);
  delay(200);
  tone(PIN_BUZZER, 1975, 200);
  delay(400);
  tone(PIN_BUZZER, 1975, 200);
  delay(200);
  tone(PIN_BUZZER, 2093, 200);
  delay(200);
  tone(PIN_BUZZER, 2349, 200);
  delay(400);
  tone(PIN_BUZZER, 2637, 200);
  delay(400);
  tone(PIN_BUZZER, 2093, 200);
  delay(400);
  tone(PIN_BUZZER, 1760, 200);
  delay(400);
  tone(PIN_BUZZER, 1760, 200);
  delay(800);
  tone(PIN_BUZZER, 1760, 200);
  delay(400);
  tone(PIN_BUZZER, 2349, 200);
  delay(200);
  tone(PIN_BUZZER, 2794, 200);
  delay(200);
  tone(PIN_BUZZER, 3520, 200);
  delay(400);
  tone(PIN_BUZZER, 3136, 200);
  delay(200);
  tone(PIN_BUZZER, 2794, 200);
  delay(200);
  tone(PIN_BUZZER, 2637, 200);
  delay(600);
  tone(PIN_BUZZER, 2093, 200);
  delay(200);
  tone(PIN_BUZZER, 2637, 200);
  delay(400);
  tone(PIN_BUZZER, 2349, 200);
  delay(200);
  tone(PIN_BUZZER, 2093, 200);
  delay(200);
  tone(PIN_BUZZER, 1975, 200);
  delay(400);
  tone(PIN_BUZZER, 1975, 200);
  delay(200);
  tone(PIN_BUZZER, 2093, 200);
  delay(200);
  tone(PIN_BUZZER, 2349, 200);
  delay(400);
  tone(PIN_BUZZER, 2637, 200);
  delay(400);
  tone(PIN_BUZZER, 2093, 200);
  delay(400);
  tone(PIN_BUZZER, 1760, 200);
  delay(400);
  tone(PIN_BUZZER, 1760, 200);
  delay(800);
}
