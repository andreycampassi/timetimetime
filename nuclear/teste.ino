#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <Bounce2.h>

const char client_id[] = "microcontroller";     //arbitrary identification
const char client_key[] = "testebatata123";                   //token KEY from shiftr.io
const char client_secret[] = "teste123";                //token SECRET from shiftr.io

const char ssid[] = "Esgoto";     //name of the used Wi-Fi network
const char pass[] = "ratorato";     //password of the Wi-Fi network

WiFiClient net;
MQTTClient client;
const int QoS = 1;

// PushButton
// https://github.com/efduarte/pincello/blob/master/sensor-push-button.md
const int button_pin = D0;
Bounce button_debouncer = Bounce();

// Ultrasonic Distance
// https://github.com/efduarte/pincello/blob/master/sensor-ultrasonic-distance-hc-sr04.md
const int trigger_pin = D1;
const int echo_pin = D2;
int distance = 0;

// LEDs
// https://github.com/efduarte/pincello/blob/master/actuator-led.md
// const int led_alerta = D0;
// const int led_poste = D0;
// const int led_predio = D0;
// const int led_reator = D0;
// RGB LED


// Buzzer
// https://github.com/efduarte/pincello/blob/master/actuator-buzzer.md
//const int buzzer_pin = D0;
//int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440}; //Frequencies in Hertz

void setup()
{
    Serial.begin(115200);

    pinMode(button_pin, INPUT);
    button_debouncer.attach(button_pin);
    button_debouncer.interval(5);

    pinMode(trigger_pin, OUTPUT);
    digitalWrite(trigger_pin, LOW);
    pinMode(echo_pin, INPUT);

    Serial.println("Pre WiFi0");
    connectWIFI();
    Serial.println("Pos WiFi");
    client.begin("broker.shiftr.io", net);
    client.onMessage(messageReceived);
    Serial.println("Pre MQTT");
    connectMQTT();
    Serial.println("Pos MQTT");
    client.subscribe("/startVideo");
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWIFI();
    }
    client.loop();
    delay(10);  // fixes some issues with WiFi stability
    if (!client.connected())
    {
        connectMQTT();
    }


    button_debouncer.update();
    if (button_debouncer.rose() == true)
    {
        Serial.println("Button pressed!");
        client.publish("/startVideo", "1", false, QoS);
    }
    if (button_debouncer.fell() == true)
    {
        //Serial.println("Button released!");
    }

    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);

    distance = pulseIn(echo_pin, HIGH) * 0.034 / 2;
    
    //Serial.print("Distance (cm): ");
    if(distance <= 15){
        Serial.println("Started Video");
        client.publish("/startVideo", "2", false, QoS);
    }
    //Serial.println(distance);
}

void connectWIFI()
{
    Serial.print("Connecting Wi-Fi: ");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Wi-Fi connected!");
}

void connectMQTT()
{
    Serial.print("Connecting MQTT: ");
    while (!client.connect(client_id, client_key, client_secret))
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" MQTT connected!");
}

void messageReceived(String &topic, String &payload)
{
    Serial.println("New message: " + topic + " - " + payload);
}
