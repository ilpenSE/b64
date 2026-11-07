#include "b64.h"

#define BW_IMPLEMENTATION
#include "bw.h"

#include "str.h"

char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64_table_find(char c)
{
  void *r = memchr(b64_table, c, 64);
  if (!r) return -1;
  return (char*)r - b64_table;
}

String b64_encode(StringView input, bool omit_trailing_equals)
{
  String s = str_new(input.count);
  BitReader br = {.buf=(const bw_byte_t*)input.data, .pos=input.count*8};

  uint8_t trailing_equals = 0;

  for (size_t i = 0; i < br.pos; i += 6) {
    uint8_t index = 0;
    if (i + 6 > br.pos) {
      index = br_gets(&br, i, br.pos - i);
      index <<= 6 - (br.pos - i);
      if (!omit_trailing_equals) trailing_equals += (6 - (br.pos - i))/2;
    } else {
      index = br_gets(&br, i, 6);
    }
    if (!str_append(&s, b64_table[index])) goto fail;
  }

  if (!omit_trailing_equals) {
    for (uint8_t i = 0; i < trailing_equals; i++) {
      if (!str_append(&s, '=')) goto fail;
    }
  }

  return s;
fail:
  str_free(&s);
  return STR_INVALID;
}

String b64_decode(StringView encoded, bool omit_trailing_equals)
{
  BitWriter bw = bw_new(encoded.count);
  if (!omit_trailing_equals && encoded.count % 4 != 0) return STR_INVALID;

  for (size_t i = 0; i < encoded.count; i++) {
    if (!omit_trailing_equals) {
      if (encoded.data[i] == '=') continue;
    }
    int8_t idx = b64_table_find(encoded.data[i]);
    if (bw_puts(&bw, idx, 6) != 6) return STR_INVALID;
  }

  return (String){.data=(char *)bw.buf, .count=bw.pos >> 3, .capacity=bw.capacity};
}
