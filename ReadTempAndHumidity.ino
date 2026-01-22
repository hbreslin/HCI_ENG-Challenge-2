// code adapted from this DHT library example 
// https://github.com/adafruit/DHT-sensor-library/blob/master/examples/DHTtester/DHTtester.ino

// sensor info
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#pragma region variables

float ideal_humidity = 45;
float ideal_humidity_range = 5;
float bad_humidity_range = 10;

float ideal_temperature = 68;
float ideal_temperature_range = 3;
float bad_temperature_range = 5;

int redLightOutput = D5;
int yellowLightOutput = D4;
int greenLightOutput = D3;

enum roomCondition {
    GOOD,
    MED,
    BAD
};

#pragma endregion

roomCondition check_condition(float temperature, float humidity) {
    //given room temperature and condition return a roomCondition Enum value
    // GOOD, MED, or BAD reflecting the difference between the current
    // room state and the desired room state
    float humidity_diff = abs(ideal_humidity - humidity);
    float temperature_diff = abs(ideal_temperature - temperature);

    // far range
    if (humidity_diff > bad_humidity_range ||
        temperature_diff > bad_temperature_range) {
        return BAD;
    }
    //close range
    else if (humidity_diff > ideal_humidity_range ||
             temperature_diff > ideal_temperature_range) {
        return MED;
    }
    //good range
    else {
        return GOOD;
    }
}

void setLights(roomCondition currentCondition) {
    // set the lights based on the current condition 
    // (red for bad, yellow is med, green is good!)
    switch (currentCondition) {
        case GOOD:
            //green light on, others off 
            digitalWrite(greenLightOutput, HIGH);
            digitalWrite(yellowLightOutput, LOW);
            digitalWrite(redLightOutput, LOW);
            Serial.println("Condition: GOOD");
            break;

        case MED:
            //yellow light on, others off 
            digitalWrite(greenLightOutput, LOW);
            digitalWrite(yellowLightOutput, HIGH);
            digitalWrite(redLightOutput, LOW);
            Serial.println("Condition: MED");
            break;

        case BAD:
            //red light on, others off 
            digitalWrite(greenLightOutput, LOW);
            digitalWrite(yellowLightOutput, LOW);
            digitalWrite(redLightOutput, HIGH);
            Serial.println("Condition: BAD");
            break;
    }
}


// somewhere to store the incoming message
String incoming = "";

void setup() {
    // delay(2000);
    // setting revelvent pinmodes
    pinMode(redLightOutput, OUTPUT);
    pinMode(yellowLightOutput, OUTPUT);
    pinMode(greenLightOutput, OUTPUT);
    // pinMode(D2, INPUT);

    Serial.begin(9600);
    dht.begin();
    delay(1000); //give the sensor a few seconds
}

void loop() {
    delay(2000); 
    //the library advises using a delay + leaving a few seconds between measurements

    if (Serial.available() > 0) {
        //read the incoming message
        incoming = Serial.readStringUntil('\n');
        incoming.trim();

        if (incoming.startsWith("T:")) { // temperature message
            ideal_temperature = incoming.substring(2).toInt();
        }
        else if (incoming.startsWith("H:")) { // humidity message
            ideal_humidity = incoming.substring(2).toInt();
        }
    }

    float temperature = dht.readTemperature(true); //true to read in as farenheit
    float humidity = dht.readHumidity();

    Serial.print(temperature);
    Serial.print(" ");
    Serial.print(humidity);
    Serial.print(" ");
    Serial.print(ideal_temperature);
    Serial.print(" ");
    Serial.println(ideal_humidity);

    roomCondition currentCondition = check_condition(temperature, humidity);
    setLights(currentCondition);        

}
