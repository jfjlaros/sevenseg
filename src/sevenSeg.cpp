/*!
 * Standard layout for a 7-segment display:
 *
 *           A
 *        F     B
 *           G
 *        E     C
 *           D
 */
#include <Arduino.h>

#include "sevenSeg.h"

//! Character encoding.
const uint8_t _chars[] PROGMEM = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, // 0-9
  0x77, 0x7f, 0x39, 0x1f, 0x79, 0x71, 0x3d, 0x76, 0x30, 0x1e, // A-J
  0x7a, 0x38, 0x55, 0x76, 0x3f, 0x73, 0x3b, 0x7b, 0x6d, 0x31, // K-T
  0x3e, 0x2e, 0x6a, 0x49, 0x6e, 0x5b,                         // U-Z
  0x4c, 0x7c, 0x58, 0x5e, 0x18, 0x71, 0x59, 0x74, 0x10, 0x0e, // a-j
  0x69, 0x30, 0x15, 0x54, 0x5c, 0x73, 0x67, 0x50, 0x48, 0x78, // k-t
  0x1c, 0x0c, 0x2a, 0x48, 0x6e, 0x48};                        // u-z


/*!
 * ASCII to 7-segment display character.
 *
 * \param value ASCII character.
 *
 * \return 7-segment display character.
 */
uint8_t _asciiToSeg(uint8_t value) {
  if (value >= 1 && value <= 9) {    // Individual segments.
    return 0x01 << (value - 1);
  }
  if (value >= 48 && value <= 57) {  // 0-9
    return pgm_read_byte_near(_chars + value - 48);
  }
  if (value >= 65 && value <= 90) {  // A-Z
    return pgm_read_byte_near(_chars + value - 55);
  }
  if (value >= 97 && value <= 122) {  // a-z
    return pgm_read_byte_near(_chars + value - 61);
  }
  switch (value) {
    case '"':
      return 0x22;
    case '\'':
      return 0x20;
    case '=':
      return 0x48;
    case '-':
      return 0x40;
    case '_':
      return 0x08;
  }
  return 0x00;
}


/*!
 * Make a 7-segment display interface.
 *
 * \param size Number of digits.
 */
SevenSeg::SevenSeg(uint8_t size) {
  _buffer = (char*)malloc((2 * size + 1) * sizeof(uint8_t));
  data = (uint8_t*)malloc(size * sizeof(uint8_t));
}

/*!
 * Destructor.
 */
SevenSeg::~SevenSeg(void) {
  free(_buffer);
  free(data);
}

/*!
 * Convert an ASCII string to 7-segment display data.
 *
 * See the documentation of `printf()` for more information about string
 * formatting.
 *
 * \param format Format string.
 * \param ... Parameters.
 */
void SevenSeg::text(char const* format, ...) {
  va_list args;
  va_start(args, format);

  vsprintf(_buffer, format, args);

  va_end(args);

  uint8_t i = 0;
  for (uint8_t j = 0; _buffer[j]; j++) {
    if (_buffer[j] != '.') {
      data[i] = _asciiToSeg(_buffer[j]);
      i++;
    }
    else {
      data[i - 1] &= _asciiToSeg(0x08);
    }
  }
}
