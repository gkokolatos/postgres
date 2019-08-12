/*-------------------------------------------------------------------------
 *
 * noop.c
 *		Access methods for noop indexes.
 *
 * IDENTIFICATION
 *	  contrib/noop/noop.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "optimizer/cost.h"
#include "storage/bufmgr.h"
#include "utils/rel.h"
#include "utils/selfuncs.h"

#include "noop.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(noophandler);

/*
 * NOOP handler function:
 *     Creates and populates an IndexAmRoutine
 */
Datum
noophandler(PG_FUNCTION_ARGS)
{
	IndexAmRoutine *amroutine = makeNode(IndexAmRoutine);

	amroutine->amstrategies = 0;
	amroutine->amsupport = 1;
	amroutine->amcanorder = false;
	amroutine->amcanorderbyop = false;
	amroutine->amcanbackward = false;
	amroutine->amcanunique = true;
	amroutine->amcanmulticol = true;
	amroutine->amoptionalkey = true;
	amroutine->amsearcharray = false;
	amroutine->amsearchnulls = false;
	amroutine->amstorage = false;
	amroutine->amclusterable = false;
	amroutine->ampredlocks = false;
	amroutine->amkeytype = InvalidOid;

	amroutine->ambuild = noopbuild;
	amroutine->ambuildempty = noopbuildempty;
	amroutine->aminsert = noopinsert;
	amroutine->ambulkdelete = NULL;
	amroutine->amvacuumcleanup = noopvacuumcleanup;
	amroutine->amcanreturn = NULL;
	amroutine->amcostestimate = noopcostestimate;
	amroutine->amoptions = noopoptions;
	amroutine->amproperty = NULL;
	amroutine->amvalidate = NULL;
	amroutine->ambeginscan = NULL;
	amroutine->amrescan = NULL;
	amroutine->amgettuple = NULL;
	amroutine->amgetbitmap = NULL;
	amroutine->amendscan = NULL;
	amroutine->ammarkpos = NULL;
	amroutine->amrestrpos = NULL;

	PG_RETURN_POINTER(amroutine);
}

IndexBuildResult *
noopbuild(Relation heap, Relation index, IndexInfo *indexInfo)
{
	IndexBuildResult *result;

	/*
	 * This can never happen by us and indicates corruption
	 */
	if (RelationGetNumberOfBlocks(index) != 0)
		elog(ERROR, "index \"%s\" already contains data",
			 RelationGetRelationName(index));

	result = (IndexBuildResult *) palloc(sizeof(IndexBuildResult));
	result->heap_tuples = 0;
	result->index_tuples = 0;

	return result;
}

/*
 * Build an empty noop index in the initialization fork.
 */
void
noopbuildempty(Relation index)
{
	return;
}

bytea *
noopoptions(Datum reloptions, bool validate)
{
	return NULL;
}

/*
 * This function should not be called yet it needs to be defined since the index
 * am code asserts that it is present before it moves into an insertion.
 */
void
noopcostestimate(PlannerInfo *root, IndexPath *path, double loop_count,
			   Cost *indexStartupCost, Cost *indexTotalCost,
			   Selectivity *indexSelectivity, double *indexCorrelation)
{
	IndexOptInfo *index = path->indexinfo;
	List	   *qinfos;
	GenericCosts costs;

	/* Do preliminary analysis of indexquals */
	qinfos = deconstruct_indexquals(path);

	MemSet(&costs, 0, sizeof(costs));

	/* We have to visit all index tuples anyway */
	costs.numIndexTuples = index->tuples;

	/* Use generic estimate */
	genericcostestimate(root, path, loop_count, qinfos, &costs);

	*indexStartupCost = costs.indexStartupCost;
	*indexTotalCost = costs.indexTotalCost;
	*indexSelectivity = costs.indexSelectivity;
	*indexCorrelation = costs.indexCorrelation;

	return;
}

/*
 * Return value is always false as no value can ever be found.
 */
bool
noopinsert(Relation index, Datum *values, bool *isnull,
		 ItemPointer ht_ctid, Relation heapRel, IndexUniqueCheck checkUnique)
{
	return false;
}

/*
 * Post-VACUUM cleanup.
 *
 * Result: a palloc'd struct containing statistical info for VACUUM displays.
 */
IndexBulkDeleteResult *
noopvacuumcleanup(IndexVacuumInfo *info, IndexBulkDeleteResult *stats)
{

	if (stats == NULL)
		stats = (IndexBulkDeleteResult *) palloc0(sizeof(IndexBulkDeleteResult));

	return stats;
}
