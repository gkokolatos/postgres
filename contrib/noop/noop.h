/*-------------------------------------------------------------------------
 *
 * noop.h
 *	  header file for greenplum noop access method implementation.
 *
 * IDENTIFICATION
 *	  contrib/noop/noop.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef NOOP_H
#define NOOP_H

#include "access/amapi.h"
#include "nodes/execnodes.h"
#include "fmgr.h"

extern Datum noophandler(PG_FUNCTION_ARGS);

/* NOOP index access method interface functions */
extern bytea *noopoptions(Datum reloptions, bool validate);
extern IndexBuildResult *noopbuild(Relation heap, Relation index,
				IndexInfo *indexInfo);
extern void noopbuildempty(Relation index);
extern void noopcostestimate(PlannerInfo *root, IndexPath *path, double loop_count,
				Cost *indexStartupCost, Cost *indexTotalCost,
				Selectivity *indexSelectivity, double *indexCorrelation);
extern bool noopinsert(Relation index, Datum *values, bool *isnull,
				ItemPointer ht_ctid, Relation heapRel,
				IndexUniqueCheck checkUnique);
extern IndexBulkDeleteResult *noopvacuumcleanup(IndexVacuumInfo *info,
				IndexBulkDeleteResult *stats);

#endif /* NOOP_H */
