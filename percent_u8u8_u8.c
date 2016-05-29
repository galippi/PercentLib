#include <stdint.h>

uint8_t percent_u8u8_u8(uint8_t val, uint8_t div);

uint8_t percent_u8u8_u8(uint8_t val, uint8_t div)
{
  uint8_t integer = 0;
  uint8_t result = 0;
  if (div == 0)
    return 255;
  while (val >= div)
  {
    val -= div;
    integer++;
  }
  if (val != 0)
  { /* handling 0% + x*100% */
    uint8_t shift = 0;
    while (val < 128)
    {
      shift++;
      val = val << 1;
    }
    while (div < 128)
    {
      shift--;
      div = div << 1;
    }
    {
      uint8_t part = 100;
      while((part > 0) && (val != 0))
      {
        if (val >= div)
        {
          val -= div;
          result = result + part;
        }
        if (val < 128)
        {
          val = val << 1;
        }else
        {
          div = div >> 1;
        }
        part = part >> 1;
      }
    }
    while (shift != 0)
    {
      shift--;
      result = result >> 1;
    }
  }
  while(integer != 0)
  {
    if (result > 155)
      return 255;
    result = result + 100;
    integer--;
  }
  return result;
}

#include <stdio.h>
#include <math.h>

int main(int argc, const char **argv)
{
  (void)argc;
  (void)argv;
  unsigned i, j;
  double diff_max = 0;
  {
     volatile uint8_t res = percent_u8u8_u8(7, 10);
     (void)res;
  }
  for(i = 0; i < 256; i++)
  {
    for(j = 1; j < 256; j++)
    {
      uint8_t res = percent_u8u8_u8(i, j);
      double perc = (i * 100.0 / j);
      if (perc > 254)
      {
        perc = 255;
      }
      double diff = fabs(res - perc);
      printf("%3u/%3u=%3u %8.4lf %8.4lf\n", i, j, (unsigned)res, perc, diff);
      if (diff > diff_max)
      {
        diff_max = diff;
        printf("diff_max\n");
      }
      if (diff > 1.0)
      {
        printf("diff_large\n");
      }
    }
  }
}
