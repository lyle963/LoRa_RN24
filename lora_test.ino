#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);

void setup() {
  LoRaSetup();
}

void loop() {
  LoRaSendText("Hi Prashant, Whats up?");
  delay(10000);
}

void LoRaSetup(){
  Serial.begin(57600);
  while (!Serial) {
  }
  mySerial.begin(57600);
  delay(1200);
  LoRaInit();
}

void LoRaInit(){
  LoRaSend("radio set mod lora","ok","");
  LoRaSend("radio set freq 869525000","ok","");
  LoRaSend("radio set pwr 14","ok","");
  LoRaSend("radio set sf sf9","ok","");
  LoRaSend("radio set rxbw 125","ok","");
  LoRaSend("radio set afcbw 125","ok","");
  LoRaSend("mac reset 868","ok","");
  LoRaSend("mac set devaddr 001B849D","ok","");
  LoRaSend("mac set deveui 0004A30B001B849D","ok","");
  LoRaSend("mac set nwkskey 2B7E151628AED2A6ABF7158809CF4F3C","ok","");
  LoRaSend("mac set appskey 2B7E151628AED2A6ABF7158809CF4F3C","ok","");
  LoRaSend("mac set rx2 0 869525000","ok","");
  LoRaSend("mac set ar off","ok","");
  LoRaSend("mac save","ok","");
  LoRaSend("mac join abp","ok","accepted");
}

void LoRaSend(String text,String resp1, String resp2){
  do{
    while(mySerial.available())mySerial.read();
    mySerial.println(text);
    Serial.println(text);
    delay(15);
  } while (waitForResponse(resp1,resp2));
  Serial.println("CMD complete.");
}

int waitForResponse(String resp1,String resp2){
  unsigned long timeout = millis() + 3500;
  String in = "";
  byte resp = 1;
  while (millis()<=timeout){
    if(mySerial.available()) in += (char)mySerial.read();
    if(in.length()>0)Serial.println("\t\t"+in);
    if( (resp==1 && in.equals(resp1)) || (resp==2 && (in.equals(resp2)||resp2==""))){
      Serial.println("Response "+String(resp)+" Matches");
      if(resp==1){
        resp = 2;
        while(mySerial.available()) in += (char)mySerial.read();
        in = "";
      }else if(resp==2){
        return 0; 
      }
    }
    delay(1);
  }
  return 1;
}
void LoRaSendText(String text){
  do{
    mySerial.print("mac tx cnf 5 ");
    for(int i=0;i<text.length();i++)
      mySerial.print(text[i],HEX); 
    mySerial.println("");
    Serial.print("mac tx cnf 5 ");
    for(int i=0;i<text.length();i++)
      Serial.print(text[i],HEX); 
    Serial.println("");
    delay(15);
  } while (waitForResponse("ok","mac_tx_ok"));
  Serial.println("TX complete.");
}