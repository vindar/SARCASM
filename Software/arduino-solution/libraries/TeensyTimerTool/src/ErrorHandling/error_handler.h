#pragma once

#include "Stream.h"
#include "error_codes.h"

namespace TeensyTimerTool
{
    class ErrorHandler
    {
     public:
        ErrorHandler(Stream &s);
        void operator()(errorCode code) const;

     protected:
        Stream &stream;
    };
} // namespace TeensyTimerTool