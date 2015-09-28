\ Vortex math stuff ...

: vmath.module ;

: reduce dup 
	if 
		9 mod dup 0= if drop 9 then
	then ;

\ other testing crap

: num-vectors 10 ;
num-vectors ARRAY vectors

: !vector vectors + ! ;
: @vector vectors + @ ;
: >vector @vector EXECUTE ;
: .vector @vector ?DUP IF BODY>HEAD HEAD>NAME COUNT TYPE ELSE ." null." THEN ;
: .vectors num-vectors 0 DO I . '-' EMIT .BL I .vector .CR LOOP ;

: t.size 10 ;
t.size array t
: t.init t.size 1+ 1 do i i t >array loop ; 
