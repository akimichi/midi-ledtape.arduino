// #include <M5Atom.h>
#include <FastLED.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"

#define DATA_PIN        26  // LEDテープ信号端子
#define NUM_LEDS  72  // LEDの数

CRGB leds[NUM_LEDS];
int velocity = 0;
char macStr[18];

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *payload, int data_len) {
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
      mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println();
  Serial.printf("Last Packet Recv from: %s\n", macStr);
  Serial.printf("Last Packet Recv Data(%d): ", data_len);
  for (int i = 0; i < data_len; i++) {
    Serial.print(payload[i]);
    Serial.print(" ");
  }
  uint8_t command = payload[0];
  uint8_t channel = payload[1];
  uint8_t note = payload[2];
  uint8_t velocity = payload[3];
  Serial.print("command: "); Serial.println(command);
  Serial.print("channel: "); Serial.println(channel);
  Serial.print("note: "); Serial.println(note);
  Serial.print("velocity: "); Serial.println(velocity);

  switch(command)      // Get the type of the message we caught
  {
    case MidiType::InvalidType:
      clear();
      break;
    case MidiType::NoteOn:
      Serial.println("NoteOn");
      // payload: command chan note velocity
      // MIDI.sendNoteOn(payload[2],payload[3],payload[1]);  // ノートオン(pitch 42, velo 127 on channel 1)
      // fill_solid(&(leds[payload[2]]), NUM_LEDS, CHSV((payload[2]%13)*20, 300, payload[3]));
      // fill_solid(leds, NUM_LEDS, CHSV((note%13)*20, 300, velocity));

      leds[note - 24] = CHSV(map((note % 12)*30, 0, 12*30, 0, 255), 255, map(velocity, 0, 127, 0, 255));
      FastLED.show();
      break;
    case MidiType::NoteOff:
      Serial.println("NoteOff");
      leds[note - 24] = CRGB::Black;
      FastLED.show();
      break;
    case MidiType::ProgramChange:
      Serial.println("ProgramChange");
      // MIDI.sendProgramChange(payload[2], 1);  // 
      break;
    case MidiType::PitchBend:
      Serial.println("PitchBend");
      // MIDI.sendPitchBend(payload[2]*100, 1);
      break;
    case MidiType::AfterTouchChannel:
      break;
    case MidiType::AfterTouchPoly:
      break;
    case MidiType::ControlChange:
      Serial.println("ControlChange");
      // MIDI.sendControlChange(payload[1], payload[3], 1);  // 
      break;
    default:
      break;
  }
  delay(20);
};


void setup() {
  Serial.begin(115200);
  delay(1000);

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);

  // ESP-NOW初期化
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if(esp_now_init() != ESP_OK) {
  Serial.print("ESP-Now Init Failed....");
    return;
  } 
  esp_now_register_recv_cb(OnDataRecv);

  clear();
}

void clear(){
  Serial.println("Clearing display");
  for(int num=0; num<NUM_LEDS; num++) {
    Serial.println(num);
    leds[num] = CRGB::Black;
    // delay(50);
  }
  FastLED.show();
}

void loop() { }


