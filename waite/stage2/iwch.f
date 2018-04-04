      SUBROUTINE IWRCH(JCHAR,JBUFF,JINDX,JFLAG,JLENG) 
C 
C  THIS ROUTINE ENTERS A CHARACTER INTO THE LINE BUFFER.  IF THE
C  CHARACTER IS NEGATIVE, IT IS TAKEN TO BE A TERMINATOR.  IF THE LINE
C  BUFFER IS FULL, IT IS ALSO TERMINATED AND THE CHARACTER IS IGNORED.
C  IF THE LINE BUFFER IS TERMINATED, A FLAG IS RETURNED TO THE USER 
C  INDICATING THIS FACT.  THE LINE BUFFER LIMIT IS SET TO INDEX THE 
C  POSITION BEYOND THE LAST SIGNIFICANT CHARACTER.  IF THE LINE BUFFER
C  IS NOT TERMINATED, THEN THE CHARACTER IS ENTERED AT THE INDEXED
C  POSITION AND THE INDEX IS INCREMENTED.  THE LIMIT INDEX IS NOT 
C  ALTERED IN THIS CASE.
C 
C         JCHAR  IS THE INTEGER REPRESENTING THE CHARACTER
C 
C         JBUFF  IS THE ADDRESS OF THE LINE BUFFER
C 
C         JINDX  IS THE INDEX TO THE LINE BUFFER
C 
C         JFLAG IS SET TO ZERO IF THE LINE BUFFER IS NOT FULL AND THE 
C              CHARACTER INSERTED WAS NOT CR/LF.  IT IS SET TO 1
C              OTHERWISE
C 
C        JLENG IS THE LAST+1 INDEX OF INFORMATION IN THE BUFFER 
C 
      DIMENSION JBUFF(81) 
C 
C         IF THE CHARACTER IS NEGATIVE, THE BUFFER MUST BE TERMINATED.
C         IT IS IMMATERIAL WHETHER THE BUFFER IS FULL OR NOT. 
C 
      IF (JCHAR .LT. 0)  GO TO 1
C 
C        IF THE USER IS ATTEMPTING TO PUT A SIGNIFICANT CHARACTER INTO
C         THE LINE BUFFER, WE MUST ENSURE THAT THERE IS ENOUGH ROOM.
C 
      IF (80 .LT. JINDX)  GO TO 1 
C 
C         IF THERE IS ENOUGH ROOM, PUT THE CHARACTER INTO THE BUFFER AND
C         INCREMENT THE CURRENT CHARACTER POSITION. 
C 
      JBUFF(JINDX)=JCHAR
      JINDX=JINDX+1 
C 
C         SET THE FLAG TO INDICATE ACCEPTANCE OF THE CURRENT CHARACTER
C         AND RETURN TO THE CALLER. 
C 
      JFLAG=0 
      RETURN
C 
C         THE LINE BUFFER HAS BEEN TERMINATED.  SET THE TERMINATING 
C         CHARACTER AND THE LIMIT INDEX.
C 
    1 JBUFF(JINDX)=-1 
      JLENG=JINDX 
C 
C         RESET THE USER INDEX FOR THE NEXT LINE. 
C 
      JINDX=1 
C 
C         SET THE FLAG TO INDICATE REJECTION OF THE CURRENT CHARACTER 
C         AND RETURN TO THE CALLER. 
C 
      JFLAG=1 
      RETURN
C 
      END 
