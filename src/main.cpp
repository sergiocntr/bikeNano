#include <main.h>
void smartDelay(uint16_t ms){
  unsigned long start = millis();
  do
  {
    while (Serial.available())
      gps.encode(Serial.read());
  } while (millis() - start < ms);
}
void setup(void) {
  //u8g2.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  Serial.begin(GPSBaud);
  pinMode(Pin_SW, INPUT_PULLUP);
  digitalWrite(Pin_SW, HIGH);  // attiva la resistenza interna di pullup 10Kohm sul pin 2
  attachInterrupt(digitalPinToInterrupt(Pin_SW), gate_interrupt_handler, FALLING);
  sei () ; // Enable the Global Interrupt Enable flag so that interrupts can be processed
}
void loop(void) {
  smartDelay(1000);
  
  print_date(gps);
  
  }
static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    oled.println("****");
  else
  {
    String s;
    uint8_t sat = (uint8_t)gps.satellites();
    uint16_t alt = (uint16_t)gps.f_altitude();
    //char sz[20];
    //sprintf(sz, "%02d:%02d  %ld  %f", hour, minute, sat,  alt);
    s= hour + ":" + minute + ' ' + sat +' ' +  alt;
    //oled.clear();
  //oled.setFont(fixednums15x31);
    oled.setFont(fixednums8x16);
    oled.setCursor(0,12);
    oled.println(s);
    oled.setFont(fixednums15x31);
    oled.println(gps.f_speed_kmph());
    oled.setFont(fixednums8x16);
    oled.println(readVcc());
  }
}
uint16_t readVcc() {
  uint16_t result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADC;
  //result |= ADCH<<8;
  result = 1230000 / result; // Back-calculate AVcc in mV
  return result;
}
void gate_interrupt_handler() // per apricanclello a pulsante
{
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200)
 {
   send433(0);
 }
 last_interrupt_time = interrupt_time;
}
void send433(uint8_t cosa){
  switch (cosa)
  {
  case 0:
  // Transmitter is connected to Arduino Pin #10
    mySwitch.enableTransmit(Pin_433);

  // Optional set protocol (default is 1, will work for most outlets)
    mySwitch.setProtocol(6);

  // Optional set pulse length.
    mySwitch.setPulseLength(328);

  // Optional set number of transmission repetitions.
    mySwitch.setRepeatTransmit(5);

    mySwitch.send("010000000011");
    break;

  default:
    break;
  }
}