/*

Wysylanie danej po serialu zeby sprawdzic czy BLE wysyla ja.

*/


void setup()  
{
  Serial.begin(57600);
}

void loop() // run over and over
{
  for(int i = 0; i < 100; i++){
    Serial.print(i); Serial.print("\t"); Serial.print("A\t"); Serial.print("B\t"); Serial.print("C\t"); Serial.print("\n");
    delay(1000);
  }

}
