
// Time to sleep (in seconds):
const int sleepTimeS = 10;

void setup() {
  Serial.println("Awake !!");
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop() {}
