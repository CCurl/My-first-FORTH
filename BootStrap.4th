HERE 999 ! LAST 998 !
HERE (cw) ! LAST , 0 , 1 , ']' , (cw) @ , 1 STATE ! 1 STATE ! ;
HERE (cw) ! LAST , 0 , 4 , '.' , 'b' , 's' , 'r' , (cw) @ , ] 999 @ (HERE) ! 998 @ (LAST) ! ;
HERE (cw) ! LAST , 0 , 9 , 'I' , 'M' , 'M' , 'E' , 'D' , 'I' , 'A' , 'T' , 'E' , (cw) @ , ] 1 LAST 1 + ! ;
HERE (cw) ! LAST , 0 , 2 , '?' , ']' , (cw) @ , ] STATE @ ; 
HERE (cw) ! LAST , 0 , 1 , '[' , (cw) @ , ] 0 STATE ! ; IMMEDIATE

HERE (cw) ! LAST , 0 , 2 , 'S' , 'W' , (cw) @ , ] HERE (cw) ! LAST , 0 , ; 

SW 3 , 'D' , 'U' , 'P' , (cw) @ , ] ?] IF 4 , ELSE [ 4 , ] ENDIF ; IMMEDIATE
SW 2 , '1' , '+' , (cw) @ , ] ?] IF 9 , ELSE [ 9 , ] ENDIF ; IMMEDIATE

SW 4 , 'S' , 'W' , 'A' , 'P' , (cw) @ , ] ?] IF 5 , ELSE [ 5 , ] ENDIF ; IMMEDIATE
SW 4 , 'D' , 'R' , 'O' , 'P' , (cw) @ , ] ?] IF 6 , ELSE [ 6 , ] ENDIF ; IMMEDIATE
SW 4 , 'O' , 'V' , 'E' , 'R' , (cw) @ , ] ?] IF 31 , ELSE [ 31 , ] ENDIF ; IMMEDIATE

SW 1 , '=' , (cw) @ , ] ?]       IF 14 , ELSE [ 14 , ] ENDIF ; IMMEDIATE
SW 2 , '<' , '>' , (cw) @ , ] ?] IF 15 , ELSE [ 15 , ] ENDIF ; IMMEDIATE
SW 1 , '>' , (cw) @ , ] ?]       IF 17 , ELSE [ 17 , ] ENDIF ; IMMEDIATE

SW 2 , 'D' , 'O' , (cw) @ ,                   ] ?] IF 22 , ENDIF ; IMMEDIATE
SW 1 , 'I' , (cw) @ ,                         ] ?] IF 23 , ENDIF ; IMMEDIATE
SW 5 , 'L' , 'E' , 'A' , 'V' , 'E' , (cw) @ , ] ?] IF 24 , ENDIF ; IMMEDIATE
SW 4 , 'L' , 'O' , 'O' , 'P' , (cw) @ ,       ] ?] IF 25 , ENDIF ; IMMEDIATE
SW 5 , '+' , 'L' , 'O' , 'O' , 'P' , (cw) @ , ] ?] IF 26 , ENDIF ; IMMEDIATE

SW 5 , '(' , 'i' , 'n' , 'c' , ')' , (cw) @ , ] DUP @ 1+ OVER ! ;
SW 6 , '.' , 'g' , 'e' , 't' , 'c' , 'h' , (cw) @ , ] >IN (inc) DROP SOURCE >IN @ + @ ;

SW 2 , 'B' , 'L' , (cw) @ , ] 32 ;
SW 3 , '.' , 's' , 's' , (cw) @ , ] 
	100 0
	DO 
		SOURCE @ >IN @ > 
		IF LEAVE 
		ELSE
			SOURCE >IN @ + @ BL =
			IF
				>IN (inc) DROP
			ELSE
				LEAVE
			ENDIF
		ENDIF
	LOOP ;

SW 4 , 'I' , 'S' , 'W' , 'S' , (cw) @ , ] 
		DUP 13 = IF DROP BL ENDIF
		DUP 10 = IF DROP BL ENDIF
		DUP BL = 
	;

SW 5 , 'C' , 'O' , 'U' , 'N' , 'T' , (cw) @ , ] DUP @ SWAP 1+ SWAP ;

SW 4 , '.' , 'w' , 'r' , 'd' , (cw) @ , ] 
	0 PAD !
	31 0 DO
		.getch ISWS 
		IF 
			DROP LEAVE
		ELSE
			PAD (inc) DUP @ + !
		ENDIF
	LOOP ;

SW 6 , 'C' , 'R' , 'E' , 'A' , 'T' , 'E' , (cw) @ , ] 
	.ss
	.wrd
	HERE (cw) ! LAST , 0 , PAD @ ,
	PAD COUNT 0
	DO DUP @ , 1+ LOOP 
	(cw) @ ,
	DROP
	;

SW 1 , ':' , (cw) @ , ] CREATE ] ;
