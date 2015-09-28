\ Vortex math stuff ...

forget reduce

: mod 2dup / * - ;

: reduce 
	dup
	if 
		9 mod dup 0=
		if 9 then
	then 
	;
