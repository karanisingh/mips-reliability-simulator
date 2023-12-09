// logging.c
#include "reliability_logging.h"
#include "mips_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

static FILE *log_file = NULL;

void log_init(const char *file_name) {
    log_file = fopen(file_name, "a"); // Open the log file for appending
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }
}

void log_write(const char *format, ...) {
    if (log_file != NULL) {
        // Start processing variable arguments
        va_list args;
        va_start(args, format);

        // Write the log message with a timestamp
        fprintf(log_file, "[Cycle %d] ", cycle); // Print the timestamp part of the message
        vfprintf(log_file, format, args);        // Print the user's formatted message
        fprintf(log_file, "\n");                // Print a newline at the end

        va_end(args); // Clean up the variable argument list

        fflush(log_file); // Flush the stream
    }
}

void log_close(void) {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}
