#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <Bounce2.h>
#include <Servo.h>

const char client_id[] = "microcontroller";     //arbitrary identification
const char client_key[] = "testebatata123";                   //token KEY from shiftr.io
const char client_secret[] = "teste123";                //token SECRET from shiftr.io

const char ssid[] = "Esgoto";     //name of the used Wi-Fi network
const char pass[] = "ratorato";     //password of the Wi-Fi network

const int PLAYING_GOOD = 1;
const int PLAYING_BAD = 2;
const int NOTHING_PLAYING = 0;
const char CLIENT_VIDEO_PLAY[] = "/startVideo";
const char CLIENT_VIDEO_STATUS[] = "/videoStatus";

int estadoAtual = NOTHING_PLAYING;

WiFiClient net;
MQTTClient client;
const int QoS = 1;

// PushButton
// https://github.com/efduarte/pincello/blob/master/sensor-push-button.md
const int button_pin = D0;
Bounce button_debouncer = Bounce();

// Ultrasonic Distance
// https://github.com/efduarte/pincello/blob/master/sensor-ultrasonic-distance-hc-sr04.md
const int trigger_pin = D4;
const int echo_pin = D5;
int distance = 0;

// LEDs
// https://github.com/efduarte/pincello/blob/master/actuator-led.md
// const int led_alerta = D0;
// const int led_poste = D0;
// const int led_predio = D0;
// const int led_reator = D0;
// RGB LED

const int led_poste = D8;
const int led_vermelho = D1;
const int led_verde = D2;


// Servo Motor
// Controla com myServo.write(angulo);
// Angulo de 0 a 180 graus int
const int tempo_1 = 160;
const int tempo_2 = 130;
const int tempo_3 = 10;

const int servo_motor = D3;

Servo myServo;

// Buzzer
// https://github.com/efduarte/pincello/blob/master/actuator-buzzer.md
// const int buzzer_pin = D0;
// int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440}; //Frequencies in Hertz

void setColor(int r, int g, int b)
{
    r = map(r, 0, 255, 0, 1023);
    g = map(g, 0, 255, 0, 1023);
    //b = map(b, 0, 255, 0, 1023);
    analogWrite(led_vermelho, r);
    analogWrite(led_verde, g);
    //analogWrite(blue_pin, b);
}

void setup()
{
    Serial.begin(115200);

    pinMode(led_poste, OUTPUT);
    digitalWrite(led_poste, HIGH);

    myServo.attach(servo_motor);
    myServo.write(160);

    pinMode(button_pin, INPUT);
    button_debouncer.attach(button_pin);
    button_debouncer.interval(5);

    pinMode(trigger_pin, OUTPUT);
    digitalWrite(trigger_pin, LOW);
    pinMode(echo_pin, INPUT);

    pinMode(led_vermelho, OUTPUT);
    pinMode(led_verde, OUTPUT);

    connectWIFI();
    client.begin("broker.shiftr.io", net);
    client.onMessage(messageReceived);
    connectMQTT();

    client.subscribe(CLIENT_VIDEO_PLAY);
    estadoAtual = NOTHING_PLAYING;
}

void changeLedStatus(int status, int newStatus){
    if(status != newStatus){
        if(newStatus == PLAYING_GOOD){
            Serial.println("Tocando Bom");
            myServo.write(tempo_2);
            setColor(0,255,0);
            //digitalWrite(led_poste, HIGH);
        } else {
            Serial.println("Tocando Ruim");
            myServo.write(tempo_3);
            setColor(255,0,0);
            //digitalWrite(led_poste, LOW);
        }
        Serial.println("Nenhum fi");
        estadoAtual = newStatus;
    }
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
        // Apertaram Botao - Video Bom
        Serial.println("Button pressed!");
        client.publish(CLIENT_VIDEO_PLAY, "1", false, QoS);
        myServo.write(tempo_2);
        digitalWrite(led_poste, HIGH);
        //changeLedStatus(estadoAtual, 1);
    }

    digitalWrite(trigger_pin, HIGH); 
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);

    distance = pulseIn(echo_pin, HIGH) * 0.034 / 2;
    
    //Serial.print("Distance (cm): ");
    if(distance <= 25){
        Serial.println("Started Video");
        client.publish(CLIENT_VIDEO_PLAY, "2", false, QoS);
        myServo.write(tempo_3);
        digitalWrite(led_poste, LOW);
        //changeLedStatus(estadoAtual, 2);
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
    int msgCode;
    Serial.println("New message: " + topic + " - " + payload);

    if(topic == CLIENT_VIDEO_STATUS) 
    {
        //msgCode = payload.toInt();
        //changeLedStatus(estadoAtual, msgCode);
        //estadoAtual = msgCode;
    }
}
