#include <ucx.h>
#include <device.h>
#include <vt100.h>


/* vt100 terminal device API function wrappers */
int vt100_term(const struct device_s *dev, uint8_t cmd)
{
	struct vt100_api_s *api;

	api = (struct vt100_api_s *)dev->custom_api;
	return api->term(dev, cmd);
}

int vt100_textattr(const struct device_s *dev, uint8_t attr)
{
	struct vt100_api_s *api;

	api = (struct vt100_api_s *)dev->custom_api;
	return api->textattr(dev, attr);
}

int vt100_cursor(const struct device_s *dev, uint8_t cmd)
{
	struct vt100_api_s *api;

	api = (struct vt100_api_s *)dev->custom_api;
	return api->cursor(dev, cmd);
}

int vt100_cursormove(const struct device_s *dev, uint8_t cmd, uint8_t cnt)
{
	struct vt100_api_s *api;

	api = (struct vt100_api_s *)dev->custom_api;
	return api->cursormove(dev, cmd, cnt);
}

int vt100_cursorpos(const struct device_s *dev, uint8_t lin, uint8_t col)
{
	struct vt100_api_s *api;

	api = (struct vt100_api_s *)dev->custom_api;
	return api->cursorpos(dev, lin, col);
}

int vt100_scrollrows(const struct device_s *dev, uint8_t rstart, uint8_t rend)
{
	struct vt100_api_s *api;

	api = (struct vt100_api_s *)dev->custom_api;
	return api->scrollrows(dev, rstart, rend);
}

int vt100_edit(const struct device_s *dev, uint8_t cmd)
{
	struct vt100_api_s *api;

	api = (struct vt100_api_s *)dev->custom_api;
	return api->edit(dev, cmd);
}

/* vt100 device driver implementation */
static int driver_term(const struct device_s *dev, uint8_t cmd)
{
	int retval = 0;
	
	NOSCHED_ENTER();
	switch (cmd) {
	case TERM_RESET:		printf("\033c"); break;
	case TERM_SOFTRESET:		printf("\033[!p"); break;
	case TERM_NEWLINE:		printf("\033[20h"); break;
	case TERM_LINEFEED:		printf("\033[20l"); break;
	case TERM_132CHARS:		printf("\033[?3h"); break;
	case TERM_80CHARS:		printf("\033[?3l"); break;
	case TERM_SMOOTHSCROLL:		printf("\033[?4h"); break;
	case TERM_JUMPSCROLL:		printf("\033[?4l"); break;
	case TERM_INVERSEVID:		printf("\033[?5h"); break;
	case TERM_NORMALVID:		printf("\033[?5l"); break;
	case TERM_LINEWRAP:		printf("\033[?7h"); break;
	case TERM_NOLINEWRAP:		printf("\033[?7l"); break;
	case TERM_DISPLAYON:		printf("\033[?75h"); break;
	case TERM_DISPLAYOFF:		printf("\033[?75l"); break;
	default: retval = -1;
	}
	NOSCHED_LEAVE();
	
	return retval;
}

static int driver_textattr(const struct device_s *dev, uint8_t attr)
{
	NOSCHED_ENTER();
	printf("\033[%dm", attr);
	NOSCHED_LEAVE();
	
	return 0;
}

static int driver_cursor(const struct device_s *dev, uint8_t cmd)
{
	int retval = 0;
	
	NOSCHED_ENTER();
	switch (cmd) {
	case CURSOR_ON:			printf("\033[?25h"); break;
	case CURSOR_OFF:		printf("\033[?25l"); break;
	case CURSOR_BLINK:		printf("\033[?12h"); break;
	case CURSOR_STATIC:		printf("\033[?12l"); break;
	case CURSOR_HOME:		printf("\033[H"); break;
	case CURSOR_UP:			printf("\033[A"); break;
	case CURSOR_DOWN:		printf("\033[B"); break;
	case CURSOR_RIGHT:		printf("\033[C"); break;
	case CURSOR_LEFT:		printf("\033[D"); break;
	case CURSOR_SAVE:		printf("\0337"); break;
	case CURSOR_RESTORE:		printf("\0338"); break;
	case SCROLL_SCREEN:		printf("\033[r"); break;
	case SCROLL_UP:			printf("\033D"); break;
	case SCROLL_DOWN:		printf("\033M"); break;
	case TAB_SET:			printf("\033H"); break;
	case TAB_CLEAR:			printf("\033[g"); break;
	case TAB_CLEAR_ALL:		printf("\033[3g"); break;
	default: retval = -1;
	}
	NOSCHED_LEAVE();
	
	return retval;
}

static int driver_cursormove(const struct device_s *dev, uint8_t cmd, uint8_t cnt)
{
	int retval = 0;
	
	NOSCHED_ENTER();
	switch (cmd) {
	case CURSOR_UP_N:		printf("\033[%dA", cnt); break;
	case CURSOR_DOWN_N:		printf("\033[%dB", cnt); break;
	case CURSOR_RIGHT_N:		printf("\033[%dC", cnt); break;
	case CURSOR_LEFT_N:		printf("\033[%dD", cnt); break;
	default: retval = -1;
	}
	NOSCHED_LEAVE();
	
	return retval;
}

static int driver_cursorpos(const struct device_s *dev, uint8_t lin, uint8_t col)
{
	NOSCHED_ENTER();
	printf("\033[%d;%dH", lin, col);
	NOSCHED_LEAVE();
	
	return 0;
}

static int driver_scrollrows(const struct device_s *dev, uint8_t rstart, uint8_t rend)
{
	NOSCHED_ENTER();
	printf("\033[%d;%dr", rstart, rend);
	NOSCHED_LEAVE();
	
	return 0;
}

static int driver_edit(const struct device_s *dev, uint8_t cmd)
{
	int retval = 0;
	
	NOSCHED_ENTER();
	switch (cmd) {
	case MODE_INSERT:		printf("\033[4h"); break;
	case MODE_REPLACE:		printf("\033[4l"); break;
	case ERASE_CHAR_RIGHT:		printf("\033[P"); break;
	case ERASE_CHAR_LEFT:		printf("\033[M"); break;
	case ERASE_EOL:			printf("\033[K"); break;
	case ERASE_SOL:			printf("\033[1K"); break;
	case ERASE_LINE:		printf("\033[2K"); break;
	case ERASE_CLEAR_DOWN:		printf("\033[J"); break;
	case ERASE_CLEAR_UP:		printf("\033[1J"); break;
	case ERASE_CLEAR_SCREEN:	printf("\033[2J"); break;
	case INSERT_LINE:		printf("\033[L"); break;
	default: retval = -1;
	}
	NOSCHED_LEAVE();
	
	return retval;
}


struct vt100_api_s vt100_api = {
	.term = driver_term,
	.textattr = driver_textattr,
	.cursor = driver_cursor,
	.cursormove = driver_cursormove,
	.cursorpos = driver_cursorpos,
	.scrollrows = driver_scrollrows,
	.edit = driver_edit
};


/* vt100 device driver implementation, generic interface via dev_ioctl() */
static int vdriver_ioctl(const struct device_s *dev, unsigned int req, va_list args)
{
	unsigned int a0, a1;
	
	a0 = va_arg(args, unsigned int);
	a1 = va_arg(args, unsigned int);
	
	switch (req) {
	case TERM: 		return driver_term(dev, a0);
	case TEXTATTR:		return driver_textattr(dev, a0);
	case CURSOR:		return driver_cursor(dev, a0);
	case CURSORMOVE:	return driver_cursormove(dev, a0, a1);
	case CURSORPOS:		return driver_cursorpos(dev, a0, a1);
	case SCROLLROWS:	return driver_scrollrows(dev, a0, a1);
	case EDIT:		return driver_edit(dev, a0);
	default:
		return -1;
	}
}

static int driver_ioctl(const struct device_s *dev, unsigned int req, ...)
{
	int retval;
	
	va_list valist;

	va_start(valist, req);
	retval = vdriver_ioctl(dev, req, valist);
	va_end(valist);
	
	return retval;
}


struct device_api_s vt100_api_generic = {
	.dev_ioctl = driver_ioctl
};
