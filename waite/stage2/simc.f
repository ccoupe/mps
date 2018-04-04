      SUBROUTINE PROGR
      DIMENSION LIST(12000) 
C 
C         SET A LIMIT ON THE ARRAY INDEX TO ENSURE THAT A READ
C         OPERATION WILL NOT OVERRUN THE ARRAY BOUNDS.
C 
      KMAX=12000-80 
C 
C     THE ARRAY LIST IS USED AS FOLLOWS-
C 
C         LIST(1) = SOURCE END-OF-LINE FLAG.
C         LIST(2) = SOURCE PARAMETER FLAG.
C         LIST(3) = MCT END-OF-LINE FLAG. 
C         LIST(4) = MCT PARAMETER FLAG. 
C         LIST(5) = THE CHARACTER ZERO. 
C         LIST(6) = SYMBOL GENERATOR (INITIALIZED TO 100) 
C         LIST(7) = SPACE FOR A PARAMETER 0 VALUE.
C         LIST(8) THROUGH LIST(16) = SPACE FOR PARAMETER 1-9 VALUES.
C         LIST(17) AND UP = SPACE FOR MACRO DEFINITIONS AND SOURCE LINE.
C 
C     VARIABLES I,J,K,L,M,N ARE USED AS POINTERS AND TEMPORARIES. 
C 
C     EACH MACRO IS STORED AS A BLOCK.  IF LIST(M) IS THE FIRST ELEMENT,
C     THEN THE LAYOUT OF THE BLOCK IS-
C 
C         LIST(M) = INDEX OF NEXT MACRO TEMPLATE. 
C         LIST(M+1) = LARGEST MODIFIER OF A PARAMETER 0 CALL USED IN
C                   THIS MACRO.  IF PARAMETER ZERO IS NOT USED IN THE 
C                   MACRO, LIST(M+1)=-1.  THE SYMBOL GENERATOR WILL BE
C                   INCREMENTED BY (LIST(M+1)+1) AFTER THE CODE BODY
C                   HAS BEEN COMPLETELY SCANNED.
C         LIST(M+2) = FIRST CHARACTER OF THE TEMPLATE.  SUCCESSIVE
C                   ELEMENTS OF LIST CONTAIN SUCCESSIVE CHARACTERS OF 
C                   THE TEMPLATE.  THE END-OF-LINE FLAG IS REPLACED BY
C                   -1.  THE CODE BODY LINES FOLLOW, 1 CHARACTER PER
C                   WORD, EACH TERMINATED BY -1.
C 
C     WITHIN A CODE BODY, EACH PARAMETER CALL IS REPRESENTED BY THREE 
C     ELEMENTS.  THE FIRST ELEMENT CONTAINS -2, THE SECOND IS THE INDEX 
C     OF THE ELEMENT OF LIST IN WHICH THE PARAMETER VALUE WILL BE STORED
C     BY THE TEMPLATE MATCH, AND THE THIRD CONTAINS THE INTEGER VALUE 
C     OF THE MODIFIER DIGIT.  FOR EXAMPLE, IF ' IS THE MCT PARAMETER
C     FLAG, THEN '41 IS STORED AS-
C 
C         -2        (MARKER TO INDICATE THE PARAMETER CALL) 
C         +11       (PARAMETER 4 VALUE WILL BE STORED AT LIST(11))
C         +1        (INTEGER EQUAL TO CHARACTER 1)
C 
C 
C         READ THE FLAG LINE. 
C 
      IF (IOOP(-1,1,LIST,1,I) .NE. 0)  STOP 10
C 
C         INITIALIZE THE SYMBOL GENERATOR AND THE MACRO DEFINITION INDEX
C 
      LIST(6)=100 
      K=18
C 
C         READ ONE MACRO DEFINITION.  THE SYMBOL GENERATOR INCREMENT
C         FOR THE MACRO IS INITIALLY -1.  IT WILL BE MODIFIED AS CALLS
C         ON PARAMETER ZERO ARE RECOGNIZED. 
C 
    1 LIST(K)=-1
C 
C         READ THE TEMPLATE LINE FOR THE MACRO. 
C 
      IF (IOOP(-1,1,LIST(2),K,I) .NE. 0)  STOP 11 
      IF (I .GE. KMAX) STOP 20
C 
C         GUARANTEE THAT THERE IS A SOURCE CODE END-OF-LINE SYMBOL AT 
C         THE END OF THE TEMPLATE LINE. 
C 
      LIST(I+1)=LIST(1) 
C 
C         SCAN THE TEMPLATE LINE FOR THE SOURCE CODE END-OF-LINE. 
C 
      I=K 
    2 I=I+1 
      IF (LIST(I)-LIST(1))  2,13,2
C 
C         PROCESS THE LINES OF THE CODE BODY.  A CODE BODY LINE IS READ 
C         IN AT LABEL 13.  AT LABEL 10 THE LINE IS IN THE ARRAY.  IT
C         MUST BE SCANNED FOR FLAG CHARACTERS.
C 
   10 I=I+1 
C 
C         SIGNAL THE END OF THE LINE WHEN AN END-OF-LINE FLAG APPEARS.
C 
      IF (LIST(I) .EQ. LIST(3))  GO TO 12 
C 
C         CONTINUE IF THE CURRENT CHARACTER IS NOT A FLAG.
C 
   11 IF (LIST(I) .NE. LIST(4))  GO TO 10 
C 
C         REPLACE THE PARAMETER FLAG WITH A MARKER, AND TRANSLATE THE 
C         PARAMETER DIGIT AND MODIFIER TO INTEGERS. 
C 
      LIST(I)=-2
      LIST(I+1)=LIST(I+1)-LIST(5)+7 
      I=I+2 
      LIST(I)=LIST(I)-LIST(5) 
C 
C         IF THE CALL IS NOT ON PARAMETER ZERO, THEN THERE IS NOTHING 
C         FURTHER TO BE DONE. 
C 
      IF (LIST(I-1) .NE. 7)  GO TO 10 
C 
C         CHECK THE CURRENT VALUE OF THE SYMBOL GENERATOR MODIFIER, AND 
C         INCREASE IT IF NECESSARY. 
C 
      IF (LIST(K) .LT. LIST(I))  LIST(K)=LIST(I)
      GO TO 10
C 
C         AN END-OF-LINE FLAG HAS BEEN RECOGNIZED.  REPLACE IT WITH -1. 
C 
   12 LIST(I)=-1
C 
C         READ THE NEXT LINE OF THE CODE BODY.
C 
   13 I=I+1 
      IF (IOOP(-1,1,LIST,I,J) .NE. 0)  STOP 12
      IF (J .GE. KMAX)  STOP 21 
C 
C         ENSURE THAT IT CONTAINS AN END-OF-LINE FLAG.
C 
      LIST(J)=LIST(3) 
C 
C         IF THE FLAG IS NOT IN THE FIRST CHARACTER POSITION, THEN THE
C         CURRENT LINE IS A VALID LINE OF THE CODE BODY.
C 
      IF (LIST(I) .NE. LIST(3))  GO TO 11 
C 
C         OTHERWISE WE HAVE COME TO THE END OF THE CURRENT MACRO.  THE
C         NEXT MACRO LINK MUST BE SET TO POINT TO THE POSITION WHERE
C         THE NEXT TEMPLATE WILL BE READ, AND THE MACRO DEFINITION INDEX
C         MUST BE ADVANCED. 
C 
      LIST(K-1)=I 
      K=I+1 
C 
C         IF THE LAST LINE DID NOT CONSIST OF TWO SUCCESSIVE END-OF-LINE
C         FLAGS, THEN THERE ARE MORE MACRO DEFINITIONS. 
C 
      IF (K .GE. J)  GO TO 1
      IF (LIST(K) .NE. LIST(3))  GO TO 1
C 
C         READ IN THE NEXT LINE TO BE EXPANDED, AND ENSURE THAT IT
C         CONTAINS A SOURCE CODE END-OF-LINE FLAG.  WHEN AN EOF IS
C         DETECTED, ALL OF THE SOURCE CODE HAS BEEN EXPANDED. 
C 
   20 IF (IOOP(-1,1,LIST,I,N) .NE. 0)  RETURN 
      LIST(N)=LIST(1) 
C 
C         INITIALIZE THE MACRO DEFINITION INDEX FOR THE TEMPLATE MATCH. 
C 
      M=17
C 
C         TRY MATCHING THE CURRENT INPUT LINE TO THE NEXT TEMPLATE. 
C         SET THE PARAMETER SPACE INDEX, AND POINT J TO THE START OF THE
C         TEMPLATE. 
C 
   30 L=8 
      J=M+1 
C 
C         MATCH THE LINE TO THE TEMPLATE, CHARACTER BY CHARACTER. 
C 
      DO 33 K=I,N 
      J=J+1 
C 
C         IF THE CURRENT TEMPLATE CHARACTER IS A PARAMETER FLAG, THEN 
C         STORE THE CORRESPONDING CHARACTER OF THE INPUT LINE.
C 
      IF (LIST(J) .EQ. LIST(2))  GO TO 32 
C 
C         OTHERWISE, IF THE CURRENT CHARACTER OF THE INPUT LINE DOES
C         NOT MATCH THE CORRESPONDING CHARACTER OF THE TEMPLATE, THEN 
C         MOVE ON TO THE NEXT MACRO DEFINITION. 
C 
      IF (LIST(J) .NE. LIST(K))  GO TO 31 
C 
C         IF THE LAST CHARACTER MATCHED WAS A SOURCE CODE END-OF-LINE 
C         FLAG, THEN GO PUT OUT THE CODE BODY.  OTHERWISE, CONTINUE 
C         SCANNING THE LINE.
C 
      IF (LIST(J)-LIST(1))  33,40,33
C 
C         A MISMATCH HAS OCCURRED.  MOVE ON TO THE NEXT MACRO DEFINITION
C         IF ANOTHER DEFINITION EXISTS. 
C 
   31 M=LIST(M) 
      IF (M .LT. I)  GO TO 30 
C 
C         OTHERWISE WRITE OUT THE INPUT LINE UNCHANGED AND READ ANOTHER.
C 
      IF (IOOP(1,3,LIST,I,N) .NE. 0)  STOP 30 
      GO TO 20
C 
C         A PARAMETER FLAG HAS BEEN RECOGNIZED IN THE TEMPLATE.  IF THE 
C         CORRESPONDING INPUT CHARACTER IS A SOURCE CODE END-OF-LINE
C         FLAG, THEN SIGNAL A MISMATCH. 
C 
   32 IF (LIST(K) .EQ. LIST(1))  GO TO 31 
C 
C         OTHERWISE STORE THE INPUT CHARACTER IN THE CURRENT PARAMETER
C         STORE AND INCREMENT THE PARAMETER INDEX.
C 
      LIST(L)=LIST(K) 
      L=L+1 
   33 CONTINUE
C 
C         CONSTRUCT AN OUTPUT LINE FROM A CODE BODY LINE OF THE MACRO 
C         WHOSE TEMPLATE WAS MATCHED.  ENTRY TO THE ROUTINE IS AT 40. 
C 
C         MOVE ONE CHARACTER FROM THE CODE BODY TO THE LINE BEING BUILT.
C 
   41 LIST(K)=LIST(J) 
C 
C         ADVANCE THE INDEX OF THE LINE BEING BUILT.
C 
   42 K=K+1 
C 
C         ADVANCE THE INDEX OF THE CODE BODY. 
C 
   43 J=J+1 
C 
C         IF THE CODE BODY IS EXHAUSTED, CLEAN UP AND QUIT. 
C 
      IF (LIST(M) .EQ. J)  GO TO 48 
C 
C         IF THE NEXT CHARACTER OF THE CODE BODY IS A PARAMETER CALL OR 
C         END-OF-LINE, TREAT IT SPECIALLY.  OTHERWISE COPY IT TO THE
C         LINE BEING BUILT. 
C 
      IF (LIST(J)+1)  44,47,41
C 
C         GET THE PARAMETER INDEX AND ADVANCE THE CODE BODY INDEX TO
C         THE LAST CHARACTER OF THE PARAMETER CALL. 
C 
   44 L=LIST(J+1) 
      J=J+2 
C 
C         TREAT A CALL ON PARAMETER ZERO SPECIALLY. 
C 
      IF (L .EQ. 7)  GO TO 45 
C 
C         IF THE MODIFIER OF THE PARAMETER CALL WAS NOT 0, THEN THE 
C         CHARACTER MUST BE CONVERTED TO THE EQUIVALENT DECIMAL INTEGER.
C 
      IF (LIST(J) .NE. 0)  GO TO 46 
C 
C         OTHERWISE SIMPLY COPY THE PARAMETER CHARACTER INTO THE LINE 
C         BEING BUILT.
C 
      LIST(K)=LIST(L) 
      GO TO 42
C 
C         COMPUTE THE GENERATED SYMBOL FOR A PARAMETER 0 CALL.
C 
   45 LIST(7)=LIST(J)+LIST(6) 
C 
C         TRANSLATE A PARAMETER INTO AN EQUIVALENT DECIMAL INTEGER.  THE
C         CONVERSION WILL ALWAYS YIELD 3 DIGITS.
C 
   46 LIST(K)=LIST(L)/100 
      N=LIST(L)/10
      LIST(K+1)=N-LIST(K)*10+LIST(5)
      LIST(K+2)=LIST(L)-N*10+LIST(5)
      LIST(K)=LIST(K)+LIST(5) 
C 
C         ADVANCE THE INDEX OF THE LINE BEING BUILT.
C 
      K=K+3 
      GO TO 43
C 
C         WHEN AN END-OF-LINE FLAG IS RECOGNIZED, WRITE OUT THE LINE
C         WHICH WAS BUILT FROM THE CODE BODY LINE.
C 
   47 IF (IOOP(1,3,LIST,I,K) .NE. 0)  STOP 31 
C 
C         RESET THE INDEX AND COMMENCE BUILDING A NEW LINE FROM THE CODE
C         BODY OF THE CURRENT MACRO.
C 
   40 K=I 
      GO TO 43
C 
C         THE CURRENT MACRO HAS BEEN COMPLETELY EXPANDED.  INCREMENT
C         THE SYMBOL GENERATOR AND READ ANOTHER INPUT LINE. 
C 
   48 LIST(6)=LIST(M+1)+LIST(6)+1 
      GO TO 20
C 
      END 
