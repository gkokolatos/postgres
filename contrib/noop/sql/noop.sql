CREATE EXTENSION noop;

CREATE TABLE noop_test (a integer, b integer, c text);
CREATE UNIQUE INDEX noop_test_b_key ON noop_test USING NOOP (b);
CREATE UNIQUE INDEX noop_test_c_key ON noop_test USING NOOP (c);
\d noop_test
\d noop_test_b_key
\d noop_test_c_key

SELECT
	c2.relname,
	i.indisprimary,
	i.indisunique,
	i.indisclustered,
	i.indisvalid,
	pg_catalog.pg_get_indexdef(i.indexrelid, 0, true),
	pg_catalog.pg_get_constraintdef(con.oid, true),
	contype,
	condeferrable,
	condeferred,
	i.indisreplident,
	c2.reltablespace
FROM
	pg_catalog.pg_class c,
	pg_catalog.pg_class c2,
	pg_catalog.pg_index i   LEFT JOIN
	pg_catalog.pg_constraint con
ON
(
	conrelid = i.indrelid AND
	conindid = i.indexrelid AND
	contype IN ('p','u','x')
)
WHERE
	(c2.relname = 'noop_test_b_key' OR
	c2.relname = 'noop_test_c_key') AND
	c.oid = i.indrelid AND
	i.indexrelid = c2.oid
ORDER BY
	i.indisprimary DESC,
	i.indisunique DESC,
	c2.relname
;

INSERT INTO noop_test (a, b, c) SELECT generate_series(0, 9) as a, 1 as b, 'one' as c;
SELECT count(b) as "b multiples", count(c) as "c multiples", b, c FROM noop_test GROUP BY b, c;

INSERT INTO noop_test SELECT i, i % 10, substr(md5(i::text), 1, 1) FROM generate_series(10,2009) i;
VACUUM ANALYZE noop_test;

DELETE FROM noop_test WHERE a %  3 = 0;
VACUUM FULL noop_test;

EXPLAIN (COSTS OFF) SELECT count(*) FROM noop_test WHERE b != 1;
EXPLAIN (COSTS OFF) SELECT count(*) FROM noop_test WHERE c = 'one';

SET enable_seqscan=off;
SET enable_bitmapscan=on;
SET enable_indexscan=on;

EXPLAIN (COSTS OFF) SELECT count(*) FROM noop_test WHERE b != 1;
EXPLAIN (COSTS OFF) SELECT count(*) FROM noop_test WHERE c = 'one';

RESET enable_seqscan;
RESET enable_bitmapscan;
RESET enable_indexscan;
