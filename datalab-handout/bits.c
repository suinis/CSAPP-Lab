/*
 * CS:APP Data Lab
 *
 * <崔志伟-SA23225194>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif
// 1
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y)
{
  int allis_one = x & y;           // 找到全为1的位
  int allis_zero = (~x) & (~y);    // 找到全为0的位
  int notall_one = ~allis_one;     // 全为1的位为0
  int notall_zero = ~allis_zero;   // 全为0的为为0
  return notall_one & notall_zero; // 全为1和全为0的某一位中，必在一个变量中为0，取&必为0，剩下为1的就是x,y中不全为0/1的位
}
/*
 * tmin - return minimum two's complement integer  //返回最小二进制补码整数
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void)
{
  int min8_t = 128; // 0x00 00 00 80
  return min8_t << 24;
}
// 2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x)
{
  // return !((~x) + (~x));
  return !((x + 1) ^ (~x)) & !!~x; //!!~x排除掉0xffffffff的情况
}
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x)
{
  int allodd8_t = 0xAA;
  // int allodd16_t = (allodd8_t << 8) + allodd8_t;
  // int allodd32_t = (allodd16_t << 16) + allodd16_t; //0xAAAAAAAA
  // return !((allodd32_t & x) ^ allodd32_t); //(allodd32_t & x)若x奇数位全为1，则结果一定与0xAAAAAAAA相同
  return !(((x & (x >> 8) & (x >> 16) & (x >> 24)) & allodd8_t) ^ allodd8_t);
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x)
{
  return (~x) + 1;
}
// 3
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x)
{
  int asciiDigitMin = 0x30;
  int asciiDigitMax = 0x39;
  //>>31:只取符号位判断是否在指定区间
  return !(((~asciiDigitMin + 1) + x) >> 31) & !(((~x + 1) + asciiDigitMax) >> 31);
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z)
{
  int con = ~!x + 1; // x为0时：全为1。否则：全为0
  return (y & (~con)) | (z & con);
}
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y)
{
  int min = 0x80 << 24;
  int symbol_x = x >> 31;
  int symbol_y = y >> 31;
  // 符号相同
  int symbol_equal = !(symbol_x ^ symbol_y) & (~y + x) >> 31;
  // int symbol_equal = !(symbol_x ^ symbol_y) & ((~x + 1) + y) >> 31;
  //(~y + x)是精髓：为什么(~x + 1) + y，~x + y，都不行？
  // 原本计算-x + y应为：(~x + 1) + y，但在x = 0x80000000时，按位取反后，会因为加上1，从正数溢出为负数的情况。

  // 符号不同
  int symbol_notequal = symbol_x & (!symbol_y);
  return !(x ^ min) | !(x ^ y) | symbol_equal | symbol_notequal;
}

// 4
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x)
{
  // return (((~x + 1) >> 31) ^ (x >> 31)) & 1; //负数右移前面添1，导致&1时结果不能必为0/1
  return (((~x) & (~(~x + 1))) >> 31) & 1; //负数右移前面补1，需要将负数右移比价转换为正数右移比较
}

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x)
{
  int temp = x ^ (x >> 31); // 将x转为正数（-1 -> 0）, （1000，0001 -> 0111,1110），后面计算位数需要（负数符号位为1）
  int isZero = !temp;
  int notZeroMask = (!(!temp) << 31) >> 31; //0xffffffff/0：确定最终返回值中的某些位是否为 1，为0表示x就是0
  int bit_16, bit_8, bit_4, bit_2, bit_1;
  bit_16 = !(!(temp >> 16)) << 4; //计算高16位是否有位
  temp = temp >> bit_16; //如果有则右移16位，查看高16位情况。没有则查看低16位情况
  bit_8 = !(!(temp >> 8)) << 3; 
  temp = temp >> bit_8;
  bit_4 = !(!(temp >> 4)) << 2;
  temp = temp >> bit_4;
  bit_2 = !(!(temp >> 2)) << 1;
  temp = temp >> bit_2;
  bit_1 = !(!(temp >> 1));
  temp = bit_16 + bit_8 + bit_4 + bit_2 + bit_1 + 2; //+2：符号位+至少一个非0位
  return isZero | (temp & notZeroMask); //0的话返回1，其他正常返回
}

// float
/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf)
{
  int exp = (uf & 0x7f800000) >> 23; //获取单精度浮点数的阶码，左移方面后面自加1
  int sign = uf & (1 << 31);  //获取符号位，符号位在第一位，方面后面按位或后return
  if (exp == 0) //非规格化数/0，非规格化数：指数部分全为0，尾数部分不全为0的数
    return uf << 1 | sign; //非规格化数，尾数左移一位即可
  if (exp == 255) //特殊值（正负无穷大）直接返回
    return uf;
  exp++; //到这说明是规格化数，*2操作：阶码+1
  if (exp == 255) //*2后，结果会溢出，返回对应符号溢出值
    return 0x7f800000 | sign;
  return (exp << 23) | (uf & 0x807fffff); //否则正常返回
}

/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf)
{
  int exp = (uf >> 23) & 0xFF;
  int frac = uf & 0x007FFFFF;
  int sign = uf & 0x80000000;
  int bias = exp - 127; //去掉偏置值：2^(阶码位数-1) - 1

  if (exp == 255 || bias > 30) //过大超出int表示范围
  {
    return 0x80000000u;
  }
  else if (!exp || bias < 0) //过小int无法表示
  {
    return 0;
  }

  //在尾数前加上隐藏位1
  frac = frac | 1 << 23;

  //根据偏置将尾数向左或向右移动，以将其转换为整数。
  if (bias > 23)
  {
    //阶码>23需要，将尾数左移获取int值
    frac = frac << (bias - 23);
  }
  else
  {
    //阶码<=23,需要将尾数右移对应位
    frac = frac >> (23 - bias);
  }

  if (sign)
  {
    //负数
    frac = ~(frac) + 1;
  }

  return frac;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x)
{
  if (!!(x >> 31)) //负数：2的负数幂太小不在int表示范围内
    return 0;
  if (!(x >> 7)) //若x<=127（254 - 127），阶码值最大为254。说明在单精度表示范围内
    return (x + 127) << 23;
  return (0xff << 23); //不在返回正无穷大
}
