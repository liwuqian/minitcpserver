#pragma once

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string.h>

namespace myspace {
    namespace Log {
        const static uint32_t max_time_string_length = 32;
        const static uint32_t max_message_length = 4096;
        const static uint32_t max_file_string_length = 256;
        const static uint32_t max_function_string_length = 96;
        const static uint32_t log_buffer_size = 65000;
        const static uint32_t max_d_length = 128;
        const static uint32_t max_ld_length = 32;
        const static uint32_t sleep_for_logging = 10;
        const static uint32_t sleep_ctr_for_logging = 10000;
        const static uint32_t sleep_for_terminate = 10;
        const static uint32_t max_log_source = 48;
        const static uint32_t max_log_level = 32;
        const static uint32_t max_file_path_length = 256;

        const static std::string log_for_testing = "log/test_for_logging.log";
        const static char log_default_path[] = "log.txt";

        const static char log_source[] = "uMR/Common/MRLog";
        const static uint32_t log_id = 404040;

        struct MsgInfo {
            char path[max_file_path_length];
            char message[max_message_length];
            char func[max_function_string_length];
            char file[max_file_string_length];
            char source[max_log_source];
            char level[max_log_level];
            char time[max_time_string_length];
            uint32_t mode;
            uint32_t length;
            uint32_t line;
            uint32_t log_id;
            uint32_t pid;

            MsgInfo():line(0), log_id(0), pid(0) {
                memset(&message, 0x00, max_message_length);
                memset(&file, 0x00, max_file_string_length);
                memset(&func, 0x00, max_function_string_length);
                memset(&source, 0x00, max_log_source);
                memset(&level, 0x00, max_log_level);
                memset(&time, 0x00, max_time_string_length);
                memcpy(path, log_default_path, max_file_path_length);
            }
        };
    }
}

