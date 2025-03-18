import curses


def check_unicode_support(unicode_chars):
    def curses_main(stdscr):
        stdscr.clear()
        for char in unicode_chars:
            stdscr.addstr(0, 0, char)
            stdscr.refresh()
            if stdscr.getch() == ord("q"):
                break

    curses.wrapper(curses_main)


unicode_chars = [
    "\u2705",
    "\u274C",
    "\U0001F535",
    "\U0001F518",
    "\u2B24",
    "\u26AA",
    "\u25CB",
    "\u26AB",
    "\U0001F532",
    "\U0001F533",
]

check_unicode_support(unicode_chars)
