#pragma once

#include "str.h"
#include <stddef.h>

String b64_encode(StringView input, bool omit_trailing_equals);
String b64_decode(StringView input, bool omit_trailing_equals);
