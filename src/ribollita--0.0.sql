CREATE FUNCTION copytext(text) RETURNS text
     AS 'MODULE_PATHNAME', 'copytext'
     LANGUAGE C STRICT;

CREATE FUNCTION reversetext(text) RETURNS text
     AS 'MODULE_PATHNAME', 'reversetext'
     LANGUAGE C STRICT;
