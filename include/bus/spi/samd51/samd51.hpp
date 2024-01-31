#pragma once

#include <SPI.h>
#include <Adafruit_ZeroDMA.h>
#include <utility/dma.h>
#include <wiring_private.h>

#include "bus/spi/spi.hpp"

namespace glue {

namespace native {

template
  < uint8_t MISO = PIN_SPI_MISO,
    uint8_t SCK  = PIN_SPI_SCK,
    uint8_t MOSI = PIN_SPI_MOSI >
class spi: public glue::spi<SPIClass> {
private:
  const SPISettings      &_conf;
  Adafruit_ZeroDMA  _dmac;
  DmacDescriptor   *_desc;
  ZeroDMAstatus     _stat;
  bool              _done;
  size_t            _curr; // double buffer index

public:
  using spi_t = spi;
  using base_spi_t = glue::spi<SPIClass>;

  static spi_t &instance() {
    static spi_t _instance;
    return _instance;
  }

  spi(SERCOM *sercom = &PERIPH_SPI,
    SercomSpiTXPad tx = PAD_SPI_TX, SercomRXPad rx = PAD_SPI_RX)
      : base_spi_t(sercom, MISO, SCK, MOSI, tx, rx),
       _conf(SPISettings(24000000, MSBFIRST, SPI_MODE0)) {}

  constexpr bool is_native() { return true; }

  void init() {
    base_spi_t::init();
    pinPeripheral(MISO, PIO_SERCOM);
    pinPeripheral(SCK,  PIO_SERCOM);
    pinPeripheral(MOSI, PIO_SERCOM);
    _dmac.setTrigger(SERCOM1_DMAC_ID_TX);
    _dmac.setAction(DMA_TRIGGER_ACTON_BEAT);
    _stat = _dmac.allocate();
    // return DMA_STATUS_OK == _stat;
  }

  void tx(void *buf, size_t len) {
    if (!_desc) {
      _desc = _dmac.addDescriptor(buf, (void *)(&SERCOM1->SPI.DATA.reg),
        len, DMA_BEAT_SIZE_BYTE, true, false);
      _dmac.setCallback([&](Adafruit_ZeroDMA *) { _done = true; });
    } else {
      while (!_done) {}
      endTransaction();
      _dmac.changeDescriptor(_desc, buf);
    }
    beginTransaction(_conf);
    _done = false;
    _stat = _dmac.startJob();
    _curr = 1 - _curr;
    // return DMA_STATUS_OK == _stat;

    // beginTransaction(_conf);
    // transfer(buf, nullptr, len);
    // endTransaction();
  }
};

} // namespace native

} // namespace glue
