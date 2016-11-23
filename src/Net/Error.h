#ifndef ERROR_H
#define ERROR_H
#include <syslog.h>
#include <stdio.h>
#include <errno.h>

#include <string>

#include "../String.h"

namespace yak
{
    
    class Error
    {
    public:
        Error() = delete;
        ~Error() = delete;

        static bool is_daemon() { return false;};

        // Nonfatal error related to system call
        // Print message and return
        template<typename ... Args>
        static
        void
        err_ret(const char *fmt, Args ... args)
        {
            err_doit(1, LOG_INFO, fmt, args ...);
            return;
        }

        // Fatal error related to system call
        // Print message and terminate
        template<typename ... Args>
        static
        void
        err_sys(const char *fmt, Args ... args)
        {
            err_doit(1, LOG_ERR, fmt, args ...);
            exit(1);
        }

        // Fatal error related to system call
        // Print message, dump core, and terminate
        template<typename ... Args>
        static
        void
        err_dump(const char *fmt, Args ... args)
        {
            err_doit(1, LOG_ERR, fmt, args ...);
            abort();  // dump core and terminate 
            exit(1);
        }

        // Nonfatal error unrelated to system call
        // Print message and return
        template<typename ... Args>
        static
        void
        err_msg(const char *fmt, Args ... args)
        {
            err_doit(0, LOG_INFO, fmt, args ...);
            return;
        }

        // Fatal error unrelated to system call
        // Print message and terminate
        template<typename ... Args>
        static
        void
        err_quit(const char *fmt, Args ... args)
        {
            err_doit(0, LOG_ERR, fmt, args ...);
            exit(1);
        }

    private:
        template<typename ... Args>
        static
        void
        err_doit(int errnoflag, int level, const char *fmt, Args ... args)
        {
            int errno_save = errno;
            std::string buf = string_format(fmt, args ...);

            if (errnoflag)
                buf = buf + string_format(": %s", strerror(errno_save));
            buf = buf + "\n";
            if (is_daemon()) {
                syslog(level, buf.c_str());
            } else {
                fflush(stdout);		
                fputs(buf.c_str(), stderr);
                fflush(stderr);
            }
        }

    };
}
#endif
