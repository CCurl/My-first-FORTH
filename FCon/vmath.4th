\ Vortex math stuff ...

: vmath.module ;

: reduce dup 
	if 
		9 mod dup 0= if drop 9 then
	then ;

\ other testing crap

: num-vectors 10 ;
num-vectors ARRAY vectors

: >vector vectors >array ;
: vector> vectors array> ;
: .vector vector> ?DUP IF BODY>HEAD HEAD>NAME COUNT TYPE ELSE ." null." THEN ;
: vector.go vector> EXECUTE ;
: .vectors num-vectors 0 DO I . '-' EMIT .BL I .vector .CR LOOP ;

: t.size 10 ;
t.size array t
: t.init t.size 0 do 0 i t >array loop ; 

