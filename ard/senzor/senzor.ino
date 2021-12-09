#define ECHO 6
#define TRIGGER 7
#define TRIGGER_pulse 1
#define motor 11
int DURATION;
float DISTANCE;
float DIST;
float LED;
int num=1;

void setup() {
// set up the LCD's number of columns and rows:
pinMode(TRIGGER, OUTPUT);
pinMode(ECHO, INPUT);
pinMode(motor, OUTPUT);
Serial.begin(9600);

//lcd.setCursor(0,1);
//lcd.print(" :");
}

void loop() {
digitalWrite(TRIGGER, HIGH);
delay(TRIGGER_pulse);
digitalWrite(TRIGGER, LOW);
//see how long it takes to receive the response pulse
DURATION = pulseIn(ECHO,HIGH);
//Distance in centimeters
DISTANCE = 0.01716*DURATION;
//LED=((100-DISTANCE)/100)255;
DIST = 1.25*DISTANCE;
Serial.println(DISTANCE);

if(DIST <= 40){
digitalWrite(11,LOW);

}

if(DIST >=100){
digitalWrite(11,HIGH);

}

}
