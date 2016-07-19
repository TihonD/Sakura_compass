#include "Wire.h"
#include "HMC5883L.h"
 
HMC5883L compass;
 
void setup(){
  Serial.begin(19200);
  Wire.begin();

  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
   
  compass = HMC5883L();  // создаем экземпляр HMC5883L библиотеки
  setupHMC5883L();       // инициализация HMC5883L
}
 
void loop(){
  int x, y=0;
  float heading = getHeading();
  //Serial.println(heading);
  x=heading;
  Serial.print(">");
  if(x>=0) Serial.print("+");
  else
  {
    Serial.print("-");
    x=-x;
  }
  Serial.print(x/100);
  Serial.print((x/10)%10);
  Serial.print(x%10);
  Serial.println("<");

  if((heading>=345)||(heading<=15)) digitalWrite(16,LOW);
  else digitalWrite(16,HIGH);
  
  if((heading>=75)&&(heading<=105)) digitalWrite(17,LOW);
  else digitalWrite(17,HIGH);

  if((heading>=165)&&(heading<=195)) digitalWrite(18,LOW);
  else digitalWrite(18,HIGH);

  if((heading>=255)&&(heading<=285)) digitalWrite(19,LOW);
  else digitalWrite(19,HIGH);
  
  if(y>=10) digitalWrite(15,LOW);
  else digitalWrite(15,HIGH);
  y++;
  if(y==20) y=1; 
  
  delay(100);
}
 
void setupHMC5883L(){
  // инициализация HMC5883L, и проверка наличия ошибок
  int error;  
  error = compass.SetScale(0.88); // чувствительность датчика из диапазона: 0.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1
  if(error != 0) Serial.println(compass.GetErrorText(error)); // если ошибка, то выводим ее
 
  error = compass.SetMeasurementMode(Measurement_Continuous); // установка режима измерений как Continuous (продолжительный)
  if(error != 0) Serial.println(compass.GetErrorText(error)); // если ошибка, то выводим ее
}
 
float getHeading(){
  // считываем данные с HMC5883L и рассчитываем  направление
  MagnetometerScaled scaled = compass.ReadScaledAxis(); // получаем масштабированные элементы с датчика
  float heading = atan2(scaled.YAxis, scaled.XAxis);    // высчитываем направление
 
  // корректируем значения с учетом знаков
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;
 
  return heading * RAD_TO_DEG; // переводим радианы в градусы
}
