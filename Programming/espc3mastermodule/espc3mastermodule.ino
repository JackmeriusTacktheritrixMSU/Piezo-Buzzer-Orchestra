
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

const int BUTTON_BUZZ_PIN = 2; 
const int BUTTON_FOREVER_PIN = 3; 

const uint8_t ESPNOW_CHANNEL = 6;

const wifi_power_t TX_POWER = WIFI_POWER_2dBm;

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

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

bool lastBuzzButton = HIGH;
bool lastForeverButton = HIGH;

unsigned long lastBuzzDebounce = 0;
unsigned long lastForeverDebounce = 0;
const unsigned long debounceMs = 40;

void addBroadcastPeer() {
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = ESPNOW_CHANNEL;
  peerInfo.encrypt = false;
  peerInfo.ifidx = WIFI_IF_STA;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add broadcast peer");
  }
}

void sendCommand(Command cmd) {
  Message msg;
  msg.magic = MAGIC;
  msg.command = cmd;
  msg.nonce = esp_random();


  for (int i = 0; i < 5; i++) {
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&msg, sizeof(msg));

    if (result == ESP_OK) {
      Serial.print("Sent command: ");
      Serial.println(cmd);
    } else {
      Serial.print("Send failed, error: ");
      Serial.println(result);
    }

    delay(60);
  }
}

void setupEspNow() {
  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(TX_POWER);
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    ESP.restart();
  }

  addBroadcastPeer();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(BUTTON_BUZZ_PIN, INPUT_PULLUP);
  pinMode(BUTTON_FOREVER_PIN, INPUT_PULLUP);

  setupEspNow();

  Serial.println("Master ready.");
  Serial.print("Master MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  bool buzzButton = digitalRead(BUTTON_BUZZ_PIN);
  bool foreverButton = digitalRead(BUTTON_FOREVER_PIN);


  if (buzzButton != lastBuzzButton) {
    lastBuzzDebounce = millis();
    lastBuzzButton = buzzButton;
  }

  if ((millis() - lastBuzzDebounce) > debounceMs) {
    static bool buzzAlreadyPressed = false;

    if (buzzButton == LOW && !buzzAlreadyPressed) {
      buzzAlreadyPressed = true;
      Serial.println("Button 1 pressed: buzz once");
      sendCommand(CMD_BUZZ_ONCE);
    }

    if (buzzButton == HIGH) {
      buzzAlreadyPressed = false;
    }
  }

  // Button 2: forever/5-minute mode
  if (foreverButton != lastForeverButton) {
    lastForeverDebounce = millis();
    lastForeverButton = foreverButton;
  }

  if ((millis() - lastForeverDebounce) > debounceMs) {
    static bool foreverAlreadyPressed = false;

    if (foreverButton == LOW && !foreverAlreadyPressed) {
      foreverAlreadyPressed = true;
      Serial.println("Button 2 pressed: forever mode");

      for (int i = 0; i < 20; i++) {
        sendCommand(CMD_FOREVER_MODE);
        delay(100);
      }
    }

    if (foreverButton == HIGH) {
      foreverAlreadyPressed = false;
    }
  }
}