/* compat-btn-stylus3.h — this system's kernel headers predate BTN_STYLUS3
 * (added to linux/input-event-codes.h around 2023, well after this board's
 * kernel headers were generated), so define it manually with its known,
 * stable upstream value. */
#ifndef BTN_STYLUS3
#define BTN_STYLUS3 0x149
#endif
