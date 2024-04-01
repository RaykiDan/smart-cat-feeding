#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <Wifi.h>

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;
#define WIFI_SSID "Pesonabali"
#define WIFI_PASSWORD "TU260922"
#define API_KEY "AIzaSyDtph1iI1NOzzjmFMvUTEffg1oFrMgGCLk"
#define DATABASE_URL "https://makankucing-1419d-default-rtdb.firebaseio.com/"

//variabel buat Ultrasonik
#define echoPin 25 //atur aja sesuai pin yg mau dipake
#define trigPin 26 //atur aja sesuai pin yg mau dipake
long duration, distance;

//variabel buat PIR
#define PIR 32//atur aja
int infoGerakan;

//variabel tambahan
String dataChar;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  if (Firebase.signUp(&config, &auth, "","")){
    Serial.println("Firebase success");
    digitalWrite(LED_BUILTIN, LOW);
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
  //setup UltraSonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //setup PIR
  pinMode(PIR, INPUT);
}

void loop() {
  //Baca data Ultrasonik
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58.2;
  Serial.print("Distance: ");
  Serial.println(distance);
  //Baca data PIR
  infoGerakan = digitalRead(PIR);
  if(infoGerakan == LOW){
    Serial.println("Tidak_Terdeteksi_Gerakan");
    Serial.println("");
    dataChar="Tidak_Terdeteksi_Gerakan";
  }
  else {
    Serial.println("Terdeteksi_Gerakan"); 
    Serial.println("");
    dataChar="Terdeteksi_Gerakan";
  }
  Firebase.RTDB.setFloat(&fbdo, "Makanan", distance);
  String pindahData = dataChar;
  Firebase.RTDB.setString(&fbdo, "Kucing", pindahData);
  delay(5000);
}
