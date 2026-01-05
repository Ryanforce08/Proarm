// Parameters for the message size
const int charsPerLine = 30;   // this is the max # of chars per line
const int num_lines = 30;

// parallel port pin# = arduino pin#

// Robot command terminal pins
const int data_0 = 39;
const int data_1 = 37;
const int data_2 = 35;
const int data_3 = 33;
const int data_4 = 31;
const int data_5 = 29;
const int data_6 = 27;
const int data_7 = 25;
const int nStrobe = 41;
const int nAck = 23; //3;
const int busy = 52; //2;

const int strobeWait = 0.005;   // microseconds to strobe for


void tryHandleLine(const String& s) {
  for (auto& cmd : commands) {
    if (s.startsWith(cmd.name)) {
      cmd.handler(s);
      // Serial.printf("OK %s\n", cmd.name);
      return;
    }
  }
  Serial.println("ERR Unknown command");
}

void readSerial() {
  while (Serial.available()) {
      char c = (char)Serial.read();
      if (c == '\n') {
        String s = lineBuf;
        if (s.endsWith("\r")) s.remove(s.length() - 1);
        tryHandleLine(s);
        lineBuf = "";
      } else {
        lineBuf += c;
        if (lineBuf.length() > 200) lineBuf.remove(0, 200);
      }
  }
}

// Setup Loop. This only runs one time
void setup() {
  Serial.begin(9600); // Establish Baud Rate

  // Robot command terminal pins
  pinMode(nStrobe, OUTPUT);      // is active LOW
  digitalWrite(nStrobe, HIGH);   // set HIGH
  pinMode(data_0, OUTPUT);
  pinMode(data_1, OUTPUT);
  pinMode(data_2, OUTPUT);
  pinMode(data_3, OUTPUT);
  pinMode(data_4, OUTPUT);
  pinMode(data_5, OUTPUT);
  pinMode(data_6, OUTPUT);
  pinMode(data_7, OUTPUT);
  pinMode(nAck, INPUT);     // is active LOW
  pinMode(busy, INPUT);

  delay(500); // Wait half second
  resetRobot(); // Run reset robot routine

  Serial.println("Delay for half sec");
  delay(500);
  Serial.println("Startup complete");
}

// Program Loop. This runs indefinitely
void loop() {

}

// Function for emulating handshake protocol
void printByte(byte inByte) {
  while (digitalRead(busy) == HIGH) { // if robot is busy, wait
    // wait for busy to go low
  }

  digitalWrite(nStrobe, LOW);       // strobe nStrobe to initiate handshake
  delayMicroseconds(strobeWait);

  int b0 = bitRead(inByte, 0);
  int b1 = bitRead(inByte, 1);
  int b2 = bitRead(inByte, 2);
  int b3 = bitRead(inByte, 3);
  int b4 = bitRead(inByte, 4);
  int b5 = bitRead(inByte, 5);
  int b6 = bitRead(inByte, 6);
  int b7 = bitRead(inByte, 7);

  digitalWrite(data_0, b0);        // set data bit pins
  digitalWrite(data_1, b1);
  digitalWrite(data_2, b2);
  digitalWrite(data_3, b3);
  digitalWrite(data_4, b4);
  digitalWrite(data_5, b5);
  digitalWrite(data_6, b6);
  digitalWrite(data_7, b7);

  digitalWrite(nStrobe, HIGH);  // exit the handshake

  while (digitalRead(busy) == HIGH) { // Wait for robot to perform commmand
    // wait for busy line to go low
  }
}