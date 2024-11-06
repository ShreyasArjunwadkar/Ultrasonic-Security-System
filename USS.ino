ultra sonic security system final code#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
const char* ssid = "why5";// Enter your WIFI SSID
const char* password = "Shreyas@2002"; // Enter your WIFI Password


#define BOTtoken "6066222918:AAGNwIUOsB6UpWI80jdgjRgiroTC2OibaxA" // Enter the bottoken you got from botfather
#define CHAT_ID "1305548944" // Enter your chatID you got from chatid bot


X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int const trigPin = 4;
int const echoPin = 5;
int const buzzPin = 15;


void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  pinMode(D8, OUTPUT);
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");      
  client.setTrustAnchors(&cert);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int a = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    a++;
  }
  
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT);
pinMode(buzzPin, OUTPUT);


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Wifi Connected!", "");
  bot.sendMessage(CHAT_ID, "System has Started!!", "");
}
void loop() {
  int duration, distance;
digitalWrite(trigPin, HIGH);
delay(1);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
if (distance <= 10 && distance >= 0) {
digitalWrite(buzzPin, HIGH);

bot.sendMessage(CHAT_ID, "ALERT! MOTION DETECTED!!", "");

} else {
digitalWrite(buzzPin, LOW);

}
  
 
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "8284210207" ) {
      Serial.println("Access Granted!");
     
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
      bot.sendMessage(CHAT_ID, "ACCESS GRANTED!", "");
      delay(30000);
       

    
  }
    else { 
      Serial.println("Access Denied!");
      digitalWrite(D8, HIGH);
      delay(2000);
      digitalWrite(D8, LOW);
      bot.sendMessage(CHAT_ID, "ACCESS DENIED!   INTRUDER ALERT!!!!", "");
    }
  }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  
 


}