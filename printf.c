#include "main.h"

#define BUFF_SIZE 1024

typedef struct {
    int flags;
    int width;
    int precision;
    int size;
} FormatSpec;

int custom_printf(const char *format, ...);

static void print_buffer(char buffer[], int *buff_ind);

int custom_printf(const char *format, ...)
{
    int printed_chars = 0;
    va_list args;
    char buffer[BUFF_SIZE];
    int buff_ind = 0;

    va_start(args, format);

    for (int i = 0; format && format[i] != '\0'; i++)
    {
        if (format[i] != '%')
        {
            buffer[buff_ind++] = format[i];
            if (buff_ind == BUFF_SIZE)
            {
                print_buffer(buffer, &buff_ind);
            }
            printed_chars++;
        }
        else
        {
            print_buffer(buffer, &buff_ind);
            FormatSpec spec = {0};
            spec.flags = get_flags(format, &i);
            spec.width = get_width(format, &i, args);
            spec.precision = get_precision(format, &i, args);
            spec.size = get_size(format, &i);
            i++;
            int printed = handle_print(format, &i, args, buffer, spec.flags, spec.width, spec.precision, spec.size);
            if (printed == -1)
            {
                va_end(args);
                return -1;
            }
            printed_chars += printed;
        }
    }

    print_buffer(buffer, &buff_ind);

    va_end(args);

    return printed_chars;
}

static void print_buffer(char buffer[], int *buff_ind)
{
    if (*buff_ind > 0)
    {
        write(1, buffer, *buff_ind);
    }

    *buff_ind = 0;
}

