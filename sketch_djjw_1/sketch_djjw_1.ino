int rxChar;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.print("This is djjw talking again ...\r\n");

}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(200);
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(800);

  if(Serial.available())
  {
    rxChar = Serial.read();
    Serial.write(rxChar);
    if(isDigit(rxChar))
    {
      for(int i = 0; i < (rxChar - '0'); i++)
      {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(200);
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
      }
    }
  }
  

}
