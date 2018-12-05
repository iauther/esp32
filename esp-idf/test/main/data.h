#ifndef __DATA_H__
#define __DATA_H__

#include "type.h"

#define MAGIC   0xff8899aa

#define IO_BT   (1<<0)
#define IO_I2C  (1<<1)
#define IO_I2S  (1<<2)
#define IO_SPI  (1<<3)
#define IO_USB  (1<<4)
#define IO_GPIO (1<<5)
#define IO_UART (1<<6)
#define IO_ETH  (1<<7)
#define IO_WIFI (1<<8)

enum {
    TYPE_HDR=0,
    TYPE_GAIN,
    TYPE_EQ,
    TYPE_DYN,
    TYPE_SETUP,
    TYPE_PARAS,
};

typedef struct {
    s16 value;
}gain_t;

var eq_t={
    u8      aa;
    u8      bb;
    gain_t  g;
};

var setup_t={
    u8      lang;
    u16     cnt;
};

var paras_t={
    u8      ver;
    eq_t    eq;
    setup_t setup;
};


typedef struct {
    u32 magic;
    u8  pack[4];
    u8  itype;
    u8  dtype;
    u32 dlen;
    u8  data[];
}hdr_t;

#endif
