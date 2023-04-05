#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Replace with your network credentials
const char *ssid = "your_SSID";
const char *password = "your_PASSWORD";

// Create a web server on port 80
ESP8266WebServer server(80);

// Initialize the DHT sensor
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Initialize the motion sensor
#define MOTIONPIN D2
volatile int motionDetected = 0;

void IRAM_ATTR handleMotionInterrupt()
{
    motionDetected = 1;
}

// Initialize the button
#define BUTTONPIN D3
int alarmEnabled = 0;

// Initialize the photoresistor
#define PHOTOCELLPIN A0
int lightLevel = 0;

// Initialize the LEDs
#define REDPIN D4
#define YELLOWPIN D5
void turnOnRedLED()
{
    digitalWrite(REDPIN, HIGH);
}
void turnOffRedLED()
{
    digitalWrite(REDPIN, LOW);
}
void turnOnYellowLED()
{
    digitalWrite(YELLOWPIN, HIGH);
}
void turnOffYellowLED()
{
    digitalWrite(YELLOWPIN, LOW);
}

void setup()
{
    Serial.begin(115200);

    // Connect to Wi-Fi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());

    // Initialize the web server
    server.on("/", []()
              {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    String message = "<html><head><title>Smart Home</title><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css\"><style>h1{font-size:36px;color:#333;}p{font-size:24px;color:#666;}</style></head><body><h1>Smart Home</h1><p><i class=\"fa fa-thermometer-empty\"></i> Temperature: " + String(temp) + " &deg;C<br><i class=\"fa fa-tint\"></i> Humidity: " + String(hum) + "%<br><i class=\"fa fa-sun-o\"></i> Light Level: " + String(lightLevel) + "</p><p><i class=\"fa fa-bell\"></i> Alarm Enabled: " + String(alarmEnabled) + "<br><i class=\"fa fa-eye\"></i> Motion Detected: " + String(motionDetected) + "</p></body></html>";
    server.send(200, "text/html", message); });
    server.begin();
    Serial.println("Web server started");

    // Initialize the DHT sensor
    dht.begin();

    // Initialize the motion sensor
    pinMode(MOTIONPIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MOTIONPIN), handleMotionInterrupt, RISING);

    // Initialize the button
    pinMode(BUTTONPIN, INPUT_PULLUP);

    // Initialize the photoresistor
    pinMode(PHOTOCELLPIN, INPUT);

    // Initialize the LEDs
    pinMode(REDPIN, OUTPUT);
    pinMode(YELLOWPIN, OUTPUT);
}

void loop()
{
    server.handleClient();

    // Read the light level
    lightLevel = analogRead(PHOTOCELLPIN);

    // Turn on the yellow LED if the light level is too low
    if (lightLevel < 100)
    {
        turnOnYellowLED();
    }
    else
    {
        turnOffYellowLED();
    }

    // Turn on the red LED if motion is detected
    if (motionDetected)
    {
        turnOnRedLED();
        motionDetected = 0;
    }
    else
    {
        turnOffRedLED();
    }

    // Enable or disable the alarm based on the button state
    if (digitalRead(BUTTONPIN) == LOW)
    {
        alarmEnabled = !alarmEnabled;
        delay(50); // debounce the button
    }

    // Trigger the alarm if enabled and motion is detected
    if (alarmEnabled && motionDetected)
    {
        // TODO: implement alarm activation
    }

    // Read the temperature and humidity from the DHT sensor
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    // Print the sensor data to the serial monitor
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("C / Humidity: ");
    Serial.print(hum);
    Serial.println("%");

    delay(2000);
}
/*
This code sets up a web server on the NodeMCU and reads data from the DHT11 temperature and humidity sensor, motion sensor, button, and photoresistor. The code also turns on the red LED when motion is detected and the yellow LED when the light level is too low. The web server displays the sensor data in a simple HTML page with icons.
Again, please note that this code is intended as an example only and may require modifications to work with your specific hardware configuration and project requirements.
*/
