#ifndef DFU_MANAGER_H_
#define DFU_MANAGER_H_

#include "Arduino.h"

#define TARGET_ANNA

#if defined (TARGET_ANNA)
#include "SPIFBlockDevice.h"
#else
#include "FlashIAPBlockDevice.h"
#endif

#include "LittleFileSystem.h"

enum DFUType {
  DFU_INTERNAL,
  DFU_EXTERNAL 
};

enum DFUAckCode {
  DFUAck = 0x0F,
  DFUNack = 0x00
};

struct __attribute__((packed)) DFUPacket {
  uint8_t last: 1;
  union {
    uint16_t index: 15;
    uint16_t remaining: 15;
  };
  uint8_t data[64];
};

class DFUManager {
public: 
  DFUManager();
  virtual ~DFUManager();

  void begin();
  void processPacket(DFUType dfuType, const uint8_t* data);

  uint8_t acknowledgment();
  void debug(Stream &stream);

private:
#if defined (TARGET_ANNA)
  static SPIFBlockDevice _bd;
#else 
  static FlashIAPBlockDevice _bd;
#endif
  static mbed::LittleFileSystem _fs;
  FILE* _target;
  Stream *_debug;

  uint8_t _acknowledgment;
};

extern DFUManager dfuManager;

#endif
