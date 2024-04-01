#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <Wifi.h>
#include <Servo.h>

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;
#define WIFI_SSID "Pesonabali"
#define WIFI_PASSWORD "TU260922"
#define API_KEY "AIzaSyDtph1iI1NOzzjmFMvUTEffg1oFrMgGCLk"
#define DATABASE_URL "https://makankucing-1419d-default-rtdb.firebaseio.com/"


//variabel LED
#define ledHijau 25 //atur aja sesuai pin yg mau dipake
#define ledMerah 32 //atur aja sesuai pin yg mau dipake

//variabel buat Buzzer
int buzzer = 26; 

//variabel buat Servo
Servo myservo; //variabel untuk perhitungan sudut putar
int sudut;
static const int servoPin = 33;

//var global
float nilaiFloat=0;
String nilaiString;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  if (Firebase.signUp(&config, &auth, "","")){
    Serial.println("Firebase success");
    signupOK = true;
  }
  else{
    String firebaseErrorMessage =config.signer.signupError.message.c_str();
    Serial.printf("%s\n",firebaseErrorMessage);
  }
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  //setup servo
  myservo.attach(servoPin); // servo atur pin lokasinya
  //setup led
  pinMode(ledHijau, OUTPUT);
  pinMode(ledMerah, OUTPUT);
  //setup buzzer
  pinMode(buzzer, OUTPUT);
}

void loop() {
  Firebase.RTDB.getInt(&fbdo, "Makanan");
  if (fbdo.dataType() == "int") {
      nilaiFloat = fbdo.to<int>();
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
  Firebase.RTDB.getString(&fbdo, "Tombol");
  if (fbdo.dataType() == "string") {
      nilaiString = fbdo.to<const char *>();
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
  if(nilaiFloat <= 20){
    digitalWrite(ledHijau, HIGH);
    digitalWrite(ledMerah, LOW);
    myservo.write(90);
    digitalWrite(buzzer,LOW);
  }
  else {
    digitalWrite(ledHijau, LOW);
    digitalWrite(ledMerah, HIGH);
    myservo.write(180);
    digitalWrite(buzzer,HIGH);
  }
  delay(3500);
  if(nilaiString == "1"){
    myservo.write(180);
    delay(3500);
    Firebase.RTDB.setString(&fbdo, "Tombol", 0);
  }
  else{delay(3500);}
}
