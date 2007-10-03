
#define DEFAULT_SERIAL "/dev/rfcomm0"
#define SOFTWEDGE_VERSION "1.0"



int sw_open_serial(const char *port);

void sw_init();

void sw_read_loop();
