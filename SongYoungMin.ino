#include <SoftwareSerial.h>
#define RxD 13
#define TxD 12
#define DEBUG_ENABLED 1
SoftwareSerial blueToothSerial(RxD, TxD);
int ledPin = 11;
int humidityPin = A5;
int humidityValue = 0;
unsigned char chByte = 0;

const int buttonPin1 = A4;
const int buttonPin2 = 1;

int buttonValue1;
int buttonValue2;
//----------------------------------------------------------------------
#include <avr/pgmspace.h>
#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

#define CLK 8
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
//----------------------------------------------------------------------
static const int8_t PROGMEM sinetab[256] = {
     0,   2,   5,   8,  11,  15,  18,  21,
    24,  27,  30,  33,  36,  39,  42,  45,
    48,  51,  54,  56,  59,  62,  65,  67,
    70,  72,  75,  77,  80,  82,  85,  87,
    89,  91,  93,  96,  98, 100, 101, 103,
   105, 107, 108, 110, 111, 113, 114, 116,
   117, 118, 119, 120, 121, 122, 123, 123,
   124, 125, 125, 126, 126, 126, 126, 126,
   127, 126, 126, 126, 126, 126, 125, 125,
   124, 123, 123, 122, 121, 120, 119, 118,
   117, 116, 114, 113, 111, 110, 108, 107,
   105, 103, 101, 100,  98,  96,  93,  91,
    89,  87,  85,  82,  80,  77,  75,  72,
    70,  67,  65,  62,  59,  56,  54,  51,
    48,  45,  42,  39,  36,  33,  30,  27,
    24,  21,  18,  15,  11,   8,   5,   2,
     0,  -3,  -6,  -9, -12, -16, -19, -22,
   -25, -28, -31, -34, -37, -40, -43, -46,
   -49, -52, -55, -57, -60, -63, -66, -68,
   -71, -73, -76, -78, -81, -83, -86, -88,
   -90, -92, -94, -97, -99,-101,-102,-104,
  -106,-108,-109,-111,-112,-114,-115,-117,
  -118,-119,-120,-121,-122,-123,-124,-124,
  -125,-126,-126,-127,-127,-127,-127,-127,
  -128,-127,-127,-127,-127,-127,-126,-126,
  -125,-124,-124,-123,-122,-121,-120,-119,
  -118,-117,-115,-114,-112,-111,-109,-108,
  -106,-104,-102,-101, -99, -97, -94, -92,
   -90, -88, -86, -83, -81, -78, -76, -73,
   -71, -68, -66, -63, -60, -57, -55, -52,
   -49, -46, -43, -40, -37, -34, -31, -28,
   -25, -22, -19, -16, -12,  -9,  -6,  -3
};

const float radius1  = 65.2, radius2  = 92.0, radius3  = 163.2, radius4  = 176.8,
            centerx1 = 64.4, centerx2 = 46.4, centerx3 =  93.6, centerx4 =  16.4, 
            centery1 = 34.8, centery2 = 26.0, centery3 =  56.0, centery4 = -11.6;
float       angle1   =  0.0, angle2   =  0.0, angle3   =   0.0, angle4   =   0.0;
long        hueShift =  0;
//----------------------------------------------------------------------
void setup() {  
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  
  pinMode(buttonPin1, INPUT);  
  pinMode(buttonPin2, INPUT);
  setupBlueToothConnection(); 
//----------------------------------------------------------------------
  matrix.begin();
  start();
//---------------------------------------------------------------------- 
}

void loop() {
  char recvChar;
  while(1){        
    
    //int i = humidityValue;
    //blueToothSerial.print(i);
    //2014.11.07
//----------------------------------------------------------------------    
//----------------------------------------------------------------------    
    int buttonState1 = digitalRead(buttonPin1);
    int buttonState2 = digitalRead(buttonPin2);
    //Serial.println(buttonState1);
    //Serial.println(buttonState2);    
    
    if(buttonValue1 == 0 && buttonState1 == 1){
      analogWrite(ledPin, 64);
      delay(500);
      buttonValue1 = 1;
    }else if(buttonValue1 == 1 && buttonState1 == 1){
      analogWrite(ledPin, 128);
      delay(500);
      buttonValue1 = 2;
    }else if(buttonValue1 == 2 && buttonState1 == 1){
      analogWrite(ledPin, 192);
      delay(500);
      buttonValue1 = 3;
    }else if(buttonValue1 == 3 && buttonState1 == 1){
      analogWrite(ledPin, 255);
      delay(500);
      buttonValue1 = 4;
    }else if(buttonValue1 == 4 && buttonState1 == 1){
      analogWrite(ledPin, 0);
      delay(500);
      buttonValue1 = 0;
    }
    
    if(buttonValue2 == 0 && buttonState2 == 0){
      delay(500);
      buttonValue2 = 1;
    }else if(buttonValue2 == 1 && buttonState2 == 0){
      delay(500);
      buttonValue2 = 0;
    }
    
//----------------------------------------------------------------------   
//----------------------------------------------------------------------    
    
    humidityValue = analogRead(humidityPin);
    //delay(1000);
    //Serial.println(humidityValue);
    
    //blueToothSerial.print(humidityValue);
    //delay(100);
    
    if(buttonValue2 == 0 && humidityValue > 470){
      one();
      blueToothSerial.print('a');
    }else if(buttonValue2 == 0 && humidityValue <= 470 && humidityValue > 450){
      one();
      blueToothSerial.print('b');
    }else if(buttonValue2 == 0 && humidityValue <= 450 && humidityValue > 430){
      two();
      blueToothSerial.print('c');
    }else if(buttonValue2 == 0 && humidityValue <= 430 && humidityValue > 410){
      two();
      blueToothSerial.print('d');
    }else if(buttonValue2 == 0 && humidityValue <= 410 && humidityValue > 380){
      two(); 
      blueToothSerial.print('e');
    }else if(buttonValue2 == 0 && humidityValue <= 380 && humidityValue > 350){
      two();
      blueToothSerial.print('f');
    }else if(buttonValue2 == 0 && humidityValue <= 350 && humidityValue > 320){
      two();
      blueToothSerial.print('g'); 
    }else if(buttonValue2 == 0 && humidityValue <= 320 && humidityValue > 290){
      two(); 
      blueToothSerial.print('h');
    }else if(buttonValue2 == 0 && humidityValue <= 290 && humidityValue > 260){
      two();
      blueToothSerial.print('i');
    }else if(buttonValue2 == 0 && humidityValue <= 260 && humidityValue > 230){
      two();
      blueToothSerial.print('j');  
    }else if(buttonValue2 == 0 && humidityValue <= 230 && humidityValue > 200){
      three(); 
      blueToothSerial.print('k');
    }else if(buttonValue2 == 0 && humidityValue <= 200){
      three();
      blueToothSerial.print('l');
    }else if(buttonValue2 == 1 && humidityValue < 1000){
      back();  
    }                      
    
    if(blueToothSerial.available()){
    chByte = blueToothSerial.read();
    //Serial.print( chByte,DEC);    
    //Serial.print(",");
    
    if(chByte==101){ 
      if(blueToothSerial.available()){
        chByte = blueToothSerial.read();
        //Serial.print( chByte,DEC);    
        //Serial.println("");
        analogWrite(ledPin, chByte*25);
      }
    }       
    
    }
    if(Serial.available()){
      recvChar = Serial.read();
      blueToothSerial.print(recvChar);
    }
  }
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

void setupBlueToothConnection()
{
  blueToothSerial.begin(9600);
  blueToothSerial.print("\r\n+STWMOD=0\r\n");
  blueToothSerial.print("\r\n+STNA=TEST\r\n");
  blueToothSerial.print("\r\n+STOAUT=1\r\n");
  blueToothSerial.print("\r\n+STAUTO=0\r\n");
  delay(2000);
  blueToothSerial.print("\r\n+INQ=1\r\n");
  Serial.println("The slave bluetooth is inquirable!");
  delay(2000);
  blueToothSerial.flush();
}
//----------------------------------------------------------------------

void start(){
  matrix.setCursor(4, 4);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('S');
  matrix.setTextColor(matrix.Color333(7,7,7)); 
  matrix.print('O');
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('N');
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('G');
  delay(1500);
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(1, 4);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('Y');
  matrix.setTextColor(matrix.Color333(7,7,7)); 
  matrix.print('O');
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('U');
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('N');
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('G');
  delay(1500);
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(7, 4);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('M');
  matrix.setTextColor(matrix.Color333(7,7,7)); 
  matrix.print('I');
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('N');
  delay(1500);
  matrix.fillScreen(matrix.Color333(0, 0, 0));
    
  for(int i=1; i<8; i++){
  matrix.drawPixel(6, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(7, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(9, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(11, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(12, 7, matrix.Color333(7, 0, i));  

  matrix.drawPixel(15, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 11, matrix.Color333(7, 0, i));
  
  matrix.drawPixel(19, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(20, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(22, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(24, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(25, 6, matrix.Color333(7, 0, i));
  delay(300);
  } 
  matrix.fillScreen(matrix.Color333(0, 0, 0));
}

void back(){
  int           x1, x2, x3, x4, y1, y2, y3, y4, sx1, sx2, sx3, sx4;
  unsigned char x, y;
  long          value;
  sx1 = (int)(cos(angle1) * radius1 + centerx1);
  sx2 = (int)(cos(angle2) * radius2 + centerx2);
  sx3 = (int)(cos(angle3) * radius3 + centerx3);
  sx4 = (int)(cos(angle4) * radius4 + centerx4);
  y1  = (int)(sin(angle1) * radius1 + centery1);
  y2  = (int)(sin(angle2) * radius2 + centery2);
  y3  = (int)(sin(angle3) * radius3 + centery3);
  y4  = (int)(sin(angle4) * radius4 + centery4);
  for(y=0; y<matrix.height(); y++) {
    x1 = sx1; x2 = sx2; x3 = sx3; x4 = sx4;
    for(x=0; x<matrix.width(); x++) {
      value = hueShift
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x1 * x1 + y1 * y1) >> 4))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x2 * x2 + y2 * y2) >> 4))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x3 * x3 + y3 * y3) >> 5))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x4 * x4 + y4 * y4) >> 5));
      matrix.drawPixel(x, y, matrix.ColorHSV(value * 3, 255, 255, true));
      x1--; x2--; x3--; x4--;
    }
    y1--; y2--; y3--; y4--;
  }
  angle1 += 0.03;
  angle2 -= 0.07;
  angle3 += 0.13;
  angle4 -= 0.15;
  hueShift += 2;
  matrix.swapBuffers(false);
}

void one(){
  // ^o^  
  for(int i=1; i<8; i++){
  matrix.drawPixel(6, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(7, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(9, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(11, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(12, 7, matrix.Color333(7, 0, i));  

  matrix.drawPixel(13, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(13, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(14, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(14, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(17, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(17, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(18, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(18, 10, matrix.Color333(7, 0, i));  
  
  matrix.drawPixel(19, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(20, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(22, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(24, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(25, 7, matrix.Color333(7, 0, i));
  delay(100);
  }
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  for(int i=7; i>0; i--){
  matrix.drawPixel(6, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(7, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(9, 3, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(11, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(12, 6, matrix.Color333(7, 0, i));  

  matrix.drawPixel(13, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(13, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(14, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(14, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(17, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(17, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(18, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(18, 9, matrix.Color333(7, 0, i));  
  
  matrix.drawPixel(19, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(20, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(22, 3, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(24, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(25, 6, matrix.Color333(7, 0, i));
  delay(100);
  }
  matrix.fillScreen(matrix.Color333(0, 0, 0));
}

void two(){
 //^_^
  for(int i=1; i<8; i++){
  matrix.drawPixel(6, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(7, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(9, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(11, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(12, 7, matrix.Color333(7, 0, i));  

  matrix.drawPixel(12, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(13, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(14, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(17, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(18, 11, matrix.Color333(7, 0, i));
  matrix.drawPixel(19, 11, matrix.Color333(7, 0, i));
  
  matrix.drawPixel(19, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(20, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(22, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(24, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(25, 7, matrix.Color333(7, 0, i));
  delay(100);
  }
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  for(int i=7; i>0; i--){
  matrix.drawPixel(6, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(7, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(9, 3, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(11, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(12, 6, matrix.Color333(7, 0, i));  

  matrix.drawPixel(12, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(13, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(14, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(17, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(18, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(19, 10, matrix.Color333(7, 0, i));
  
  matrix.drawPixel(19, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(20, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(22, 3, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 4, matrix.Color333(7, 0, i));
  matrix.drawPixel(24, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(25, 6, matrix.Color333(7, 0, i));
  delay(100);
  }
  matrix.fillScreen(matrix.Color333(0, 0, 0));
}

void three(){
  //T_T
  for(int i=1; i<8; i++){
  matrix.drawPixel(6, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(7, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(9, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 9, matrix.Color333(7, 0, i)); 
  matrix.drawPixel(10, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(11, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(12, 6, matrix.Color333(7, 0, i));

  matrix.drawPixel(13, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(14, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(17, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(18, 10, matrix.Color333(7, 0, i));
  
  matrix.drawPixel(19, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(20, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(22, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 9, matrix.Color333(7, 0, i)); 
  matrix.drawPixel(23, 10, matrix.Color333(7, 0, i));
  matrix.drawPixel(24, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(25, 6, matrix.Color333(7, 0, i));
  delay(100);
  }
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  for(int i=7; i>0; i--){
  matrix.drawPixel(6, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(7, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(8, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(9, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(10, 8, matrix.Color333(7, 0, i)); 
  matrix.drawPixel(10, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(11, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(12, 5, matrix.Color333(7, 0, i));

  matrix.drawPixel(13, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(14, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(15, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(16, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(17, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(18, 9, matrix.Color333(7, 0, i));
  
  matrix.drawPixel(19, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(20, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 8, matrix.Color333(7, 0, i));
  matrix.drawPixel(21, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(22, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 6, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 7, matrix.Color333(7, 0, i));
  matrix.drawPixel(23, 8, matrix.Color333(7, 0, i)); 
  matrix.drawPixel(23, 9, matrix.Color333(7, 0, i));
  matrix.drawPixel(24, 5, matrix.Color333(7, 0, i));
  matrix.drawPixel(25, 5, matrix.Color333(7, 0, i));
  delay(100);
  }
  matrix.fillScreen(matrix.Color333(0, 0, 0));
}
//----------------------------------------------------------------------
