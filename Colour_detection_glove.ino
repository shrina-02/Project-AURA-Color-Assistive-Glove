#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// TCS3200 Pins
const int s0 = D3;
const int s1 = D4;
const int s2 = D5;
const int s3 = D6;
const int out = D7;

// Variables
unsigned long red = 0, green = 0, blue = 0;
String colorName = "Unknown";

// Color Counters
int redCount = 0, greenCount = 0, blueCount = 0, yellowCount = 0;
int magentaCount = 0, brownCount = 0, purpleCount = 0, whiteCount = 0, blackCount = 0;

// Web Server
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ESP Color Finder");

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi: ");
  Serial.println(ssid);

  lcd.setCursor(0, 1);
  lcd.print("Connecting WiFi");

  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 20000;

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\n❌ WiFi Failed!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed!");
    lcd.setCursor(0, 1);
    lcd.print("Check SSID/PWD");
    return;
  }

  Serial.println("\n✅ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());

  server.begin();
  delay(2000);
  lcd.clear();
}

void loop() {
  detectColor();

  lcd.setCursor(0, 0);
  lcd.print("Color:          ");
  lcd.setCursor(7, 0);
  lcd.print(colorName);
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());

  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE html><html><head><title>Color Detection</title>");
    client.println("<meta http-equiv='refresh' content='2'>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<style>");
    client.println("body { font-family: Arial; background:#f4f4f4; text-align:center; }");
    client.println(".card { background:white; padding:20px; margin:20px auto; width:85%; border-radius:10px; box-shadow:0 0 10px #ccc; }");
    client.println("table { width:100%; border-collapse:collapse; }");
    client.println("th,td { border:1px solid #ddd; padding:10px; text-align:center; }");
    client.println("th { background:#333; color:white; }");
    client.println("</style></head><body>");
    client.println("<div class='card'>");
    client.println("<h2>ESP8266 Color Detection Dashboard</h2>");
    client.println("<h3>Detected Color: <span style='color:blue;'>" + colorName + "</span></h3>");
    client.println("<p><small>Auto-refresh every 2 seconds</small></p>");
    client.println("</div>");
    client.println("<div class='card'><h3>Color Count Summary</h3>");
    client.println("<table><tr><th>Color</th><th>Count</th></tr>");
    client.println("<tr><td>Red</td><td>" + String(redCount) + "</td></tr>");
    client.println("<tr><td>Green</td><td>" + String(greenCount) + "</td></tr>");
    client.println("<tr><td>Blue</td><td>" + String(blueCount) + "</td></tr>");
    client.println("<tr><td>Yellow</td><td>" + String(yellowCount) + "</td></tr>");
    client.println("<tr><td>Magenta</td><td>" + String(magentaCount) + "</td></tr>");
    client.println("<tr><td>Brown</td><td>" + String(brownCount) + "</td></tr>");
    client.println("<tr><td>Purple</td><td>" + String(purpleCount) + "</td></tr>");
    client.println("<tr><td>White</td><td>" + String(whiteCount) + "</td></tr>");
    client.println("<tr><td>Black</td><td>" + String(blackCount) + "</td></tr>");
    client.println("</table></div>");
    client.println("</body></html>");
    client.stop();
  }

  delay(300);
}

void detectColor() {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  float sum = red + green + blue;
  if (sum == 0) return;

  float rPerc = (float)red / sum * 100.0;
  float gPerc = (float)green / sum * 100.0;
  float bPerc = (float)blue / sum * 100.0;

  String newColor = "Unknown";

  if (rPerc > 45 && gPerc < 30 && bPerc < 30) newColor = "Red";
  else if (gPerc > 45 && rPerc < 30 && bPerc < 30) newColor = "Yellow";
  else if (bPerc > 45 && rPerc < 30 && gPerc < 30) newColor = "Yellow";
  else if (rPerc > 40 && gPerc > 35 && bPerc < 25) newColor = "Blue";
  else if (rPerc > 45 && gPerc < 30 && bPerc > 30) newColor = "Magenta";
  else if (rPerc < 30 && gPerc > 40 && bPerc > 35) newColor = "Brown";
  else if (rPerc > 35 && gPerc > 25 && bPerc > 35) newColor = "Purple";
  else if (rPerc > 60 && gPerc > 55 && bPerc > 55) newColor = "White";
  else if (rPerc < 20 && gPerc < 20 && bPerc < 20) newColor = "Black";

  if (newColor != colorName && newColor != "Unknown") {
    colorName = newColor;
    incrementColorCount(colorName);
    Serial.println("Detected: " + colorName);
  }
}

void incrementColorCount(String c) {
  if (c == "Red") redCount++;
  else if (c == "Green") greenCount++;
  else if (c == "Blue") blueCount++;
  else if (c == "Yellow") yellowCount++;
  else if (c == "Magenta") magentaCount++;
  else if (c == "Brown") brownCount++;
  else if (c == "Purple") purpleCount++;
  else if (c == "White") whiteCount++;
  else if (c == "Black") blackCount++;
}
