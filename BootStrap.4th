HERE 999 ! LAST 998 !
: .bsr 999 @ (HERE) ! 998 @ (LAST) ! ;

: IMMEDIATE 1 LAST 1 + ! ;
: ] 1 STATE ! ;
: [ 0 STATE ! ; IMMEDIATE
: ?] STATE @ ;

: SWAP  ?] IF  5 , ELSE [  5 , ] THEN ; IMMEDIATE
: DROP  ?] IF  6 , ELSE [  6 , ] THEN ; IMMEDIATE
: PICK  ?] IF  7 , ELSE [  7 , ] THEN ; IMMEDIATE
: ROT   ?] IF  8 , ELSE [  8 , ] THEN ; IMMEDIATE
: 1+    ?] IF  9 , ELSE [  9 , ] THEN ; IMMEDIATE
: -     ?] IF 11 , ELSE [ 11 , ] THEN ; IMMEDIATE
: *     ?] IF 12 , ELSE [ 12 , ] THEN ; IMMEDIATE
: /     ?] IF 13 , ELSE [ 13 , ] THEN ; IMMEDIATE
: =     ?] IF 14 , ELSE [ 14 , ] THEN ; IMMEDIATE
: <>    ?] IF 15 , ELSE [ 15 , ] THEN ; IMMEDIATE
: <     ?] IF 16 , ELSE [ 16 , ] THEN ; IMMEDIATE
: >     ?] IF 17 , ELSE [ 17 , ] THEN ; IMMEDIATE
: >R    ?] IF 18 , ELSE [ 18 , ] THEN ; IMMEDIATE
: R>    ?] IF 19 , ELSE [ 19 , ] THEN ; IMMEDIATE
: R@    ?] IF 20 , ELSE [ 20 , ] THEN ; IMMEDIATE
: DO    ?] IF 22 , ELSE [ 22 , ] THEN ; IMMEDIATE
: I     ?] IF 23 , ELSE [ 23 , ] THEN ; IMMEDIATE
: LEAVE ?] IF 24 , ELSE [ 24 , ] THEN ; IMMEDIATE
: LOOP  ?] IF 25 , ELSE [ 25 , ] THEN ; IMMEDIATE
: +LOOP ?] IF 26 , ELSE [ 26 , ] THEN ; IMMEDIATE
: OVER  ?] IF 31 , ELSE [ 31 , ] THEN ; IMMEDIATE
: 1-    ?] IF 34 , ELSE [ 34 , ] THEN ; IMMEDIATE
: 0=    ?] IF 35 , ELSE [ 35 , ] THEN ; IMMEDIATE
: 2+    ?] IF  9 , 9 , ELSE 1+ 1+ THEN ; IMMEDIATE

: BEGIN  ?] IF HERE   THEN ; IMMEDIATE
: EXIT   ?] IF 99 ,   THEN ; IMMEDIATE
: REPEAT ?] IF 27 , , THEN ; IMMEDIATE

:  <= 1+ < ;
:  >= 1 - > ;

: BL 32 ;

: .isws.
	DUP 13 = IF DROP BL THEN
	DUP 10 = IF DROP BL THEN
	DUP BL = ;

: ( SOURCE >IN @ DO DUP I + @ ')' = IF DROP I 2+ >IN ! LEAVE THEN LOOP ; IMMEDIATE
( : ." SOURCE >IN @ DO DUP I + @ '"' = IF DROP I 2+ >IN ! LEAVE ELSE DUP I + @ EMIT THEN LOOP ; IMMEDIATE )
: VAR CREATE 33 , LAST , 3 , HERE 2+ , 99 , 0 , ;
: ALLOT 0 DO 0 , LOOP ;

: /MOD ( n -- q r ) DUP BASE @ / DUP BASE @ * ROT SWAP - ;

: .BL BL EMIT ;
: .CR 13 EMIT 10 EMIT ;

: 2DUP OVER OVER ;
: ?DUP DUP IF DUP THEN ;

: str+ ( c addr -- ) DUP .inc. DUP @ + ! ;
: strclr ( addr -- ) 0 SWAP ! ;

: strcat ( from-addr to-addr -- ) SWAP COUNT 0 DO
		DUP @ 2 PICK str+ 1+
	LOOP DROP DROP
	;
	
: strcpy ( from-addr to-addr -- ) DUP strclr strcat ;

: strcmp ( addr1 addr2 -- bool ) 2DUP @ SWAP @ =
	IF
		-1 >R
		1+ SWAP COUNT 0 
		DO
			2DUP @ SWAP @ <>
			IF 
				R> DROP 0 >R LEAVE 
			ELSE
				1+ SWAP 1+
			THEN
		LOOP
		DROP DROP R>
	ELSE
		DROP DROP
		0
	THEN ;

: HEAD>NAME 3 + ;
: HEAD>BODY 2+ @ ;
: NAME>HEAD 3 - ;
: BODY>HEAD 1+ @ ;

: .fw. ( addr1 -- word-addr|0 ) LAST 
	BEGIN
		DUP MEM_LAST = IF 
			DROP DROP 0 EXIT
		THEN

		2DUP HEAD>NAME strcmp
		IF SWAP DROP EXIT THEN

		DUP @ +
	REPEAT
	;

: FIND-WORD .word. PAD .fw. ;
: ' FIND-WORD DUP IF HEAD>BODY THEN ;
: EXECUTE ( addr -- ) ?DUP IF >R THEN ;

: FILL ( addr n b -- ) ROT ROT OVER + SWAP DO DUP I ! LOOP DROP ;
: ERASE ( addr n -- ) 0 FILL ;

: :NONAME ( -- code-addr ) HERE ] ;
: NONAME; ?] IF 99 , 0 STATE ! THEN ; IMMEDIATE
( Usage: :NONAME 1 2 3 + + . NONAME; )

: .fl.  LAST HEAD>BODY (HERE) ! LAST DUP @ + (LAST) ! ;

: .wordcount. ( -- n ) 0 LAST
	BEGIN
		DUP MEM_LAST = IF 
			DROP EXIT
		THEN

		SWAP 1+ SWAP

		DUP @ +
	REPEAT
	;

break;
