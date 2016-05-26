/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#ifndef _COMMON_H_
#define _COMMON_H_





/// 当前字符是数字
#define IsDigit(c)         (c >= '0' && c <= '9')

/// 当前字符是8进制数据
#define IsOctDigit(c)      (c >= '0' && c <= '7')

/// 当前字符是16进制数据
#define IsHexDigit(c)      (IsDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))

/// 当前字符是字母或者_
#define IsLetter(c)        ((c >= 'a' && c <= 'z') || (c == '_') || (c >= 'A' && c <= 'Z'))

/// 当前自负是否满足C的变量命名规则
#define IsLetterOrDigit(c) (IsLetter(c) || IsDigit(c))
#define IsIdentifier    IsLetterOrDigit

/// 当前字符是空白字符
#define IsSpace(c)  ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')



/// 取出当前信息的高4位
#define HIGH_4BIT(v)       ((v) >> (8 * sizeof(int) - 4) & 0x0f)
/// 取出当前信息的高3位
#define HIGH_3BIT(v)       ((v) >> (8 * sizeof(int) - 3) & 0x07)
/// 取出当前信息的高1位
#define HIGH_1BIT(v)       ((v) >> (8 * sizeof(int) - 1) & 0x01)
//#define ALIGN(size, align) ((size + align - 1) & (~(align - 1)))

#define BitGet(number, pos) ((number) >> (pos) & 1)     /// 用宏得到某数的某位
#define BitSet(number, pos) ((number) |= 1 << (pos))    /// 把某位置1
#define BitClr(number, pos) ((number) &= ~(1 << (pos))) /// 把某位清0
#define BitCpl(number, pos) ((number) ^= 1 << (pos))    /// 把number的POS位取反


///03: 得到指定地址上的一个字节或字
#define  MEM_B(x) (*((byte *)(x)))
#define  MEM_W(x) (*((word *)(x)))

///04: 求最大值和最小值
#define  MAX(x,y) (((x)>(y)) ? (x) : (y))
#define  MIN(x,y) (((x) < (y)) ? (x) : (y))

///05: 得到一个field在结构体(struct)中的偏移量
#define FPOS(type,field) ((dword)&((type *)0)->field)

///06: 得到一个结构体中field所占用的字节数
#define FSIZ(type,field) sizeof(((type *)0)->field)

///07: 按照LSB格式把两个字节转化为一个Word
#define FLIPW(ray) ((((word)(ray)[0]) * 256) + (ray)[1])

///08: 按照LSB格式把一个Word转化为两个字节
#define FLOPW(ray,val) (ray)[0] = ((val)/256); (ray)[1] = ((val) & 0xFF)

///09: 得到一个变量的地址（word宽度）
#define B_PTR(var)  ((byte *) (void *) &(var))
#define W_PTR(var)  ((word *) (void *) &(var))

///10: 得到一个字的高位和低位字节
#define WORD_LO(xxx)  ((byte) ((word)(xxx) & 255))
#define WORD_HI(xxx)  ((byte) ((word)(xxx) >> 8))

///11: 返回一个比X大的最接近的8的倍数
#define RND8(x) ((((x) + 7)/8) * 8)

///12: 将一个字母转换为大写
#define UPCASE(c) (((c)>='a' && (c) <= 'z') ? ((c) - 0x20) : (c))
#define ToUpper(c)		   (c & ~0x20)

///13: 判断字符是不是10进值的数字
#define  DECCHK(c) ((c)>='0' && (c)<='9')

///14: 判断字符是不是16进值的数字
#define HEXCHK(c) (((c) >= '0' && (c)<='9') ((c)>='A' && (c)<= 'F') \
((c)>='a' && (c)<='f'))

///15: 防止溢出的一个方法
#define INC_SAT(val) (val=((val)+1>(val)) ? (val)+1 : (val))

///16: 返回数组元素的个数
#define ARR_SIZE(a)  (sizeof((a))/sizeof((a[0])))

///17: 返回一个无符号数n尾的值MOD_BY_POWER_OF_TWO(X,n)=X%(2^n)
#define MOD_BY_POWER_OF_TWO( val, mod_by ) ((dword)(val) & (dword)((mod_by)-1))

///18: 对于IO空间映射在存储空间的结构,输入输出处理
#define inp(port) (*((volatile byte *)(port)))
#define inpw(port) (*((volatile word *)(port)))
#define inpdw(port) (*((volatile dword *)(port)))
#define outp(port,val) (*((volatile byte *)(port))=((byte)(val)))
#define outpw(port, val) (*((volatile word *)(port))=((word)(val)))
#define outpdw(port, val) (*((volatile dword *)(port))=((dword)(val)))



#define DEBUG

/**
 * Macros to help with debugging. Set SCULL_DEBUG to 1 enable
 * debugging (which you can do from the Makefile); these macros work
 * in both kernelspace and userspace.
 */

/* undef it, just in case someone else defined it. */

#ifdef dbgprint
#undef dbgprint
#endif // dbgprint

#ifdef dprint
#undef dprint
#endif // dprint

#ifdef dout
#undef dount
#endif // dout


/**when you define DEBUG macro
    dbgprint to use printk with line and funcitonname in kernel
    dprint to use print without line and funcitonname in kernel


    dbgprint to use printf with line and funcitonname in userspace
    dprint to use printf without line and funcitonname in userspace
    it's the same to dbgcout and dcout in userspace

*/
#ifdef DEBUG

    #ifdef __KERNEL__
        #ifdef __FILE__
        #ifdef __LINE__
            //printk with line and function name
            #define dbgprint(format, args...) \
            printk(KERN_INFO "[%s, %d] : "format, __FILE__, __LINE__, ##args)
        #endif // __FILE__ && __LINE__
        #endif // __FILE__ && __LINE__

        //printk without line and function name
        #define dprint(format,args...) printk(KERN_INFO format, ##args)
        #define dprintk(format,args...) printk(KERN_INFO format, ##args)


    #else   //  userspace

        #ifdef __FILE__
        #ifdef __LINE__
            //printf with line and function name
            #define dbgprint(format, args...) \
            printf("[%s,%d] : "format, __FILE__, __LINE__, ##args)
            #define dbgcout std::cout <<"[" <<__FILE__ <<", " <<__LINE__ <<"] : "

        #endif // __FILE__ && __LINE__
        #endif // __FILE__ && __LINE__

            /* Debugging is on and we are in userspace. */
            #define dprint(format, args...) printf(format, ## args)
            #define dprintf(format, args...) printf(format, ## args)
            #define dcout cout


    #endif // __KERNEL__

#else           /* Not debugging: do nothing. */
    #define dbgprint(format,args...)

    #define dprint(format, args...)
    #define dprintf(format, args...)
    #define dcout 0 && cout
#endif

/* PDEBUGG is a placeholder that makes it easy to "comment out" the debugging
   statements without deleting them. */
#undef undprint
#define undprint(format, args...)

#undef undcout
#define undcout 0 && count

#undef undbgprint
#define undprint(format, args...)

#undef undbgcout
#define undbgcout 0 && cout



/*
*		#define DEBUG		,dbginfo() will add more debug information into nomal printk.
*		#define SILENCE ,dbginfo() will print nothing.
*		Otherwise				,dbginfo() equals to printk.
*/


#ifdef __FUNCTION__
#ifdef __LINE__
	#ifdef DEBUG
		//printk with line and function name
		#define dbginfo(format,args...); \
		printk(KERN_INFO"MemFI-[%s, %d] : "format, __FUNCTION__, __LINE__, ##args);
	#elif defined SILENCE
			#define dbginfo(format,args...);
	#else								//printk normally
			#define dbginfo(format,args...); printk(KERN_INFO format, ##args);
	#endif
#endif
#endif

#define undbginfo(format,args...);

/*
*	safe sprintf
* It will not be out of range.
*/
/*
#define safe_sprintf(start, n, p, format, args...);                     \
    {                                                                   \
        if( (p - start) < n )                                           \
        {                                                               \
            snprintf( (char *)p, (n - (p - start)), format, ##args );   \
        }                                                               \
    }
*/
#define safe_sprintf(start, n, p, format, args...)                      \
	(((p - start) < n )  && snprintf( (char *)p, (n - (p - start)), format, ##args ))

/*
 * 3.10以后内核的proc文件
 * 废弃了原来的create_proc_entry接口
 * 3.9      http://lxr.free-electrons.com/ident?v=3.9&i=create_proc_entry
 * 2.10     http://lxr.free-electrons.com/ident?v=3.10;i=create_proc_entry
 *
 * 而引入了
 * 1.   proc_create创建proc文件,
 * 2.   seq_file(Sequence file：序列文件)接口
 * 3.   signal_file接口
 *
 * **proc_create创建proc文件**
 * 使用proc_create需要关联file_operations
 * 首先要调用创建proc文件的函数，需要绑定flie_operations
 *
 * ***seq_file接口操作proc文件*
 * http://blog.chinaunix.net/uid-28253945-id-3382865.html
 * 由于procfs的默认操作函数只使用一页的缓存，
 * 在处理较大的proc文件时就有点麻烦，
 * 并且在输出一系列结构体中的数据时也比较不灵活，
 * 需要自己在read_proc函数中实现迭代，容易出现Bug。
 * 所以内核黑客们对一些/proc代码做了研究，抽象出共性，
 * 最终形成了seq_file（Sequence file：序列文件）接口。
 * 这个接口提供了一套简单的函数来解决以上proc接口编程时存在的问题，
 * 使得编程更加容易，降低了Bug出现的机会。
 * 在需要创建一个由一系列数据顺序组合而成的虚拟文件或一个较大的虚拟文件时，
 * 推荐使用seq_file接口。
 *
 * 一般地，内核通过在procfs文件系统下建立文件来向用户空间提供输出信息，
 * 用户空间可以通过任何文本阅读应用查看该文件信息，
 * 但是procfs 有一个缺陷，如果输出内容大于1个内存页，需要多次读，
 * 因此处理起来很难，另外，如果输出太大，速度比较慢，
 * 有时会出现一些意想不到的情况，
 * Alexander Viro实现了一套新的功能，使得内核输出大文件信息更容易，
 * 该功能出现在2.4.15（包括2.4.15）以后的所有2.4内核以及2.6内核中，
 * 尤其 是在2.6内核中，已经大量地使用了该功能。
 * 要想使用seq_file功能，开发者需要包含头文件linux/seq_file.h，
 * 并定义与设置一个seq_operations结构（类似于file_operations结构）:
 *
 * 但是我个人认为，并不是只有procfs才可以使用这个seq_file接口，
 * 因为其实seq_file是实现的是一个操作函数集，这个函数集并不是与proc绑定的，
 * 同样可以用在其他的地方。
 *
 **/
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)

    #define     CREATE_PROC_ENTRY
    #warning    "use create_proc_entry in " #LINUX_VERSION_CODE

#else

    #define     PROC_CREATE
    #warning    "use proc_create in " #LINUX_VERSION_CODE
    #define     PROC_SEQ_FILE_OPERATIONS
    #include <linux/proc_fs.h>
    #include <linux/seq_file.h>
#endif

//#ifndef CREATE_PROC_ENTRY || PROC_CREATE
//#error "error"
//#endif

#define NAME_TO_STR(name)   (#name)




#endif	/* _COMMON_H_ */
