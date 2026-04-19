#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
#include <DHT.h>
#include <Servo.h>

#define FAN_PIN       2       
#define HEATER_PIN    8       
#define DHTPIN        13
#define DHTTYPE       DHT11
const float TEMP_HOT_LIMIT  = 30.0; 
const float TEMP_COLD_LIMIT = 20.0; 

const int RAIN_PIN     = 11;   
const int LDR_PIN      = A1;   
const int LED_PIN      = 3;    
const int SERVO1_PIN   = 4;    
const int SERVO2_PIN   = 5;   

const int SOIL_PIN     = 9;    
const int PUMP_PIN     = 10;   
const int SOIL_DRY     = HIGH; // Mức 1 là khô (tuỳ cảm biến)

const int TH_DIM       = 400;  
const int TH_SUNNY     = 700;  
const int RAIN_DETECTED = LOW; 

const int ANGLE_CLOSE  = 0;
const int ANGLE_OPEN   = 180; 
const int SERVO_SPEED  = 15;   

LiquidCrystal_I2C lcd(0x27, 20, 4); 

DHT dht(DHTPIN, DHTTYPE);
Servo s1, s2;

int targetAngle  = ANGLE_CLOSE; 
int currentAngle = ANGLE_CLOSE; 
bool isRaining   = false;
String curtainStatus = "INIT"; 

bool isPumpOn = false;
String soilStatus = "---";

unsigned long lastServoMoveTime = 0;
unsigned long lastDHTRead       = 0;

float t = 0;
float h = 0;
bool isFanOn = false;
bool isHeaterOn = false; 

byte degreeSymbol[8] = {
  0b00111, 0b00101, 0b00111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000
};

void handleServoMovement() {
    if (millis() - lastServoMoveTime >= SERVO_SPEED) {
        if (currentAngle < targetAngle) {
            currentAngle++;
            s1.write(currentAngle); s2.write(currentAngle);
        } 
        else if (currentAngle > targetAngle) {
            currentAngle--;
            s1.write(currentAngle); s2.write(currentAngle);
        }
        lastServoMoveTime = millis();
    }
}

void runTempLogic() {
    if (millis() - lastDHTRead >= 2000) {
        lastDHTRead = millis();
        float newT = dht.readTemperature();
        float newH = dht.readHumidity();
        if (!isnan(newT)) {
            t = newT; h = newH;
            if (t > TEMP_HOT_LIMIT) {
                digitalWrite(FAN_PIN, HIGH); digitalWrite(HEATER_PIN, LOW);
                isFanOn = true; isHeaterOn = false;
            } else if (t < TEMP_COLD_LIMIT) {
                digitalWrite(FAN_PIN, LOW); digitalWrite(HEATER_PIN, HIGH);
                isFanOn = false; isHeaterOn = true;
            } else {
                digitalWrite(FAN_PIN, LOW); digitalWrite(HEATER_PIN, LOW);
                isFanOn = false; isHeaterOn = false;
            }
        }
    }
}

void runCurtainLogic() {
    int rainStatus = digitalRead(RAIN_PIN);
    int ldrVal     = analogRead(LDR_PIN);

    if (rainStatus == RAIN_DETECTED) {
        isRaining = true;
        targetAngle = ANGLE_CLOSE;   
        digitalWrite(LED_PIN, HIGH); 
        curtainStatus = "MUA!";
    } else {
        isRaining = false;
        digitalWrite(LED_PIN, LOW); 
        if (ldrVal > TH_SUNNY) {
            targetAngle = ANGLE_OPEN; curtainStatus = "NANG";
        } else if (ldrVal < TH_DIM) {
            targetAngle = ANGLE_CLOSE; curtainStatus = "TOI ";
        } else {
            curtainStatus = "GIU ";
        }
    }
}

void runIrrigationLogic() {
    int sensorVal = digitalRead(SOIL_PIN); 

    if (sensorVal == SOIL_DRY) {
        digitalWrite(PUMP_PIN, HIGH);
        isPumpOn = true;
        soilStatus = "KHO (Bom ON)";
    } else {
        digitalWrite(PUMP_PIN, LOW);
        isPumpOn = false;
        soilStatus = "AM  (Bom OF)";
    }
}

void updateLCD() {
    lcd.setCursor(0, 0);
    lcd.print("T:"); lcd.print((int)t); lcd.write(0); 
    lcd.print(" H:"); lcd.print((int)h); lcd.print("%");
    lcd.print(" L:"); 
    lcd.print(analogRead(LDR_PIN));
    lcd.print("   "); 
    
    lcd.setCursor(0, 1);
    lcd.print("Fan:"); lcd.print(isFanOn ? "ON " : "OFF");
    lcd.print("  Heat:"); lcd.print(isHeaterOn ? "ON " : "OFF");

    lcd.setCursor(0, 2);
    lcd.print("Dat:"); lcd.print(soilStatus); // In ra: Dat:KHO (Bom ON)

    lcd.setCursor(0, 3);
    if (isRaining) {
        lcd.print("!MUA TO! DONG REM");
    } else {
        lcd.print("Rem:"); lcd.print(currentAngle > 10 ? "MO " : "DG ");
        lcd.print("("); lcd.print(curtainStatus); lcd.print(")");
    }
}

void setup() {
    Serial.begin(9600);

    pinMode(FAN_PIN, OUTPUT); digitalWrite(FAN_PIN, LOW);
    pinMode(HEATER_PIN, OUTPUT); digitalWrite(HEATER_PIN, LOW);
    pinMode(PUMP_PIN, OUTPUT); digitalWrite(PUMP_PIN, LOW);
    pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, LOW);
    
    pinMode(RAIN_PIN, INPUT); 
    pinMode(SOIL_PIN, INPUT);

    lcd.init(); 
    lcd.backlight(); 
    lcd.createChar(0, degreeSymbol);
    
    dht.begin();
    s1.attach(SERVO1_PIN);
    s2.attach(SERVO2_PIN);
    s1.write(ANGLE_CLOSE); s2.write(ANGLE_CLOSE);
    
    lcd.setCursor(2, 0); lcd.print("HE THONG NHA KINH");
    lcd.setCursor(4, 1); lcd.print("THONG MINH");
    lcd.setCursor(0, 3); lcd.print("Dang khoi tao...");
    delay(2000); 
    lcd.clear();
}

void loop() {
    runTempLogic();        
    runCurtainLogic();     
    runIrrigationLogic(); 
    handleServoMovement(); 

    static unsigned long lcdTimer = 0;
    if (millis() - lcdTimer > 500) {
        updateLCD();
        lcdTimer = millis();
    }
}