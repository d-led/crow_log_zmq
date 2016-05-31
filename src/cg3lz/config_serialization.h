#pragma once

#include "../common/config.h"

#include <picojson_serializer.h>

namespace picojson {
    namespace convert {
        template <class Archive>
        void json(Archive& ar, config& c) {
            ar & picojson::convert::member("logging", c.logging);
            ar & picojson::convert::member("port", c.port);
            ar & picojson::convert::member("zeromq_log_port", c.zeromq_log_port);
            ar & picojson::convert::member("log_path", c.log_path);
            ar & picojson::convert::member("max_file_size", c.max_file_size);
            ar & picojson::convert::member("max_number_of_files", c.max_number_of_files);
            ar & picojson::convert::member("flush_period_seconds", c.flush_period_seconds);
        }
    }
}
