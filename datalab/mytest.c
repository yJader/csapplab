#include <stdio.h>

int floatFloat2Int(unsigned uf) {
  unsigned s = uf & (1<<31);
  unsigned e = (uf & 0x7f800000)>>23;
  unsigned frac = uf & 0x007fffff;

  int E = e - 127; // 阶码真值
  if(e == 0) E = 0;
  else frac = frac | 0x00800000; //隐藏位
  if(e == 255 || E > 31) return 0x80000000u; //超出int表示范围
  if(E<0) return 0; //小数 直接返回

  int value = E>23 ? frac<<(E-23) : frac>>(23-E); //注意: int右移会进行模运算
  if(s) value*=-1; // int为补码, frac为原码

  return value; // frac为小数, 已经右移23位
}

int main() {
    printf("%x\t %d\n", floatFloat2Int(0x800000), floatFloat2Int(0x800000));
}