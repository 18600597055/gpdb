/*-------------------------------------------------------------------------
 *
 * smgrdesc.c
 *	  rmgr descriptor routines for catalog/storage.c
 *
 * Portions Copyright (c) 1996-2014, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/access/rmgrdesc/smgrdesc.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "catalog/catalog.h"
#include "catalog/storage_xlog.h"


void
smgr_desc(StringInfo buf, XLogRecord *record)
{
	uint8           info = record->xl_info & ~XLR_INFO_MASK;
	char            *rec = XLogRecGetData(record);

	if (info == XLOG_SMGR_CREATE)
	{
		xl_smgr_create *xlrec = (xl_smgr_create *) rec;
		char	   *path = relpathperm(xlrec->rnode, xlrec->forkNum);

		appendStringInfo(buf, "file create: %s", path);
		pfree(path);
	}
	else if (info == XLOG_SMGR_TRUNCATE)
	{
		xl_smgr_truncate *xlrec = (xl_smgr_truncate *) rec;
		char	   *path = relpathperm(xlrec->rnode, MAIN_FORKNUM);

		appendStringInfo(buf, "file truncate: %s to %u blocks", path,
						 xlrec->blkno);
		pfree(path);
	}
	else
		appendStringInfoString(buf, "UNKNOWN");
}
