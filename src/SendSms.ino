SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler serlog;

#define RADIO Cellular

int sendMessage(const char* telNr, const char* msg);

void setup() {
  RADIO.on();
  RADIO.connect();
}

void loop() {
  static uint32_t ms = 0;
  if (millis() - ms > 60000) {
    ms = millis();
    sendMessage("+639154120990", "Hi there, I'm Electron!");
  }
}

int sendMessage(const char* telNr, const char* msg)
{
  int  ret;
  char cmd[64];

  if (!RADIO.ready())
  {
    RADIO.on();
    RADIO.connect();
    if (!waitFor(RADIO.ready, 300000))
    {
      Log.warn("no radio connection");
      return -1;
    }
  }

  ret = Cellular.command("AT+CMGF=1\r\n");
  ret = Cellular.command("AT+CMGS=\"%s\",145\r\n", telNr);
  ret = Cellular.command("%s\x1a", msg);
  switch (ret)
  {
    case WAIT:
      Log.info("WAITING, but probably");
    case RESP_OK:
      Log.info("Sent: '%s' to %s", msg, telNr);
      RADIO.disconnect();
      RADIO.off();
      Log.info("Radio offset");
      break;
    default:
      Log.warn("Message not sent (%d)", ret);
      break;
  }

  return ret;
}
