/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: my_printf.c
 * The my_vprintf function constructs the formatted output into a buffer instead of directly printing it.
 * snprintf is used to format and add content to the buffer based on the format specifiers.
 * The my_printf function allocates a buffer, calls my_vprintf to construct the output into the buffer, 
 * then prints the content of the buffer, and finally frees the allocated memory.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1000 // Maximum buffer size, adjust as needed

void my_vprintf(const char *format, va_list args, char *buffer) {
    size_t buffer_index = 0;
    while (*format != '\0') {
        if (*format == '%') {
            format++; // Move past the '%'
            switch (*format) {
                case 'd': {
                    int value = va_arg(args, int);
                    buffer_index += snprintf(buffer + buffer_index, MAX_BUFFER_SIZE - buffer_index, "%d", value);
                    break;
                }
                case 's': {
                    char *value = va_arg(args, char*);
                    buffer_index += snprintf(buffer + buffer_index, MAX_BUFFER_SIZE - buffer_index, "%s", value);
                    break;
                }
                case 'f': {
                    double value = va_arg(args, double);
                    buffer_index += snprintf(buffer + buffer_index, MAX_BUFFER_SIZE - buffer_index, "%.2f", value);
                    break;
                }
                // Add more cases for other format specifiers as needed

                default:
                    buffer[buffer_index++] = *format;
            }
        } else {
            buffer[buffer_index++] = *format;
        }
        format++;
    }
    // Null-terminate the buffer
    buffer[buffer_index] = '\0';
}

void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char *buffer = (char *)malloc(MAX_BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    my_vprintf(format, args, buffer);
    va_end(args);

    printf("%s", buffer);
    free(buffer);
}

int main() {
    my_printf("This is a custom printf function: %d %s %.2f\n", 10, "hello", 3.14159);
    return 0;
}
