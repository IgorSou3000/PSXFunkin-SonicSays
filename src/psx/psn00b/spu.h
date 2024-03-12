/*
 * PSn00bSDK hardware registers definitions
 * (C) 2022 spicyjpeg - MPL licensed
 */
 
#pragma once

#include <stdint.h>


#define _ADDR8(addr)		((volatile uint8_t *) (addr))
#define _ADDR16(addr)		((volatile uint16_t *) (addr))
#define _ADDR32(addr)		((volatile uint32_t *) (addr))
#define _MMIO8(addr)		(*_ADDR8(addr))
#define _MMIO16(addr)		(*_ADDR16(addr))
#define _MMIO32(addr)		(*_ADDR32(addr))

/* Constants */

#define IOBASE				0xbf800000

/* SPU */

#define SPU_MASTER_VOL_L	_MMIO16(IOBASE | 0x1d80)
#define SPU_MASTER_VOL_R	_MMIO16(IOBASE | 0x1d82)
#define SPU_REVERB_VOL_L	_MMIO16(IOBASE | 0x1d84)
#define SPU_REVERB_VOL_R	_MMIO16(IOBASE | 0x1d86)
#define SPU_KEY_ON1			_MMIO16(IOBASE | 0x1d88)
#define SPU_KEY_ON2			_MMIO16(IOBASE | 0x1d8a)
#define SPU_KEY_OFF1		_MMIO16(IOBASE | 0x1d8c)
#define SPU_KEY_OFF2		_MMIO16(IOBASE | 0x1d8e)
#define SPU_FM_MODE1		_MMIO16(IOBASE | 0x1d90)
#define SPU_FM_MODE2		_MMIO16(IOBASE | 0x1d92)
#define SPU_NOISE_MODE1		_MMIO16(IOBASE | 0x1d94)
#define SPU_NOISE_MODE2		_MMIO16(IOBASE | 0x1d96)
#define SPU_REVERB_ON1		_MMIO16(IOBASE | 0x1d98)
#define SPU_REVERB_ON2		_MMIO16(IOBASE | 0x1d9a)
#define SPU_CHAN_STATUS1	_MMIO16(IOBASE | 0x1d9c)
#define SPU_CHAN_STATUS2	_MMIO16(IOBASE | 0x1d9e)

#define SPU_REVERB_ADDR		_MMIO16(IOBASE | 0x1da2)
#define SPU_IRQ_ADDR		_MMIO16(IOBASE | 0x1da4)
#define SPU_ADDR			_MMIO16(IOBASE | 0x1da6)
#define SPU_DATA			_MMIO16(IOBASE | 0x1da8)

#define SPU_CTRL			_MMIO16(IOBASE | 0x1daa)
#define SPU_DMA_CTRL		_MMIO16(IOBASE | 0x1dac)
#define SPU_STAT			_MMIO16(IOBASE | 0x1dae)

#define SPU_CD_VOL_L		_MMIO16(IOBASE | 0x1db0)
#define SPU_CD_VOL_R		_MMIO16(IOBASE | 0x1db2)
#define SPU_EXT_VOL_L		_MMIO16(IOBASE | 0x1db4)
#define SPU_EXT_VOL_R		_MMIO16(IOBASE | 0x1db6)
#define SPU_CURRENT_VOL_L	_MMIO16(IOBASE | 0x1db8)
#define SPU_CURRENT_VOL_R	_MMIO16(IOBASE | 0x1dba)

// These are not named SPU_VOICE_* to avoid name clashes with SPU attribute
// flags defined in psxspu.h.
#define SPU_CH_VOL_L(N)		_MMIO16((IOBASE | 0x1c00) + (16 * (N)))
#define SPU_CH_VOL_R(N)		_MMIO16((IOBASE | 0x1c02) + (16 * (N)))
#define SPU_CH_FREQ(N)		_MMIO16((IOBASE | 0x1c04) + (16 * (N)))
#define SPU_CH_ADDR(N)		_MMIO16((IOBASE | 0x1c06) + (16 * (N)))
#define SPU_CH_ADSR1(N)		_MMIO16((IOBASE | 0x1c08) + (16 * (N)))
#define SPU_CH_ADSR2(N)		_MMIO16((IOBASE | 0x1c0a) + (16 * (N)))
#define SPU_CH_ADSR_VOL(N)	_MMIO16((IOBASE | 0x1c0c) + (16 * (N)))
#define SPU_CH_LOOP_ADDR(N)	_MMIO16((IOBASE | 0x1c0e) + (16 * (N)))

/* Macros */

#define getSPUAddr(addr)		((uint16_t) (((addr) + 7) / 8))
#define getSPUSampleRate(rate)	((uint16_t) (((rate) * (1 << 12)) / 44100))

#define getSPUADSR(ar, dr, sr, rr, sl) ( \
	(sl) | \
	((dr) <<  4) | \
	((ar) <<  8) | \
	((rr) << 16) | \
	((sr) << 22) | \
	(1    << 30) \
)