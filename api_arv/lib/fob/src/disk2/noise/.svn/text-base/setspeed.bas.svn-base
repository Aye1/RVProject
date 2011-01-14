'Filename=SETSPEED.bas
'
'AUTOMATICALLY SETS THE BIRD MEASUREMENT RATE BY READING THE SPEED AND
'BAUD RATE CHARACTERISTICS FROM A FILE CALLED SPEED.BRD and OUTPUTTING THIS
'INFO TO THE BIRD
'
'programming language is Microsoft quickbasic
'Written July 24, 1993


DECLARE SUB DUMPbyte (BYTEaddr AS INTEGER, MEM.TYPE AS INTEGER, BYTE AS INTEGER)
DECLARE SUB GET232inputWORD (WORD AS INTEGER, AD AS SINGLE)
DECLARE SUB GETBIRDparameter (Parameter AS INTEGER)
DECLARE SUB INITbirdADDRESSES ()
DECLARE SUB INITcom1 ()
DECLARE SUB initFLOCKaddresses ()
DECLARE SUB INTtoBYTES (WORD AS INTEGER, MSbyte AS INTEGER, LSbyte AS INTEGER)
DECLARE SUB PAUSE ()
DECLARE SUB READsoftwareREV ()
DECLARE SUB SPEEDsend ()
DECLARE SUB STATUSdecode (STATUS AS INTEGER)
DECLARE SUB BYTEStoWORD (MSbyte AS INTEGER, LSbyte AS INTEGER, WORD AS INTEGER, AD AS SINGLE)




OPTION BASE 1         'ARRAYS START AT 1

DIM command AS INTEGER, OUTMODE AS INTEGER, done AS INTEGER
DIM birddata(48) AS SINGLE
DIM Numwords AS INTEGER, POSK AS SINGLE, ANGK AS SINGLE, DTR AS SINGLE
DIM Bmax(12) AS SINGLE, Bmin(12) AS SINGLE, mean(12) AS SINGLE
DIM DEVIATION(12) AS SINGLE, PKtoPK(12) AS SINGLE, SX(12) AS DOUBLE
DIM SX2(12) AS DOUBLE, NOISEstat AS INTEGER, Naver AS INTEGER
DIM clock AS SINGLE, ItoF AS SINGLE
DIM revno AS SINGLE, CRYSTAL AS INTEGER
DIM CONFIGrb(36) AS INTEGER
DIM GRAPHICSflag AS INTEGER
DIM Image(1 TO 500)  AS INTEGER
DIM firstentry AS INTEGER
DIM Xpos AS SINGLE, Ypos AS SINGLE, Zpos AS SINGLE, M(3, 3) AS SINGLE
DIM Xo  AS SINGLE, Yo AS SINGLE, Zo AS SINGLE
DIM SevenValues(14)  AS INTEGER
DIM sa(3) AS SINGLE, xa(3) AS SINGLE
DIM XDR(3, 3) AS SINGLE, SDR(3, 3) AS SINGLE, angle(3) AS SINGLE
DIM flockaddr(10) AS INTEGER, POWERSUPPLYdacaddr AS INTEGER, AtoDmuxaddr AS INTEGER
DIM pcb004rev AS INTEGER, Nflockaddr AS INTEGER
DIM flockaddress AS INTEGER, RTD AS SINGLE, RATE AS SINGLE
DIM XMTR.TIME.CNT AS SINGLE


CLS     'clear the screen
PRINT
PRINT
PRINT "***************************************************************"
PRINT " Copyright 1993, Ascension Technology Corp. All rights reserved."
PRINT "***************************************************************"
PRINT




KEY 15, CHR$(0) + CHR$(1)           'trap on escape key with no caps lock
KEY 16, CHR$(64) + CHR$(1)          'trap on escape key with caps lock
ON KEY(15) GOSUB ESCAPEkeyTRAP         'WAY OUT IF ESCAPE KEY HIT
ON KEY(16) GOSUB ESCAPEkeyTRAP
KEY(15) ON       'TURN ON THE TRAP
KEY(16) ON

ON TIMER(2) GOSUB ESCAPEkeyTRAP       'if more than 2 sec in readBIRDdata



filename$ = "SPEED.BRD"
OPEN filename$ FOR INPUT AS #1
'read past the 9 lines of instructions in the file
FOR i = 1 TO 9
LINE INPUT #1, a$
NEXT i

LINE INPUT #1, BAUD$       'read the baud rate from the file
LINE INPUT #1, COMPORT$         'read the com port to use
LINE INPUT #1, SPEED$       'read the measurement rate
CLOSE #1

PRINT
PRINT "VALUES READ FROM FILE SPEED.BRD"
PRINT "BAUD RATE = "; BAUD$
PRINT "COM PORT = "; COMPORT$
PRINT "SPEED = "; SPEED$
PRINT

CALL INITcom1          'initialize the rs232 port

CALL READsoftwareREV      'get software rev number into revno
CALL GETBIRDparameter(2)   'get the CPU crystal speed and return CLOCK

CALL SPEEDsend              'send the measurement rate to the bird

CLOSE #1
END
STOP


'***************************************************************************

'SUBROUTINE TO PROVIDE WAY OUT WHEN ESCAPE KEY HIT

ESCAPEkeyTRAP:
CLOSE #1
CLS        'clear the screen
PRINT
PRINT
PRINT "***You hit the ESCAPE key***"
PRINT "            or"
PRINT "***The RS-232c port is hung***"
PRINT
PRINT
PRINT "The program will restart on hitting carriage return"
PRINT
CALL PAUSE
RUN             'startup again

RETURN

SUB BYTEStoWORD (MSbyte AS INTEGER, LSbyte AS INTEGER, WORD AS INTEGER, AD AS SINGLE)

'SUBROUTINE BYTEStoWORD( MSbyte, LSbyte, WORD, AD )

'CONVERT BYTES TO A 16 BIT WORD
'
'GIVEN THE BYTES MSbyte AND LSbyte
'RETURNS AN INTEGER 'WORD' IN BIPOLAR FORMAT BETWEEN -32768 AND 32767
'AND RETURNS THE REAL 'AD' BETWEEN 0 AND 65535

'WRITTEN MAY 3, 1990


DIM AN AS SINGLE


AD = MSbyte * 256! + LSbyte
AN = AD

'CONVERT TO 2'S COMPLEMENT BETWEEN +32767 / -32768
IF AD > 32767! THEN AN = AD - 65536!
WORD = AN


END SUB

SUB DUMPbyte (BYTEaddr AS INTEGER, MEM.TYPE AS INTEGER, BYTE AS INTEGER)


'GETS 1 'BYTE' FROM BIRD'S MEMORY AT OFFSET ADDR='BYTEaddr

'IF MEM.TYPE=0 THE BYTE COMES FROM THE DATA SEGMENT (RAM MEMORY)
'IF MEM.TYPE=1 THE BYTE COMES FROM THE CODE SEGMENT (PROM MEMORY)

DIM MSBaddr AS INTEGER, LSBaddr AS INTEGER, i AS INTEGER



'CONVERT 16 BIT 'BYTEaddr' INTO TWO BYTES

MSBaddr = INT(BYTEaddr / 256)
LSBaddr = BYTEaddr - MSBaddr * 256

SENDFORbyte:
'SEND DUMP COMMAND TO BIRD

IF MEM.TYPE = 0 THEN
   PRINT #1, "d"; CHR$(LSBaddr); CHR$(MSBaddr);    'GET BYTE FROM RAM
ELSE
   PRINT #1, "k"; CHR$(LSBaddr); CHR$(MSBaddr);    'GET BYTE FROM PROM
END IF

FOR i = 1 TO 500                'CHECK FOR MISSED COMMAND
IF EOF(1) = 0 GOTO READABYTE    'NOT MISSED GO GET BYTE

NEXT i

PRINT "INPUT TIMEOUT"
a$ = INKEY$
IF a$ <> "" THEN END   'A KEY HIT TO GET OUT IF LOCKED UP

GOTO SENDFORbyte                'TRY AGAIN


READABYTE:
'READ BYTE FROM BIRD
B$ = INPUT$(1, #1)
'FIND NUMERIC VALUE OF BYTE  RETURNS WITH VALUE IN INTEGER CALLED BYTE
BYTE = ASC(B$)

END SUB

SUB GET232inputWORD (WORD AS INTEGER, AD AS SINGLE)

'READS TWO BYTES FROM THE RS232 PORT AND RETURNS AN INTEGER 'WORD'
'BETWEEN -32768 AND 32767 ALSO RETURNS A SINGLE 'AD' BETWEEN 0-65535

DIM MSbyte AS INTEGER, LSbyte AS INTEGER

'READ LSbyte FROM BIRD'D RS232 PORT
'WAIT TO SEE IF BIRD GOT COMMAND
CHECKINPUT:
FOR i = 1 TO 1500        'AT 9600 BAUD SEEMS YOU CAN USE 10 TO 10000
IF EOF(1) = 0 THEN GOTO READBYTE          'DATA IN BUFFER READ IT
NEXT i
 PRINT "INPUT TIME OUT - HIT A KEY TO RETURN"
a$ = INKEY$
IF a$ <> "" THEN EXIT SUB        'PROVIDE A WAY OUT IF LOCKUP BY HITTING A KEY
GOTO CHECKINPUT           'TRY AGAIN

READBYTE:

'READ THE LSbyte FROM BIRD
B$ = INPUT$(1, #1)
'
'READ MSbyte FROM BIRD
c$ = INPUT$(1, #1)
'CONVERT STRING TO BYTES TO WORD
LSbyte = ASC(B$)
MSbyte = ASC(c$)

'GET AD FROM 0 TO 65535 AND WORD FROM -32768 TO 32767
CALL BYTEStoWORD(MSbyte, LSbyte, WORD, AD)


END SUB

SUB GETBIRDparameter (Parameter AS INTEGER)

SHARED CONFIGrb() AS INTEGER, SevenValues() AS INTEGER



'given a parameter number uses the Birds EXAMINE VALUE command to get
'and decode the parameter

'parameter numbers that can be passed
'
'    0          System status
'    1          Software revision number
'    2          Bird computer crystal speed
'    3          Maximum range scaling
'    4          Filter ON/OFF status
'    5          Filter constant ALPHA_MIN
'    6          System measurement rate
'    7          CONFIGURATION data
'   12          VM
'   13          ALPHA_MAX
'   14          Sudden output change


'written  10/19/90 by ebblood
'revised 10/30/90 rev # calculation was in error, fractional is MSbyte
'revised 11/5/90 added to FILToff 2 cycle ac filter
'revised 2/2/91 dac filter deleted and 2 cycle ac filter takes its place
'revised 5/15/91 now gets CONFIGURATION data
'revised 8/28/91 now gets Vm filter data
'revised 3/4/92 alpha_min, alpha_max now a function of xampcode


SHARED clock AS SINGLE, revno AS SINGLE, POSK AS SINGLE, CRYSTAL AS INTEGER

DIM WORD AS INTEGER, AD AS SINGLE, i AS INTEGER
DIM MSbyte AS INTEGER, LSbyte AS INTEGER


'send EXAMINE VALUE command to Bird
PRINT #1, "O"; CHR$(Parameter);

IF Parameter = 7 GOTO readconfig
IF Parameter = 5 OR Parameter = 12 OR Parameter = 13 GOTO read7values
IF Parameter = 14 GOTO readlock
IF Parameter = 15 GOTO readmodel

'get the word output from the Bird in response to the EXAMINE VALUE command
CALL GET232inputWORD(WORD, AD)


'decode the word sent by The Bird

IF Parameter = 0 THEN CALL STATUSdecode(WORD)

IF Parameter = 1 THEN

'form the rev no as 2.0,2.1,2.21 ect
'fractional part of rev # is MSbyte, integer part is LSbyte

	CALL INTtoBYTES(WORD, MSbyte, LSbyte)    'convert to bytes

	IF MSbyte < 10 THEN
		revno = LSbyte + MSbyte / 10!
	ELSE
		revno = LSbyte + MSbyte / 100!
	END IF
	PRINT "Software revision number = "; revno

END IF


IF Parameter = 2 THEN
	IF WORD = 0 THEN
	PRINT "*** cannot read CPU SPEED ***  assuming 20 MHz"
	WORD = 20
	END IF
	PRINT "CPU crystal speed = "; WORD; " MHz"
	clock = 8! / WORD      'cpu clock rate in microseconds
	CRYSTAL = WORD         'save the value
END IF


IF Parameter = 3 THEN
	IF WORD = 0 THEN
		PRINT "Maximum range scaling= 36 inches"
		POSK = 36! / 32768!  'SET RANGE SCALING CONSTANT
	END IF

	IF WORD = 1 THEN
		PRINT "Maximum range scaling= 72 inches"
		POSK = 72! / 32768!
	END IF

	IF ABS(WORD) > 1 THEN PRINT "Error in Maximum range scaling"
END IF


IF Parameter = 4 THEN
PRINT WORD
	IF WORD AND 1 THEN PRINT "DC filter is OFF" ELSE PRINT "DC filter is ON"
	IF WORD AND 2 THEN PRINT "AC NOTCH filter is OFF" ELSE PRINT "AC NOTCH filter is ON"
	IF WORD AND 4 THEN PRINT "2 cycle AC filter is OFF" ELSE PRINT "2 cycle AC filter is ON"
	
END IF


IF Parameter = 5 THEN PRINT "ALPHA MIN= "; : PRINT USING "#.###"; WORD / 32768!

IF Parameter = 6 THEN

STIME = AD * clock / 1000!   'CONVERT XMTR SETTLING COUNTs TO MILLISECONDS

PRINT "CURRENT XMTR_TIME_CNT IS "; AD; " COUNTS = "; STIME; " MS"

cycletime = (4! * STIME + .3) / 1000! 'approx measurement cycle in seconds
PRINT "APPROXIMATE MEASUREMENT RATE IS  ";
PRINT USING "### "; 1! / cycletime;
PRINT " measurements per second"
END IF

EXIT SUB


readconfig:

'read 36 bytes of configuration data from the bird
FOR i = 1 TO 36
c$ = INPUT$(1, #1)
'CONVERT STRING TO BYTES TO WORD
CONFIGrb(i) = ASC(c$)
NEXT i

EXIT SUB

readlock:
c$ = INPUT$(1, #1) 'get byte from bird
WORD = ASC(c$)

IF WORD = 0 THEN PRINT "outputs are not locked"
IF WORD = 1 THEN PRINT "outputs are locked"
EXIT SUB

read7values:      'read Vm(i) or alpha_min(i) or alpha_max(i)

'read 14 bytes (7 words) of vm or alpha_min or alpha_max
FOR i = 1 TO 7
'get the word output from the Bird in response to the EXAMINE VALUE command
CALL GET232inputWORD(WORD, AD)
SevenValues(i) = WORD
NEXT i

EXIT SUB

readmodel:


'read 10 bytes of system model id
model$ = ""
FOR i = 1 TO 10
c$ = INPUT$(1, #1)
'CONVERT STRING TO BYTES TO WORD
model$ = model$ + c$
NEXT i
PRINT
PRINT "model = ", model$
PRINT

EXIT SUB




END SUB

SUB INITcom1

'initializes this host computer's communications port


DIM BAUDN AS INTEGER
SHARED COMPORT$, BAUD$

' Trap the baud rate initialization if the baud rate is greater than 19200
' ... since the BASIC OPEN will not allow higher rates
'
IF VAL(BAUD$) > 19200 THEN
	 initbaud$ = "19200"
ELSE
	 initbaud$ = BAUD$
END IF


'since the flock of birds uses the rs232 line RTS request to send
'to put the system into reset you must control the line when you open
'the port. If RS not specified basic then RTS is held electrically high
'a space (3-25v) causing the birds reset line to be held in reset.
'If RS is specified then the RST line is held low,a mark, (-3 to -25v)
'taking the bird out of reset to allow it to run


com$ = COMPORT$ + initbaud$ + ",N,8,1,CD0,DS0,CS0,RS,OP100,BIN"

OPEN com$ FOR RANDOM AS #1


' Fill in the divisor register for the higher baudrates
'
IF VAL(BAUD$) > 19200 THEN
	 IF COMPORT$ = "COM1:" THEN
	  serialportaddr% = &H3F8         ' com 1 base address
	 ELSE
	  serialportaddr% = &H2F8         ' com 2 base address
	 END IF

	 '
	 ' Determine the divisor register value
	 '
	 divisorreglow% = INT(maxbaudrate& / VAL(BAUD$))
	 divisorreghigh% = 0     ' 0 for all baud rates

	 '
	 ' Set the divisor register in the 8250 Type UART
	 '
	 OUT serialportaddr% + 3, &H83               ' DLAB bit set, 8 bit, no parity
	 OUT serialportaddr% + 0, divisorreglow%     ' set the divisor reg
	 OUT serialportaddr% + 1, divisorreghigh%    ' set the divisor reg
	 OUT serialportaddr% + 3, &H3                ' DLAB bit reset 8 bit, no parity
END IF



END SUB

SUB INTtoBYTES (WORD AS INTEGER, MSbyte AS INTEGER, LSbyte AS INTEGER)

DIM WORD1 AS SINGLE

 'PROCEDURE INT_TO_BYTES( WORD, MSbyte, LSbyte )
 '
 ' REVISED 7/12/89 WORD NO LONGER CHANGED SINCE IT CHANGES PASSED PARAMETERS
 '
 'CONVERTS A SIGNED OR UNSIGNED INTEGER 16 BIT WORD TO 2 INTEGER BYTES
 '
WORD1 = WORD
IF WORD1 < 0 THEN WORD1 = WORD1 + 65536
MSbyte = INT(WORD1 / 256)
LSbyte = INT(WORD1 - MSbyte * 256!)


END SUB

SUB PAUSE


'WAITS TILL A KEY IS HIT

PRINT
PRINT "HIT ANY KEY TO CONTINUE"
PRINT
WAITFORCONTINUE:
a$ = INKEY$
IF a$ = "" GOTO WAITFORCONTINUE

END SUB

SUB READsoftwareREV

'READS SOFTWARE REV NUMBER AND DATE FROM BIRD'S PROM MEMORY

'WRITTEN MAY 11, 1990
'revised 10/20/90 software rev # formed as fraction and shared with other programs

SHARED revno AS SINGLE
DIM i AS INTEGER, REVaddr AS INTEGER, BYTES(5) AS INTEGER, BYTE AS INTEGER

REVaddr = 3        'OFFSET ADDR FROM START OF CS FOR REV INTO

FOR i = 1 TO 5      'READ 5 BYTES
  CALL DUMPbyte(REVaddr, 1, BYTES(i))  'READ A BYTE FROM CS
  REVaddr = REVaddr + 1                'INCR TO NEXT BYTE
NEXT i


'form the rev no as 2.0,2.1,2.21 ect
IF BYTES(2) < 10 THEN
	revno = BYTES(1) + BYTES(2) / 10!
ELSE
	revno = BYTES(1) + BYTES(2) / 100!
END IF


'PRINT THE REV INFO

PRINT
PRINT
PRINT "BIRD'S SOFTWARE REV"
PRINT
PRINT "REV NO. "; revno
PRINT "REV DATE "; BYTES(3); "/"; BYTES(4); "/"; BYTES(5)
PRINT

END SUB

SUB SPEEDsend


'sends the speed value to the bird

SHARED clock AS SINGLE, SPEED$




DIM WORD AS INTEGER, AD AS SINGLE
DIM Parameter AS INTEGER, LSbyte AS INTEGER, MSbyte AS INTEGER

Parameter = 6          'parameter no to examine xt_time_cnt




SPEEDcurrent = VAL(SPEED$)
ANOTHERspeed:

	XTIME = (1000! / SPEEDcurrent - .3) * .25

	'convert to xmtr timer counts
	AD = 1000! * XTIME / clock
IF AD >= 65535 THEN
	PRINT
	PRINT "SPEED is too low, increase it"
	PRINT
	STOP
END IF

	'convert to a legal timer word
	IF AD > 32767 THEN
		WORD = AD - 65536
		ELSE
		WORD = AD
	END IF



'convert word to be output to bytes
CALL INTtoBYTES(WORD, MSbyte, LSbyte)

'send the new value to The Bird
PRINT #1, "P"; CHR$(Parameter); CHR$(LSbyte); CHR$(MSbyte);


PRINT
PRINT " ***   NEW SPEED SENT TO THE BIRD   ***"
PRINT

END SUB

SUB STATUSdecode (STATUS AS INTEGER)

'decodes the Bird STATUS byte

'written 10/20/90 by ebblood
'revised 11/2/90 bit 6 message changed from xon/off to factory use
'revised 10/6/92 decodes 16 bits now

DIM bmode AS INTEGER

PRINT "STATUS BIT #        "

'decode bit 15
PRINT "     15           ";
IF (STATUS AND &H8000) / -32768 = 1 THEN
PRINT "This Bird is the MASTER"
ELSE
PRINT "This Bird is a SLAVE"
END IF

'decode bit 14
PRINT "     14           ";
IF (STATUS AND &H4000) / 16384 = 1 THEN
PRINT "This Bird has been initialized"
ELSE
PRINT "This Bird has not been initialized"
END IF


'decode bit 13
PRINT "     13           ";
IF (STATUS AND &H2000) / 8192 = 1 THEN
PRINT "An error has been detected"
ELSE
PRINT "No errors detected"
END IF

'decode bit 12
PRINT "     12           ";
IF (STATUS AND &H1000) / 4096 = 1 THEN
PRINT "The Bird is asleep"
ELSE
PRINT "The Bird is awake"
END IF

'decode bit 11
PRINT "     11           ";
IF (STATUS AND &H800) / 2048 = 1 THEN
PRINT "HOST SYNC MODE ENABLED"
ELSE
PRINT "HOST SYNC MODE NOT ENABLED "
END IF

'decode bit 10
PRINT "     10           ";
IF (STATUS AND &H400) / 1024 = 1 THEN
PRINT "INTRA BIRD FBB IN USE"
ELSE
PRINT "INTRA BIRD FBB NOT IN USE "
END IF

'decode bit 9
PRINT "     9           ";
IF (STATUS AND &H200) / 512 = 1 THEN
PRINT "CRT SYNC ENABLED"
ELSE
PRINT "NO CRT SYNC MODES ENABLED "
END IF

'decode bit 8
PRINT "     8           ";
IF (STATUS AND &H100) / 256 = 1 THEN
PRINT "NO SYNC MODES ARE ENABLED"
ELSE
PRINT "A SYNC MODE IS ENABLED"
END IF

'decode bit 7
PRINT "     7           ";
IF (STATUS AND &H80) / 128 = 1 THEN
PRINT "FACTORY TEST COMMANDS ENABLED"
ELSE
PRINT "USER COMMANDS ENABLED "
END IF

'decode bit 6  XON/XOFF STATUS
PRINT "     6           ";
IF (STATUS AND &H40) / 64 = 1 THEN
PRINT "Xoff"
ELSE
PRINT "Xon"
END IF

'decode bit 5
PRINT "     5           ";
IF (STATUS AND &H10) / 16 = 1 THEN
PRINT "The Bird is asleep"
ELSE
PRINT "The Bird is awake"
END IF

'decode bits 4,3,2,1
PRINT "   4,3,2,1         ";
bmode = (STATUS AND &H1E) / 2   'keep bits 4,3,2,1
IF bmode = 1 THEN PRINT "POSITION outputs selected"
IF bmode = 2 THEN PRINT "ANGLE outputs selected"
IF bmode = 3 THEN PRINT "MATRIX outputs selected"
IF bmode = 4 THEN PRINT "POSITION/ANGLE outputs selected"
IF bmode = 5 THEN PRINT "POSITION/MATRIX outputs selected"
IF bmode = 6 THEN PRINT "FACTORY CAL outputs selected"
IF bmode = 7 THEN PRINT "QUATERNION outputs selected"
IF bmode = 8 THEN PRINT "POSITION/QUATERNION outputs selected"

'decode bit 0
PRINT "     0           ";

IF (STATUS AND &H1) = 1 THEN
PRINT "Stream mode"
ELSE
PRINT "Point mode"
END IF

PRINT
PRINT

END SUB

