/*
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted.
 *
 * LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Comments, suggestions, bug fixes and questions about these routines
 * are welcome.  Send EMail to the address given below.
 *
 * Happy bit twiddling,
 *
 *			Landon Curt Noll
 *
 *			chongo@toad.com
 *			...!{pyramid,sun,uunet}!hoptoad!chongo
 *
 * chongo was here	/\../\
 */

#if defined(CUSTOM)

#include <stdio.h>
#include <ctype.h>

#include "../have_const.h"
#include "../value.h"
#include "../custom.h"

#include "../config.h"
#include "../calc.h"
#include "../longbits.h"
#include "../longlong.h"
#include "../block.h"
#include "../calcerr.h"
#include "../conf.h"
#include "../endian_calc.h"
#include "../fposval.h"
#include "../hist.h"
#include "../prime.h"
#include "../zrand.h"
#include "../zrandom.h"


/*
 * sys_info - names and values of selected #defines
 */
struct infoname {
	char *name;	/* name of #define converted to all UPPER_CASE */
	char *meaning;	/* brief explanation of the #define */
	char *str;	/* non-NULL ==> value of #define is a string */
	FULL nmbr;	/* if str==NULL ==> value fo #define as a FULL */
};
static struct infoname sys_info[] = {
    {"A55", "slots in an additive 55 table", NULL, (FULL)A55},
    {"BASE", "base for calculations", NULL, (FULL)BASE},
    {"BASE1", "one less than base", NULL, (FULL)BASE},
    {"BASEB", "bits in the calculation base", NULL, (FULL)BASEB},
    {"BASEDIG", "number of digits in base", NULL, (FULL)BASEDIG},
    {"BIG_ENDIAN", "Most Significant Byte first symbol", NULL, (FULL)BIG_ENDIAN},
    {"BLK_CHUNKSIZE", "default allocation chunk size for blocks", NULL, (FULL)BLK_CHUNKSIZE},
    {"BLK_DEF_MAXPRINT", "default block octets to print", NULL, (FULL)BLK_DEF_MAXPRINT},
    {"BLUM_PREGEN", "non-default predefined Blum generators", NULL, (FULL)BLUM_PREGEN},
    {"BOOL_B64", "if we have 64 bit type (TRUE or FALSE)", NULL, (FULL)BOOL_B64},
    {"CALCEXT", "extension for files read in", CALCEXT, (FULL)0},
    {"CALC_BYTE_ORDER", "Byte order (LITTLE_ENDIAN or BIG_ENDIAN)", NULL, (FULL)CALC_BYTE_ORDER},
    {"CUSTOMHELPDIR", "location of the custom help directory", CUSTOMHELPDIR, (FULL)0},
    {"DEFAULTCALCBINDINGS", "default key bindings file", DEFAULTCALCBINDINGS, (FULL)0},
    {"DEFAULTCALCHELP", "help file that -h prints", DEFAULTCALCHELP, (FULL)0},
    {"DEFAULTCALCPAGER", "default pager", DEFAULTCALCPAGER, (FULL)0},
    {"DEFAULTCALCPATH", "default :-separated search path", DEFAULTCALCPATH, (FULL)0},
    {"DEFAULTCALCRC", "default :-separated startup file list", DEFAULTCALCRC, (FULL)0},
    {"DEFAULTSHELL", "default shell to use", DEFAULTSHELL, (FULL)0},
    {"DEV_BITS", "device number size in bits", NULL, (FULL)DEV_BITS},
    {"DISPLAY_DEFAULT", "default digits for float display", NULL, (FULL)DISPLAY_DEFAULT},
    {"ECHO", "where the echo command is located", ECHO, (FULL)0},
    {"EPSILONPREC_DEFAULT", "2^-EPSILON_DEFAULT <= EPSILON_DEFAULT", NULL, (FULL)EPSILONPREC_DEFAULT},
    {"EPSILON_DEFAULT", "allowed error for float calculations", EPSILON_DEFAULT, (FULL)0},
    {"ERRMAX", "default errmax value", NULL, (FULL)ERRMAX},
    {"E_USERDEF", "base of user defined errors", NULL, (FULL)E_USERDEF},
    {"E__BASE", "calc errors start above here", NULL, (FULL)E__BASE},
    {"E__COUNT", "number of calc errors", NULL, (FULL)E__COUNT},
    {"E__HIGHEST", "highest calc error", NULL, (FULL)E__HIGHEST},
    {"FALSE", "boolean false", NULL, (FULL)FALSE},
    {"FILEPOS_BITS", "file position size in bits", NULL, (FULL)FILEPOS_BITS},
    {"FULL_BITS", "bits in a FULL", NULL, (FULL)FULL_BITS},
    {"HELPDIR", "location of the help directory", HELPDIR, (FULL)0},
    {"HIST_BINDING_FILE", "Default binding file", HIST_BINDING_FILE, (FULL)0},
    {"HIST_SIZE", "Default history size", NULL, (FULL)HIST_SIZE},
    {"INIT_J", "initial 1st walking a55 table index", NULL, (FULL)INIT_J},
    {"INIT_K", "initial 2nd walking a55 table index", NULL, (FULL)INIT_K},
    {"INODE_BITS", "inode number size in bits", NULL, (FULL)INODE_BITS},
    {"LITTLE_ENDIAN", "Least Significant Byte first symbol", NULL, (FULL)LITTLE_ENDIAN},
    {"LONGLONG_BITS", "length of a long long, or 0", NULL, (FULL)LONGLONG_BITS},
    {"LONG_BITS", "bit length of a long", NULL, (FULL)LONG_BITS},
    {"MAP_POPCNT", "number of odd primes in pr_map", NULL, (FULL)MAP_POPCNT},
    {"MAXCMD", "max length of command invocation", NULL, (FULL)MAXCMD},
    {"MAXDIM", "max number of dimensions in matrices", NULL, (FULL)MAXDIM},
    {"MAXERROR", "max length of error message string", NULL, (FULL)MAXERROR},
    {"MAXFILES", "max number of opened files", NULL, (FULL)MAXFILES},
    {"MAXFULL", "largest SFULL value", NULL, (FULL)MAXFULL},
    {"MAXHALF", "largest SHALF value", NULL, (FULL)MAXHALF},
    {"MAXINDICES", "max number of indices for objects", NULL, (FULL)MAXINDICES},
    {"MAXLABELS", "max number of user labels in function", NULL, (FULL)MAXLABELS},
    {"MAXLEN", "longest storage size allowed", NULL, (FULL)MAXLEN},
    {"MAXLONG", "largest long val", NULL, (FULL)MAXLONG},
    {"MAXOBJECTS", "max number of object types", NULL, (FULL)MAXOBJECTS},
    {"MAXPRINT_DEFAULT", "default number of elements printed", NULL, (FULL)MAXPRINT_DEFAULT},
    {"MAXREDC", "number of entries in REDC cache", NULL, (FULL)MAXREDC},
    {"MAXSCANCOUNT", "default max scan errors before an abort", NULL, (FULL)MAXSCANCOUNT},
    {"MAXSTACK", "max depth of evaluation stack", NULL, (FULL)MAXSTACK},
    {"MAXSTRING", "max size of string constant", NULL, (FULL)MAXSTRING},
    {"MAXUFULL", "largest FULL value", NULL, (FULL)MAXUFULL},
    {"MAXULONG", "largest unsigned long val", NULL, (FULL)MAXULONG},
    {"MAX_MAP_PRIME", "larest prime in pr_map", NULL, (FULL)MAX_MAP_PRIME},
    {"MAX_MAP_VAL", "larest bit in pr_map", NULL, (FULL)MAX_MAP_VAL},
    {"MAX_PFACT_VAL", "max x, for which pfact(x) is a long", NULL, (FULL)MAX_PFACT_VAL},
    {"MAX_SM_PRIME", "larest 32 bit prime", NULL, (FULL)MAX_SM_PRIME},
    {"MAX_SM_VAL", "larest 32 bit value", NULL, (FULL)MAX_SM_VAL},
    {"MUL_ALG2", "default size for alternative multiply", NULL, (FULL)MUL_ALG2},
    {"NEW_EPSILONPREC_DEFAULT", "2^-EPSILON_DEFAULT <= EPSILON_DEFAULT", NULL, (FULL)NEW_EPSILONPREC_DEFAULT},
    {"NEW_EPSILON_DEFAULT", "newstd EPSILON_DEFAULT", NEW_EPSILON_DEFAULT, (FULL)0},
    {"NXT_MAP_PRIME", "smallest odd prime not in pr_map", NULL, (FULL)NXT_MAP_PRIME},
    {"NXT_PFACT_VAL", "next prime for higher pfact values", NULL, (FULL)NXT_PFACT_VAL},
    {"OFF_T_BITS", "file offset size in bits", NULL, (FULL)OFF_T_BITS},
    {"PATHSIZE", "max length of path name", NULL, (FULL)PATHSIZE},
    {"PATHSIZE", "max length of path name", NULL, (FULL)PATHSIZE},
    {"PIX_32B", "max pix() value", NULL, (FULL)PIX_32B},
    {"POW_ALG2", "default size for using REDC for powers", NULL, (FULL)POW_ALG2},
    {"REDC_ALG2", "default size using alternative REDC alg", NULL, (FULL)REDC_ALG2},
    {"SBITS", "size of additive or shuffle entry in bits", NULL, (FULL)SBITS},
    {"SBYTES", "size of additive or shuffle entry in bytes", NULL, (FULL)SBYTES},
    {"SCNT", "length of additive 55 table in FULLs", NULL, (FULL)SCNT},
    {"SEEDXORBITS", "low bits of a55 seed devoted to xor", NULL, (FULL)SEEDXORBITS},
    {"SHALFS", "size of additive or shuffle entry in HALFs", NULL, (FULL)SHALFS},
    {"SHUFCNT", "size of shuffle table in entries", NULL, (FULL)SHUFCNT},
    {"SHUFLEN", "length of shuffle table in FULLs", NULL, (FULL)SHUFLEN},
    {"SHUFMASK", "mask for shuffle table entry selection", NULL, (FULL)SHUFMASK},
    {"SHUFPOW", "power of 2 size of the shuffle table", NULL, (FULL)SHUFPOW},
    {"SLEN", "number of FULLs in a shuffle table entry", NULL, (FULL)SLEN},
    {"SQ_ALG2", "default size for alternative squaring", NULL, (FULL)SQ_ALG2},
    {"SYMBOLSIZE", "max symbol name size", NULL, (FULL)SYMBOLSIZE},
    {"TEN_MAX", "10^(2^TEN_MAX): largest base10 conversion const", NULL, (FULL)TEN_MAX},
    {"TOPFULL", "highest bit in FULL", NULL, (FULL)TOPFULL},
    {"TOPHALF", "highest bit in a HALF", NULL, (FULL)TOPHALF},
    {"TOPLONG", "top long bit", NULL, (FULL)TOPLONG},
    {"TRUE", "boolean true", NULL, (FULL)TRUE},
    {"USUAL_ELEMENTS", "usual number of elements for objects", NULL, (FULL)USUAL_ELEMENTS},

     /* must be last */
    {NULL, NULL, NULL, (FULL)0}
};


/*
 * forward declarations
 */
static void dump_name_meaning(void);	/* custom("sysinfo", 0) */
static void dump_name_value(void);	/* custom("sysinfo", 1) */
static void dump_mening_value(void);	/* custom("sysinfo", 2) */


/*
 * c_sysinfo - return a calc #define value
 *
 * given:
 *	vals[0]	  if given, name of #define to print
 *		  otherwise a list of #defines are printed
 *
 * returns:
 *	value of #define if given (int or string)
 *	null if no #define arg was given
 */
/*ARGSUSED*/
VALUE
c_sysinfo(char *name, int count, VALUE **vals)
{
	VALUE result;		/* what we will return */
	struct infoname *p;	/* current infoname */
	char *buf;		/* upper case value of vals[0] */
	char *q;		/* to upper case converter */
	char *r;		/* to upper case converter */

	/*
	 * we will return NULL if a value was not found
	 */
	result.v_type = V_NULL;

	/*
	 * case 0: if no args, then dump the table with no values
	 */
	if (count == 0) {

		/* dump the entire table */
		dump_name_meaning();

	/*
	 * case 1: numeric arg is given
	 */
	} else if (vals[0]->v_type == V_NUM) {

		/* firewall - must be a tiny non-negative integer */
		if (qisneg(vals[0]->v_num) ||
		    qisfrac(vals[0]->v_num) ||
		    zge31b(vals[0]->v_num->num)) {
			math_error("sysinfo: arg must be string, 0, 1 or 2");
			/*NOTREACHED*/
		}

		/*
		 * select action based on numeric value of arg
		 */
		switch (z1tol(vals[0]->v_num->num)) {
		case 0:		/* print all infonames and meanings */
			dump_name_meaning();
			break;
		case 1:		/* print all infonames and values */
			dump_name_value();
			break;
		case 2:		/* print all values and meanings */
			dump_mening_value();
			break;
		default:
			math_error("sysinfo: arg must be string, 0, 1 or 2");
			/*NOTREACHED*/
		}

	/*
	 * case 2: string arg is given
	 *
	 * The string is taken to be the infoname we want to print.
	 */
	} else if (vals[0]->v_type == V_STR) {

		/* convert vals[0] to upper case string */
		buf = (char *)malloc(strlen((char *)vals[0]->v_str)+1);
		for (q = (char *)vals[0]->v_str, r = buf; *q; ++q, ++r) {
			if (isascii(*q) && islower(*q)) {
				*r = *q - 'a' + 'A';
			} else {
				*r = *q;
			}
		}
		*r = '\0';

		/* search the table for the infoname */
		for (p = sys_info; p->name != NULL; ++p) {

			if (strcmp(p->name, buf) == 0) {

				/* found the infoname */
				if (p->str == NULL) {
					/* return value as integer */
					result.v_type = V_NUM;
					result.v_num = utoq( p->nmbr);
				} else {
					/* return value as string */
					result.v_type = V_STR;
					result.v_subtype = V_NOSUBTYPE;
					result.v_str = (STRING *)p->str;
				}

				/* return found infotype as value */
				break;
			}
		}

	/*
	 * bad arg given
	 */
	} else {
		math_error("sysinfo: arg must be string, 0, 1 or 2");
		/*NOTREACHED*/
	}

	/*
	 * return what we found or didn't find
	 */
	return result;
}


/*
 * dump_name_meaning - print all infonames and meanings
 */
static void
dump_name_meaning(void)
{
	struct infoname *p;	/* current infoname */

	/* dump the entire table */
	for (p = sys_info; p->name != NULL; ++p) {
		printf("%s%-23s\t%s\n",
		    (conf->tab_ok ? "\t" : ""), p->name, p->meaning);
	}

}


/*
 * dump_name_value - print all infonames and values
 */
static void
dump_name_value(void)
{
	struct infoname *p;	/* current infoname */

	/* dump the entire table */
	for (p = sys_info; p->name != NULL; ++p) {
		if (p->str == NULL) {
#if LONG_BITS == FULL_BITS || FULL_BITS == 32 || !defined(HAVE_LONGLONG)
			printf("%s%-23s\t%-8lu\t(0x%lx)\n",
			    (conf->tab_ok ? "\t" : ""), p->name,
			    (unsigned long)p->nmbr,
			    (unsigned long)p->nmbr);
#else
			printf("%s%-23s\t%-8llu\t(0x%llx)\n",
			    (conf->tab_ok ? "\t" : ""), p->name,
			    (unsigned long long)p->nmbr,
			    (unsigned long long)p->nmbr);
#endif
		} else {
			printf("%s%-23s\t\"%s\"\n",
			    (conf->tab_ok ? "\t" : ""), p->name, p->str);
		}
	}

}


/*
 * dump_mening_value - print all values and meanings
 */
static void
dump_mening_value(void)
{
	struct infoname *p;	/* current infoname */

	/* dump the entire table */
	for (p = sys_info; p->name != NULL; ++p) {
		if (p->str == NULL) {
#if LONG_BITS == FULL_BITS || FULL_BITS == 32 || !defined(HAVE_LONGLONG)
			printf("%s%-36.36s\t%-8lu\t(0x%lx)\n",
			    (conf->tab_ok ? "\t" : ""), p->meaning,
			    (unsigned long)p->nmbr,
			    (unsigned long)p->nmbr);
#else
			printf("%s%-36.36s\t%-8llu\t(0x%llx)\n",
			    (conf->tab_ok ? "\t" : ""), p->meaning,
			    (unsigned long long)p->nmbr,
			    (unsigned long long)p->nmbr);
#endif
		} else {
			printf("%s%-36.36s\t\"%s\"\n",
			    (conf->tab_ok ? "\t" : ""), p->meaning, p->str);
		}
	}

}

#endif /* CUSTOM */