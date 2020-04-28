#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include "mrp.h"

#include "printf.h"

#define PRINTF_NTOA_BUFFER_SIZE 32u
#define PRINTF_FTOA_BUFFER_SIZE 32u
#define PRINTF_DEFAULT_FLOAT_PRECISION 6u
#define PRINTF_MAX_FLOAT 1e9

#define FLAGS_ZEROPAD (1u << 0u)
#define FLAGS_LEFT (1u << 1u)
#define FLAGS_PLUS (1u << 2u)
#define FLAGS_SPACE (1u << 3u)
#define FLAGS_HASH (1u << 4u)
#define FLAGS_UPPERCASE (1u << 5u)
#define FLAGS_CHAR (1u << 6u)
#define FLAGS_SHORT (1u << 7u)
#define FLAGS_LONG (1u << 8u)
#define FLAGS_LONG_LONG (1u << 9u)
#define FLAGS_PRECISION (1u << 10u)
#define FLAGS_ADAPT_EXP (1u << 11u)

typedef void (*out_fct_type)(char character, void *buffer, size_t idx, size_t maxlen); //输出函数

// wrapper (used as buffer) for output function type
typedef struct
{
	void (*fct)(char character, void *arg);
	void *arg;
} out_fct_wrap_type;

#define arg_va(arg, type) (*(type *)arg_get_item((arg), sizeof(type)))

/*
获取参数
参数：
	arg_list		参数列表对象
	len				参数长度（单位：字节）
*/
void *arg_get_item(arg_list *arg, int32 len)
{
	//取得参数所在位置
	uint8 *pos = (uint8 *)arg + 4 + arg->n;
	//计算下一个参数位置
	if (len <= 4 && arg->n < 16)
		arg->n += 4;
	else if (arg->n < 16)
	{
		while (arg->n < 16 && len > 0)
		{
			arg->n += 4;
			len -= 4;
		}
		if (len > 0)
			arg->n += len;
	}
	else
		arg->n += len;
	//返回指针
	return pos;
}

// internal buffer output
static inline void _out_buffer(char character, void *buffer, size_t idx, size_t maxlen)
{
	if (idx < maxlen)
	{
		((char *)buffer)[idx] = character;
	}
}

// internal null output
static inline void _out_null(char character, void *buffer, size_t idx, size_t maxlen)
{
	(void)character;
	(void)buffer;
	(void)idx;
	(void)maxlen;
}

// internal _putchar wrapper
static inline void _out_char(char character, void *buffer, size_t idx, size_t maxlen)
{
	(void)buffer;
	(void)idx;
	(void)maxlen;
	if (character)
	{
		putchar(character);
	}
}

// internal output function wrapper
static inline void _out_fct(char character, void *buffer, size_t idx, size_t maxlen)
{
	(void)idx;
	(void)maxlen;
	if (character)
	{
		((out_fct_wrap_type *)buffer)->fct(character, ((out_fct_wrap_type *)buffer)->arg);
	}
}

// internal secure strlen
// return The length of the string (excluding the terminating 0) limited by 'maxsize'
static inline unsigned int _strnlen_s(char *str, size_t maxsize)
{
	char *s;
	for (s = str; *s && maxsize--; ++s)
		;
	return (unsigned int)(s - str);
}

// internal test if char is a digit (0-9)
// return true if char is a digit
static inline bool _is_digit(char ch)
{
	return (ch >= '0') && (ch <= '9');
}

// internal ASCII string to unsigned int conversion
static unsigned int _atoi(char **str)
{
	unsigned int i = 0U;
	while (_is_digit(**str))
	{
		i = i * 10U + (unsigned int)(*((*str)++) - '0');
	}
	return i;
}

// output the specified string in reverse, taking care of any zero-padding
static size_t _out_rev(out_fct_type out, char *buffer, size_t idx, size_t maxlen, char *buf, size_t len, unsigned int width, unsigned int flags)
{
	size_t start_idx = idx;

	// pad spaces up to given width
	if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD))
	{
		for (size_t i = len; i < width; i++)
		{
			out(' ', buffer, idx++, maxlen);
		}
	}

	// reverse string
	while (len)
	{
		out(buf[--len], buffer, idx++, maxlen);
	}

	// append pad spaces up to given width
	if (flags & FLAGS_LEFT)
	{
		while (idx - start_idx < width)
		{
			out(' ', buffer, idx++, maxlen);
		}
	}

	return idx;
}

// internal itoa format
static size_t _ntoa_format(out_fct_type out, char *buffer, size_t idx, size_t maxlen, char *buf, size_t len, bool negative, unsigned int base, unsigned int prec, unsigned int width, unsigned int flags)
{
	// pad leading zeros
	if (!(flags & FLAGS_LEFT))
	{
		if (width && (flags & FLAGS_ZEROPAD) && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE))))
		{
			width--;
		}
		while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE))
		{
			buf[len++] = '0';
		}
		while ((flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_NTOA_BUFFER_SIZE))
		{
			buf[len++] = '0';
		}
	}

	// handle hash
	if (flags & FLAGS_HASH)
	{
		if (!(flags & FLAGS_PRECISION) && len && ((len == prec) || (len == width)))
		{
			len--;
			if (len && (base == 16U))
			{
				len--;
			}
		}
		if ((base == 16U) && !(flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE))
		{
			buf[len++] = 'x';
		}
		else if ((base == 16U) && (flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE))
		{
			buf[len++] = 'X';
		}
		else if ((base == 2U) && (len < PRINTF_NTOA_BUFFER_SIZE))
		{
			buf[len++] = 'b';
		}
		if (len < PRINTF_NTOA_BUFFER_SIZE)
		{
			buf[len++] = '0';
		}
	}

	if (len < PRINTF_NTOA_BUFFER_SIZE)
	{
		if (negative)
		{
			buf[len++] = '-';
		}
		else if (flags & FLAGS_PLUS)
		{
			buf[len++] = '+'; // ignore the space if the '+' exists
		}
		else if (flags & FLAGS_SPACE)
		{
			buf[len++] = ' ';
		}
	}

	return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

// internal itoa for 'long' type
static size_t _ntoa_long(out_fct_type out, char *buffer, size_t idx, size_t maxlen, unsigned long value, bool negative, unsigned long base, unsigned int prec, unsigned int width, unsigned int flags)
{
	char buf[PRINTF_NTOA_BUFFER_SIZE];
	size_t len = 0U;

	// no hash for 0 values
	if (!value)
	{
		flags &= ~FLAGS_HASH;
	}

	// write if precision != 0 and value is != 0
	if (!(flags & FLAGS_PRECISION) || value)
	{
		do
		{
			char digit = (char)(value % base);
			buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
			value /= base;
		} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
	}

	return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}

static size_t _ntoa_long_long(out_fct_type out, char *buffer, size_t idx, size_t maxlen, unsigned long long value, bool negative, unsigned long long base, unsigned int prec, unsigned int width, unsigned int flags)
{
	char buf[PRINTF_NTOA_BUFFER_SIZE];
	size_t len = 0U;

	// no hash for 0 values
	if (!value)
	{
		flags &= ~FLAGS_HASH;
	}

	// write if precision != 0 and value is != 0
	if (!(flags & FLAGS_PRECISION) || value)
	{
		do
		{
			char digit = (char)(value % base);
			buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
			value /= base;
		} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
	}

	return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}

// forward declaration so that _ftoa can switch to exp notation for values > PRINTF_MAX_FLOAT
static size_t _etoa(out_fct_type out, char *buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags);

// internal ftoa for fixed decimal floating point
static size_t _ftoa(out_fct_type out, char *buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags)
{
	char buf[PRINTF_FTOA_BUFFER_SIZE];
	size_t len = 0U;
	double diff = 0.0;

	// powers of 10
	static double pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

	// test for special values
	if (value != value)
		return _out_rev(out, buffer, idx, maxlen, "nan", 3, width, flags);
	if (value < -DBL_MAX)
		return _out_rev(out, buffer, idx, maxlen, "fni-", 4, width, flags);
	if (value > DBL_MAX)
		return _out_rev(out, buffer, idx, maxlen, (flags & FLAGS_PLUS) ? "fni+" : "fni", (flags & FLAGS_PLUS) ? 4U : 3U, width, flags);

	// test for very large values
	// standard printf behavior is to print EVERY whole number digit -- which could be 100s of characters overflowing your buffers == bad
	if ((value > PRINTF_MAX_FLOAT) || (value < -PRINTF_MAX_FLOAT))
	{
		return _etoa(out, buffer, idx, maxlen, value, prec, width, flags);
	}

	// test for negative
	bool negative = false;
	if (value < 0)
	{
		negative = true;
		value = 0 - value;
	}

	// set default precision, if not set explicitly
	if (!(flags & FLAGS_PRECISION))
	{
		prec = PRINTF_DEFAULT_FLOAT_PRECISION;
	}
	// limit precision to 9, cause a prec >= 10 can lead to overflow errors
	while ((len < PRINTF_FTOA_BUFFER_SIZE) && (prec > 9U))
	{
		buf[len++] = '0';
		prec--;
	}

	int whole = (int)value;
	double tmp = (value - whole) * pow10[prec];
	unsigned long frac = (unsigned long)tmp;
	diff = tmp - frac;

	if (diff > 0.5)
	{
		++frac;
		// handle rollover, e.g. case 0.99 with prec 1 is 1.0
		if (frac >= pow10[prec])
		{
			frac = 0;
			++whole;
		}
	}
	else if (diff < 0.5)
	{
	}
	else if ((frac == 0U) || (frac & 1U))
	{
		// if halfway, round up if odd OR if last digit is 0
		++frac;
	}

	if (prec == 0U)
	{
		diff = value - (double)whole;
		if ((!(diff < 0.5) || (diff > 0.5)) && (whole & 1))
		{
			// exactly 0.5 and ODD, then round up
			// 1.5 -> 2, but 2.5 -> 2
			++whole;
		}
	}
	else
	{
		unsigned int count = prec;
		// now do fractional part, as an unsigned number
		while (len < PRINTF_FTOA_BUFFER_SIZE)
		{
			--count;
			buf[len++] = (char)(48U + (frac % 10U));
			if (!(frac /= 10U))
			{
				break;
			}
		}
		// add extra 0s
		while ((len < PRINTF_FTOA_BUFFER_SIZE) && (count-- > 0U))
		{
			buf[len++] = '0';
		}
		if (len < PRINTF_FTOA_BUFFER_SIZE)
		{
			// add decimal
			buf[len++] = '.';
		}
	}

	// do whole part, number is reversed
	while (len < PRINTF_FTOA_BUFFER_SIZE)
	{
		buf[len++] = (char)(48 + (whole % 10));
		if (!(whole /= 10))
		{
			break;
		}
	}

	// pad leading zeros
	if (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD))
	{
		if (width && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE))))
		{
			width--;
		}
		while ((len < width) && (len < PRINTF_FTOA_BUFFER_SIZE))
		{
			buf[len++] = '0';
		}
	}

	if (len < PRINTF_FTOA_BUFFER_SIZE)
	{
		if (negative)
		{
			buf[len++] = '-';
		}
		else if (flags & FLAGS_PLUS)
		{
			buf[len++] = '+'; // ignore the space if the '+' exists
		}
		else if (flags & FLAGS_SPACE)
		{
			buf[len++] = ' ';
		}
	}

	return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

// internal ftoa variant for exponential floating-point type, contributed by Martijn Jasperse <m.jasperse@gmail.com>
static size_t _etoa(out_fct_type out, char *buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags)
{
	// check for NaN and special values
	if ((value != value) || (value > DBL_MAX) || (value < -DBL_MAX))
	{
		return _ftoa(out, buffer, idx, maxlen, value, prec, width, flags);
	}

	// determine the sign
	bool negative = value < 0;
	if (negative)
	{
		value = -value;
	}

	// default precision
	if (!(flags & FLAGS_PRECISION))
	{
		prec = PRINTF_DEFAULT_FLOAT_PRECISION;
	}

	// determine the decimal exponent
	// based on the algorithm by David Gay (https://www.ampl.com/netlib/fp/dtoa.c)
	union {
		uint64_t U;
		double F;
	} conv;

	conv.F = value;
	int exp2 = (int)((conv.U >> 52U) & 0x07FFU) - 1023;			 // effectively log2
	conv.U = (conv.U & ((1ULL << 52U) - 1U)) | (1023ULL << 52U); // drop the exponent so conv.F is now in [1,2)
	// now approximate log10 from the log2 integer part and an expansion of ln around 1.5
	int expval = (int)(0.1760912590558 + exp2 * 0.301029995663981 + (conv.F - 1.5) * 0.289529654602168);
	// now we want to compute 10^expval but we want to be sure it won't overflow
	exp2 = (int)(expval * 3.321928094887362 + 0.5);
	double z = expval * 2.302585092994046 - exp2 * 0.6931471805599453;
	double z2 = z * z;
	conv.U = (uint64_t)(exp2 + 1023) << 52U;
	// compute exp(z) using continued fractions, see https://en.wikipedia.org/wiki/Exponential_function#Continued_fractions_for_ex
	conv.F *= 1 + 2 * z / (2 - z + (z2 / (6 + (z2 / (10 + z2 / 14)))));
	// correct for rounding errors
	if (value < conv.F)
	{
		expval--;
		conv.F /= 10;
	}

	// the exponent format is "%+03d" and largest value is "307", so set aside 4-5 characters
	unsigned int minwidth = ((expval < 100) && (expval > -100)) ? 4U : 5U;

	// in "%g" mode, "prec" is the number of *significant figures* not decimals
	if (flags & FLAGS_ADAPT_EXP)
	{
		// do we want to fall-back to "%f" mode?
		if ((value >= 1e-4) && (value < 1e6))
		{
			if ((int)prec > expval)
			{
				prec = (unsigned)((int)prec - expval - 1);
			}
			else
			{
				prec = 0;
			}
			flags |= FLAGS_PRECISION; // make sure _ftoa respects precision
			// no characters in exponent
			minwidth = 0U;
			expval = 0;
		}
		else
		{
			// we use one sigfig for the whole part
			if ((prec > 0) && (flags & FLAGS_PRECISION))
			{
				--prec;
			}
		}
	}

	// will everything fit?
	unsigned int fwidth = width;
	if (width > minwidth)
	{
		// we didn't fall-back so subtract the characters required for the exponent
		fwidth -= minwidth;
	}
	else
	{
		// not enough characters, so go back to default sizing
		fwidth = 0U;
	}
	if ((flags & FLAGS_LEFT) && minwidth)
	{
		// if we're padding on the right, DON'T pad the floating part
		fwidth = 0U;
	}

	// rescale the float value
	if (expval)
	{
		value /= conv.F;
	}

	// output the floating part
	size_t start_idx = idx;
	idx = _ftoa(out, buffer, idx, maxlen, negative ? -value : value, prec, fwidth, flags & ~FLAGS_ADAPT_EXP);

	// output the exponent part
	if (minwidth)
	{
		// output the exponential symbol
		out((flags & FLAGS_UPPERCASE) ? 'E' : 'e', buffer, idx++, maxlen);
		// output the exponent value
		idx = _ntoa_long(out, buffer, idx, maxlen, (expval < 0) ? -expval : expval, expval < 0, 10, 0, minwidth - 1, FLAGS_ZEROPAD | FLAGS_PLUS);
		// might need to right-pad spaces
		if (flags & FLAGS_LEFT)
		{
			while (idx - start_idx < width)
				out(' ', buffer, idx++, maxlen);
		}
	}
	return idx;
}


static int32 _vsnprintf(out_fct_type out, char *buffer, size_t maxlen, char *format, uint8 *memBase, arg_list *arg)
{
	unsigned int flags, width, precision, n;
	size_t idx = 0u;

	char *testStr = "%+*d";

	if (!buffer)
	{
		out = _out_null;
	}

	while (*format)
	{
		// 格式化标识  %[flags][width][.precision][length]

		//不是就继续
		if (*format != '%')
		{
			out(*format, buffer, idx++, maxlen);
			format++;
			continue;
		}
		//遇到%先跳过%
		else
		{
			format++;
		}

		// 取falgs
		flags = 0u;
		do
		{
			switch (*format)
			{
			case '0':
				flags |= FLAGS_ZEROPAD;
				format++;
				n = 1u;
				break;
			case '-':
				flags |= FLAGS_LEFT;
				format++;
				n = 1u;
				break;
			case '+':
				flags |= FLAGS_PLUS;
				format++;
				n = 1u;
				break;
			case ' ':
				flags |= FLAGS_SPACE;
				format++;
				n = 1u;
				break;
			case '#':
				flags |= FLAGS_HASH;
				format++;
				n = 1u;
				break;
			default:
				n = 0u;
				break;
			}
		} while (n);

		// 取width
		width = 0u;
		if (_is_digit(*format))
		{
			width = _atoi(&format);
		}
		else if (*format == '*')
		{
			int w = arg_va(arg, int);
			if (w < 0)
			{
				flags |= FLAGS_LEFT;
				width = (unsigned int)-w;
			}
			else
			{
				width = (unsigned int)w;
			}
			format++;
		}

		// 取precision
		precision = 0u;
		if (*format == '.')
		{
			flags |= FLAGS_PRECISION;
			format++;
			if (_is_digit(*format))
			{
				precision = _atoi(&format);
			}
			else if (*format == '*')
			{
				int prec = (int)arg_va(arg, int);
				precision = prec > 0 ? (unsigned int)prec : 0u;
				format++;
			}
		}

		// 取length
		switch (*format)
		{
		case 'l':
			flags |= FLAGS_LONG;
			format++;
			if (*format == 'l')
			{
				flags |= FLAGS_LONG_LONG;
				format++;
			}
			break;
		case 'h':
			flags |= FLAGS_SHORT;
			format++;
			if (*format == 'h')
			{
				flags |= FLAGS_CHAR;
				format++;
			}
			break;
		case 't':
			flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
			format++;
			break;
		case 'j':
			flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
			format++;
			break;
		case 'z':
			flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
			format++;
			break;
		default:
			break;
		}

		//处理fmt
		switch (*format)
		{
		case 'd':
		case 'i':
		case 'u':
		case 'x':
		case 'X':
		case 'o':
		case 'b':
		{
			unsigned int base;
			if (*format == 'x' || *format == 'X')
			{
				base = 16u;
			}
			else if (*format == 'o')
			{
				base = 8u;
			}
			else if (*format == 'b')
			{
				base = 2u;
			}
			else
			{
				base = 10u;
				flags &= ~FLAGS_HASH;
			}
			if (*format == 'X')
			{
				flags |= FLAGS_UPPERCASE;
			}

			if ((*format != 'i') && (*format != 'd'))
			{
				flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
			}

			if (flags & FLAGS_PRECISION)
			{
				flags &= ~FLAGS_ZEROPAD;
			}

			if ((*format == 'i') || (*format == 'd'))
			{
				// signed
				if (flags & FLAGS_LONG_LONG)
				{
					long long value = arg_va(arg, long long);
					idx = _ntoa_long_long(out, buffer, idx, maxlen, (unsigned long long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
				}
				else if (flags & FLAGS_LONG)
				{
					long value = arg_va(arg, long);
					idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
				}
				else
				{
					int value = (flags & FLAGS_CHAR) ? (char)arg_va(arg, int) : (flags & FLAGS_SHORT) ? (short int)arg_va(arg, int) : arg_va(arg, int);
					idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
				}
			}
			else
			{
				// unsigned
				if (flags & FLAGS_LONG_LONG)
				{
					idx = _ntoa_long_long(out, buffer, idx, maxlen, arg_va(arg, unsigned long long), false, base, precision, width, flags);
				}
				else if (flags & FLAGS_LONG)
				{
					idx = _ntoa_long(out, buffer, idx, maxlen, arg_va(arg, unsigned long), false, base, precision, width, flags);
				}
				else
				{
					unsigned int value = (flags & FLAGS_CHAR) ? (unsigned char)arg_va(arg, unsigned int) : (flags & FLAGS_SHORT) ? (unsigned short int)arg_va(arg, unsigned int) : arg_va(arg, unsigned int);
					idx = _ntoa_long(out, buffer, idx, maxlen, value, false, base, precision, width, flags);
				}
			}
			format++;
			break;
		}
		case 'f':
		case 'F':
			if (*format == 'F')
				flags |= FLAGS_UPPERCASE;
			idx = _ftoa(out, buffer, idx, maxlen, arg_va(arg, double), precision, width, flags);
			format++;
			break;
		case 'e':
		case 'E':
		case 'g':
		case 'G':
			if ((*format == 'g') || (*format == 'G'))
				flags |= FLAGS_ADAPT_EXP;
			if ((*format == 'E') || (*format == 'G'))
				flags |= FLAGS_UPPERCASE;
			idx = _etoa(out, buffer, idx, maxlen, arg_va(arg, double), precision, width, flags);
			format++;
			break;
		case 'c':
		{
			unsigned int l = 1U;
			if (!(flags & FLAGS_LEFT))
			{
				while (l++ < width)
				{
					out(' ', buffer, idx++, maxlen);
				}
			}
			out((char)arg_va(arg, int), buffer, idx++, maxlen);
			// post padding
			if (flags & FLAGS_LEFT)
			{
				while (l++ < width)
				{
					out(' ', buffer, idx++, maxlen);
				}
			}
			format++;
			break;
		}

		case 's':
		{
			vmpt sp = arg_va(arg, uint32);
			char *p = (char *)(memBase + sp);
			unsigned int l = _strnlen_s(p, precision ? precision : (size_t)-1);
			if (flags & FLAGS_PRECISION)
			{
				l = (l < precision ? l : precision);
			}
			if (!(flags & FLAGS_LEFT))
			{
				while (l++ < width)
				{
					out(' ', buffer, idx++, maxlen);
				}
			}
			while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--))
			{
				out(*(p++), buffer, idx++, maxlen);
			}
			if (flags & FLAGS_LEFT)
			{
				while (l++ < width)
				{
					out(' ', buffer, idx++, maxlen);
				}
			}
			format++;
			break;
		}

		case 'p':
		{
			width = sizeof(void *) * 2U;
			flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
			bool is_ll = sizeof(uintptr_t) == sizeof(long long);
			if (is_ll)
			{
				idx = _ntoa_long_long(out, buffer, idx, maxlen, (uintptr_t)arg_va(arg, void *), false, 16U, precision, width, flags);
			}
			else
			{
				idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)((uintptr_t)arg_va(arg, void *)), false, 16U, precision, width, flags);
			}
			format++;
			break;
		}

		case '%':
			out('%', buffer, idx++, maxlen);
			format++;
			break;

		default:
			out(*format, buffer, idx++, maxlen);
			format++;
			break;
		}
	}

	// termination
	out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

	// return written chars without terminating \0
	return (int)idx;
}

int32 fmt_printf(char *format, uint8 *memBase, arg_list *arg)
{
	char buffer[1];
	return _vsnprintf(_out_char, buffer, (size_t)-1, format, memBase, arg);
}

int32 fmt_sprintf(char *buffer, char *format, uint8 *memBase, arg_list *arg)
{
	return _vsnprintf(_out_buffer, buffer, (size_t)-1, format, memBase, arg);
}

int32 snprintf_(char *buffer, uint32 count, char *format, uint8 *memBase, arg_list *arg)
{
	return _vsnprintf(_out_buffer, buffer, count, format, memBase, arg);
}