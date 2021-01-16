CREATE FUNCTION copytext(text) RETURNS text
     AS 'MODULE_PATHNAME', 'copytext'
     LANGUAGE C STRICT;
