: BootStrap.4th ;

HERE 999 ! LAST 998 !
: .bsr 999 @ (HERE) ! 998 @ (LAST) ! ;

: ] 1 STATE ! ;
: [ 0 STATE ! ; IMMEDIATE
: ?] STATE @ ;

: + ?] IF 10 , ELSE [ 10 , ] THEN ; IMMEDIATE

: // SOURCE + >IN ! ; 

// Primitives ... macro-assembler building blocks ...
: SWAP   ?] IF  5 , ELSE [  5 , ] THEN ; IMMEDIATE
: DROP   ?] IF  6 , ELSE [  6 , ] THEN ; IMMEDIATE
: PICK   ?] IF  7 , ELSE [  7 , ] THEN ; IMMEDIATE
: ROT    ?] IF  8 , ELSE [  8 , ] THEN ; IMMEDIATE
: 1+     ?] IF  9 , ELSE [  9 , ] THEN ; IMMEDIATE // not required as an opcode, for performance
//             10 (+) was needed earlier
: -      ?] IF 11 , ELSE [ 11 , ] THEN ; IMMEDIATE
: *      ?] IF 12 , ELSE [ 12 , ] THEN ; IMMEDIATE
: /      ?] IF 13 , ELSE [ 13 , ] THEN ; IMMEDIATE
: =      ?] IF 14 , ELSE [ 14 , ] THEN ; IMMEDIATE
: <>     ?] IF 15 , ELSE [ 15 , ] THEN ; IMMEDIATE
: <      ?] IF 16 , ELSE [ 16 , ] THEN ; IMMEDIATE
: >      ?] IF 17 , ELSE [ 17 , ] THEN ; IMMEDIATE
: >R     ?] IF 18 , ELSE [ 18 , ] THEN ; IMMEDIATE
: R>     ?] IF 19 , ELSE [ 19 , ] THEN ; IMMEDIATE
: R@     ?] IF 20 , ELSE [ 20 , ] THEN ; IMMEDIATE
//             21 is the IF runtime logic
: DO     ?] IF 22 , ELSE [ 22 , ] THEN ; IMMEDIATE
: I      ?] IF 23 , ELSE [ 23 , ] THEN ; IMMEDIATE
: LEAVE  ?] IF 24 , ELSE [ 24 , ] THEN ; IMMEDIATE
: LOOP   ?] IF 25 , ELSE [ 25 , ] THEN ; IMMEDIATE
: +LOOP  ?] IF 26 , ELSE [ 26 , ] THEN ; IMMEDIATE
: BEGIN  ?] IF HERE THEN ; IMMEDIATE // No opcode needed for this
: REPEAT ?] IF 27 , , THEN ; IMMEDIATE
: .      ?] IF 28 , ELSE [ 28 , ] THEN ; IMMEDIATE
//             29 used to be TYPE, now it is obsolete
//             30 is CALL
: OVER   ?] IF 31 , ELSE [ 31 , ] THEN ; IMMEDIATE // not required as an opcode, for performance
: EMIT   ?] IF 32 , ELSE [ 32 , ] THEN ; IMMEDIATE
//             33 is DICTP, a NOOP to skip over the dictionary entry back pointer
: 1-     ?] IF 34 , ELSE [ 34 , ] THEN ; IMMEDIATE // not required as an opcode, for performance
: 0=     ?] IF 35 , ELSE [ 35 , ] THEN ; IMMEDIATE // not required as an opcode, for performance
: EXIT   ?] IF 99 , THEN ; IMMEDIATE 

: <= 1+ < ;
: >= 1- > ;
: 2* 2 * ;
: 2/ 2 / ;
: 2+ 1+ 1+ ;

: NIP SWAP DROP ;
: TUCK SWAP OVER ;
: -ROT ROT ROT ;
: NEGATE 0 SWAP - ;
: ABS DUP 0 < IF NEGATE THEN ;

: 2DROP DROP DROP ;
: 2DUP OVER OVER ;
: ?DUP DUP IF DUP THEN ;

: mod 2dup / * - ;

// string stuff
: TYPE 0 DO DUP @ EMIT 1+ LOOP DROP ;

: STR+ DUP .INC. DUP @ + ! ;      // ( C ADDR -- ) 
: STRCLR 0 SWAP ! ;               // ( ADDR -- ) 

// ( from-addr to-addr -- ) 
: STRCAT SWAP COUNT 0 DO 
		DUP @ 2 PICK STR+ 1+
	LOOP 2DROP
	;

//	( from-addr to-addr -- )
: STRCPY DUP STRCLR STRCAT ;

: BL 32 ;

: .ISWS.
	DUP 13 = IF DROP BL THEN
	DUP 10 = IF DROP BL THEN
	DUP BL = ;

: string, COUNT DUP , 0 DO DUP @ , 1+ LOOP DROP ;

: ( SOURCE >IN @ DO DUP I + @ ')' = IF DROP I 1+ >IN ! LEAVE THEN LOOP ; IMMEDIATE

// ( c min max -- bool )
: BETWEEN 2 PICK >= >R >= R> = ;

: ALLOCATE HERE SWAP 0 DO 0 , LOOP ;
: ALLOT ALLOCATE DROP ;
: VARIABLE 1 ALLOCATE CREATE 33 , LAST , 3 , , 99 , ;

// ( n -- q r )
: /MOD DUP BASE @ / DUP BASE @ * ROT SWAP - ;

: .BL BL EMIT ;
: .CR 13 EMIT 10 EMIT ;

// Case sensitve string compare
// ( addr1 addr2 -- bool ) 
: strcmp 2DUP @ SWAP @ =
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
		2DROP R>
	ELSE
		2DROP
		0
	THEN ;

// ( C -- c )
: TO-UPPER DUP 'a' 'z' BETWEEN IF 32 - THEN ;

// Case insensitve string compare
: STRCMPI ( ADDR1 ADDR2 -- BOOL ) 2DUP @ SWAP @ =
	IF
		-1 >R
		1+ SWAP COUNT 0 
		DO
			2DUP @ TO-UPPER SWAP @ TO-UPPER <>
			IF 
				R> DROP 0 >R LEAVE 
			ELSE
				1+ SWAP 1+
			THEN
		LOOP
		2DROP R>
	ELSE
		2DROP
		0
	THEN ;

: HEAD>NAME 3 + ;
: HEAD>BODY 2+ @ ;
: NAME>HEAD 3 - ;
: BODY>HEAD 1+ @ ;

: .FW. ( ADDR1 -- WORD-ADDR|0 ) LAST 
	BEGIN
		DUP MEM_LAST = IF 
			2DROP 0 EXIT
		THEN

		2DUP HEAD>NAME STRCMPI
		IF NIP EXIT THEN

		DUP @ +
	REPEAT
	;

: FIND-WORD .WORD. PAD .FW. ;
: ' FIND-WORD DUP IF HEAD>BODY THEN ;
: EXECUTE ( addr -- ) ?DUP IF >R THEN ;

: FILL ( addr n b -- ) -rot OVER + SWAP DO DUP I ! LOOP DROP ;
: ERASE ( addr n -- ) 0 FILL ;

: :NONAME ( -- code-addr ) HERE ] ;
: NONAME; ?] IF 99 , 0 STATE ! THEN ; IMMEDIATE
( Usage: :NONAME 1 2 3 + + . NONAME; )

: .wordcount. ( -- n ) 0 LAST
	BEGIN
		DUP MEM_LAST = IF 
			DROP . EXIT
		THEN

		SWAP 1+ SWAP

		DUP @ +
	REPEAT
	;

: .dictsize. MEM_LAST LAST - . ;
: .codesize. HERE 1001 - . ;
: .S DEPTH . '-' EMIT .BL DEPTH IF -1 DEPTH 1- 1- DO I PICK . -1 +LOOP THEN ;

: .dict. ( -- n ) LAST
	BEGIN
		DUP MEM_LAST = IF 
			DROP EXIT
		THEN
		DUP . DUP 1+ @ . DUP HEAD>BODY . DUP HEAD>NAME COUNT TYPE .CR 
		DUP @ +
	REPEAT
	;

: words ( -- n ) LAST
	BEGIN
		DUP MEM_LAST = IF 
			DROP EXIT
		THEN
		
		DUP HEAD>NAME COUNT TYPE .BL
		DUP @ +
	REPEAT
	;

: .collect. ( addr delim -- )
	>R DUP strclr
	SOURCE >IN @ 1+
	DO DUP I + @ DUP R@ = 
		IF 
			2DROP I 1+ >IN ! LEAVE 
		ELSE
			2 PICK str+ 
		THEN 
	LOOP
	R> 2DROP
	;

: ." PAD '"' .collect. ?] 
	IF 
	  3 , HERE 0 , 30 , [ 3 , ' COUNT , ] , 30 , [ 3 , ' TYPE , ] , 27 , HERE SWAP 0 , HERE SWAP !
		PAD string,
		HERE SWAP !
	ELSE 
		PAD [ 30 , ' COUNT , 30 , ' TYPE , ]
	THEN ; IMMEDIATE

// ********************************************************************************
// Arrays return the starting address of the array.
// This is a "counted" array, just like strings, so the first cell is the size.
// Use COUNT to get the number of elements and address of the first cell.
// Indexes go from 0 to n-1 (base 0)
// 10 array test    ... 
// 12 3 test >array ... stores 12 in position 3 of array 'test'
// 3 test array>    ... fetch value at position 3 from array 'test'
// ********************************************************************************

: ARRAY DUP 1+ ALLOCATE TUCK ! CREATE 33 , LAST , 3 , , 99 , ;
: ARRAY.Check.Bounds COUNT 1- 2 pick swap 0 swap between if -1 else ." index out of bounds." 0 then ;
: >ARRAY ( val pos array -- ) Array.Check.Bounds if + ! else 2drop then ;
: ARRAY> ( pos array -- val ) Array.Check.Bounds if + @ else 2drop then ;
: .ARRAY COUNT 0 DO DUP @ . 1+ LOOP DROP ;

// ********************************************************************************

: forget find-word ?dup if dup HEAD>BODY (HERE) ! DUP @ + (LAST) ! then ;
: forget.last  LAST HEAD>BODY (HERE) ! LAST DUP @ + (LAST) ! ;

: ?free last here - ;
: .free ?free . ;

break;
