#include "SkpUtil.h"
#include <string.h>
#include <HardwareSerial.h>

#define __ornull(a) ((a) ? (a) : "<null>")

void _skp_log(const char *prefix, const char *message, const char *file, const char *func, const int line)
{
    Serial.printf("[%s] %s at %s:%i > %s\n", __ornull(prefix), __ornull(func), __ornull(file), line, __ornull(message));
}

void _skp_log(const char *prefix, const String& message, const char *file, const char *func, const int line)
{
    _skp_log(prefix, message.c_str(), file, func, line);
}

void _skp_logf(const char *prefix, const char *file, const char *func, const int line)
{
    Serial.printf("[%s] %s at %s:%i > ", __ornull(prefix), __ornull(func), __ornull(file), line);
}

void _skp_halt()
{
    while (true) delay(100);
}

bool skp_str2uint(String s, int *result)
{
    if (s.isEmpty())
    {
        skp_log_error("Empty string");
        return false;
    }

    const char *str = s.c_str();
    if (str[0] == '-')
    {
        skp_log_error("Minus found");
        return false;
    }

    if (s.length() - 1 != strcspn(s.c_str(), "0123456789"))
    {
        skp_log_error("Failed to parse string");
        return false;
    }

    if (result)
    {
        *result = (uint)s.toInt();
    }

    return true;
}

bool skp_str2float(String s, float *result)
{
    if (s.isEmpty())
    {
        skp_log_error("Empty string");
        return false;
    }

    //! Won't work
    /*const char *str = s.c_str(); 
    if (str[0] == '-')
    {
        str++;
    }

    int invalid = strcspn(str, "0123456789");
    if (str[invalid] != '.')
    {
        skp_log_error("Failed to parse string");
        return false;
    }
    str += invalid;

    if (str[strcspn(str, "0123456789")] != '\0')
    {
        skp_log_error("Failed to parse string");
        return false;
    }*/
    
    //? Possibly correct, however no debug means bleh :3
    /*const char *str = s.c_str();
    int offset = 0;
    if (str[0] == '-')
    {
        offset = 1;
        str++;
    }

    int i;
    if ((i = s.indexOf('.')) != -1)
    {
        if (strcspn(str, "0123456789") != i + offset || strcspn(str + i, "0123456789") != s.length() - i - offset)
        {
            skp_log_error("Failed to parse string");
            return false;
        }
    }
    else
    {
        if (strcspn(str + i, "0123456789") != s.length() - offset)
        {
            skp_log_error("Failed to parse string");
            return false;
        }
    }*/

    //? The simple, dirty way to do it
    if (s[s.length() - 1] == '.')
    {
        skp_log_error("Trailing dot");
        return false;
    }

    bool flag = false;
    for (size_t i = 0; i < s.length(); i++)
    {
        switch (s.charAt(i))
        {
        case '-':
            if (i != 0)
            {
                skp_log_error("Found minus not at 0");
                return false;
            }
            break;

        case '.':
            if (flag)
            {
                skp_log_error("Found two dots");
                return false;
            }
            flag = true;
            break;
        
        case '0' ... '9':
            break;

        default:
            skp_log_error("Invalid char");
            return false;
        }
    }
    

    if (result)
    {
        *result = s.toFloat();
    }

    return true;
}