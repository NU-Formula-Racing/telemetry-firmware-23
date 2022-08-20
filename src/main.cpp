#include <Arduino.h>
#include "../lib/CAN/include/app_can.h"

#define SERIAL_DEBUG
#define ALLOWED_CAN_FAULTS 1000

struct example_timer_t
{
  unsigned long ref = 0;
  unsigned long dur = 100;
} hp_can_time, lp_can_time;

app_can_message_t tx_msg;

int can_fault_counter;
bool can_fault;

int buffer_size = 40;

void setup()
{
  Serial.begin(115200);

  app_can_init();

  tx_msg.id = 0x410;
  tx_msg.len = 0x8;
  for (int i = 0; i < tx_msg.len; i++)
  {
    tx_msg.data[i] = i;
  }

  can_fault_counter = 0;
  can_fault = false;

  hp_can_time.dur = 100;
  lp_can_time.dur = 200;
}

void loop()
{
  // Read HP CAN every 10ms
  if (hp_can_time.ref + hp_can_time.dur <= millis())
  {
    hp_can_time.ref = millis();
    Serial.println("Reading HP CAN message: ");

    app_can_message_t *rx_msg_ptr = app_can_read();

    if ((rx_msg_ptr->id != 0x0) && (can_fault == false))
    {
      #ifdef SERIAL_DEBUG
          char buffer[buffer_size];
          sprintf(buffer, "ID: %X \t Len: %u \t Data: ", rx_msg_ptr->id, rx_msg_ptr->len);
          Serial.println(buffer);
          for (int i = 0; i < rx_msg_ptr->len; i++)
          {
            sprintf(buffer,"%X \t", rx_msg_ptr->data[i]);
            Serial.println(buffer);
          }
          Serial.println("");
      #endif
    }
  }

  // Read LP CAN every 20ms
  if (lp_can_time.ref + lp_can_time.dur <= millis())
  {
    lp_can_time.ref = millis();
    Serial.println("Reading LP CAN message: ");

    app_can_message_t *rx_msg_ptr = app_can_read();

    if ((rx_msg_ptr->id != 0x0) && (can_fault == false))
    {
        #ifdef SERIAL_DEBUG
          char buffer[buffer_size];
          sprintf(buffer, "ID: %X \t Len: %u \t Data: ", rx_msg_ptr->id, rx_msg_ptr->len);
          Serial.println(buffer);
          for (int i = 0; i < rx_msg_ptr->len; i++)
          {
            sprintf(buffer,"%X \t", rx_msg_ptr->data[i]);
            Serial.println(buffer);
          }
          Serial.println("");
        #endif
    }
  }
}