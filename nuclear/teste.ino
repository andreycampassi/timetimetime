#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <Bounce2.h>

const char client_id[] = "microcontroller";     //arbitrary identification
const char client_key[] = "";                   //token KEY from shiftr.io
const char client_secret[] = "";                //token SECRET from shiftr.io

const char ssid[] = "";     //name of the used Wi-Fi network
const char pass[] = "";     //password of the Wi-Fi network

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

    pinMode(vibration_pin, INPUT);
    pinMode(red_pin, OUTPUT);
    pinMode(green_pin, OUTPUT);
    pinMode(blue_pin, OUTPUT);

    connectWIFI();
    client.begin("broker.shiftr.io", net);
    client.onMessage(messageReceived);
    connectMQTT();

    client.subscribe("/red");
    client.subscribe("/green");
    client.subscribe("/blue");
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

    setColor(red, green, blue);

    if (digitalRead(vibration_pin) == HIGH)
    {
        count = 0;
        if (vibrating == false)
        {
            vibrating = true;
            client.publish("/vibration", "1", false, QoS);
            Serial.println("Published: /vibration 1");
        }
    }
    else if (digitalRead(vibration_pin) == LOW)
    {
        if(vibrating == true)
        {
            count++;
            delay(10);
            if (count > 30)
            {
                vibrating = false;
                client.publish("/vibration", "0", false, QoS);
                Serial.println("Published: /vibration 0");
            }
        }
    }

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

    if (topic == "/red")
    {
        red = payload.toInt();
    }

    if (topic == "/green")
    {
        green = payload.toInt();
    }

    if (topic == "/blue")
    {
        blue = payload.toInt();
    }
}

void setColor(int r, int g, int b)
{
    r = map(r, 0, 255, 0, 1023);
    g = map(g, 0, 255, 0, 1023);
    b = map(b, 0, 255, 0, 1023);
    analogWrite(red_pin, r);
    analogWrite(green_pin, g);
    analogWrite(blue_pin, b);
}
