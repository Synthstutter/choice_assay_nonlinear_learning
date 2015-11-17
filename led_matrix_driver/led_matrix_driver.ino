//SERIAL COMMUNICATION VARIABLES/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ir_sensors=2; //how many ir sensors are you using?

signed int threshold[]={125,125};
const int readPin[]={A0, A1}; 
unsigned int sensVal[ir_sensors];
boolean passed[ir_sensors];
boolean previousPassed[ir_sensors];
unsigned long int calSensVal[ir_sensors];
int count[ir_sensors];
int incomingByte;


//LED MATRIX VARIABLES///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int dataIn = 12;
int load = 10;
int clock = 11;
int maxInUse = 2;    //change this variable to set how many MAX7219's you'll use
int e = 0;           // just a variable
 
// define max7219 registers
byte max7219_reg_noop        = 0x00;
byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x07;
byte max7219_reg_digit7      = 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

//LED MATRIX FUNCTIONS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 

void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask){            // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    }else{
      digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}
 
void maxSingle( byte reg, byte col) {    
//maxSingle is the "easy"  function to use for a single max7219
 
  digitalWrite(load, LOW);       // begin    
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data  
  digitalWrite(load, LOW);       // and load da stuff
  digitalWrite(load,HIGH);
}
 
void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin    
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}
 
void maxOne(byte maxNr, byte reg, byte col) {    
//maxOne is for addressing different MAX7219's,
//while having a couple of them cascaded
 
  int c = 0;
  digitalWrite(load, LOW);  // begin    
 
  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }
 
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
 
  for ( c =maxNr-1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }
 
  digitalWrite(load, LOW); // and load da stuff
  digitalWrite(load,HIGH);
}
 
void vert_stripes(int matrix){
  maxOne(matrix,1,255);                    
  maxOne(matrix,2,255);                    
  maxOne(matrix,3,0);                      
  maxOne(matrix,4,0);                      
  maxOne(matrix,5,0);                      
  maxOne(matrix,6,0);                      
  maxOne(matrix,7,255);                    
  maxOne(matrix,8,255);                    

}

void hor_stripes(int matrix){
  maxOne(matrix,1,195);       
  maxOne(matrix,2,195);       
  maxOne(matrix,3,195);         
  maxOne(matrix,4,195);         
  maxOne(matrix,5,195);         
  maxOne(matrix,6,195);         
  maxOne(matrix,7,195);       
  maxOne(matrix,8,195);       
  
}

//MAIN CODE//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup () {
  Serial.begin(9600); 
  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);
  digitalWrite(13, HIGH);  
 
//initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
   for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
}  
 
void loop () {
  if (Serial.available()>0){
  incomingByte=Serial.read();
  }

  if (incomingByte==1){    
   int i=0;
   for (i=0;i<2;i++){
     if (calSensVal[i] = NULL || calSensVal[i] > analogRead(readPin[i])){
       calSensVal[i]=analogRead(readPin[i]);};}

     String calib_concat="\nSensor 0: " + String(calSensVal[0]) +
			 "\nSensor 1: " + String(calSensVal[1]) +
			 
     Serial.println(calib_concat);
     incomingByte=false;
 }
  if (incomingByte==2){
     int i=0;
     for (i=0;i<6;i= i+1){
       sensVal[i]=analogRead(readPin[i]);    
	 if (sensVal[i] >= calSensVal[i]*threshold[i]/100 && previousPassed[i]==false){
	     previousPassed[i]=true;
	     Serial.print(i);   
	     delay(2);
	 }
	 if (abs(sensVal[i]-calSensVal[i]) <3){
	   previousPassed[i]=false;
	 }     
     }

 }
  if (incomingByte==3){
    hor_stripes(1);
    vert_stripes(2);

  }

  if (incomingByte==4){
    hor_stripes(2);
    vert_stripes(1);
  }
  

 
  delay(2000);
  incomingByte=0;
}
