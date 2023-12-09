// reliability_logging.h
#ifndef RELIABILITY_LOGGING_H
#define RELIABILITY_LOGGING_H

void log_init(const char *file_name);
void log_write(const char *format, ...);
void log_close(void);

#endif // RELIABILITY_LOGGING_H
