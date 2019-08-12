/* contrib/noop/noop--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION noop" to load this file. \quit

CREATE FUNCTION noophandler(internal)
RETURNS index_am_handler
AS 'MODULE_PATHNAME'
LANGUAGE C;

-- Access method
CREATE ACCESS METHOD noop TYPE INDEX HANDLER noophandler;
COMMENT ON ACCESS METHOD noop IS 'noop index access method';

-- Opclasses

CREATE OPERATOR CLASS int4_ops
DEFAULT FOR TYPE int4 USING noop AS
	OPERATOR	1	=(int4, int4),
	FUNCTION	1	hashint4(int4);

CREATE OPERATOR CLASS text_ops
DEFAULT FOR TYPE text USING noop AS
	OPERATOR	1	=(text, text),
	FUNCTION	1	hashtext(text);
