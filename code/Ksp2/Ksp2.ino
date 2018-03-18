int now;
int controlTime;
int controlTimeOld;

#define details(name) (uint8_t*)&name,sizeof(name)
 
 int THROTTLEpin = A0;
  
        struct ControlPacket {
          byte id;
          byte MainControls;                  //SAS RCS Lights Gear Brakes Precision Abort Stage 
          byte Mode;                          //(Not implemented)
          unsigned int ControlGroup;                //control groups 1-10 in 2 bytes
          byte NavballSASMode;                //AutoPilot mode (See above for AutoPilot modes)(Ignored if the equal to zero or out of bounds (>10)) //Navball mode
          byte AdditionalControlByte1;
          short Pitch;                        //-1000 -> 1000
          short Roll;                         //-1000 -> 1000
          short Yaw;                          //-1000 -> 1000
          short TX;                           //-1000 -> 1000
          short TY;                           //-1000 -> 1000
          short TZ;                           //-1000 -> 1000
          short WheelSteer;                   //-1000 -> 1000
          short Throttle;                     // 0 -> 1000
          short WheelThrottle;                // 0 -> 1000
    };

    ControlPacket cPacket;

    struct HandShakePacket {
  byte id;
  byte M1;
  byte M2;
  byte M3;
};

HandShakePacket HPacket;

void Handshake(){
  HPacket.id = 0;
  HPacket.M1 = 3;
  HPacket.M2 = 1;
  HPacket.M3 = 4;
  KSPBoardSendData(details(HPacket));
}

void InitTxPackets() {
  HPacket.id = 0;  
  cPacket.id = 101;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

 pinMode(THROTTLEpin, INPUT);

  InitTxPackets();

 
}

void loop() {
  // put your main code here, to run repeatedly:
  send_control_packet();

}




void KSPBoardSendData(uint8_t * address, uint8_t len){
  uint8_t CS = len;
  Serial.write(0xBE);
  Serial.write(0xEF);
  Serial.write(len);
  
  for(int i = 0; i<len; i++){
    CS^=*(address+i);
    Serial.write(*(address+i));
  }
  
  Serial.write(CS);
}

void send_control_packet() {
  now = millis();
  controlTime = now - controlTimeOld;
  if (controlTime > 10) {
    Handshake();
    controlTimeOld = now;
    //define_control_packet();
    cPacket.Throttle = constrain(map(analogRead(THROTTLEpin), 0,952, 0, 1000), 0, 1000);
    KSPBoardSendData(details(cPacket));
  }
}



  
