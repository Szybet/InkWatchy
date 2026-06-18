// from https://github.com/T-vK/ESP32-BLE-Keyboard and it's many forks!

#include "BleKeyboard.h"

#if BLE_PERIPHERAL

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "sdkconfig.h"

#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02

static const uint8_t _hidReportDescriptor[] = {
    USAGE_PAGE(1), 0x01,
    USAGE(1), 0x06,
    COLLECTION(1), 0x01,
    REPORT_ID(1), KEYBOARD_ID,
    USAGE_PAGE(1), 0x07,
    USAGE_MINIMUM(1), 0xE0,
    USAGE_MAXIMUM(1), 0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1), 0x01,
    REPORT_COUNT(1), 0x08,
    HIDINPUT(1), 0x02,
    REPORT_COUNT(1), 0x01,
    REPORT_SIZE(1), 0x08,
    HIDINPUT(1), 0x01,
    REPORT_COUNT(1), 0x05,
    REPORT_SIZE(1), 0x01,
    USAGE_PAGE(1), 0x08,
    USAGE_MINIMUM(1), 0x01,
    USAGE_MAXIMUM(1), 0x05,
    HIDOUTPUT(1), 0x02,
    REPORT_COUNT(1), 0x01,
    REPORT_SIZE(1), 0x03,
    HIDOUTPUT(1), 0x01,
    REPORT_COUNT(1), 0x06,
    REPORT_SIZE(1), 0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,
    USAGE_PAGE(1), 0x07,
    USAGE_MINIMUM(1), 0x00,
    USAGE_MAXIMUM(1), 0x65,
    HIDINPUT(1), 0x00,
    END_COLLECTION(0),
    USAGE_PAGE(1), 0x0C,
    USAGE(1), 0x01,
    COLLECTION(1), 0x01,
    REPORT_ID(1), MEDIA_KEYS_ID,
    USAGE_PAGE(1), 0x0C,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1), 0x01,
    REPORT_COUNT(1), 0x10,
    USAGE(1), 0xB5,
    USAGE(1), 0xB6,
    USAGE(1), 0xB7,
    USAGE(1), 0xCD,
    USAGE(1), 0xE2,
    USAGE(1), 0xE9,
    USAGE(1), 0xEA,
    USAGE(2), 0x23, 0x02,
    USAGE(2), 0x94, 0x01,
    USAGE(2), 0x92, 0x01,
    USAGE(2), 0x2A, 0x02,
    USAGE(2), 0x21, 0x02,
    USAGE(2), 0x26, 0x02,
    USAGE(2), 0x24, 0x02,
    USAGE(2), 0x83, 0x01,
    USAGE(2), 0x8A, 0x01,
    HIDINPUT(1), 0x02,
    END_COLLECTION(0)};

BleKeyboard::BleKeyboard(String deviceName, String deviceManufacturer, uint8_t batteryLevel)
    : hid(nullptr), deviceName(deviceName.substring(0, 15)), deviceManufacturer(deviceManufacturer.substring(0, 15)), batteryLevel(batteryLevel)
{
  _keyReport.modifiers = 0;
  _keyReport.reserved = 0;
  for (int i = 0; i < 6; i++)
    _keyReport.keys[i] = 0;
  _mediaKeyReport[0] = 0;
  _mediaKeyReport[1] = 0;
}

BleKeyboard::~BleKeyboard()
{
  this->end();
}

void BleKeyboard::begin(void)
{
  BLEDevice::init(deviceName);
  BLEDevice::setPower(ESP_PWR_LVL_P20);

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(this);

  hid = new BLEHIDDevice(pServer);
  inputKeyboard = hid->inputReport(KEYBOARD_ID);
  outputKeyboard = hid->outputReport(KEYBOARD_ID);
  inputMediaKeys = hid->inputReport(MEDIA_KEYS_ID);

  outputKeyboard->setCallbacks(this);

  hid->manufacturer()->setValue(deviceManufacturer);

  hid->pnp(0x02, vid, pid, version);
  hid->hidInfo(0x00, 0x01);

  pSecurity = new BLESecurity();
  // Use standard bonding without mandatory MITM/Passkeys if not strictly needed
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  pSecurity->setCapability(ESP_IO_CAP_NONE);
  // pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
  delete pSecurity;

  hid->reportMap((uint8_t *)_hidReportDescriptor, sizeof(_hidReportDescriptor));
  hid->startServices();

  onStarted(pServer);

  advertising = pServer->getAdvertising();
  advertising->setAppearance(HID_KEYBOARD);
  advertising->addServiceUUID(hid->hidService()->getUUID());
  advertising->setScanResponse(false);
  advertising->start();
  hid->setBatteryLevel(batteryLevel);
}

void BleKeyboard::end(void)
{
  if (advertising != nullptr)
  {
    advertising->stop();
  }

  /*
  if (hid != nullptr)
  {
    delete hid;
    hid = nullptr;
  }
  */

  if (inputKeyboard != nullptr)
  {
    inputKeyboard->setCallbacks(nullptr);
  }
  if (outputKeyboard != nullptr)
  {
    outputKeyboard->setCallbacks(nullptr);
  }
  if (inputMediaKeys != nullptr)
  {
    inputMediaKeys->setCallbacks(nullptr);
  }
  BLEServer *pServer = BLEDevice::getServer();
  if (pServer != nullptr)
  {
    pServer->setCallbacks(nullptr);
  }

  BLEDevice::deinit();

  /*
  if(pSecurity != nullptr) {
    delete pSecurity;
  }
  */

  advertising = nullptr;
  inputKeyboard = nullptr;
  outputKeyboard = nullptr;
  inputMediaKeys = nullptr;
  connected = false;
}

bool BleKeyboard::isConnected(void)
{
  return this->connected;
}

void BleKeyboard::setBatteryLevel(uint8_t level)
{
  this->batteryLevel = level;
  if (hid != nullptr)
    this->hid->setBatteryLevel(this->batteryLevel);
}

void BleKeyboard::setName(String deviceName)
{
  this->deviceName = deviceName;
}

void BleKeyboard::set_vendor_id(uint16_t vid)
{
  this->vid = vid;
}

void BleKeyboard::set_product_id(uint16_t pid)
{
  this->pid = pid;
}

void BleKeyboard::set_version(uint16_t version)
{
  this->version = version;
}

void BleKeyboard::sendReport(KeyReport *keys)
{
  if (this->isConnected() && this->inputKeyboard != nullptr)
  {
    this->inputKeyboard->setValue((uint8_t *)keys, sizeof(KeyReport));
    this->inputKeyboard->notify();
  }
}

void BleKeyboard::sendReport(MediaKeyReport *keys)
{
  if (this->isConnected() && this->inputMediaKeys != nullptr)
  {
    this->inputMediaKeys->setValue((uint8_t *)keys, sizeof(MediaKeyReport));
    this->inputMediaKeys->notify();
  }
}

#define SHIFT 0x80
const uint8_t _asciimap[128] =
    {
        0x00, // NUL
        0x00, // SOH
        0x00, // STX
        0x00, // ETX
        0x00, // EOT
        0x00, // ENQ
        0x00, // ACK
        0x00, // BEL
        0x2a, // BS   Backspace
        0x2b, // TAB  Tab
        0x28, // LF   Enter
        0x00, // VT
        0x00, // FF
        0x00, // CR
        0x00, // SO
        0x00, // SI
        0x00, // DEL
        0x00, // DC1
        0x00, // DC2
        0x00, // DC3
        0x00, // DC4
        0x00, // NAK
        0x00, // SYN
        0x00, // ETB
        0x00, // CAN
        0x00, // EM
        0x00, // SUB
        0x00, // ESC
        0x00, // FS
        0x00, // GS
        0x00, // RS
        0x00, // US

        0x2c,         //  ' '
        0x1e | SHIFT, // !
        0x34 | SHIFT, // "
        0x20 | SHIFT, // #
        0x21 | SHIFT, // $
        0x22 | SHIFT, // %
        0x24 | SHIFT, // &
        0x34,         // '
        0x26 | SHIFT, // (
        0x27 | SHIFT, // )
        0x25 | SHIFT, // *
        0x2e | SHIFT, // +
        0x36,         // ,
        0x2d,         // -
        0x37,         // .
        0x38,         // /
        0x27,         // 0
        0x1e,         // 1
        0x1f,         // 2
        0x20,         // 3
        0x21,         // 4
        0x22,         // 5
        0x23,         // 6
        0x24,         // 7
        0x25,         // 8
        0x26,         // 9
        0x33 | SHIFT, // :
        0x33,         // ;
        0x36 | SHIFT, // <
        0x2e,         // =
        0x37 | SHIFT, // >
        0x38 | SHIFT, // ?
        0x1f | SHIFT, // @
        0x04 | SHIFT, // A
        0x05 | SHIFT, // B
        0x06 | SHIFT, // C
        0x07 | SHIFT, // D
        0x08 | SHIFT, // E
        0x09 | SHIFT, // F
        0x0a | SHIFT, // G
        0x0b | SHIFT, // H
        0x0c | SHIFT, // I
        0x0d | SHIFT, // J
        0x0e | SHIFT, // K
        0x0f | SHIFT, // L
        0x10 | SHIFT, // M
        0x11 | SHIFT, // N
        0x12 | SHIFT, // O
        0x13 | SHIFT, // P
        0x14 | SHIFT, // Q
        0x15 | SHIFT, // R
        0x16 | SHIFT, // S
        0x17 | SHIFT, // T
        0x18 | SHIFT, // U
        0x19 | SHIFT, // V
        0x1a | SHIFT, // W
        0x1b | SHIFT, // X
        0x1c | SHIFT, // Y
        0x1d | SHIFT, // Z
        0x2f,         // [
        0x31,         // bslash
        0x30,         // ]
        0x23 | SHIFT, // ^
        0x2d | SHIFT, // _
        0x35,         // `
        0x04,         // a
        0x05,         // b
        0x06,         // c
        0x07,         // d
        0x08,         // e
        0x09,         // f
        0x0a,         // g
        0x0b,         // h
        0x0c,         // i
        0x0d,         // j
        0x0e,         // k
        0x0f,         // l
        0x10,         // m
        0x11,         // n
        0x12,         // o
        0x13,         // p
        0x14,         // q
        0x15,         // r
        0x16,         // s
        0x17,         // t
        0x18,         // u
        0x19,         // v
        0x1a,         // w
        0x1b,         // x
        0x1c,         // y
        0x1d,         // z
        0x2f | SHIFT, // {
        0x31 | SHIFT, // |
        0x30 | SHIFT, // }
        0x35 | SHIFT, // ~
        0             // DEL
};

size_t BleKeyboard::press(uint8_t k)
{
  uint8_t i;
  if (k >= 136)
  {
    k = k - 136;
  }
  else if (k >= 128)
  {
    _keyReport.modifiers |= (1 << (k - 128));
    k = 0;
  }
  else
  {
    k = pgm_read_byte(_asciimap + k);
    if (!k)
    {
      setWriteError();
      return 0;
    }
    if (k & 0x80)
    {
      _keyReport.modifiers |= 0x02;
      k &= 0x7F;
    }
  }

  if (_keyReport.keys[0] != k && _keyReport.keys[1] != k &&
      _keyReport.keys[2] != k && _keyReport.keys[3] != k &&
      _keyReport.keys[4] != k && _keyReport.keys[5] != k)
  {

    for (i = 0; i < 6; i++)
    {
      if (_keyReport.keys[i] == 0x00)
      {
        _keyReport.keys[i] = k;
        break;
      }
    }
    if (i == 6)
    {
      setWriteError();
      return 0;
    }
  }
  sendReport(&_keyReport);
  return 1;
}

size_t BleKeyboard::press(const MediaKeyReport k)
{
  uint16_t k_16 = k[1] | (k[0] << 8);
  uint16_t mediaKeyReport_16 = _mediaKeyReport[1] | (_mediaKeyReport[0] << 8);
  mediaKeyReport_16 |= k_16;
  _mediaKeyReport[0] = (uint8_t)((mediaKeyReport_16 & 0xFF00) >> 8);
  _mediaKeyReport[1] = (uint8_t)(mediaKeyReport_16 & 0x00FF);
  sendReport(&_mediaKeyReport);
  return 1;
}

size_t BleKeyboard::release(uint8_t k)
{
  uint8_t i;
  if (k >= 136)
  {
    k = k - 136;
  }
  else if (k >= 128)
  {
    _keyReport.modifiers &= ~(1 << (k - 128));
    k = 0;
  }
  else
  {
    k = pgm_read_byte(_asciimap + k);
    if (!k)
    {
      return 0;
    }
    if (k & 0x80)
    {
      _keyReport.modifiers &= ~(0x02);
      k &= 0x7F;
    }
  }

  for (i = 0; i < 6; i++)
  {
    if (0 != k && _keyReport.keys[i] == k)
    {
      _keyReport.keys[i] = 0x00;
    }
  }
  sendReport(&_keyReport);
  return 1;
}

size_t BleKeyboard::release(const MediaKeyReport k)
{
  uint16_t k_16 = k[1] | (k[0] << 8);
  uint16_t mediaKeyReport_16 = _mediaKeyReport[1] | (_mediaKeyReport[0] << 8);
  mediaKeyReport_16 &= ~k_16;
  _mediaKeyReport[0] = (uint8_t)((mediaKeyReport_16 & 0xFF00) >> 8);
  _mediaKeyReport[1] = (uint8_t)(mediaKeyReport_16 & 0x00FF);
  sendReport(&_mediaKeyReport);
  return 1;
}

void BleKeyboard::releaseAll(void)
{
  _keyReport.keys[0] = 0;
  _keyReport.keys[1] = 0;
  _keyReport.keys[2] = 0;
  _keyReport.keys[3] = 0;
  _keyReport.keys[4] = 0;
  _keyReport.keys[5] = 0;
  _keyReport.modifiers = 0;
  _mediaKeyReport[0] = 0;
  _mediaKeyReport[1] = 0;
  sendReport(&_keyReport);
  sendReport(&_mediaKeyReport);
}

size_t BleKeyboard::write(uint8_t c)
{
  uint8_t p = press(c);
  release(c);
  return p;
}

size_t BleKeyboard::write(const MediaKeyReport c)
{
  uint16_t p = press(c);
  release(c);
  return p;
}

size_t BleKeyboard::write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  while (size--)
  {
    if (*buffer != '\r')
    {
      if (write(*buffer))
      {
        n++;
      }
      else
      {
        break;
      }
    }
    buffer++;
  }
  return n;
}

void BleKeyboard::onConnect(BLEServer *pServer)
{
  this->connected = true;

  BLE2902 *desc = (BLE2902 *)this->inputKeyboard->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  if (desc != nullptr)
    desc->setNotifications(true);

  desc = (BLE2902 *)this->inputMediaKeys->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  if (desc != nullptr)
    desc->setNotifications(true);
}

void BleKeyboard::onDisconnect(BLEServer *pServer)
{
  this->connected = false;
  this->releaseAll();

  if (this->advertising != nullptr)
  {
    if (this->inputKeyboard != nullptr)
    {
      BLE2902 *desc = (BLE2902 *)this->inputKeyboard->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      if (desc != nullptr)
        desc->setNotifications(false);
    }

    if (this->inputMediaKeys != nullptr)
    {
      BLE2902 *desc = (BLE2902 *)this->inputMediaKeys->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      if (desc != nullptr)
        desc->setNotifications(false);
    }

    this->advertising->start();
  }
}

void BleKeyboard::onWrite(BLECharacteristic *me)
{
  String rxValue = me->getValue();
  uint8_t *value = (uint8_t *)rxValue.c_str();
  (void)value;
}
#endif
