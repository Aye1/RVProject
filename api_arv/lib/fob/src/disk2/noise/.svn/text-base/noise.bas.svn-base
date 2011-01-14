'Filename=NOISE.bas
'
'Evaluates the amount of noise in the environment to determine
'the best Bird measurement to minimize the noise effects.
'
'programming language is Microsoft quickbasic
'Written May 5, 1993




DECLARE SUB INSTRUCTIONS ()
DECLARE SUB SPEEDstep ()
DECLARE SUB INTtoBYTES (WORD AS INTEGER, MSbyte AS INTEGER, LSbyte AS INTEGER)
DECLARE SUB XMTRsettling ()
DECLARE SUB ANGLESgraphics ()
DECLARE SUB CHANGEspeed ()
DECLARE SUB initFLOCKaddresses ()
DECLARE SUB KEYBOARDhit (B$)
DECLARE SUB readbird ()
DECLARE SUB GRAPHICSinit ()
DECLARE SUB GETBIRDparameter (Parameter AS INTEGER)
DECLARE SUB STATUSdecode (STATUS AS INTEGER)
DECLARE SUB EXAMINEvalue ()
DECLARE SUB CHANGEvalue ()
DECLARE SUB CRTsync ()
DECLARE SUB GET232inputWORD (WORD AS INTEGER, AD AS SINGLE)
DECLARE SUB MEMORYcheckSUM ()
DECLARE SUB READsoftwareREV ()
DECLARE SUB XMTRsettlingSTEP ()
DECLARE SUB NOISEstatISTICS (TYPEdata AS INTEGER, Naver AS INTEGER, scale AS SINGLE)
DECLARE SUB delay (Dtime AS SINGLE)
DECLARE SUB ENTERfract (WORDaddr AS INTEGER, WORDtoSEND AS SINGLE)
DECLARE SUB INPUTvalue (a$, VALUE AS SINGLE)
DECLARE SUB INITbirdADDRESSES ()
DECLARE SUB KEYBOARDcommands ()
DECLARE SUB GETpositionANGLES ()
DECLARE SUB GETangles ()
DECLARE SUB readBIRDdata ()
DECLARE SUB GETposition ()
DECLARE SUB PAUSE ()
DECLARE SUB INITcom1 ()
DECLARE SUB DUMPbyte (BYTEaddr AS INTEGER, MEM.TYPE AS INTEGER, BYTE AS INTEGER)
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



ANGK = 180! / 32768!   'CONVERT INTEGER TO DEGREES
DTR = 3.141593 / 180!  'CONVERT DEGREES TO RADIANS
RTD = 180! / 3.141593
ItoF = 1! / 32768!     'INTEGER TO FRACTION


CALL INITcom1


PRINT #1, "z";              'enable debug commands in bird
CALL READsoftwareREV      'get software rev number into revno
CALL INITbirdADDRESSES    'initialize REV specific Bird addresses
CALL GETBIRDparameter(2)   'get the CPU crystal speed and return CLOCK
CALL GETBIRDparameter(3)   'get the position scaling POSK
PAUSE
PRINT



TYPEmenu:
SCREEN 0           'non-graphics screen
PRINT

OPmenu:
PRINT "Enter a command code:"
PRINT
PRINT "OPERATING  INSTRUCTIONS  ( 0 )"
PRINT "POSITION                 ( 1 )"
PRINT "ANGLES                   ( 2 )"
PRINT "POSITION/ANGLES          ( 4 )"
PRINT "SCAN for noise           ( 11 )"
PRINT "CRT SYNCHRONIZATION      ( 12 )"
PRINT "EXAMINE VALUE            ( 13 )"
PRINT "CHANGE VALUE             ( 14 )"
PRINT "EXIT program             ( 22 )"
INPUT command
PRINT

IF command = 0 THEN CALL INSTRUCTIONS
IF command > 0 AND command <= 5 THEN

INPUT "point mode ( 0 ), continous point mode ( 1 ), stream mode ( 2 ) ", OUTMODE

NOISEstat = 0             'no noise statistics
IF command = 4 THEN
INPUT "noise statistics no ( 0 )  yes ( 1 ) ", NOISEstat

IF NOISEstat <> 0 THEN INPUT "Number of samples to average ", Naver
END IF


IF OUTMODE = 2 THEN
PRINT
PRINT "***W A R N I N G*****"
PRINT "STREAM mode may cause the communications buffer to overflow in your"
PRINT "computer unless it can execute this Microsoft Basic program very fast"
PRINT " It usually takes a program written in assembly language to handle the"
PRINT " STREAM data when high baud rates are selected."
PRINT " Try lowering the baud rate and/or setting the REPORT RATE to less"
PRINT " then full speed"
PRINT
CALL PAUSE
END IF
END IF



IF command = 1 THEN CALL GETposition

IF command = 2 THEN
INPUT "graphics output no ( 0 ), yes ( 1 ) ", GRAPHICSflag
IF GRAPHICSflag = 0 THEN
CALL GETangles
ELSE
CALL ANGLESgraphics
END IF
END IF

IF command = 4 THEN CALL GETpositionANGLES
IF command = 11 THEN CALL SPEEDstep


IF command = 12 THEN CALL CRTsync
IF command = 13 THEN CALL EXAMINEvalue
IF command = 14 THEN CALL CHANGEvalue
IF command = 22 THEN END

PRINT
CALL PAUSE
PRINT
PRINT
GOTO TYPEmenu

STOP


'***************************************************************************

'SUBROUTINE TO PROVIDE WAY OUT WHEN ESCAPE KEY HIT

ESCAPEkeyTRAP:

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

SUB ANGLESgraphics

'graphs the the Euler angle outputs from the bird


SHARED birddata() AS SINGLE, ANGK AS SINGLE, Numwords AS INTEGER
SHARED done AS INTEGER, OUTMODE AS INTEGER
SHARED GRAPHICSflag AS INTEGER, Image() AS INTEGER
SHARED firstentry AS INTEGER, RATE AS SINGLE, XMTR.TIME.CNT AS SINGLE




DIM azim AS SINGLE, elev AS SINGLE, roll AS SINGLE, CHANGESCALE AS INTEGER
DIM Xv AS INTEGER, Yv AS INTEGER, Xold AS INTEGER, Yold AS INTEGER

MAXazim = -999!
MINazim = 999!
MAXelev = -999!
MINelev = 999!
MAXroll = -999!
MINroll = 999!

'define the screen scaling for azim,elev
'degrees per full scale ie degrees for 1/2 screen deflection from center
AZIMscale = 2!
ELEVscale = 2!

basetime = 10    '10 seconds per sweep
eraseimage = 1   'do not erase old image

PRINT
INPUT "elev vs azim  (0), elev vs time (1) ,azim vs time (2) ", basetype
IF basetype = 0 THEN eraseimage = 0   'erase of old image

PRINT
PRINT "AZIM +/- full scale in degrees ="; AZIMscale
PRINT "ELEV +/- full scale in degrees ="; ELEVscale
PRINT
INPUT "Change the scaling values NO ( 0 ) ,  YES ( 1 )  ", CHANGESCALE
IF CHANGESCALE <> 0 THEN
INPUT "AZIM full scale in degrees  ", AZIMscale
INPUT "ELEV full scale in degrees  ", ELEVscale
END IF
CLS
PRINT
PRINT
PRINT " hit Q to quit"
PRINT " hit I to increase measurement rate"
PRINT " hit D to decrease measurement rate"
PRINT
PRINT
PRINT
CALL PAUSE

CALL GRAPHICSinit

'draw horizontal and vertical scales
LINE (0, 240)-(640, 240)     'horizontal
LINE (320, 0)-(320, 480)



'send angle command to bird
PRINT #1, "W";
Numwords = 3                '3 words to return from bird
IF OUTMODE = 2 THEN PRINT #1, "@";     'turn on stream mode


readangles2:

CALL readBIRDdata         'get the azim,elev,roll angle words

'convert angles to degrees
azim = birddata(1) * ANGK
elev = birddata(2) * ANGK
roll = birddata(3) * ANGK

'find the peak to peak variations in the angles

IF azim > MAXazim THEN MAXazim = azim
IF azim < MINazim THEN MINazim = azim
IF elev > MAXelev THEN MAXelev = elev
IF elev < MINelev THEN MINelev = elev
IF roll > MAXroll THEN MAXroll = roll
IF roll < MINroll THEN MINroll = roll


IF firstentry = 0 THEN 'if first time through collect offsets to center graphics
	azim0 = azim
	elev0 = elev
	roll0 = roll
	t0 = TIMER
END IF




azim = azim - azim0
elev = elev - elev0
roll = roll - roll0


'CONVERT AZIM TO X POSITION OF THE SCREEN
'SCREEN X COORDINATES  GO FROM 0 TO 680 LEFT TO RIGHT
'SCREEN Y COORDINATES GO FROM 0 TO 480 TOP TO BOTTOM


IF basetype >= 1 THEN  'USE TIME AS THE HORIZONTAL AXIS

'if using time as the base instead of azim
t = TIMER - t0
tbase = t

IF tbase > basetime THEN
CLS
tbase = 0    'basetime seconds of data on screen
t0 = TIMER

'draw horizontal and vertical scales
LINE (0, 240)-(640, 240)     'horizontal
LINE (320, 0)-(320, 480)

'compute peak to peak noise for last pass
azimPP = ABS(MAXazim - MINazim)
elevPP = ABS(MAXelev - MINelev)
rollPP = ABS(MAXroll - MINroll)

'reset noise statistics

MAXazim = -999!
MINazim = 999!
MAXelev = -999!
MINelev = 999!
MAXroll = -999!
MINroll = 999!

END IF

Xv = tbase / basetime * 680!
END IF

'convert to graph resolution
IF basetype = 0 THEN Xv = 320 + 320! / AZIMscale * azim
IF basetype <= 1 THEN Yv = 240 - 240! / ELEVscale * elev
IF basetype = 2 THEN Yv = 240 - 240! / AZIMscale * azim


IF Xv > 619 THEN Xv = 619        'keep cross within screen. cross is 20x20
IF Yv > 459 THEN Yv = 459
IF Xv < 0 THEN Xv = 0
IF Yv < 0 THEN Yv = 0


IF firstentry = 0 THEN
firstentry = 1      'don't erase something that doesn't exist it leaves an image

ELSE

'erase old image if it moved. if you erase it all the time you will get flicker

IF eraseimage = 0 THEN   ' erase each draw
PUT (Xold, Yold), Image, XOR      'erase
END IF

PUT (Xv, Yv), Image, XOR  'draw new

END IF
Xold = Xv
Yold = Yv

LOCATE 25, 1
PRINT "Peak to Peak Noise in Degrees - azim,elev,roll"
PRINT USING "####.#  "; azimPP; elevPP; rollPP


LOCATE 28, 1
CALL CHANGEspeed   'HIT I TO INCREASE SPEED, D TO DECREASE, ANY OTHER TO EXIT

PRINT USING "###.##  "; RATE;
PRINT "measurement rate          ";
PRINT USING "#####  "; XMTR.TIME.CNT;
PRINT "XMTR_TIME_CNT"

IF done = 1 THEN EXIT SUB   'a key stroke terminated collection

GOTO readangles2



END SUB

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

SUB CHANGEspeed

'each time the 'I' key is hit the speed is increased
'each time the 'D' key is hit the speed is decreased
'IF 'Q' THEN EXIT

DIM Parameter AS INTEGER, AD AS SINGLE, WORD AS INTEGER

SHARED clock AS SINGLE, MSbyte AS INTEGER, LSbyte AS INTEGER, done AS INTEGER
SHARED RATE AS SINGLE, XMTR.TIME.CNT AS SINGLE



SPEEDAGAIN:
speedincr = .25    '.25 hz speed change

done = 0
a$ = UCASE$(INKEY$)    'convert all key hits to upper case
IF a$ = "" THEN EXIT SUB

IF a$ = "Q" THEN
done = 1
EXIT SUB
END IF

IF a$ = "I" THEN SPEEDchange = speedincr

IF a$ = "D" THEN SPEEDchange = -speedincr

'*****************************************************
'get current speed


Parameter = 6
'send EXAMINE VALUE command to Bird
'CALL sendrs232address         'send address preface if multiple flocks on one rs232
PRINT #1, "O"; CHR$(Parameter);


'get the word output from the Bird in response to the EXAMINE VALUE command
CALL GET232inputWORD(WORD, XMTR.TIME.CNT)


STIME = XMTR.TIME.CNT * clock / 1000!   'CONVERT XMTR SETTLING COUNTs TO MILLISECONDS
'PRINT "CURRENT SETTLING TIME IS "; XMTR.TIME.CNT; " COUNTS = "; STIME; " MS"
CURRENTspeed = 1000! / (4! * STIME + .3)


'*******************************************************************

'now change the speed
	  RATE = CURRENTspeed + SPEEDchange



aagainrate:
 '       INPUT "enter measurement rate in cycles/sec ", RATE

	IF RATE > 144 THEN
		PRINT
		PRINT "rate must be LESS than 144"
		GOTO SPEEDAGAIN
	END IF

	'convert to xmtr settling time in ms
	STIME = (1000! / RATE - .3) / 4!

	'convert to xmtr timer counts
	XMTR.TIME.CNT = 1000! * STIME / clock

IF XMTR.TIME.CNT >= 65535 THEN
	PRINT
	PRINT "rate is too slow, increase it"
	PRINT
	GOTO SPEEDAGAIN
END IF

	'convert to a legal timer word
	IF XMTR.TIME.CNT > 32767 THEN
		WORD = XMTR.TIME.CNT - 65536
		ELSE
		WORD = XMTR.TIME.CNT
	END IF

'send a change value WORD
'convert word to be output to bytes
CALL INTtoBYTES(WORD, MSbyte, LSbyte)

'send the new value to The Bird
PRINT #1, "P"; CHR$(Parameter); CHR$(LSbyte); CHR$(MSbyte);


END SUB

SUB CHANGEvalue

'change the value of a Bird system parameter

'written 10/19/90 by ebblood
'revised 11/5/90 FILToff now allows 2 cycle AC filter if AC notch is off
'revised 2/2/91 dac ac filter deleted, 2 cycle ac takes its place
'revised 8/28/91 added change Vm
'revised 2/27/92 change value scale factor was wrong

SHARED clock AS SINGLE, CRYSTAL AS INTEGER, POSK AS SINGLE
DIM i AS INTEGER, J AS INTEGER, K AS INTEGER, L AS INTEGER, ALPHAmin AS SINGLE
DIM MSbyte AS INTEGER, LSbyte AS INTEGER, STIME AS SINGLE
DIM Parameter AS INTEGER, WORD AS INTEGER, AD AS SINGLE

PRINT
PRINT "specify parameter to change"
PRINT
PRINT "Maximum range scaling       ( 3 )"
PRINT "FILTER ON/OFF status        ( 4 )"
PRINT "ALPHA_MIN table             ( 5 )"
PRINT "System measurement rate     ( 6 )"
PRINT "Vm dc filter constants      ( 12 )"
PRINT "ALPHA_MAX table             ( 13 )"
PRINT "Sudden Output Change Lock   ( 14 )"
PRINT "FBB Auto Configuration      ( 50 )"

INPUT Parameter
PRINT
PRINT

IF Parameter = 3 THEN

againr:

	INPUT "SET MAXIMUM RANGE TO 36 inches ( 0 ), or 72 inches ( 1 )  ", WORD
	IF WORD > 1 THEN GOTO againr
	PRINT
	'reset position constants for scaling print out
	IF WORD = 0 THEN POSK = 36! / 32768!
	IF WORD = 1 THEN POSK = 72! / 32768!
	GOSUB CHANGEword
	
END IF


IF Parameter = 4 THEN

	INPUT "turn OFF DC filter  YES ( 1 ), NO ( 0 )   ", i
	INPUT "turn OFF notch AC filter  YES ( 1 ), NO ( 0 )  ", J
	INPUT "turn OFF 2 cycle AC filter YES ( 1 ), NO ( 0 )  ", K
	WORD = i + 2 * J + 4 * K      'build FILToff
	GOSUB CHANGEword
END IF


IF Parameter = 5 THEN
	PRINT "input minimum filter constant ALPHA_MIN for each XAMPCODE"
	PRINT "ALPHA_MIN must be >0 and <.9996"
	PRINT "nominal value is 0.02"
	PRINT "increase this value to reduce amount of filtering"
	PRINT "decrease this value to increase amount of filtering"
	PRINT "ALPHA_MIN=.9996 means there is almost no exponential filtering"
	PRINT
	PRINT #1, "P"; CHR$(Parameter);       'send the command,the data will follow
	scale = 32768!        'convert fractional to integer
	GOSUB enter7           'enter the seven vm values
END IF


IF Parameter = 6 THEN
	PRINT "*** WARNING do not reduce the measurement rate if synchronized to a CRT ***"
	PRINT

AGAINrate:
	INPUT "enter measurement rate in cycles/sec ", RATE

	'convert to xmtr settling time in ms
	STIME = (1000! / RATE - .3) / 4!

	'convert to xmtr timer counts
	AD = 1000! * STIME / clock

IF AD > 65535 THEN
	PRINT
	PRINT "rate is too low, increase it"
	GOTO AGAINrate
END IF

	'convert to a legal timer word
	IF AD > 32767 THEN
		WORD = AD - 65536
		ELSE
		WORD = AD
	END IF

GOSUB CHANGEword
END IF


IF Parameter = 12 THEN       'enter the 7 vm values

PRINT #1, "P"; CHR$(Parameter);       'send the command,the data will follow
scale = 1!             'conversion for integer entry
GOSUB enter7           'enter the seven vm values
END IF


IF Parameter = 13 THEN
	PRINT "input filter constant ALPHA_MAX for each XAMPCODE"
	PRINT "ALPHA_MAX must be >ALPHA_MIN and <.9996"
	PRINT "nominal value is 0.9"
	PRINT "increase this value to reduce amount of filtering during fast movements"
	PRINT "decrease this value to increase amount of filtering during fast movements"
	PRINT
	PRINT #1, "P"; CHR$(Parameter);       'send the command,the data will follow
	scale = 32768!        'convert fractional to integer
	GOSUB enter7           'enter the seven alpha_max values
END IF

IF Parameter = 14 THEN
INPUT " allow the outputs to change if a glitch ( 0 ), don't allow the outputs to change ( 1 ) ", LSbyte
GOSUB CHANGEbyte
END IF



IF Parameter = 50 THEN        'FBB auto configuration
INPUT "Specify the number of receivers running from the one transmitter ", LSbyte
GOSUB CHANGEbyte
END IF


EXIT SUB






'********************************************************************
'
'subroutine to send a change value BYTE

CHANGEbyte:
'send the BYTE to The Bird
PRINT #1, "P"; CHR$(Parameter); CHR$(LSbyte);

RETURN



'********************************************************************
'
'subroutine to send a change value WORD
CHANGEword:

'convert word to be output to bytes
CALL INTtoBYTES(WORD, MSbyte, LSbyte)

'send the new value to The Bird
PRINT #1, "P"; CHR$(Parameter); CHR$(LSbyte); CHR$(MSbyte);

RETURN
'********************************************************************
'subroutine to get 7 values from the user and send to the bird

enter7:
PRINT "enter the 7 values, one for each receiver amplifier gain state"
PRINT
FOR i = 1 TO 7
PRINT i - 1; "   ";
INPUT realvalue
WORD = realvalue * scale     'convert to integer
'convert word to be output to bytes
CALL INTtoBYTES(WORD, MSbyte, LSbyte)

'send the new VM or alpha_max or alpha_min value to The Bird
PRINT #1, CHR$(LSbyte); CHR$(MSbyte);
NEXT i
RETURN


END SUB

SUB CRTsync

SHARED clock AS SINGLE
DIM SYNCTYPE AS INTEGER, LSbyte AS INTEGER, MSbyte AS INTEGER
DIM CRT.AMP AS INTEGER, CRT.SPEED AS SINGLE, WORD AS INTEGER
DIM CRTvolts AS SINGLE, HERTZ  AS SINGLE



'PERFORMS CRT SYNCHRONIZATION COMMANDS
'
'WRITTEN 9/23/90 BY EBBLOOD
'REVISED 10/15/90 CRT.SPEED NOW IN HERTZ AND CLOCK COUNTS
'revised 3/6/91 can now sync to 1 crt cycle also

PRINT " DO NOT SYNC BIRD TO THE CRT       ( 0 )"
PRINT " SYCN ONE BIRD CYCLE TO THE CRT    ( 1 )"
PRINT " SYNC TWO BIRD CYCLES TO THE CRT   ( 2 )"
PRINT " LOCATED SYNC PICKUP ON CRT       ( 255 )"
INPUT SYNCTYPE
'
IF SYNCTYPE <> 255 THEN
	PRINT #1, "A"; CHR$(SYNCTYPE);      'SEND SYN COMMAND 0,1 OR 2 TO BIRD
END IF


IF SYNCTYPE = 255 THEN
	PRINT
	PRINT "TWO NUMBERS WILL BE PRINTED, THE FIRST IS THE VOLTAGE"
	PRINT "PICKED UP BY THE PICKUP ATTACHED TO THE BIRD'S SYNC CONNECTOR."
	PRINT "ADJUST THE LOCATION OF THE PICKUP ON YOUR MONITOR CASE UNTIL"
	PRINT "YOU GET A READING OF AT LEAST 3 VOLTS OR PREFERABLY HIGHER."
	PRINT "MAXIMUM READING WILL BE OBTAIN ON THE TOP OR SIDE OF THE CRT"
	PRINT "NEAR THE CRT'S DEFLECTION YOKE.  THE DEFLECTION YOKE IS USUALLY"
	PRINT "LOCATED ABOUT HALFWAY BETWEEN THE FRONT AND BACK OF THE CRT"
	PRINT
	PRINT "THE SECOND NUMBER IS THE VERTICAL RETRACE RATE OF YOUR MONITOR"
	PRINT "IN HERTZ, IF THIS NUMBER IS ZERO AND THE VOLTAGE IS NON-ZERO IT"
	PRINT "MERELY MEANS THAT THE VERTICAL RETRACE RATE IS LESS THAN 47 HZ."
	PRINT "THE BIRD WILL STILL SYNCHRONIZE WITH YOUR SYSTEM AT THESE LOWER RATES."
	PRINT "IF THE RETRACE RATE IS GREATER THAN 68 HZ THE BIRD CANNOT SYNCHRONIZE"
	PRINT "TO THIS HIGHER SPEED. CHECK YOUR SET UP TO ENSURE THAT ALL IS CORRECT"
	PRINT "SINCE DISPLAYES WITH VERTICAL RETRACE RATES GREATER THAN 68 HERTZ"
	PRINT "HAVE NOT YET BEEN BUILD COMMERCIALLY."
	PRINT
	CALL PAUSE

GET255:

	PRINT #1, "A"; CHR$(SYNCTYPE);                'SEND 255
'
	'READ THE FIRST WORD
	CALL GET232inputWORD(CRT.AMP, AD)
	'CONVERT TO VOLTS
	CRTvolts = CRT.AMP * 5! / 32768!

	'READ THE SECOND WORD RETURNED BY BIRD
	CALL GET232inputWORD(WORD, CRT.SPEED) 'GET SPEED AS 0 TO 65535

	'CONVERT CRT.SPEED COUNTS TO SPEED IN HERTZ
	HERTZ = 0
	IF CRT.SPEED <> 0 THEN HERTZ = 1000000! / (clock * CRT.SPEED)
	PRINT CRTvolts, CRT.SPEED, HERTZ
	a$ = INKEY$
	IF a$ = "" GOTO GET255          'REPEAT COMMANDS TILL KEY HIT
 END IF

END SUB

SUB delay (Dtime AS SINGLE)

'DELAYS FOR Dtime NUMBER OF SECONDS
'SMALLEST DELAY POSSIBLE IS APPROX .03 SEC

'WRITTEN MAY 3, 1990

START = TIMER            'READ TIMER
FINISH = START + Dtime   'COMPUTE WHAT FINISH TIME SHOULD BE
NOTtime:
ANOTHER = TIMER                    'READ TIMER AGAIN
IF ANOTHER < FINISH GOTO NOTtime   'WAIT TILL DELAY UP


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

SUB EXAMINEvalue

'examine various Bird system parameters
'written  10/19/90 by ebblood
'revised 5/15/91 examine CONFIGURATION data added
'revised 8/28/91 examine Vm added

SHARED CONFIGrb() AS INTEGER, SevenValues() AS INTEGER


DIM Parameter AS INTEGER, i AS INTEGER



PRINT
PRINT "specify parameter to examine"
PRINT
PRINT "System STATUS               ( 0 )"
PRINT "Software revision number    ( 1 )"
PRINT "Bird computer crystal speed ( 2 )"
PRINT "Maximum range scaling       ( 3 )"
PRINT "FILTER ON/OFF status        ( 4 )"
PRINT "ALPHA_MIN table             ( 5 )"
PRINT "System measurement rate     ( 6 )"
PRINT "CONFIGURATION data          ( 7 )"
PRINT "Vm filter constants         ( 12 )"
PRINT "ALPHA_MAX table             ( 13 )"
PRINT "Sudden Output Change Lock   ( 14 )"
PRINT "System Model Identification ( 15 )"

INPUT Parameter

CALL GETBIRDparameter(Parameter)       'gets selected value

IF Parameter = 7 THEN
FOR i = 1 TO 36
PRINT CONFIGrb(i)
NEXT i
END IF

IF Parameter = 5 OR Parameter = 12 OR Parameter = 13 THEN
scale = 1! / 32768!

	IF Parameter = 12 THEN scale = 1!
	FOR i = 1 TO 7
	PRINT i - 1, SevenValues(i) * scale
	NEXT i
END IF

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

SUB GETangles

'gets and prints the Euler angle outputs from the bird

SHARED birddata() AS SINGLE, ANGK AS SINGLE, Numwords AS INTEGER
SHARED done AS INTEGER, OUTMODE AS INTEGER


DIM azim AS SINGLE, elev AS SINGLE, roll AS SINGLE



'send angle command to bird
PRINT #1, "W";
Numwords = 3                '3 words to return from bird
IF OUTMODE = 2 THEN PRINT #1, "@";     'turn on stream mode


readangles:

CALL readBIRDdata         'get the azim,elev,roll angle words
IF done = 1 THEN EXIT SUB   'a key stroke terminated collection

'
'convert angles to degrees
azim = birddata(1) * ANGK
elev = birddata(2) * ANGK
roll = birddata(3) * ANGK

PRINT USING "####.##   "; azim; elev; roll

GOTO readangles


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

PRINT
PRINT

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

SUB GETposition

'gets and prints the position output from the bird

SHARED birddata() AS SINGLE, POSK AS SINGLE, Numwords AS INTEGER
SHARED done AS INTEGER, OUTMODE AS INTEGER
SHARED firstentry AS INTEGER, GRAPHICSflag AS INTEGER, Image() AS INTEGER


IF GRAPHICSflag <> 0 THEN
PRINT "Ypos is on the horizontal axis"
PRINT "Zpos is on the vertical axis"
PRINT
INPUT " input Ypos offset to center cursor ", Yoffset
INPUT "input Zpos offset to center cursor ", Zoffset
INPUT " Scale factor (inches of pos per inch of display) ", scale
'approximately 480 pixels per 7 inches
scalefactor = 480! / (7! * scale)
END IF


'send position command to bird


PRINT #1, "V";
Numwords = 3               '3 words to return from bird
IF OUTMODE = 2 THEN PRINT #1, "@";     'turn on stream mode

readxyz:
CALL readBIRDdata        'get the x,y,z position words
IF done = 1 THEN EXIT SUB  'exit if keyboard command
'convert position to inches
Xpos = birddata(1) * POSK
Ypos = birddata(2) * POSK
Zpos = birddata(3) * POSK


IF GRAPHICSflag = 0 THEN
PRINT USING "####.##   "; Xpos; Ypos; Zpos

ELSE

'SCREEN Y COORDINATES  GO FROM 0 TO 680 LEFT TO RIGHT
'SCREEN Z COORDINATES GO FROM 0 TO 480 TOP TO BOTTOM

'MAKE Y=0, Z=0 THE CENTER OF THE SCREEN
Xv = 320 + scalefactor * (Ypos - Yoffset)
Yv = 240 - scalefactor * (-Zpos + Zoffset)    '-ZPOS to get correct movement

IF Xv > 619 THEN Xv = 619        'keep cross within screen. cross is 20x20
IF Yv > 459 THEN Yv = 459
IF Xv < 0 THEN Xv = 0
IF Yv < 0 THEN Yv = 0
PUT (Xv, Yv), Image, XOR     'draw new image
IF firstentry = 0 THEN
firstentry = 1      'don't erase something that doesn't exist it leaves an image
ELSE
'erase old image if it moved. if you erase it all the time you will get flicker
'IF (Xold - Xv) OR (Yold - Yv) <> 0 THEN PUT (Xold, Yold), image, XOR 'erase old image
PUT (Xold, Yold), Image, XOR
END IF
Xold = Xv
Yold = Yv
END IF
GOTO readxyz




END SUB

SUB GETpositionANGLES

'GETS AND PRINTS POSITION/ANGLES OUTPUT

SHARED birddata() AS SINGLE, POSK AS SINGLE, Numwords AS INTEGER
SHARED ANGK AS SINGLE, done AS INTEGER, OUTMODE AS INTEGER
SHARED NOISEstat AS INTEGER, Naver AS INTEGER
SHARED mean() AS SINGLE, Bmax() AS SINGLE, Bmin() AS SINGLE
SHARED DEVIATION() AS SINGLE, PKtoPK() AS SINGLE


DIM azim AS SINGLE, elev AS SINGLE, roll AS SINGLE
DIM Xpos AS SINGLE, Ypos AS SINGLE, Zpos AS SINGLE

'send position/angle command to bird
PRINT #1, "Y";
Numwords = 6                 '6 words to return from bird
IF OUTMODE = 2 THEN PRINT #1, "@";     'turn on stream mode

readxyzang:

IF NOISEstat = 0 THEN          'do if no noise statistics selected

CALL readBIRDdata           'get the x,y,z and azim,elev,roll words

'convert position to inches
Xpos = birddata(1) * POSK
Ypos = birddata(2) * POSK
Zpos = birddata(3) * POSK

'convert angles to degrees
azim = birddata(4) * ANGK
elev = birddata(5) * ANGK
roll = birddata(6) * ANGK

PRINT USING "####.##   "; Xpos; Ypos; Zpos; azim; elev; roll

ELSE                  'do when statistics selected

CALL NOISEstatISTICS(1, Naver, 0)
CALL KEYBOARDcommands            'its called from readBIRDdata but with the

PRINT "NOISE FOR "; Naver; " SAMPLES"
PRINT USING "####.##   "; POSK * mean(1); POSK * mean(2); POSK * mean(3);
PRINT USING "####.##   "; ANGK * mean(4); ANGK * mean(5); ANGK * mean(6);
PRINT "   MEAN"
PRINT USING "####.##   "; POSK * Bmax(1); POSK * Bmax(2); POSK * Bmax(3);
PRINT USING "####.##   "; ANGK * Bmax(4); ANGK * Bmax(5); ANGK * Bmax(6);
PRINT "   MAX"
PRINT USING "####.##   "; POSK * Bmin(1); POSK * Bmin(2); POSK * Bmin(3);
PRINT USING "####.##   "; ANGK * Bmin(4); ANGK * Bmin(5); ANGK * Bmin(6);
PRINT "   MIN"
PRINT USING "####.##   "; POSK * PKtoPK(1); POSK * PKtoPK(2); POSK * PKtoPK(3);
PRINT USING "####.##   "; ANGK * PKtoPK(4); ANGK * PKtoPK(5); ANGK * PKtoPK(6);
PRINT "   PEAK TO PEAK VAR"
PRINT USING "####.##   "; POSK * DEVIATION(1); POSK * DEVIATION(2); POSK * DEVIATION(3);
PRINT USING "####.##   "; ANGK * DEVIATION(4); ANGK * DEVIATION(5); ANGK * DEVIATION(6);
PRINT "   STD DEVIATION"
PRINT
PRINT

END IF
IF done = 1 THEN EXIT SUB   'keyboard was hit to getout

GOTO readxyzang


END SUB

SUB GRAPHICSinit

'initialize screen for doing a graphics cross
SHARED Image() AS INTEGER
SHARED firstentry AS INTEGER

firstentry = 0     'clear the flag that tells first time into graphics
SCREEN 12          '480x640 vga graphics

'make a big cross
'LINE (10, 0)-(10, 20)                'vertical line
'LINE (0, 10)-(20, 10)                'horizontal line

'store the image of the cross into the array image()
'GET (0, 0)-(20, 20), Image

'make a small cross
LINE (2, 0)-(2, 4)
LINE (0, 2)-(4, 2)
GET (0, 0)-(4, 4), Image

END SUB

SUB INITbirdADDRESSES

'INITIALIZES BIRD ADDRESSES
'ADDRESSES ARE SOFTWARE REV SPECIFIC

SHARED saddr AS INTEGER, XCaddr AS INTEGER, SCaddr AS INTEGER
SHARED xmtraxisaddr AS INTEGER, senaxisaddr AS INTEGER, ampcodeaddr AS INTEGER
SHARED xmtrgainaddr AS INTEGER, xdacaddr AS INTEGER, ydacaddr AS INTEGER
SHARED zdacaddr AS INTEGER, xt.timeaddr AS INTEGER
SHARED STATUSaddr AS INTEGER
SHARED NOTCH1addr AS INTEGER, NOTCH2addr AS INTEGER, NOTCH3addr AS INTEGER
SHARED revno AS SINGLE
SHARED x.settleaddr AS INTEGER, y.settleaddr AS INTEGER, z.settleaddr AS INTEGER



IF revno <= 2.6 THEN          'REV 2.X BEFORE REV 2.6?
xt.timeaddr = VAL("&H50")
saddr = VAL("&H0B9")
XCaddr = VAL("&H019E")
SCaddr = VAL("&H1B0")
xmtraxisaddr = VAL("&H205")
senaxisaddr = VAL("&H207")
ampcodeaddr = VAL("&H20B")
xmtrgainaddr = VAL("&H20D")
STATUSaddr = VAL("&H28A")
xdacaddr = VAL("&H29C")
ydacaddr = VAL("&H29E")
zdacaddr = VAL("&H2A0")
NOTCH1addr = VAL("&H31A")
NOTCH2addr = VAL("&H31C")
NOTCH3addr = VAL("&H31E")
x.settleaddr = VAL("&h28B")
y.settleaddr = VAL("&h28D")
z.settleaddr = VAL("&H28F")
END IF


IF revno >= 2.7 AND revno < 3! THEN      'REV 2.70 or greater

xt.timeaddr = VAL("&H50")
saddr = VAL("&H0B9")
XCaddr = VAL("&H01c2")
SCaddr = VAL("&H1d4")
xmtraxisaddr = VAL("&H229")
senaxisaddr = VAL("&H22b")
ampcodeaddr = VAL("&H22f")
xmtrgainaddr = VAL("&H231")
STATUSaddr = VAL("&H2bf")
x.settleaddr = VAL("&h2c0")
y.settleaddr = VAL("&h2c2")
z.settleaddr = VAL("&H2c4")
xdacaddr = VAL("&H2d1")
ydacaddr = VAL("&H2d3")
zdacaddr = VAL("&H2d5")
NOTCH1addr = VAL("&H355")
NOTCH2addr = VAL("&H357")
NOTCH3addr = VAL("&H359")

END IF


IF revno >= 3 AND revno <= 3.17 THEN      'use rev 3.0 addresses

xt.timeaddr = VAL("&H2c4")
saddr = VAL("&H0BE")
XCaddr = VAL("&HDC8")
SCaddr = VAL("&H185B")
xmtraxisaddr = VAL("&H1b2")
senaxisaddr = VAL("&H1b4")
ampcodeaddr = VAL("&H1b8")
xmtrgainaddr = VAL("&H1ba")
STATUSaddr = VAL("&H2b7")
xdacaddr = VAL("&H2e4")
ydacaddr = VAL("&H2e6")
zdacaddr = VAL("&H2e8")
NOTCH1addr = VAL("&H38E")
NOTCH2addr = VAL("&H390")
NOTCH3addr = VAL("&H392")
x.settleaddr = VAL("&h2bc")
y.settleaddr = VAL("&h2be")
z.settleaddr = VAL("&H2c0")

END IF

IF revno >= 3.18 THEN CALL initFLOCKaddresses



END SUB

SUB INITcom1

'initializes this host computer's communications port
'works with 19200 although microsoft basic says 9600 is max

'WRITTEN MAY 4, 1990
'revised 10/20/90   150 baud deleted from menu since not supported by Bird
'revised 2/16/91    op100,bin added to initialization
'revised 2/17/91    higher baud rates added for FOBs
'revised 5/6/93     com2 added


DIM BAUDN AS INTEGER
SHARED com$


maxbaudrate& = 115200  ' max baud rate on a PC compatible

'INITIALIZE THIS HOST COMPUTER'S COMMUNICATION PORT
CLOSE #1
PRINT
PRINT "Select baud rate for RS-232C communications to The Bird"
PRINT
PRINT "115,200   ( 0 )"
PRINT " 57,600   ( 1 )"
PRINT " 38,400   ( 2 )"
PRINT " 19,200   ( 3 )"
PRINT "  9,600   ( 4 )"
PRINT "  4,800   ( 5 )"
PRINT "  2,400   ( 6 )"
PRINT
PRINT "BAUD RATES 300 - 1,200 are not valid for FOBs"
PRINT
PRINT "  1,200   ( 7 )"
PRINT "    600   ( 8 )"
PRINT "    300   ( 9 )"
PRINT
INPUT "Input a baud number of 0-9  ", BAUDN
IF BAUDN = 0 THEN baud$ = "115200"
IF BAUDN = 1 THEN baud$ = "57600"
IF BAUDN = 2 THEN baud$ = "38400"
IF BAUDN = 3 THEN baud$ = "19200"
IF BAUDN = 4 THEN baud$ = "9600"
IF BAUDN = 5 THEN baud$ = "4800"
IF BAUDN = 6 THEN baud$ = "2400"
IF BAUDN = 7 THEN baud$ = "1200"
IF BAUDN = 8 THEN baud$ = "600"
IF BAUDN = 9 THEN baud$ = "300"



' Trap the baud rate initialization if the baud rate is greater than 19200
' ... since the BASIC OPEN will not allow higher rates
'
IF VAL(baud$) > 19200 THEN
	 initbaud$ = "19200"
ELSE
	 initbaud$ = baud$
END IF

PRINT
PRINT "specify the COM port number being used by the RS232"
PRINT "COM1    ( 1 )"
PRINT "COM2    ( 2 )"
INPUT cnum
IF cnum = 1 THEN comport$ = "COM1:"
IF cnum = 2 THEN comport$ = "COM2:"


'since the flock of birds uses the rs232 line RTS request to send
'to put the system into reset you must control the line when you open
'the port. If RS not specified basic then RTS is held electrically high
'a space (3-25v) causing the birds reset line to be held in reset.
'If RS is specified then the RST line is held low,a mark, (-3 to -25v)
'taking the bird out of reset to allow it to run


com$ = comport$ + initbaud$ + ",N,8,1,CD0,DS0,CS0,RS,OP100,BIN"

OPEN com$ FOR RANDOM AS #1


' Fill in the divisor register for the higher baudrates
'
IF VAL(baud$) > 19200 THEN
	 IF comport$ = "COM1:" THEN
	  serialportaddr% = &H3F8         ' com 1 base address
	 ELSE
	  serialportaddr% = &H2F8         ' com 2 base address
	 END IF

	 '
	 ' Determine the divisor register value
	 '
	 divisorreglow% = INT(maxbaudrate& / VAL(baud$))
	 divisorreghigh% = 0     ' 0 for all baud rates

	 '
	 ' Set the divisor register in the 8250 Type UART
	 '
	 OUT serialportaddr% + 3, &H83               ' DLAB bit set, 8 bit, no parity
	 OUT serialportaddr% + 0, divisorreglow%     ' set the divisor reg
	 OUT serialportaddr% + 1, divisorreghigh%    ' set the divisor reg
	 OUT serialportaddr% + 3, &H3                ' DLAB bit reset 8 bit, no parity
END IF


PRINT
'INPUT "AUTO-BAUD DETECT AT BIRD ? NO ( 0 )   YES ( 1 ) ", BAUDN
'IF BAUDN = 1 THEN PRINT #1, " ";    'SEND SPACE FOR AUTO-BAUD DETECTION






END SUB

SUB initFLOCKaddresses


'INITIALIZES FLOCK ADDRESSES.  REV 3.18 and greater has factory test commands
'that return the addresses.  3.17 has them also but command does not work

SHARED saddr AS INTEGER, XCaddr AS INTEGER, SCaddr AS INTEGER
SHARED xmtraxisaddr AS INTEGER, senaxisaddr AS INTEGER, ampcodeaddr AS INTEGER
SHARED xmtrgainaddr AS INTEGER, xdacaddr AS INTEGER, ydacaddr AS INTEGER
SHARED zdacaddr AS INTEGER, xt.timeaddr AS INTEGER
SHARED STATUSaddr AS INTEGER
SHARED NOTCH1addr AS INTEGER, NOTCH2addr AS INTEGER, NOTCH3addr AS INTEGER
SHARED x.settleaddr AS INTEGER, y.settleaddr AS INTEGER, z.settleaddr AS INTEGER
SHARED POWERSUPPLYdacaddr AS INTEGER, AtoDmuxaddr AS INTEGER
SHARED dipswitchaddr AS INTEGER, test.setaddr AS INTEGER



DIM address(32) AS INTEGER, i AS INTEGER, WORD AS INTEGER, AD AS SINGLE
DIM commandvalue AS INTEGER



'read the debug addresses from the flock
commandvalue = 128
FOR i = 1 TO 17     'read 17 addresses

	PRINT #1, CHR$(commandvalue);      'send the factory test command to get the Ith address

	'get the addr output

	CALL GET232inputWORD(WORD, AD)
	address(i) = WORD        'store the address
	commandvalue = commandvalue + 1
NEXT i


xdacaddr = address(1)
ydacaddr = address(2)
zdacaddr = address(3)
ampcodeaddr = address(4)
xmtrgainaddr = address(5)
senaxisaddr = address(6)
saddr = address(7)
xmtraxisaddr = address(8)
xt.timeaddr = address(9)
test.setaddr = address(10)
x.settleaddr = address(11)
y.settleaddr = address(12)
z.settleaddr = address(13)
dipswitchaddr = address(14)
revnoaddr = address(15)
POWERSUPPLYdacaddr = address(16)
AtoDmuxaddr = address(17)


END SUB

SUB INPUTvalue (a$, VALUE)

'USER CAN SEND A STRING FOR AN INPUT PROMPT MESSAGE
'IF A CARRIAGE RETURN IS INPUT FOR THE DATA REQUEST THEN THE VALUE IS
'LEFT UNCHANGE. OTHERWISE THE CHANGED VALUE IS RETURNED TO THE USER

'WRITTEN MAY 3, 1990

PRINT a$; " "; VALUE;
INPUT B$                           'GET THE INPUT FROM THE USER'S KEYBOARD HIT
IF LEN(B$) > 0 THEN VALUE = VAL(B$)


END SUB

SUB INSTRUCTIONS

'instructions on how to run the NOISE.BAS and SETSPEED.BAS programs
'written 6/28/93

PRINT "The purpose of NOISE.BAS is to determine the Bird measurement rate that will"
PRINT "minimize the noise in the position and angle outputs from the Bird."
PRINT ""
PRINT "BACKGROUND"
PRINT "The Bird uses transmitted and received magnetic signals to determine position"
PRINT "and orientation. Like a radio the received signals grow weaker as the receiver"
PRINT "moves farther away from the transmitter and like a radio the receiver also"
PRINT "picks up unwanted electrical signals that interfer with the desired signal"
PRINT "causing noise (jitter) in the position and orientation outputs.  Signals"
PRINT "that cause noise in the Bird's outputs can be generated by nearby computer"
PRINT "displays, high current power cables in the walls, nearby electrical motors,"
PRINT "or nearby power transformers.  To minimize the effects of these noise"
PRINT "sources on the Bird, separate the receiver from the source as far as "
PRINT "possible and position the transmitter as close to the receiver as"
PRINT "possible.  Each of these noise sources usually generates noise"
PRINT "at one or more frequencies.  To counter this noise the Bird filters"
CALL PAUSE
PRINT "the receiver signals, however the effectivity of the filtering is a function"
PRINT "of the frequency of the interfering signals and the measurement rate of the"
PRINT "Bird. Thus to minimize the jitter in the position and orientation outputs"
PRINT "for a given noise environment it is necessary to find the Bird measurement"
PRINT "rate that most effectively gets rid of this unwanted noise.  The NOISE.BAS"
PRINT "program allows you to vary the Bird measurement rate and obtain either a"
PRINT "graphic or numeric measure of the noise in the outputs as a function of"
PRINT "the measurement rate.  Once you have determined the best measurement rate"
PRINT "your application software can initialize the Bird to this rate on powerup"
PRINT "by using the CHANGE VALUE BIRD MEASUREMENT RATE command."

PRINT "SETUP FOR MEASURING THE NOISE"

PRINT "1. Have the Bird located in the environment where it will be used."
PRINT
PRINT "2. Have all other electronic systems in this environment running in their"
PRINT "   normal mode of operation."
CALL PAUSE
PRINT "3. Locate the Bird's receiver at the maximum operating distance from"
PRINT "   the transmitter ( 36 inches for a standard transmitter, 96 inches"
PRINT "   for an extended range transmitter)."
PRINT
PRINT "4. Rotate the receiver so that its nominal angle outputs are approximately"
PRINT "   azimuth = elevation = roll = 0"
PRINT
PRINT "5. The receiver must not move during the noise measurements."
PRINT
PRINT "6. The Bird unit must be running in the standalone mode, ie a single"
PRINT "   Bird with a transmitter and receiver.  No CRT SYNC will be used."
PRINT ""
PRINT "RUNNING NOISE.BAS"
PRINT ""
PRINT "1. Select the appropriate baud rate and com port number"
CALL PAUSE
PRINT "2. For graphics output select menu item (2) ANGLES.  For numeric output"
PRINT "   select item (11) SCAN FOR NOISE.  We'll start off with item 11 since its"
PRINT "   the most automatic and quantative."
PRINT ""
PRINT "3. If you want the data dumped to a printer as well as a screen then"
PRINT "   after you type 11 but before you do a carriage return, hit the"
PRINT "   CTRL and PRINT SCREEN keys simultaneously and then do a carriage return."
PRINT ""
PRINT "4. The SCAN program automatically changes the Bird measurement rate over"
PRINT "   a specifed range of values.  At each measurement rate it collects"
PRINT "   1000 data points and it displayes the peak to peak variation it sees in"
PRINT "   each angles, it also averages the variations of the three angles together"
PRINT "   to give an overall noise figure in degrees. This overall noise figure"
PRINT "   is listed in the last column of the print out on the screen."
CALL PAUSE
PRINT "5. The program asks for a title, you can carriage return or enter one"
PRINT "   line of descriptive information for any print out documentation."
PRINT "   don't put any commas in this descriptive line."
PRINT
PRINT "6. The program next asks for the measurement rate at the start of scan."
PRINT "   If you want the Bird to run at some speed near the default powerup"
PRINT "   speed of approximately 104 measurements/sec then you should do a scan"
PRINT "   that brakets this speed so enter a speed of say 100."
PRINT "   If you want to operate the Bird at a much lower speed because of metalic"
PRINT "   interference then start the scan at possibly 25. The lowest speed you"
PRINT "   can enter is 14."
PRINT ""
PRINT "7. Next you are prompted for speed at the end of the scan, this speed must be"
PRINT "   greater than the start speed but must be less than 144."
CALL PAUSE
PRINT "8. Next you are prompted for the scan step size, .5 is the recommended"
PRINT "   value. Values smaller than this don't have much significance and larger"
PRINT "   values will cause you to miss good locations.  You can try an initial"
PRINT "   scan over a large range with a step of 1 or 2 to get an idea where"
PRINT "   the peaks and valleys are, and then do a second scan with a small step"
PRINT "   of .5 over the area were you see a noise minimum."
PRINT ""
PRINT "9. The Bird now starts its scan by initially printing the location and"
PRINT "   Euler angles of the receiver and then the noise in the angles at each"
PRINT "   speed.  It takes a while for the measurements to be make at each"
PRINT "   speed so go take a break.  Remember there must be no movement of the"
PRINT "   transmitter or receiver while it is doing this scan and you should not"
PRINT "   be moving metal object near the transmitter or receiver during the scan."
CALL PAUSE
PRINT "10. When the scan is done search the last column under AVG for areas of"
PRINT "   minimum noise ( the smallest value ).  There will generally be several"
PRINT "   regions with small angle variations, pick the speed that corresponds"
PRINT "   to the center of the broadest region of small noise values. For example"
PRINT "   if one region of speeds had average values of.9,.6,.1,.7,.8 and another"
PRINT "   region had average values of .9,.6,.3,.2,.3,.8 then I would select"
PRINT "   the measurement rate that corresponds to the .2 value in the second"
PRINT "   region just because its has a wider band of low noise values around it."
PRINT "   If both a high Bird measurement rate and a low measurement rate show"
PRINT "   good noise characteristics, then pick the higher measurement rate"
PRINT "   to maintain minimum measurement lag and good tracking of fast movements."
PRINT
PRINT "11. You can set this speed into the Bird by selection menu item 14 CHANGE"
PRINT "    VALUE and then selecting item 6 SYSTEM MEASUREMENT RATE and then"
PRINT "    entering the desired measurement rate ie 116 measurements/sec."
CALL PAUSE
PRINT "12. You can automatically set this speed each time you run the Bird by "
PRINT "     first running the program SETSPEED.BAS.  To run this program first"
PRINT "    edit the file SPEED.BRD, inserting the desired speed and RS232"
PRINT "    interface characteristics."
PRINT
PRINT "13. You can look at the noise statistics at this speed by selecting"
PRINT "    menu item 4 POSITION/ANGLES, then item 1 CONTINUOUS POINT, then"
PRINT "    2 NOISE STATISTICS and entering say 1000 for the number of samples"
PRINT "    data is then collected and statistics printed.  To stop the printing"
PRINT "    hit any key."
PRINT ""
CALL PAUSE
PRINT
PRINT "*************************************************************************"
PRINT ""
PRINT "1.  To get a graphical view of the noise at a given measurement rate select"
PRINT "    main menu item 2 ANGLES then 1 CONTINEOUS POINT, then 1 GRAPHICS,"
PRINT "    then 1 ELEVATION VS TIME, then 0 NO CHANGE SCALE"
PRINT
PRINT "2.  The elevation angle with then be graphed as a function of time"
PRINT "    Any noise will show up as an amplitude variation"
PRINT
PRINT "3.  To change the measurement rate hit the I or D key to increase"
PRINT "    or decrease the measurement rate which is displayed at the bottom"
CALL PAUSE
PRINT "4.  Increase and decrease the rate looking for a minimum variation of"
PRINT "    the graphed values"
PRINT
PRINT "5.  hit Q to quit."


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

SUB KEYBOARDcommands

'handles keyboard commands for controlling point, continous point
'and stream modes

SHARED OUTMODE AS INTEGER, done AS INTEGER


'if OUTmode=0 its point mode. Wait till user hits 'B' key then return
'with DONE=0. When user hits any other key, return with DONE=1

IF OUTMODE = 0 THEN
readkey:
  a$ = INKEY$    'get keyboard hit
  IF a$ = "" GOTO readkey    'wait till keyboard hit
  done = 0
  IF a$ = "B" THEN EXIT SUB
  done = 1     'user hit something besides a "B", terminate point mode
END IF

'if OUTmode=1 its contineous point mode (simulated stream mode). User does
'not hit any key to initiate mode, but hits any key to terminate.
'if OUTmode=2 then its STREAM mode. Terminate data collection on any key hit

IF OUTMODE >= 1 THEN
done = 0
readkey2:
  a$ = INKEY$    'get keyboard hit
  IF a$ = "" THEN EXIT SUB    'no keyboard hit yet, continue getting data
  done = 1     'user hit something, terminate point continous mode
END IF


END SUB

SUB KEYBOARDhit (B$)


'wait till user hits keyboad and returns character in a$
waitforkeyboard:
a$ = INKEY$
IF a$ = "" THEN GOTO waitforkeyboard

END SUB

SUB NOISEstatISTICS (TYPEdata AS INTEGER, Naver AS INTEGER, scale AS SINGLE)

'collects sensor data if TYPEdata=0 or BIRD data if TYPEdata=1
'computes noise statistics for Naver samples. Scales the data per
'the scale factor


'WRITTEN MAY 3, 1990
'REVISED MAY 5, 1990 CHANGED SX(),SX2() TO DOUBLE PRECISION TO ELIMINATE
'                    INACCURACY IN STD DEVIATION FOR LARGE # OF SAMPLES

SHARED birddata() AS SINGLE, Numwords AS INTEGER
SHARED Bmax() AS SINGLE, Bmin() AS SINGLE, mean() AS SINGLE
SHARED DEVIATION() AS SINGLE, PKtoPK() AS SINGLE
SHARED SX() AS DOUBLE, SX2() AS DOUBLE
SHARED done AS INTEGER


DIM AB AS INTEGER, AD AS SINGLE, i AS INTEGER, WORDaddr AS INTEGER




FOR i = 1 TO Numwords     'Init statistics for each data item to collect
	Bmax(i) = -99999!
	Bmin(i) = 99999!
	SX(i) = 0!
	SX2(i) = 0!
NEXT i

FOR i = 1 TO Naver        'repeat for all samples

	CALL readBIRDdata  'get angle/position/matrix ect.

'FORM STATISTICS

	FOR J = 1 TO Numwords
	'FIND NEW MAX AND MIN VALUES IN DATA
	IF birddata(J) > Bmax(J) THEN Bmax(J) = birddata(J)
	IF birddata(J) < Bmin(J) THEN Bmin(J) = birddata(J)

	'SUM VALUES AND SQUARES FOR MEAN AND DEVIATION
	SX(J) = SX(J) + birddata(J)
	SX2(J) = SX2(J) + birddata(J) * birddata(J)
   NEXT J

NEXT i

'COMPUTE THE STATISTICAL MEASURES FOR EACH DATA ITEM


FOR i = 1 TO Numwords      'FOR EACH DATA ITEM
	mean(i) = SX(i) / Naver
	DEVIATION(i) = SQR(ABS(SX2(i) / Naver - mean(i) * mean(i)))
	PKtoPK(i) = ABS(Bmax(i) - Bmin(i))
NEXT i


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

SUB POSITIONMATRIXdata

'gets the position/matrix output from the bird
'and stores in shared variables

'caller must sent print #1,"Z"; ,Numwords, and OUTmode before calling this routine
'to select position/angle mode

SHARED birddata() AS SINGLE, POSK AS SINGLE
SHARED done AS INTEGER, OUTMODE AS INTEGER, ItoF AS SINGLE
SHARED Xpos AS SINGLE, Ypos AS SINGLE, Zpos AS SINGLE, M() AS SINGLE



DIM i  AS INTEGER, J AS INTEGER, K AS INTEGER


CALL readbird           'get x,y,z and orientation words

'convert position to inches
Xpos = birddata(1) * POSK
Ypos = birddata(2) * POSK
Zpos = birddata(3) * POSK

'arrange the BIRDDATA values in matrix format

i = 4
FOR J = 1 TO 3
  FOR K = 1 TO 3
	M(K, J) = birddata(i) * ItoF     'convert to fractional
	i = i + 1
  NEXT K
NEXT J


END SUB

SUB readbird

SHARED Numwords AS INTEGER, birddata() AS SINGLE, com$
SHARED done AS INTEGER, NOISEstat AS INTEGER, OUTMODE AS INTEGER


DIM CPOS AS INTEGER, II AS INTEGER, LSbyte AS INTEGER, MSbyte AS INTEGER
DIM WORD AS LONG, testb AS INTEGER



'PROCEDURE READBIRD
'
'READS BIRD DATA IN INTEGER FORMAT INTO ARRAY BIRDDATA()
'USING THE POINT MODE. USER MUST SETUP THE OUTPUT MODE, IE MATRIX, ANGLES
'ECT , BEFORE CALLING THIS ROUTINE AND USER MUST
'SET 'Numwords' TO THE NUMBER OF WORDS TO BE GOTTEN FOR A GIVEN
'MODE, IE Numwords=9 IF MATRIX OUTPUT SELECTED

'DIFFERS FROM readBIRDdata BY NOT LOOKING FOR KEYBOAD HIT AND ALWAYS IN
'POINT MODE


REQUESTdata2:
 '
PRINT #1, "B"; 'SEND REQUEST TO BIRD

'GET Numwords*2 BYTES FROM BIRD

TIMER ON                        'WILL TRAP IF RS232 HANGS

PORThung3:
IF LOC(1) = 0 GOTO PORThung3     'WAIT TILL SOMETHING IS IN BUFFER
TIMER OFF                       'SHUT OFF TIMER TRAP
B$ = INPUT$(Numwords * 2, #1)   'READ THE REQUESTED NUMBER OF BYTES FROM RS232

'EXTRACT Numwords WORDS FROM THE STRING CONVERTING TO BIPOLAR

FOR II = 1 TO Numwords
  CPOS = 2 * II - 1
  'GET LS BYTE
  LSbyte = ASC(MID$(B$, CPOS, 1))
  '
  'CHECK FOR DATA PHASING ERROR.
  'FIRST BYTE OF FIRST WORD SHOULD BE=1
  'FIRST BYTE OF ALL OTHER WORDS SHOULD BE=0
  'JUST CHECKING FIRST WORD DOESN'T SEEM TO BE ENOUGH SINCE IF BYTES ARE
  'OUT OF ORDER THE FIRST BIT MAY BE A DATA BIT =1 AND GIVE A FALSE INDICATION
  '
  IF II = 1 THEN    'CHECK LSBYTE OF WORD 1 FOR A 1
  testb = LSbyte AND &H80
  IF testb = 128 THEN GOTO FORMmsb3   'IF MSB=1 THEN OK EXIT
  PRINT "PHASING ERROR"
  CLOSE #1         'FLUSH THE BUFFER
  OPEN com$ FOR RANDOM AS #1
  a$ = INKEY$
  IF a$ <> "" THEN GOTO ENDITALL3   'GET OUT OF A LOCK UP IF KEY HIT

  GOTO REQUESTdata2
  END IF

'
 IF II = 2 THEN
	testb = LSbyte AND &H80
	IF testb = 0 THEN GOTO FORMmsb3   'IF MSB=0 THEN OK EXIT
	PRINT "PHASING ERROR"
	CLOSE #1         'FLUSH THE BUFFER
	OPEN com$ FOR RANDOM AS #1
	GOTO REQUESTdata2
 END IF
'
FORMmsb3:
  LSbyte = (LSbyte AND &H7F) * 2 'for formatted BIRD data

  'GET MS BYTE
  MSbyte = ASC(MID$(B$, CPOS + 1, 1))
  WORD = (MSbyte * 256! + LSbyte) * 2 'for formatted BIRD data

  'CONVERT TO 16 BIT BIPOLAR BETWEEN -32768 AND 32767
  IF WORD > 32767! THEN WORD = WORD - 65536!
   birddata(II) = WORD     'TEMPORARILY SAVE DATA
NEXT II

ENDITALL3:

END SUB

SUB readBIRDdata

SHARED Numwords AS INTEGER, birddata() AS SINGLE, com$
SHARED done AS INTEGER, NOISEstat AS INTEGER, OUTMODE AS INTEGER


DIM CPOS AS INTEGER, II AS INTEGER, LSbyte AS INTEGER, MSbyte AS INTEGER
DIM WORD AS LONG, testb AS INTEGER



'PROCEDURE readBIRDdata
'
'READS BIRD DATA IN INTEGER FORMAT INTO ARRAY BIRDDATA()
'USING THE POINT MODE. USER MUST SETUP THE OUTPUT MODE, IE MATRIX, ANGLES
'ECT , BEFORE CALLING THIS ROUTINE AND USER MUST
'SET 'Numwords' TO THE NUMBER OF WORDS TO BE GOTTEN FOR A GIVEN
'MODE, IE Numwords=9 IF MATRIX OUTPUT SELECTED

IF NOISEstat = 0 THEN         'check keyboard if not doing noise statistics
  CALL KEYBOARDcommands       'returns DONE=0 to get data or =1 to terminate
  IF done = 1 THEN EXIT SUB
END IF


REQUESTdata:
 '
IF OUTMODE <> 2 THEN

PRINT #1, "B"; 'SEND REQUEST TO BIRD IF NOT STREAM MODE

END IF


'GET Numwords*2 BYTES FROM BIRD

TIMER ON                        'WILL TRAP IF RS232 HANGS

PORThung:
IF LOC(1) = 0 GOTO PORThung     'WAIT TILL SOMETHING IS IN BUFFER
TIMER OFF                       'SHUT OFF TIMER TRAP

B$ = INPUT$(Numwords * 2, #1)   'READ THE REQUESTED NUMBER OF BYTES FROM RS232

'EXTRACT Numwords WORDS FROM THE STRING CONVERTING TO BIPOLAR

FOR II = 1 TO Numwords
  CPOS = 2 * II - 1
  'GET LS BYTE
  LSbyte = ASC(MID$(B$, CPOS, 1))
  '
  'CHECK FOR DATA PHASING ERROR.
  'FIRST BYTE OF FIRST WORD SHOULD BE=1
  'FIRST BYTE OF ALL OTHER WORDS SHOULD BE=0
  'JUST CHECKING FIRST WORD DOESN'T SEEM TO BE ENOUGH SINCE IF BYTES ARE
  'OUT OF ORDER THE FIRST BIT MAY BE A DATA BIT =1 AND GIVE A FALSE INDICATION
  '
  IF II = 1 THEN    'CHECK LSBYTE OF WORD 1 FOR A 1
  testb = LSbyte AND &H80
  IF testb = 128 THEN GOTO FORMmsb   'IF MSB=1 THEN OK EXIT
  PRINT "PHASING ERROR"
  CLOSE #1         'FLUSH THE BUFFER
  OPEN com$ FOR RANDOM AS #1
  a$ = INKEY$
  IF a$ <> "" THEN GOTO ENDITALL   'GET OUT OF A LOCK UP IF KEY HIT

  GOTO REQUESTdata
  END IF

'
 IF II = 2 THEN
	testb = LSbyte AND &H80
	IF testb = 0 THEN GOTO FORMmsb   'IF MSB=0 THEN OK EXIT
	PRINT "PHASING ERROR"
	CLOSE #1         'FLUSH THE BUFFER
	OPEN com$ FOR RANDOM AS #1
	GOTO REQUESTdata
 END IF
'
FORMmsb:
  LSbyte = (LSbyte AND &H7F) * 2 'for formatted BIRD data

  'GET MS BYTE
  MSbyte = ASC(MID$(B$, CPOS + 1, 1))
  WORD = (MSbyte * 256! + LSbyte) * 2 'for formatted BIRD data

  'CONVERT TO 16 BIT BIPOLAR BETWEEN -32768 AND 32767
  IF WORD > 32767! THEN WORD = WORD - 65536!
   birddata(II) = WORD     'TEMPORARILY SAVE DATA
NEXT II

ENDITALL:
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

SUB SPEEDstep


'STEPS THE measurement rate AND MEASURES THE SYSTEM NOISE
'USED TO DETERMINE BEST XMTR SETTLING TIME TO MINIMIZE AC INTERFERENCE

'WRITTEN MAY 6, 1990
'Revised Sept 26, 1990 number of samples to average increased from 100 to 1000
'                      so slow beat frequencies will be picked up
'revised 1/8/92 now uses examine/change value instead of debug commands to
'               get and change xt_time_cnt
'revised 6/6/93 steps measurement rate instead of xmtr settling count


SHARED clock AS SINGLE
SHARED Numwords AS INTEGER, Naver AS INTEGER, OUTMODE AS INTEGER
SHARED NOISEstat AS INTEGER, ANGK AS SINGLE, POSK AS SINGLE
SHARED PKtoPK() AS SINGLE, mean() AS SINGLE




DIM TIME.START AS SINGLE, TIME.END AS SINGLE, TIME.RES AS SINGLE
DIM TIMEdelay AS SINGLE, i AS INTEGER, PP AS SINGLE, ANGav AS SINGLE
DIM WORD AS INTEGER, AD AS SINGLE
DIM FIRSTPOINT AS INTEGER, Parameter AS INTEGER, LSbyte AS INTEGER, MSbyte AS INTEGER



PRINT #1, "Y";      'SEND COMMAND TO SELECT POSITION/ANGLES
Numwords = 6        '6 WORDS TO READ WITH THIS COMMAND
Naver = 1000         'AVERAGE 1000 SAMPLES
NOISEstat = 1      'NOISE STATISTICS ON
TIMEdelay = .1      '200 MS DELAY TIME
FIRSTPOINT = 0      'SET TO NON ZERO AFTER FIRST POINT MEASURED
OUTMODE = 1          'point continous

INPUT "TITLE  ", a$
PRINT
PRINT DATE$, TIME$
PRINT

'GET CURRENT XMTR SETTLING VALUE


Parameter = 6          'parameter no to examine xt_time_cnt

CALL GETBIRDparameter(Parameter)       'get/print speed

AGAINspeed:
PRINT
INPUT "Measurement rate at START of scan in measurements/sec (i.e. 100) ", SPEEDstart
INPUT "Measurement rate at END of scan (i.e. 144) ", SPEEDend
INPUT "Scan step size in measurements/sec (i.e. 0.5) ", SPEEDres
PRINT

IF SPEEDend <= SPEEDstart THEN
	PRINT "***** START rate must be less than END rate"
	PRINT
	GOTO AGAINspeed
END IF

IF SPEEDend > 144 THEN
	PRINT "****END rate must be less than or equal to 144"
	PRINT
	GOTO AGAINspeed
END IF


SPEEDcurrent = SPEEDstart
ANOTHERspeed:
a$ = INKEY$                'IF USER HITS KEY GET DONE
IF a$ <> "" THEN EXIT SUB

	XTIME = (1000! / SPEEDcurrent - .3) * .25

	'convert to xmtr timer counts
	AD = 1000! * XTIME / clock
IF AD >= 65535 THEN
	PRINT
	PRINT " rate is too low, increase it"
	PRINT
	GOTO AGAINspeed
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


CALL delay(TIMEdelay)       'DELAY BEFORE READING NOISE READING

GOSUB FINDdev2
SPEEDcurrent = SPEEDcurrent + SPEEDres

IF SPEEDcurrent <= SPEEDend GOTO ANOTHERspeed
CALL PAUSE
EXIT SUB

'*************************************************
FINDdev2:

'SUBROUTINE TO GET XYZ,AZELRL RECORDS AND FIND MAX DEVIATION


CALL NOISEstatISTICS(1, Naver, 0)
IF FIRSTPOINT = 0 THEN          'DO ON FIRST POINT
PRINT "RECEIVER LOCATION"
PRINT "   X       Y       Z      AZ      EL      RL"
PRINT USING "###.#   "; mean(1) * POSK; mean(2) * POSK; mean(3) * POSK; mean(4) * ANGK; mean(5) * ANGK; mean(6) * ANGK
PRINT
FIRSTPOINT = 1
'switch to collecting just angles for the statistics
PRINT #1, "W";      'SEND COMMAND TO SELECT ANGLES
Numwords = 3        '3 WORDS TO READ WITH THIS COMMAND

PRINT
PRINT "SPEED   XTIME  XMTR_TIME_CNT    AZ      EL       RL     AVG"
PRINT
'need to repeat first speed since we're switching data formats
'and statistics will get screwed up using position instead of angles
SPEEDcurrent = SPEEDcurrent - SPEEDres

RETURN

END IF


PRINT USING "###.##  "; SPEEDcurrent;
PRINT USING "##.###  "; XTIME;
PRINT USING "#####  "; AD;
PRINT "     ";
ANGav = 0          'AVERAGE PEAK TO PEAK FOR THE ANGLES
FOR i = 1 TO 3
PP = ANGK * PKtoPK(i)
PRINT USING "####.## "; PP;
ANGav = ANGav + PP / 3!
NEXT i
PRINT USING "####.##"; ANGav     'AVERAGE AZ,EL,RL ERROR
RETURN



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

SUB XMTRsettling


'SET TRANSMITTER SETTLING TIME

'WRITTEN MAY 6, 1990
'revised 3/5/91 so you can enter a count larger than 32767
'               can now go to a settling time equivalent to
'               the max the counter can go to 65536
'revised 1/8/92 now use examine/change value instead of debug dump/enter

SHARED xt.timeaddr AS INTEGER, clock AS SINGLE
DIM WORD AS INTEGER, AD AS SINGLE, STIME AS SINGLE
DIM REALcounts AS SINGLE, INTEGERcounts AS INTEGER
DIM Parameter AS INTEGER, LSbyte AS INTEGER, MSbyte AS INTEGER




'GET CURRENT VALUE FROM SYSTEM

Parameter = 6          'parameter no to examine xt_time_cnt

CALL GETBIRDparameter(Parameter)       'get/print time


NEWtime:
PRINT
INPUT "INPUT NEW SETTLING TIME IN MILLISECONDS ", STIME

'compute max settling time the timer can go to with 65536 counts
maxtime = 65536! * clock / 1000!


IF STIME > maxtime THEN
   PRINT "TIMER CAN ONLY COUNT TO "; maxtime; "MS"
   GOTO NEWtime
END IF

	'convert to xmtr timer counts
	AD = 1000! * STIME / clock

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


END SUB

SUB XMTRsettlingSTEP

'STEPS THE XMTR SETTLING TIME AND MEASURES THE SYSTEM NOISE
'USED TO DETERMINE BEST XMTR SETTLING TIME TO MINIMIZE AC INTERFERENCE

'WRITTEN MAY 6, 1990
'Revised Sept 26, 1990 number of samples to average increased from 100 to 1000
'                      so slow beat frequencies will be picked up
'revised 1/8/92 now uses examine/change value instead of debug commands to
'               get and change xt_time_cnt


SHARED clock AS SINGLE
SHARED Numwords AS INTEGER, Naver AS INTEGER, OUTMODE AS INTEGER
SHARED NOISEstat AS INTEGER, ANGK AS SINGLE, POSK AS SINGLE
SHARED PKtoPK() AS SINGLE, mean() AS SINGLE




DIM TIME.START AS SINGLE, TIME.END AS SINGLE, TIME.RES AS SINGLE
DIM TIMEdelay AS SINGLE, i AS INTEGER, PP AS SINGLE, ANGav AS SINGLE
DIM WORD AS INTEGER, AD AS SINGLE
DIM FIRSTPOINT AS INTEGER, Parameter AS INTEGER, LSbyte AS INTEGER, MSbyte AS INTEGER



PRINT #1, "Y";      'SEND COMMAND TO SELECT POSITION/ANGLES
Numwords = 6        '6 WORDS TO READ WITH THIS COMMAND
Naver = 1000         'AVERAGE 1000 SAMPLES
OUTMODE = 1        'CONTINOUS POINT MODE
NOISEstat = 1      'NOISE STATISTICS ON
TIMEdelay = .1      '200 MS DELAY TIME
FIRSTPOINT = 0      'SET TO NON ZERO AFTER FIRST POINT MEASURED

INPUT "TITLE  ", a$
PRINT
PRINT DATE$, TIME$
PRINT

'GET CURRENT XMTR SETTLING VALUE


Parameter = 6          'parameter no to examine xt_time_cnt

CALL GETBIRDparameter(Parameter)       'get/print speed

PRINT
INPUT "FOR SETTLING TIME SCAN INPUT START TIME IN MS ", TIME.START
INPUT "INPUT TIME AT END OF SCAN ", TIME.END
INPUT "INPUT SCAN TIME RESOLUTION IN MS ", TIME.RES
PRINT
STIME = TIME.START
ANOTHERtime:
a$ = INKEY$                'IF USER HITS KEY GET DONE
IF a$ <> "" THEN EXIT SUB

	CURRENTspeed = 1000! / (4! * STIME + .3)    'IN CYCLES/SEC

	'convert to xmtr timer counts
	AD = 1000! * STIME / clock

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


CALL delay(TIMEdelay)       'DELAY BEFORE READING NOISE READING

GOSUB FINDdev
STIME = STIME + TIME.RES

IF STIME <= TIME.END GOTO ANOTHERtime
CALL PAUSE
EXIT SUB

'*************************************************
FINDdev:

'SUBROUTINE TO GET XYZ,AZELRL RECORDS AND FIND MAX DEVIATION


CALL NOISEstatISTICS(1, Naver, 0)
IF FIRSTPOINT = 0 THEN          'DO ON FIRST POINT
PRINT "LOCATION   ";
PRINT USING "###.#    "; mean(1) * POSK; mean(2) * POSK; mean(3) * POSK; mean(4) * ANGK; mean(5) * ANGK; mean(6) * ANGK
PRINT
FIRSTPOINT = 1
PRINT
PRINT " TIME     SPEED      X        Y        Z       AZ        EL        RL     AVG"
PRINT

END IF


PRINT USING "##.###   "; STIME;
PRINT USING "###.##  "; CURRENTspeed;

'PRINT PEAK TO PEAK IN INCHES AND DEGREES
FOR i = 1 TO 3
PRINT USING "####.##  "; POSK * PKtoPK(i);
NEXT i

ANGav = 0          'AVERAGE PEAK TO PEAK FOR THE ANGLES
FOR i = 4 TO 6
PP = ANGK * PKtoPK(i)
PRINT USING "####.##  "; PP;
ANGav = ANGav + PP / 3!
NEXT i
PRINT USING "####.##"; ANGav     'AVERAGE AZ,EL,RL ERROR
RETURN



END SUB

