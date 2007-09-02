/*
 * hist - definitions for command history module
 *
 * Copyright (C) 1999-2007  David I. Bell
 *
 * Calc is open software; you can redistribute it and/or modify it under
 * the terms of the version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * Calc is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU Lesser General
 * Public License for more details.
 *
 * A copy of version 2.1 of the GNU Lesser General Public License is
 * distributed with calc under the filename COPYING-LGPL.  You should have
 * received a copy with calc; if not, write to Free Software Foundation, Inc.
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @(#) $Revision: 30.1 $
 * @(#) $Id: hist.h,v 30.1 2007/03/16 11:09:46 chongo Exp $
 * @(#) $Source: /usr/local/src/cmd/calc/RCS/hist.h,v $
 *
 * Under source code control:	1993/05/02 20:09:20
 * File existed as early as:	1993
 *
 * Share and enjoy!  :-)	http://www.isthe.com/chongo/tech/comp/calc/
 */


#if !defined(__HIST_H__)
#define __HIST_H__


/*
 * Default binding file and history size.
 */
#ifndef HIST_BINDING_FILE
#define HIST_BINDING_FILE	"/usr/lib/hist.bind"
#endif

#ifndef HIST_SIZE
#define HIST_SIZE		(1024*10)
#endif


/*
 * path search defines
 */
#define HOMECHAR	'~'	/* char which indicates home directory */
#define DOTCHAR		'.'	/* char which indicates current directory */
#define PATHCHAR	'/'	/* char which separates path components */
#if defined(__MSDOS__) || defined(__WIN32)
#define LISTCHAR	';'	/* char which separates paths in a list */
#else
#define LISTCHAR	':'	/* char which separates paths in a list */
#endif


/*
 * Possible returns from hist_init.  Note that an error from hist_init does
 * not prevent calling the other routines, but fancy command line editing
 * is then disabled.
 */
#define HIST_SUCCESS	0	/* successfully inited */
#define HIST_INITED	1	/* initialization is already done */
#define HIST_NOFILE	2	/* bindings file could not be read */
#define HIST_NOTTY	3	/* terminal modes could not be set */


E_FUNC	int	hist_init(char *filename);
E_FUNC	void	hist_term(void);
E_FUNC	size_t	hist_getline(char *prompt, char *buf, size_t len);
E_FUNC	void	hist_saveline(char *line, int len);


#endif /* !__HIST_H__ */
