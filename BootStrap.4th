HERE 999 ! LAST 998 !
HERE (cw) ! LAST , 0 , 1 , ']' , (cw) @ , 1 STATE ! 1 STATE ! ;
HERE (cw) ! LAST , 0 , 4 , '.' , 'b' , 's' , 'r' , (cw) @ , ] 999 @ (HERE) ! 998 @ (LAST) ! ;
HERE (cw) ! LAST , 0 , 9 , 'I' , 'M' , 'M' , 'E' , 'D' , 'I' , 'A' , 'T' , 'E' , (cw) @ , ] 1 LAST 1 + ! ;
HERE (cw) ! LAST , 0 , 2 , '?' , ']' , (cw) @ , ] STATE @ ; 
HERE (cw) ! LAST , 0 , 1 , '[' , (cw) @ , ] 0 STATE ! ; IMMEDIATE

HERE (cw) ! LAST , 0 , 2 , 'S' , 'W' , ] HERE (cw) ! LAST , 0 , ; 

SW 3 , 'D' , 'U' , 'P' , ] ?] IF 4 , ELSE [ 4 , ] ENDIF ; IMMEDIATE
SW 2 , '1' , '+' , ] ?] IF 9 , ELSE [ 9 , ] ENDIF ; IMMEDIATE

SW 4 , 'D' , 'R' , 'O' , 'P' , ] ?] IF 6 , ELSE [ 6 , ] ENDIF ; IMMEDIATE

SW 1 , '=' , ] ?]       IF 14 , ELSE [ 14 , ] ENDIF ; IMMEDIATE
SW 2 , '<' , '>' , ] ?] IF 15 , ELSE [ 15 , ] ENDIF ; IMMEDIATE

SW 2 , 'D' , 'O' ,                   ] ?] IF 22 , ENDIF ; IMMEDIATE
SW 1 , 'I' ,                         ] ?] IF 23 , ENDIF ; IMMEDIATE
SW 5 , 'L' , 'E' , 'A' , 'V' , 'E' , ] ?] IF 24 , ENDIF ; IMMEDIATE
SW 4 , 'L' , 'O' , 'O' , 'P' ,       ] ?] IF 25 , ENDIF ; IMMEDIATE
SW 5 , '+' , 'L' , 'O' , 'O' , 'P' , ] ?] IF 26 , ENDIF ; IMMEDIATE

SW 2 , 'B' , 'L' , ] 32 ;
SW 3 , '.' , 's' , 's' , ] 
	SOURCE @ SOURCE 1+ + >IN @  
	DO 
		I @ BL <> 
		IF I >IN ! LEAVE ENDIF 
	LOOP ;

SW 6 , 'C' , 'R' , 'E' , 'A' , 'T' , 'E' , ] 
	.ss
	HERE (cw) ! LAST , 0 , 0 ,
	0 7 !
	SOURCE @ SOURCE 1+ + >IN @  
	DO 
		I @ DUP BL = 
		IF 
			I >IN !
			DROP LEAVE 
		ELSE , 7 @ 1+ 7 !
		ENDIF 
	LOOP 7 @ (cw) @ 2 + ! ;

SW 1 , ':' , ] CREATE ] ;

: PICK ?] IF 7 , ELSE [ 7 , ] ENDIF  ; IMMEDIATE 
: OVER 1 PICK ;
: 2DUP OVER OVER ;

: (inc) DUP @ 1+ OVER ! ;
: ( SOURCE DUP @ + 1+ >IN @ DO >IN (inc) @ @ ')' = IF >IN (inc) DROP LEAVE ELSE ENDIF LOOP ; IMMEDIATE

: - ?] IF 11 , ELSE [ 11 , ] ENDIF  ; IMMEDIATE 
: * ?] IF 12 , ELSE [ 12 , ] ENDIF  ; IMMEDIATE 
: / ?] IF 13 , ELSE [ 13 , ] ENDIF  ; IMMEDIATE 
: < ?] IF 16 , ELSE [ 16 , ] ENDIF  ; IMMEDIATE 
: > ?] IF 17 , ELSE [ 17 , ] ENDIF  ; IMMEDIATE 

: SWAP ?] IF 5 , ELSE [ 5 , ] ENDIF  ; IMMEDIATE 
: ROT  ?] IF 8 , ELSE [ 8 , ] ENDIF  ; IMMEDIATE 

: r1 5 ; 
: r2 6 ; 
: r3 7 ; 
: r4 8 ; 

: . ?] IF 28 , ELSE [ 28 , ] ENDIF  ; IMMEDIATE 
: TYPE ?] IF 29 , ELSE [ 29 , ] ENDIF  ; IMMEDIATE 
: COUNT DUP @ SWAP 1+ SWAP ;
: CTYPE r2 ! r2 1 TYPE ;
: .BL BL CTYPE ;
: .CR 13 CTYPE 10 CTYPE ;

: PUSH 3 , ; 
: RET 99 , ; 

: VAR CREATE PUSH HERE 2 + , RET 0 , (cw) @ (LAST) ! ;
: ALLOT 0 DO 0 , LOOP ;

: DECIMAL 10 BASE ! ; DECIMAL 
: HEX 16 BASE ! ; : OCTAL 8 BASE ! ; : BINARY 2 BASE ! ;

: /MOD ( n1 -- n2 u ) DUP BASE @ / DUP BASE @ * ROT SWAP - ;

: ?LAST LAST 2 + COUNT TYPE ;

: WORDS LAST 
	10000 0 DO
		DUP 2 + COUNT TYPE .BL
		@ DUP 0 =
		IF DROP LEAVE ENDIF
	LOOP ;

: str+ ( c addr -- addr ) (inc) DUP @ + ! ;
: strclr ( addr -- addr ) 0 OVER ! ;

: strcpy ( addr1 addr2 -- addr2 ) strclr SWAP COUNT 0 DO
		DUP @ 2 PICK str+ 1+
	LOOP DROP
	;

: strcmp ( addr1 addr2 -- bool ) r2 ! r1 ! -1 r3 ! 
	r2 @ @ r1 @ @ = 
	IF 
		r2 @ @ 1+ 1 
		DO
			r2 @ I + @
			r1 @ I + @
			<> IF 0 r3 ! LEAVE ENDIF
		LOOP
	ELSE
		0 r3 !
	ENDIF
	r3 @ ;

: >XT DUP IF 2 + COUNT + ENDIF ;
: >NAME 2 + ;

VAR tw 
: FIND-WORD ( addr1 -- addr2|0 ) tw ! LAST 
	10000 0 DO
		DUP >NAME tw @ strcmp
		IF LEAVE 
		ELSE @ DUP 0 = 
			IF LEAVE ENDIF
		ENDIF
	LOOP ;

: ?DUP DUP IF DUP ENDIF ;

: fl LAST (HERE) ! LAST @ (LAST) ! ;

: reduce 0 SWAP 100 0 
	DO
		/MOD ROT + SWAP DUP 
		0 = IF 
			DROP DUP
			10 < IF LEAVE ELSE 0 SWAP ENDIF 
			ELSE SWAP
		ENDIF
	LOOP ;

