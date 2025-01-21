// Single function to convert hex code to RGB
bool hexToRGB(String hexCode, int *R, int *G, int *B) {
  // Remove the '#' if present
  if (hexCode.charAt(0) == '#') {
    hexCode = hexCode.substring(1);
  }

  // Ensure the hex code is exactly 6 characters long
  if (hexCode.length() != 7) {
    Serial.println("Error: Invalid hex code length");
    *R = 0;  // Red
    *G = 0;  // Green
    *B = 0;  // Blue
    return false;
  }

  // Extract and convert hex values for R, G, and B
  *R = strtol(hexCode.substring(0, 2).c_str(), NULL, 16);  // Red
  *G = strtol(hexCode.substring(2, 4).c_str(), NULL, 16);  // Green
  *B = strtol(hexCode.substring(4, 6).c_str(), NULL, 16);  // Blue
  return true;
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);
}

void loop() {
  // Empty loop
  int R, G, B;
  if(Serial.available()){
    String input = Serial.readString();
    if(hexToRGB(input, &R, &G, &B)){
      Serial.print("R: "); Serial.println(R);
      Serial.print("G: "); Serial.println(G);
      Serial.print("B: "); Serial.println(B);
      analogWrite(3, 255-R);
      analogWrite(5, 255-G);
      analogWrite(6, 255-B);
    }else{
      Serial.println("Input Error");
    }
  }
}
