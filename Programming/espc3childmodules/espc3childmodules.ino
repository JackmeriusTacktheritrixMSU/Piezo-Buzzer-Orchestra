
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Preferences.h>


const int BUZZER_PIN = 4;
const int LOCAL_RESET_PIN = 9;

const uint8_t ESPNOW_CHANNEL = 6;

const bool ACTIVE_BUZZER = true;

const int PASSIVE_BUZZ_FREQ = 2500;

const unsigned long BUZZ_TIME_MS = 1500;
const unsigned long FOREVER_INTERVAL_MS = 5UL * 60UL * 1000UL;

const uint32_t MAGIC = 0xBEEFB33F;

enum Command : uint8_t {
  CMD_BUZZ_ONCE = 1,
  CMD_FOREVER_MODE = 2
};

struct Message {
  uint32_t magic;
  uint8_t command;
  uint32_t nonce;
};

Preferences prefs;

volatile bool pendingBuzz = false;
volatile bool pendingForever = false;

bool foreverMode = false;
bool buzzing = false;
unsigned long buzzStopTime = 0;
unsigned long lastForeverBuzz = 0;

void startBuzz() {
  buzzing = true;
  buzzStopTime = millis() + BUZZ_TIME_MS;

  if (ACTIVE_BUZZER) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    tone(BUZZER_PIN, PASSIVE_BUZZ_FREQ);
  }

  Serial.println("Buzz started");
}

void stopBuzz() {
  buzzing = false;

  if (ACTIVE_BUZZER) {
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    noTone(BUZZER_PIN);
  }

  Serial.println("Buzz stopped");
}

void updateBuzz() {
  if (buzzing && millis() >= buzzStopTime) {
    stopBuzz();
  }
}

void shutOffEspNowAndWifi() {
  esp_now_deinit();
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_OFF);

  Serial.println("ESP-NOW/Wi-Fi shut off");
}

void enterForeverMode() {
  if (foreverMode) return;

  foreverMode = true;
  prefs.putBool("forever", true);

  shutOffEspNowAndWifi();
  lastForeverBuzz = millis() - FOREVER_INTERVAL_MS;

  Serial.println("Entered forever 5-minute mode");
}

#if ESP_ARDUINO_VERSION_MAJOR >= 3
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len)
#else
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
#endif
{
  if (len != sizeof(Message)) return;

  Message msg;
  memcpy(&msg, incomingData, sizeof(msg));

  if (msg.magic != MAGIC) return;

  if (msg.command == CMD_BUZZ_ONCE) {
    pendingBuzz = true;
  } else if (msg.command == CMD_FOREVER_MODE) {
    pendingForever = true;
  }
}

void setupEspNowReceiver() {
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    ESP.restart();
  }

  esp_now_register_recv_cb(onDataRecv);

  Serial.println("Receiver ESP-NOW ready.");
  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());
}

void clearForeverModeAndRestart() {
  Serial.println("Clearing forever mode and restarting...");
  prefs.putBool("forever", false);
  delay(300);
  ESP.restart();
}

void checkLocalResetButton() {
  static unsigned long pressedSince = 0;

  if (digitalRead(LOCAL_RESET_PIN) == LOW) {
    if (pressedSince == 0) {
      pressedSince = millis();
    }

    if (millis() - pressedSince > 3000) {
      clearForeverModeAndRestart();
    }
  } else {
    pressedSince = 0;
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(LOCAL_RESET_PIN, INPUT_PULLUP);

  prefs.begin("buzzer", false);
  if (digitalRead(LOCAL_RESET_PIN) == LOW) {
    clearForeverModeAndRestart();
  }

  foreverMode = prefs.getBool("forever", false);

  if (foreverMode) {
    Serial.println("Booting into saved forever mode");
    WiFi.mode(WIFI_OFF);
    lastForeverBuzz = millis() - FOREVER_INTERVAL_MS;
  } else {
    setupEspNowReceiver();
  }
}

void loop() {
  checkLocalResetButton();
  updateBuzz();

  if (!foreverMode) {
    if (pendingBuzz) {
      pendingBuzz = false;
      startBuzz();
    }

    if (pendingForever) {
      pendingForever = false;
      enterForeverMode();
    }
  }

  if (foreverMode) {
    if (!buzzing && millis() - lastForeverBuzz >= FOREVER_INTERVAL_MS) {
      lastForeverBuzz = millis();
      startBuzz();
    }
  }
}