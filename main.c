#include <stdio.h>
#include <stddef.h>
#define STR_IMPLEMENTATION
#include "str.h"
#include "b64.h"

int main(void)
{
  StringView input = SVL("abcdefe");

  String encoded = b64_encode(input, true);
  printf("encoded = '" STR_FMT "'\n", STR_ARG(encoded));

  String decoded = b64_decode(SVS(encoded), true);
  printf("decoded = '" STR_FMT "'\n", STR_ARG(decoded));

  str_free(&decoded);
  str_free(&encoded);
}
