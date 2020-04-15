//这个文件中定义了相关位操作的宏

/*
下面的宏用来从数字中取出某些特定位
*/
#define _bitm(v, a, b, size) (((v) << ((size) - (b)-1)) >> ((size) - (b)-1 + (a))) //辅助宏
#define bit1(v, a) (((v) >> (a)) & 0b1)											   //取一个数字的第a位
#define bitm64(v, a, b) _bitm(v, a, b, 64)										   //取一个64位数字的第[a:b]位
#define bitm(v, a, b) _bitm(v, a, b, 32)										   //取一个32位数字的第[a:b]位
#define bitm16(v, a, b) _bitm(v, a, b, 16)										   //取一个16位数字的第[a:b]位
#define bitm8(v, a, b) _bitm(v, a, b, 8)										   //取一个8位数字的第[a:b]位
#define bitm4(v, a, b) _bitm(v, a, b, 4)										   //取一个4位数字的第[a:b]位

/*
下面的宏用来处理符号扩展
符号扩展： 将一个不足32位的数字连同符号位一起扩展到32位
*/
#define val_bits(val) ((uint32)sizeof(val) << 3)											  //通过数字类型类型计算一个数字的位数
#define sign_extend(val, val_bits) (((int32)(val) << (32 - (val_bits))) >> (32 - (val_bits))) //带符号扩展，传递值和值当前的位数
#define sign_extend_e(val) (((int32)(val) << (32 - val_bits(val))) >> (32 - val_bits(val)))	  //带符号扩展，传递值和值当前的位数 (注意：如果val的位数不是8、16请调用sign_extend手动传递位数)

/*
下面的命令用来简化指令读取
*/
#define inst_bm(from, to) bitm(inst, from, to)	//读取指令的某些位
#define inst_b1(pos) bit1(inst, pos)			//读取指令的某一位
#define inst_b4(from) inst_bm(from, from + 3)	//读取指令的某4位
#define inst_b8(from) inst_bm(from, from + 7)	//读取指令的某8位
#define inst_b16(from) inst_bm(from, from + 15) //读取指令的某16位
#define inst_b24(from) inst_bm(from, from + 23) //读取指令的某24位