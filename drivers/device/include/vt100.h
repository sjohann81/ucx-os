/* vt100 terminal device API */
int vt100_term(const struct device_s *dev, uint8_t cmd);
int vt100_textattr(const struct device_s *dev, uint8_t attr);
int vt100_cursor(const struct device_s *dev, uint8_t cmd);
int vt100_cursormove(const struct device_s *dev, uint8_t cmd, uint8_t cnt);
int vt100_cursorpos(const struct device_s *dev, uint8_t lin, uint8_t col);
int vt100_scrollrows(const struct device_s *dev, uint8_t rstart, uint8_t rend);
int vt100_edit(const struct device_s *dev, uint8_t cmd);

enum vt100_ioctl_code {TERM, TEXTATTR, CURSOR, CURSORMOVE, CURSORPOS, SCROLLROWS, EDIT};

/* vt100 terminal device interface */
struct vt100_api_s {
	int (*term)(const struct device_s *dev, uint8_t cmd);
	int (*textattr)(const struct device_s *dev, uint8_t attr);
	int (*cursor)(const struct device_s *dev, uint8_t cmd);
	int (*cursormove)(const struct device_s *dev, uint8_t cmd, uint8_t cnt);
	int (*cursorpos)(const struct device_s *dev, uint8_t lin, uint8_t col);
	int (*scrollrows)(const struct device_s *dev, uint8_t rstart, uint8_t rend);
	int (*edit)(const struct device_s *dev, uint8_t cmd);
};

extern struct vt100_api_s vt100_api;
extern struct device_api_s vt100_api_generic;

enum terminal_cmd {
	TERM_RESET,
	TERM_SOFTRESET,
	TERM_NEWLINE,
	TERM_LINEFEED,
	TERM_132CHARS,
	TERM_80CHARS,
	TERM_SMOOTHSCROLL,
	TERM_JUMPSCROLL,
	TERM_INVERSEVID,
	TERM_NORMALVID,
	TERM_LINEWRAP,
	TERM_NOLINEWRAP,
	TERM_DISPLAYON,
	TERM_DISPLAYOFF
};

enum text_attr {
	TEXT_RESET		= 0,
	TEXT_BOLD		= 1,
	TEXT_DIM		= 2,
	TEXT_UNDERLINE		= 4,
	TEXT_BLINK		= 5,
	TEXT_INVERSE		= 7,
	TEXT_HIDDEN		= 8,

	TEXT_BLACK		= 30,
	TEXT_RED		= 31,
	TEXT_GREEN		= 32,
	TEXT_YELLOW		= 33,
	TEXT_BLUE		= 34,
	TEXT_MAGENTA		= 35,
	TEXT_CYAN		= 36,
	TEXT_WHITE		= 37,
	TEXT_DEFAULT		= 39,
	
	TEXT_BG_BLACK		= 40,
	TEXT_BG_RED		= 41,
	TEXT_BG_GREEN		= 42,
	TEXT_BG_YELLOW		= 43,
	TEXT_BG_BLUE		= 44,
	TEXT_BG_MAGENTA		= 45,
	TEXT_BG_CYAN		= 46,
	TEXT_BG_WHITE		= 47,
	TEXT_BG_DEFAULT		= 49
};

enum cursor_cmd {
	CURSOR_ON,
	CURSOR_OFF,
	CURSOR_BLINK,
	CURSOR_STATIC,
	CURSOR_HOME,
	CURSOR_UP,
	CURSOR_DOWN,
	CURSOR_RIGHT,
	CURSOR_LEFT,
	CURSOR_UP_N,
	CURSOR_DOWN_N,
	CURSOR_RIGHT_N,
	CURSOR_LEFT_N,
	CURSOR_SAVE,
	CURSOR_RESTORE,
	SCROLL_SCREEN,
	SCROLL_UP,
	SCROLL_DOWN,
	TAB_SET,
	TAB_CLEAR,
	TAB_CLEAR_ALL
};

enum edit_cmd {
	MODE_INSERT,
	MODE_REPLACE,
	ERASE_CHAR_RIGHT,
	ERASE_CHAR_LEFT,
	ERASE_EOL,
	ERASE_SOL,
	ERASE_LINE,
	ERASE_CLEAR_DOWN,
	ERASE_CLEAR_UP,
	ERASE_CLEAR_SCREEN,
	INSERT_LINE
};
