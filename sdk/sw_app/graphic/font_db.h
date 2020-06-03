/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    font_db.h
 * @brief
 * @author  phoong
 * @section MODIFY history
 */
/*****************************************************************************/
#if !defined (_FONT_DB_H_)
#define _FONT_DB_H_

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

//# English variable Font, 2nd variable Font
typedef enum {
	FNT_SZ01,	//# eng.1718, 2nd.1418
	FNT_SZ02,	//# eng.2021, 2nd.1721
	FNT_SZ03,	//# eng.2324, 2nd.2024
	FNT_SZ04,	//# eng.2628, 2nd.2328
	FNT_SZ05,	//# eng.2930, 2nd.2530

	FNT_DB_MAX
} font_db_e;

typedef struct {
	unsigned char *p;
	char wi;
	char he;
	char bz;
} font_info_t;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

int get_font_db(char **str, font_info_t *ifnt, int fnt);

#endif	//_FONT_DB_H_
