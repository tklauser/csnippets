#ifndef _CRC32_H_
#define _CRC32_H_

#include <stdint.h>

extern uint32_t crc32(uint32_t crc, const uint8_t *buf, size_t len);

#endif /* _CRC32_H_ */
