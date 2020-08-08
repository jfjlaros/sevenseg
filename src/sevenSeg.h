#ifndef SEVEN_SEG_H_
#define SEVEN_SEG_H_
/*!
 * ASCII string support for 7-segment displays.
 *
 * https://en.wikipedia.org/wiki/Seven-segment_display_character_representations
 */

/*!
 * 7-segment display interface.
 */
class SevenSeg {
  public:
    SevenSeg(void) {}
    SevenSeg(uint8_t);
    ~SevenSeg(void);
    void text(char const*, ...);
    uint8_t* data;
  private:
    char* _buffer;
};

#endif
