HERE 999 ! LAST 998 !
HERE (cw) ! LAST , 0 , 1 , ']' ,                   (cw) @ , 1 STATE ! 1 STATE ! ;
HERE (cw) ! LAST , 0 , 2 , 'S' , 'W' , (cw) @ , ] HERE (cw) ! LAST , 0 , ; 

SW 9 , 'I' , 'M' , 'M' , 'E' , 'D' , 'I' , 'A' , 'T' , 'E' , (cw) @ , ] 1 LAST 1 + ! ;
SW 1 , '[' ,                   (cw) @ , ] 0 STATE ! ; IMMEDIATE
SW 2 , '?' , ']' ,             (cw) @ , ] STATE @ ; 
SW 4 , '.' , 'b' , 's' , 'r' , (cw) @ , ] 999 @ (HERE) ! 998 @ (LAST) ! ;

SW 3 , 'D' , 'U' , 'P' ,       (cw) @ , ] ?] IF  4 , ELSE [  4 , ] THEN ; IMMEDIATE
SW 4 , 'S' , 'W' , 'A' , 'P' , (cw) @ , ] ?] IF  5 , ELSE [  5 , ] THEN ; IMMEDIATE
SW 4 , 'D' , 'R' , 'O' , 'P' , (cw) @ , ] ?] IF  6 , ELSE [  6 , ] THEN ; IMMEDIATE
SW 2 , '1' , '+' ,             (cw) @ , ] ?] IF  9 , ELSE [  9 , ] THEN ; IMMEDIATE
SW 1 , '=' ,                   (cw) @ , ] ?] IF 14 , ELSE [ 14 , ] THEN ; IMMEDIATE
SW 1 , '>' ,                   (cw) @ , ] ?] IF 17 , ELSE [ 17 , ] THEN ; IMMEDIATE
SW 2 , '>' , 'R' ,             (cw) @ , ] ?] IF 18 , ELSE [ 18 , ] THEN ; IMMEDIATE
SW 2 , 'R' , '>' ,             (cw) @ , ] ?] IF 19 , ELSE [ 19 , ] THEN ; IMMEDIATE
SW 2 , 'R' , '@' ,             (cw) @ , ] ?] IF 20 , ELSE [ 20 , ] THEN ; IMMEDIATE
SW 1 , '.' ,                   (cw) @ , ] ?] IF 28 , ELSE [ 28 , ] THEN ; IMMEDIATE
SW 4 , 'T' , 'Y' , 'P' , 'E' , (cw) @ , ] ?] IF 29 , ELSE [ 29 , ] THEN ; IMMEDIATE
SW 4 , 'O' , 'V' , 'E' , 'R' , (cw) @ , ] ?] IF 31 , ELSE [ 31 , ] THEN ; IMMEDIATE

SW 2 , 'D' , 'O' ,                   (cw) @ , ] ?] IF 22 , THEN ; IMMEDIATE
SW 1 , 'I' ,                         (cw) @ , ] ?] IF 23 , THEN ; IMMEDIATE
SW 5 , 'L' , 'E' , 'A' , 'V' , 'E' , (cw) @ , ] ?] IF 24 , THEN ; IMMEDIATE
SW 4 , 'L' , 'O' , 'O' , 'P' ,       (cw) @ , ] ?] IF 25 , THEN ; IMMEDIATE
SW 5 , '+' , 'L' , 'O' , 'O' , 'P' , (cw) @ , ] ?] IF 26 , THEN ; IMMEDIATE

SW 5 , '(' , 'i' , 'n' , 'c' , ')' ,       (cw) @ , ] DUP @ 1+ SWAP ! ;
SW 5 , 'C' , 'O' , 'U' , 'N' , 'T' ,       (cw) @ , ] DUP @ SWAP 1+ SWAP ;

SW 6 , '.' , 'g' , 'e' , 't' , 'c' , 'h' , (cw) @ , ] (source) @ >IN @ + @ >IN (inc) ;

SW 6 , 'S' , 'O' , 'U' , 'R' , 'C' , 'E' , (cw) @ , ] (source) @ COUNT ;

SW 2 , 'B' , 'L' , (cw) @ , ] 32 ;

SW 4 , 'I' , 'S' , 'W' , 'S' , (cw) @ , ] 
		DUP 13 = IF DROP BL THEN
		DUP 10 = IF DROP BL THEN
		DUP BL = 
	;

SW 3 , '.' , 's' , 's' , (cw) @ , ] 
	SOURCE >IN @
	DO
		DUP I + @ ISWS
		IF
			DROP >IN (inc)
		ELSE
			DROP LEAVE
		THEN
	LOOP DROP ;

SW 6 , 'C' , 'R' , 'E' , 'A' , 'T' , 'E' , (cw) @ , ]
	.ss
	HERE (cw) ! LAST , 0 , HERE >R 0 ,
	SOURCE >IN @ 
	DO 
		DUP >IN @ + @ DUP BL = 
		IF 
			DROP LEAVE
		ELSE
			,
			R@ (inc)
			>IN (inc)
		THEN
	LOOP
	DROP R> DROP
	(cw) @ ,
	;

999 CREATE abc ] 777 ; 888

SW 1 , ':' , (cw) @ , ] CREATE ] ;


: ( SOURCE >IN @ DO DUP I + @ ')' = IF DROP I 2 + >IN ! LEAVE THEN LOOP ; IMMEDIATE

: PICK ?] IF  7 , ELSE [  7 , ] THEN ; IMMEDIATE
: ROT  ?] IF  8 , ELSE [  8 , ] THEN ; IMMEDIATE
: -    ?] IF 11 , ELSE [ 11 , ] THEN ; IMMEDIATE
: *    ?] IF 12 , ELSE [ 12 , ] THEN ; IMMEDIATE
: /    ?] IF 13 , ELSE [ 13 , ] THEN ; IMMEDIATE
: <>   ?] IF 15 , ELSE [ 15 , ] THEN ; IMMEDIATE

: VAR CREATE 3 , HERE 2 + , 99 , 0 , (cw) @ (LAST) ! ;
: ALLOT 0 DO 0 , LOOP ;

: /MOD DUP BASE @ / DUP BASE @ * ROT SWAP - ;

VAR .ct
: CTYPE .ct ! .ct 1 TYPE ;
: .BL BL CTYPE ;
: .CR 13 CTYPE 10 CTYPE ;

: str+ ( addr -- addr ) DUP (inc) DUP @ + ! ;
: strclr ( addr -- addr ) 0 OVER ! ;

: 2DUP OVER OVER ;
: ?DUP DUP IF DUP THEN ;

: strcat ( addr -- addr ) SWAP COUNT 0 DO
		DUP @ 2 PICK str+ 1+
	LOOP DROP
	;
	
: strcpy ( addr -- addr ) strclr strcat ;
	
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

: >XT 2 + COUNT + 1+ ;
: XT> 1 - @ ;
: >NAME 2 + ;
: NAME> 2 - ;
: XT>NAME XT> >NAME ;


: FIND-WORD ( addr1 -- word-addr|0 ) LAST 
	10000 0 DO
		2DUP >NAME strcmp
		IF SWAP DROP LEAVE 
		ELSE DUP 0 =
			IF DROP DROP 0 LEAVE ELSE @ THEN
		THEN
	LOOP ;

: .wrd ;
: FW .wrd PAD FIND-WORD ;
: ' FW DUP IF >XT THEN ;
: EXECUTE ?DUP IF >R THEN ;

: .fl LAST (HERE) ! LAST @ (LAST) ! ;
