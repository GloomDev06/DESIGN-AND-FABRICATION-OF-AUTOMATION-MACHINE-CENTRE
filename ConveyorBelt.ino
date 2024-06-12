#include <AccelStepper.h>

bool sen1_flag = false;
bool sen2_flag = false;
bool sen3_flag = false;
bool mill_flag = false;
bool laser_flag = false;

const int sensor1 = 31;
const int sensor2 = 33;
const int sensor3 = 34;

int mot2_rotValue = 6200;
int mill_forw_pin = 32;

long int mill_time = 10000; // milling operation time in ms
long int laser_time = 3000; // laser operation time in ms
int pump_pin = 35;

const int step1 = 2;
const int step2 = 3;
const int step3 = 4;

const int dir1 = 5;
const int dir2 = 6;
const int dir3 = 7;

const int ms1_1 = 22;
const int ms2_1 = 23;
const int ms3_1 = 24;

const int ms1_2 = 25;
const int ms2_2 = 26;
const int ms3_2 = 27;

const int ms1_3 = 28;
const int ms2_3 = 29;
const int ms3_3 = 30;

AccelStepper stepper1(AccelStepper::DRIVER, step1, dir1);
AccelStepper stepper2(AccelStepper::DRIVER, step2, dir2);
AccelStepper stepper3(AccelStepper::DRIVER, step3, dir3);

void setup() {
    pinMode(step1, OUTPUT);
    pinMode(step2, OUTPUT);
    pinMode(step3, OUTPUT);
    pinMode(dir1, OUTPUT);
    pinMode(dir2, OUTPUT);
    pinMode(dir3, OUTPUT);
    pinMode(ms1_1, OUTPUT);
    pinMode(ms2_1, OUTPUT);
    pinMode(ms3_1, OUTPUT);
    pinMode(ms1_2, OUTPUT);
    pinMode(ms2_2, OUTPUT);
    pinMode(ms3_2, OUTPUT);
    pinMode(ms1_3, OUTPUT);
    pinMode(ms2_3, OUTPUT);
    pinMode(ms3_3, OUTPUT);
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    pinMode(sensor3, INPUT);
    pinMode(mill_forw_pin, OUTPUT);
    pinMode(pump_pin, OUTPUT);

    digitalWrite(ms1_1, HIGH);
    digitalWrite(ms2_1, HIGH);
    digitalWrite(ms3_1, HIGH);
    digitalWrite(ms1_2, HIGH);
    digitalWrite(ms2_2, HIGH);
    digitalWrite(ms3_2, HIGH);
    digitalWrite(ms1_3, HIGH);
    digitalWrite(ms2_3, HIGH);
    digitalWrite(ms3_3, HIGH);

    stepper1.setMaxSpeed(100000);
    stepper1.setAcceleration(100000);
    stepper1.setSpeed(100000);

    stepper2.setMaxSpeed(100000);
    stepper2.setAcceleration(100000);
    stepper2.setSpeed(100000);

    stepper3.setMaxSpeed(100000);
    stepper3.setAcceleration(100000);
    stepper3.setSpeed(100000);

    Serial.begin(9600);
}

void loop() {
    stepper1.move(6400);
    stepper1.runToPosition();

    while (stepper1.distanceToGo() != 1) {
        Serial.println("entered loop");
        if (digitalRead(sensor1) == LOW) {
            Serial.println("sensor detected");
            sen1_flag = true;
            break;
        }
    }

    if (stepper1.distanceToGo() == 0 && sen1_flag == true) {
        Serial.println("motor 2");

        delay(1000);
        stepper2.move(mot2_rotValue);
        stepper2.runToPosition();
        delay(500);
        digitalWrite(mill_forw_pin, HIGH);
        delay(mill_time);
        digitalWrite(mill_forw_pin, LOW);
        delay(500);
        stepper2.move(-mot2_rotValue);
        stepper2.runToPosition();
        delay(500);
        mill_flag = true;
        sen1_flag = false;
    }

    delay(1000);

    if (mill_flag == true) {
        mill_flag = false;
        stepper1.move(6400);
        stepper1.runToPosition();
        while (stepper1.distanceToGo() != 1) {
            Serial.println("entered 2 loop");
            if (digitalRead(sensor2) == LOW) {
                Serial.println("2nd sensor detected");
                sen2_flag = true;
                delay(10);
                break;
            }
        }
    }

    if (stepper1.distanceToGo() == 0 && sen2_flag == true) {
        delay(1000);
        stepper2.move(mot2_rotValue);
        stepper2.runToPosition();
        delay(laser_time);
        stepper2.move(-mot2_rotValue);
        stepper2.runToPosition();
        delay(500);
        laser_flag = true;
        sen2_flag = false;
    }

    delay(1000);

    if (laser_flag == true) {
        laser_flag = false;
        stepper1.move(6400);
        stepper1.runToPosition();
        while (stepper1.distanceToGo() != 1) {
            if (digitalRead(sensor3) == LOW) {
                sen3_flag = true;
                delay(10);
                break;
            }
        }
    }

    if (sen3_flag == true) {
        delay(1000);
        for (int i = 0; i <= 5; i++) { // stepper 3 revolutions
            stepper3.move(3200);
            stepper3.runToPosition();
            digitalWrite(pump_pin, HIGH);
            delay(100);
            stepper3.move(-3200);
            stepper3.runToPosition();
            delay(100);
        }
        digitalWrite(pump_pin, LOW);
        sen3_flag = false;
    }
}
