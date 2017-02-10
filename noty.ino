#include <Adafruit_NeoPixel.h>

AncsNotification notifications[8];

bool callHappened = false;
bool mailAvailable = false;
bool WAAvailable = false;

int delayval = 1000;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 1, NEO_GRB + NEO_KHZ800);


void setup() {
  // Serial port is initialized automatically; we don't have to do anything
  BeanAncs.enable();
  pixels.begin();
  }

void loop() {
  Bean.setLed(0, 0, 0);  
  int msgAvail = BeanAncs.notificationsAvailable();
 
  if (callHappened) {
    incomingCall();
    callHappened = false;
  }

  if (mailAvailable) {
    mailAlert();
    mailAvailable = false;
  }

  if (WAAvailable) {
    WAAlert();
    WAAvailable = false;
  }

  if (msgAvail) {
    //Bean.setLedRed(1);
    BeanAncs.getNotificationHeaders(notifications, 8);

    for (int i = 0; i < msgAvail; i++) {
      Serial.print("cat:");
      Serial.println(notifications[i].catID);
      Serial.print("msgAvail:");
      Serial.println(msgAvail);
      Serial.print("flg:");
      Serial.println(notifications[i].flags);
      Serial.print("evt:");
      Serial.println(notifications[i].eventID);
      Serial.print("cnt:");
      Serial.println(notifications[i].catCount);
      Serial.print("id: ");
      Serial.println(notifications[i].notiUID);

      if (notifications[i].catID == 1 || notifications[i].catID == 2) {
        callHappened = true;
      }

      char data[64] = {0};
      int len = BeanAncs.getNotificationAttributes(NOTI_ATTR_ID_MESSAGE,notifications[i].notiUID,39,(uint8_t *)data,5000);
     
      if (len == 22) {
        WAAvailable = true;
      }
      
      if (len == 39) {
        mailAvailable = true;
      }

      Serial.print("l: ");
      Serial.print(len);
      Serial.print(" ");
      for (int j = 0; j < len; j++) {
        Serial.print(data[j]);
      }
      Serial.println();
      delay(1000);  
    }
  }
  if (!msgAvail) {
  Bean.setLed(100, 0, 0);
  delay(500);
  Bean.setLed(0, 100, 0);
  delay(500);
  Bean.setLed(0, 0, 100);
  delay(500);
  Bean.setLed(0, 0, 0);

  pixels.setPixelColor(0, pixels.Color(150,0,0));
  pixels.show();
  delay(delayval);
  pixels.setPixelColor(0, pixels.Color(0,150,0));
  pixels.show();
  delay(delayval);
  pixels.setPixelColor(0, pixels.Color(0,0,150));
  pixels.show();
  delay(delayval);
  pixels.setPixelColor(0, pixels.Color(0,0,0)); 
  pixels.show();
  
  Bean.sleep(10000);
  
  }
}

void incomingCall() {
  for (int j = 0; j < 200; j++) {
  pixels.setPixelColor(0, pixels.Color(150,0,0));
  pixels.show();
  delay(500);
  pixels.setPixelColor(0, pixels.Color(0,0,150));
  pixels.show();
  delay(500);
  }
}

void mailAlert() {
  pixels.setPixelColor(0, pixels.Color(0,0,150));
  pixels.show();
  Bean.sleep(0xFFFFFFFF);
}

void WAAlert() {
  pixels.setPixelColor(0, pixels.Color(0,150,0));
  pixels.show();
  Bean.sleep(0xFFFFFFFF);
}
