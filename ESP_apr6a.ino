 #include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
 #include <ThingSpeak.h>


 const char *ssid = "***********";        // WiFi SSID
 const char *password = "********"; // WiFi password
const char *thingSpeakApiKey = "*********"; // ThingSpeak API Key
const unsigned long postingInterval = 3200; // Posting interval to ThingSpeak (in ms)

 WiFiClient client;


LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

   // Connect to Wi-Fi
   Serial.println("Connecting to Wi-Fi...");
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
   Serial.println("Connecting...");
   }
   Serial.println("Connected to Wi-Fi");

  // Initialize LCD
  lcd.setCursor(0, 0);
  lcd.print("Water Quality");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring");
}

void loop() {

  delay(100);
  //*************************************************Checking if there is a serial received;
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n'); // Read serial data until newline character
    
    // Check if the received data matches the expected pattern
    if (receivedData.startsWith("Ph:") && receivedData.indexOf("temp:") != -1) {
       //Find the index of "Ph:" and "temp:"
      int ph_index = receivedData.indexOf("Ph:") + 3; // Adding 3 to skip "Ph:"
      int temp_index = receivedData.indexOf("temp:") + 5; // Adding 5 to skip "temp:"
      
      // Extract the pH and temperature values
      float ph_value = receivedData.substring(ph_index, receivedData.indexOf(',', ph_index)).toFloat();
      float temp_value = receivedData.substring(temp_index).toFloat();
      
      // Print the extracted values
      Serial.print("pH: ");
      Serial.println(ph_value);
      Serial.print("Temperature: ");
      Serial.println(temp_value);

        // Update LCD with pH and temperature readings
      lcd.setCursor(0, 0);
      lcd.print("   pH:    ");
      lcd.print(ph_value);
      lcd.setCursor(0, 1);
      lcd.print("   Temp:  ");
      lcd.print(temp_value);
      lcd.print("C");
      // Send data to ThingSpeak
   ThingSpeak.begin(client);
   ThingSpeak.setField(1, ph_value);
   ThingSpeak.setField(2, temp_value);
   int status = ThingSpeak.writeFields(1, thingSpeakApiKey);
   if (status == 200) {
     Serial.println("Data sent to ThingSpeak successfully!");
   } else {
     Serial.println("Problem with sending data to ThingSpeak...");
  }

  delay(postingInterval); // Delay before next data reading and sending
    } else {
      // Invalid data format
      Serial.println("Invalid data format.");
    }

  
  }


  



  
}