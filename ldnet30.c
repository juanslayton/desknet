/*                             dnet.c
                            desknet-0.1
                   Copyright (C) 2016 by John Slayton

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

Author's address as of 11.3.14 is 260 Sundance Ct, Azusa, CA 91792. */

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/ioctl.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/io.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>

 /*                      INTEGER VARIABLES                            */

 /* global integers used for index counting.  */
int a = 0;
int az = 0;
int b = 0;
int c = 0;

int Boardnumber = 0;

 /* This flag says to load a different class when set to 1.  */
int Changeclass = 0;

 /* This is the default number of a class whose records will be downloaded. */
int Class = 1;

 /* This is also the number of a class to be loaded. */
int Classnumber = 0;

 /* Class_size holds the current class size in binary.  */
int Class_size = 0;

int Code = 0;

int Digit = 0;
 
 /* e is used for indexing each of 40 students and associated data */
int e = 0;

int Goaround = 1;

 /* used to convert binary to bcd */
int Hundreds_register = 0;    

 /* k is used to index strings returning from fetch_letter */
int k = 0;

 /* linelength stores number of characters in a printed line (hard copy). */
int Length_flag = 0;

int Letter = 0;

int Line_number = 0;

 /* Ltr is an ASCII character returned from get_event(). */
int Ltr = 0;

 /* m is the number of a message. */
int m = 0;

 /* How many pages in the math text book? */
int Max_page = 0;

 /* used to convert binary to bcd */
int Ones_register = 0;

int Operation = 0;

 /* Used to select answer file */
int Option = 0;

 /* 1 means go back to menu through any intervening routines. */
int Panic = 0;

 /* Presentation is a flag used to set entire class to a single practice line. */
int Presentation = 0;

 /* repeat is a flag that shows whether menu item M is being repeatedly 
    invoked with no other items between invocations.  The first 
    invocation  of M sets this to 1; all other menu items reset it to 0.
    Only the first invocation of M requires a password */ 
int Repeat = 0;

 /* used to convert binary to bcd */
int Tens_register = 0;

 /* Used to set session lengths in minutes. */
int Time=0;

int Value = 0;

 /* Win_score contains the score needed to win math competitions.  It 
    can  be changed by the teacher when the program is running. */
int Win_score = 90;

                            /* INTEGER ARRAYS */
 /* These are used to calculate math addition problems. */
int Addend1[41];
int Addend2[41];

 /* This array records how many points a student receives for a right 
    answer in an addition contest. */
int Addpoints[41]; 

 /* Advance is a flag that shows whether a new problem is to be 
    generated.  1 means generate it. */
int Advance[41];

 /* This array contains answers for each of the current student 
    problems in math competitions. */
int Answer[41];

 /* This flag keeps a keyboard from being read while a message is displayed. */
int Block_input[41];

 /* These are file descriptors. */
int Board_file_descriptor[41] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

 /* These are used to calculate math division problems. */
int Dividend[41];
int Divisor[41];

 /* These are used to calculate math multiplication problems. */
int Factor1[41];
int Factor2[41];

int Letra[41] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

 /* This is used to calculate math subtraction problems. */
int Minuend[41];

 /* This array holds offsets for each student from the beginning of the 
    current Wscreen line to the current cursor position. */
int Cursor[41];

 /* This array records how many points a student receives for a right 
    answer in a division contest. */
int Divpoints[41]; 

 /* 1 means student has entered a wrong answer in math competition. */
int Goofed[41];

 /* Here's a hash table to decode lower case keypresses. */
const int LC[150] = {0,27,49,50,51,52,53,54,55,56,57,48,45,61,0,9,113,119,101,114,116,121,117,105,111,112,91,93,10,0,97,115,100,102,103,104,106,107,108,59,39,96,0,92,122,120,99,118,98,110,109,44,46,47,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,130,131,0,0,0,129,127};

 /* These flag last keystroke on each keyboard as make (1) or break (0). */
int Make_or_break[41];

 /* This array records how many points a student receives for a right 
    answer in a multiplication contest. */
int Mulpoints[41]; 

 /* Here are the pages each student is working on in Check Your Math. */
int Page[41];

 /* This is used to input multi-digit numbers from student keyboards. */
int Placeflag[41];

int Problem[41];

 /* This holds accumulated points in math competitions.  score also 
    holds the number of student errors in the typing tutorial. */
int Score[41];

int Step[41];

int Stop[41];

 /* This array holds student answers for math competitions. */
int Stuans[41];

 /* This array records how many points a student receives for a right 
    answer in a subtraction contest. */
int Subpoints[41]; 

int Subtrahend[41];

 /* These arrays each hold tables of digits from which the math
    competition routines draw randomly to make the problems.  Mt1 is
    the first term of a multiplication problem.  Mt2 is the second.
    Other types of problems use these tables similarly. */
int At1[10];
int At2[10];

int Dt1[10];
int Dt2[10];

int Mt1[10];
int Mt2[10];

int St1[10];
int St2[10];

 /* These are offsets from the beginning of each student's 
    composition to the beginning of his currently displayed line. */
int Text_pos[41];

 /* These arrays hold the number of the current typing practice lines for 
    each student. */
int Tline[41]; 
int Tlineb[41]; 

 /* This array holds the message number for each line, used in checkwork */
int Tta[41];
int Ttb[41];

 /* Here's a hash table to decode upper case keypresses. */
const int UC[150] = {0,27,33,64,35,36,37,94,38,42,40,41,95,43,0,9,81,87,69,82,84,89,85,73,79,80,123,125,0,0,65,83,68,70,71,72,74,75,76,58,34,126,0,124,90,88,67,86,66,78,77,60,62,63,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,130,131,0,0,0,129,127};

 /* These flags show whether last keypress was make-shift.  A 1 means 
    interpret next make as upper case. */
int Upper_case[41];

                           /* CHARACTER ARRAYS */

char Answ[41][21];

 /* These variables hold the event number for each keyboard. */
char BOARDFILE[41][21] = {"/dev/input/event0","/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0" };

 /* Here's a buffer that holds file names for i/o operations. */
char Buf[50];

 /* These are the buffers for student compositions, limited for now to 
     5000 characters. */
char Comp[41][5001];

 /* Here's an empty buffer used to clear other buffers by strcpy. */
char Dum[41] = "";
char Dumm[10] = "";

 /* This is a buffer used to assemble strings as typed in on teacher's 
    keyboard. */
char Dummy[41] = "";

char Factor[12];

 /* Here's a buffer containing ASCII representation of points per right 
    answer given in math contests.  It's used for i/o \operations. */ 
char Hndicap[41];

 /* Answers to text exercises */
char Key[40000];

 /* A register used store the last line sent to screen, then erase it
    by reprinting it in background color. */
char Last_line[41][21];

char Display_line[41][21];

 /* Contains students' proposed answers for Check Your Math. */
char Guess[41][21];

 /*  Used for i/o operations.  Holds # of keyboards in system. */
char Net_buffer[2];

 /* This 4 digit password for the teacher keeps kids from messing 
    with important files. */
const char PASSWORD[5] = "1040";

 /* This is a buffer containing a student composition being formatted 
     for hard copy printing. */
char Proof[5001] = "";

 /* This is an array containing names of all current students.
    (10 character limit) */
char Roster[41][11];
char Rostera[41][13];

 /* This holds name of class or teacher displayed on main menu. */
char Teacher[41];

char Studint[15];

 /*  These arrays hold teacher-created hash tables used to create
     problems for math competitions.  Terms starting A,D,M,S are for
     addition, division, multiplication, or subtraction, respectively.
     Terms ending in 'a' are the first factors in multiplcation problems;
     those ending in 'b' are the second factors.  */
char A_terma[10] = "";
char A_terma_buffer[10] = "";

char A_termb[10] = "";
char A_termb_buffer[10] = "";

char D_terma[10] = "";
char D_terma_buffer[10] = "";

char D_termb[10] = "";
char D_termb_buffer[10] = "";

char M_terma[10] = "";
char M_terma_buffer[10] = "";

char M_termb[10] = "";
char M_termb_buffer[10] = "";

char S_terma[10] = "";
char S_terma_buffer[10] = "";

char S_termb[10] = "";
char S_termb_buffer[10] = "";

 /* These typing practice lines may be printed;  */
const char TTEXT[255][21]=
   {  
     "                    ", // 0
     "ff jj ff jj fj fj fj", // 1
     "jjf jjj ffj fff fjfj", // 2
     "jfj ffj fjf jjf jffj", // 3
     "dd kk dd kk dk dk kd", // 4
     "kdk ddk kdd dkk dkdk", // 5
     "jk fd jk fd kj df kj", // 6
     "dj jd dj dj kf df jf", // 7
     "jdd kkf dkd ffd jfkd", // 8
     "aa dd aa kk dada aka", // 9
     "dda jad kaj faj kdfa", // 10
     "sa sa sd sd js sj ks", // 11
     "das kas sad kad adss", // 12
     "ll aa kk ss jj dd ff", // 13
     "al sl dl fl la ls ld", // 14
     "lf lj lk all lad ask", // 15
     "fad lass fall salads", // 16
     "gg hh gg hh gg hh gh", // 17
     "gk gl ha hs dg dh hf", // 18
     "hj hk hd hs hf ha hl", // 19
     "hg gf gd gs ga kg lg", // 20
     "gas hag gal sag lags", // 21
     "jag ash dash lash gh", // 22
     "sash shag shall hall", // 23
     "gash hash flask flag", // 24
     "ed de ed de de ek ke", // 25
     "fed led head less he", // 26
     "ki ik ki ki ji ij ij", // 27
     "if is as hi he seeds", // 28 
     "did hid lid led skid", // 29
     "kiss hiss dish fades", // 30
     "less shell shed sled", // 31
     "dill dell sell fills", // 32 
     "sill jell kill skill", // 33
     "ill gill slag slides", // 34
     "hide lied like glide", // 35
     "ail sails fail jails", // 36
     "sale dale gale hails", // 37
     "dial file lead feeds", // 38
     "life fife leak seeks", // 39
     "rf rf ju ju frr juuj", // 40
     "frfe juji rffd uj jk", // 41 
     "free jug dusk disk a", // 42
     "dress jig dig rug jr", // 43
     "fur far fire sire sr", // 44
     "rid red risk dusk he", // 45
     "her she husk shirk a", // 46
     "fluff huff risk slur", // 47
     "guff gruff use fuses", // 48
     "sue lush rush rulers", // 49
     "ol lo lo oll lol klo", // 50
     "loose goose  rooster", // 51
     "so do go lo ho of or", // 52
     "hog log jog dog frog", // 53 
     "for ajar hod door jo", // 54
     "moor lore floor roar", // 55
     "hood hoof goods food", // 56
     "oil soil foil road i", // 57
     "old fold hold rolled", // 58
     "gold sold jokes soak", // 59
     "sole foal goal holes", // 60
     "horses foe goes does", // 61
     "tt rr ff gg tr rt fr", // 62
     "tf fg gr rt rg rf tg", // 63
     "it at to tut tot ate", // 64
     "tar tag fat rat toes", // 65
     "jot dot got hot lots", // 66
     "just dust gusto this", // 67
     "that there their the", // 68
     "eats seat feet great", // 69
     "heat greet feat hoot", // 70
     "tear star stall tall", // 71
     "tell till toll troll", // 72
     "toad roads load bold", // 73
     "jails pails railroad", // 74
     "trials trails tigers", // 75
     "litter latter ladder", // 76
     "loiter toil suit tad", // 77
     "yt fj ty jf jf yt fy", // 78
     "tg hy hu uy yu hu ty", // 79
     "yo you hurry hey you", // 80
     "soy joy key say days", // 81
     "lay hay ray ye delay", // 82
     "relay shay clay flay", // 83
     "shaggy froggy  risky", // 84
     "foggy gray frail sky", // 85
     "fry  dry sly lye try", // 86
     "ws sw ow wo ed ws ew", // 87
     "we ow wow wed dew aw", // 88
     "few hew was saw wits", // 89
     "jaw law yaw raw ways", // 90
     "wag wad walks washes", // 91
     "water week weak weed", // 92
     "wishes woke whiskers", // 93
     "away awoke with waft", // 94
     "pq qp p; ;p aq qa qp", // 95
     "up ape pie put quips", // 96
     "pot pet pat pit date", // 97
     "peak speak freak pry", // 98
     "quiet hire shire spy", // 99
     "spry sideswipes swig", // 100
     "quest post lost wars", // 101
     "nj jhh hn nh jhn nhj", // 102
     "no an in on tone fan", // 103
     "sing rings ding thin", // 104
     "hen then fingers not", // 105
     "notes  nothing funny", // 106
     "vff fv gv vg rv vrtf", // 107
     "dvd videos vanish va", // 108
     "dove stoves love eve", // 109
     "vine every never van", // 110
     "mj jm mn nm mh hm jn", // 111
     "km mh nm jm mn gm mg", // 112
     "kvf fnm mvn jmh jnhg", // 113
     "man name mane him he", // 114
     "map swap mad may ram", // 115
     "mashes smash stashes", // 116
     "bv vb fb bf gb bg gv", // 117
     "vm nb bn mv jb fb gb", // 118
     "hb bj java jabs bees", // 119
     "bad bid bud bed both", // 120
     "better worse best ab", // 121
     "bear web bright brag", // 122
     "cd dc sc cs cf cg gc", // 123
     "nc cn cb bc hc ch cj", // 124
     "can con cent sack ca", // 125
     "car cart carton core", // 126
     "scram brick sick sic", // 127
     "za az sz zs lz ze za", // 128
     "zq wz az dz zo zp cz", // 129
     "miz zip zap doze zoo", // 130
     "dozen zipper  zinger", // 131
     "xs zx xz dx xd mx xn", // 132
     "ex axe ox exude flex", // 133
     "fox box pox lox flax", // 134
     "Mm mM nM Mn Mr Mrs M", // 135
     "Hh Ii Jj Kk Ll Mm Nn", // 136
     "Oo Pp Uu Yy jJj MmMn", // 137
     "Aa Bb Cc Dd Ee Ff Gg", // 138
     "Qq Ww Rr Tt Zz Xx Cc", // 139
     "Vv Bb Dr Ca NY PhD G", // 140
     ".l. x. .x z. .z j. .", // 141
     "Mr. Mrs. Dr. Go now.", // 142
     "Sr. Srta. Go slowly.", // 143
     "My dog has no fleas.", // 144
     "abcdefghijklmnopqrst", // 145
     "uvwxyz Play all day.", // 146
     "He sings for dinner.", // 147
     "The dog likes bones.", // 148
     "Her car is very old.", // 149
     "I travel to Ontario.", // 150
     ",k k, ,m m, ,. ., l,", // 151
     "alpha,a beta, gamma,", // 152
     "dog, cat, rat, duck.", // 153
     "Sadly, he will lose.", // 154
     "// ;; // ;; /; ;/ ./", // 155
     "?/ /? .? ,.? ?., ?/?", // 156
     "Am I having fun yet?", // 157
     "How does a bird fly?", // 158
     "When will this end??", // 159
     "We are now finished.", // 160
     "ff gg fg fg gf gf gg", // 161
     "fgf gfg ggf ffg fffg", // 162
     "hh ff gf fg hg hf fh", // 163
     "hgf fgh jj jh hg gjf", // 164
     "dd ff dd df fd gf gd", // 165
     "dh hd gfd dfg jdg gd", // 166
     "kk jj kk hh kk kf fk", // 167
     "gk fk dk kj dgk fhfd", // 168
     "ee ff ee ff ef fe je", // 169
     "egg fed jed keg hfdg", // 170
     "uu jj uu jj uj ju uj", // 171
     "hu uh hug huff dug k", // 172
     "rr ee rr ff rf fd rd", // 173
     "rug gr red huge here", // 174
     "tt gg tt gg tg gt gg", // 175
     "tf ft fr rt tr hg tt", // 176
     "tree the get tug hut", // 177
     "there thug dud rut k", // 178
     "ii jj ii jj kk ii kj", // 179
     "hit kit kitty retire", // 180
     "cc ff dd cd fc rc ed", // 181
     "cut tick kicker true", // 182
     "bb gg bb gb bg butte", // 183
     "ss dd dc sh st ts is", // 184
     "diss this trick bugs", // 185
     "oo oi ok go dog drug", // 186
     "frisk tusk yours our", // 187
     "bold colds fold told", // 188
     "nn hh jn nj north ng", // 189
     "running singing flue", // 190
     "mm nn nm mn man sand", // 191
     "mind blind tried fly", // 192
     "aa bb ab dd add made", // 193
     "dark bark are harder", // 194
     "vv ff fv vf have vga", // 195
     "my dog has got fleas", // 196
     "ww dd wd dw wag wool", // 197
     "awesome swim crashes", // 198
     "zz ss zs sz az sizes", // 199
     "qq quick qwack queen", // 200
     "pp oo pig speak poor", // 201
     "once upon a timeline", // 202
     "please feed the bear", // 203
     "AA aa AA Aa aAa bBbB", // 204
     "Nancy Jane Mark Jose", // 205
     "Now is the time for-", // 206
     "all good men to come", // 207
     "to the aide of their", // 208
     "country. 1 2 3 4 5 6", // 209
     "7 8 9 10 counting on", // 210
     "Leave 2 spaces after", // 211
     "period at the end of", // 212
     "a sentence.  Can you", // 213
     "type the alphabet?  ", // 214
     "a b c d e f g h i j ", // 215
     "k l m n o p q r s t ", // 216
     "u v w x y z  Leave 2", // 217
     "letters after colon.", // 218
     "The sun is rising in", // 219
     "the east.  The moon ", // 220
     "is still visible in ", // 221
     "the west.  It's near", // 222
     "the time of the full", // 223
     "moon.  Sometimes you", // 224
     "can see the eclipse ", // 225
     "of the moon when it ", // 226
     "is full.  But you'll", // 227
     "see a solar eclipse ", // 228
     "only when there is a", // 229
     "new moon.  The solar", // 230
     "eclipse can be scary", // 231
     "I left off a period ", // 232
     "on the last sentence", // 233
     "And I just did it a ", // 234
     "second time.  That's", // 235
     "because the line has", // 236
     "just 20 characters. ", // 237
     "How does a bird fly?", // 238
     "When will this end??", // 239
     "We are now finished.", // 240
     "                    ", // 241
     "                    ", // 242
     "                    ", // 243
     " What's the page?   ", // 244
     " What's the problem?", // 245
     " What's the answer? ", // 246
     " What line?         ", // 247
     " TRY IT AGAIN!      ", // 248
     " GO, ",                // 249
     " SPEED IT UP...     ", // 250
     " LOOKS GOOD ...     ", // 251
     " BUMMER...          ", // 252
      };

 /* This array holds ASCII representation of the number of each 
    student's practice typing line.  Used in i/o operations.
    (Corresponding binary representations are held in Tline[]. */
char Type_linea[41] = "11111111111111111111111111111111111111111";
char Type_lineb[41] = "11111111111111111111111111111111111111111";

 /* This holds the value of Win_score in ASCII form.  It is used in 
     i/o routines. */
char Ww[2]; 

char Z[5] = {0,0,0,0,0};

                       /* ODDBALL VARIABLES */

struct timeval timeout;
struct input_event ev[0];

 /* A file descripter used in i/o operations. */
FILE * (FP);

fd_set set;
 
 /* Used in keyboarding to measure typing speed, and in math facts 
    competitions to clock timeout when wrong answer is entered. */
time_t *t= NULL;

 /* Used to time keyboarding and Check Your Math. */
unsigned long Clk[41];
long int Klok;

                          /* FUNCTIONS */

void add();

 /* Get keypress value */
int alpha();

 /* This saves current practice lines to the hard drive when returning 
    from typing tutorial routine. */
void bail();

 /* This is the main routine for typing tutorial.  Polls student 
    keyboards and saves record. */
void boarda();

 /* This is the temporary routine for typing tutorial.  Polls student 
    keyboards and does not save a record. */
void boardb();

 /* This reports the number of pages in a math book. */
void booklength();

 /* This does keyboard polling for Check Your Math. */
void check();

 /* Select a practice line for the whole class to work on. */
void choose_line();

 /* Erase all student compositions. */
void clear_pro();

 /* Present "Push Enter to Continue" page. */
void detente();

void divide();

 /* Verify desire to exit. */
void double_check();

 /* Empty event que. */
void dump();

 /* Check student typing for speed and accuracy. */
void eval_type();
void eval_typeb();

void fetch_letter();
int fetch_name();
int fetch_win_score();

 /* Save student compositions to the hard drive. */
void file_save();

 /* Pull relevant answer from text answer file */
void find_answer();

int get_char();

 /* Recover last USB network configuration. */
void get_config();

 /* Input a binary digit into variable nr.  Return ASCII value of nr, 
    or 10. */
int get_digit();

 /* Argument is e (student #).  Returns Ltr. */
int get_event(int);

 /* Download student compositions on program boot. */
void get_file();

 /* Download the class roster. */
void get_names();

void get_nine();

 /* Set the number of points a particular student will get for a right 
    answer in one of the math contests. */
int handicap();

 /* Specify which board will receive the handicap points. */
int handicapenterpage();

 /* Specify which operation the handicap will apply to. */
int pick_operation();

 /* The following routines create and display pseudo-random problems 
    for math facts contests. */
void make_add_prob();
void make_div_prob();
void make_mul_prob();
void make_sub_prob();

 /* This presents the basic menu. */
int menu();

 /* This prints a numbered message to the screen. */
void message(int e, int m);

void multiply();

 /* This routine creates a class identifier at top of main menu.  It may 
    be the teacher's name. */
int name_class();

 /* Clear this year's files; set up for next year. */
void new_year();

 /* This opens a file for each keyboard, assigns control using ioc. */
int opener();

 /* Print (hard copy) current composition for a selected student. */
int print_pro();

 /* Initiate word processor. */
void process();

 /* Return random digit which will be used as a pointer into a hash 
    table used to create problems for math competitions. */
int random_num();

void roster_page();

 /* Input and evaluate a password. */
void security();

int select_board();

 /* Pick an answer key. */
int select_book();

int select_line_page();

void splash_page();

void subtract();

 /* Sets session length in minutes. */
void timeit();

 /* Create a space at any point in student composition by moving all 
    following text one space to the right.  Do not replace original 
    character at division point; result is to duplicate it one space to 
    the right. */
void wedge(int);

void win_page();

 /* Here are the screens */

SDL_Window* windowframe = NULL;
SDL_Surface* glass = NULL;
SDL_Surface* scratch = NULL;
SDL_Renderer *rends = NULL;

SDL_Color black = {0,0,0,255};
SDL_Color green = {7,167,8,255};
SDL_Color red = {255,0,0,255};
SDL_Color yellow = {248,250,26,255};
SDL_Color white = {255,255,255,255};

SDL_Color hue[41];

struct SDL_Rect tile[42][50];  // Used to lay out keyboard/workcheck/processor screens.
struct SDL_Rect tilem[42][50];  // Used to lay out math problems.
struct SDL_Rect line[42];  // Used to lay out keyboarding screen.
struct SDL_Rect tagline[42];  // Used to lay out keyboarding screen messages.
struct SDL_Rect lines[6];
struct SDL_Rect slots[42][50];

TTF_Font  *fnt15 = NULL;
TTF_Font  *fnt20 = NULL;
TTF_Font  *fnt30 = NULL;
TTF_Font  *fnt33 = NULL;
TTF_Font  *fnt35 = NULL;
TTF_Font  *fnt40 = NULL;
TTF_Font  *fnt45 = NULL;
TTF_Font  *fnt50 = NULL;
TTF_Font  *fnt90 = NULL;
TTF_Font  *fnt110 = NULL;

void
add()
{
  char a;
  int b = 0;

                    /* OPEN EMPTY WHITE WINDOW */

  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

                 /* PUT 30 PROBLEMS ON THE SCREEN */

  for (e = 1; e <= 30; e++)
    {
      Stop[e] = 0;
      Goofed[e] = 0;
      for (b = 0; b < 20; b++) Last_line[e][b] = ' ';
      Score[e]=0;
      make_add_prob(e);
    
      Last_line[e][1] = (char)(Addend1[e])+48;
      Last_line[e][3] = '+';
      Last_line[e][5] = (char)(Addend2[e])+48;
      Last_line[e][7] = '=';
      for (c = 0; c < 20; c++)
        {
          scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], hue[e]);
          SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
          SDL_FreeSurface(scratch);
        }
      
      Placeflag[e] = 1;
    }

  SDL_UpdateWindowSurface(windowframe);

                         /* START KEYBOARD POLLING */
  while (1)
    {
      if (get_char() == 10) /* CHECK TEACHER'S KEYBOARD FOR RETURN TO MENU */
        {
          Last_line[e][15] = ' ';
          Last_line[e][16] = ' ';
          Last_line[e][1] = ' ';
          Last_line[e][3] = ' ';
          Last_line[e][5] = ' ';
          Goaround = 0;

          return;
        }

      for (e = 1; e <= 30; e++)
        { 
          if (Goofed[e] == 1)
            {
              if (time(t) - Clk[e] > 3) /* END OR CONTINUE TIMEOUT PENALTY. */
                {
                  Goofed[e] = 0;
                  scratch = TTF_RenderText_Solid(fnt40, "XX", white);
                  SDL_BlitSurface(scratch, NULL, glass, &tilem[e][10]);
                  SDL_FreeSurface(scratch);
                  SDL_UpdateWindowSurface(windowframe);
                }
            
              else
                {              
                  get_event(e);
                  continue;
                } 
            }
          
                            /* GET STUDENT INPUT */
            
          if (Goofed[e] == 0) 
            {   
              a = get_event(e);        
              if (a == 0) continue;            
              if (a == 10) goto evaluate;
              if (Stop[e] == 1) continue;
           
              if (Placeflag[e] == 1)
                 {
                   if (((int)a < 48) || ((int)a > 57))  continue;
                   Stuans[e] = (int) a - 48 ;
                   Z[0] = (int) a;  
                   Last_line[e][10] = (int) a;
                   Placeflag[e] = 2;
                                
                   scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][10]);
                   SDL_FreeSurface(scratch);
                   SDL_UpdateWindowSurface(windowframe);
                   
                   continue;
                 }
                         
              if (Placeflag[e] == 2)
                 {
                   if (((int)a < 48) || ((int)a > 57)) continue;
                     
                   Z[0] = a;  
                   Last_line[e][11] = (int) a;
                   Stuans[e] = 10 * Stuans[e] + (a - 48);
                      
                   scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                   SDL_FreeSurface(scratch);
                   SDL_UpdateWindowSurface(windowframe);

                   Stop[e] = 1;                     
                   continue;
                 }
              continue;       
            }       
                     
          continue;            
          evaluate:    
          if (Stuans[e] == Answer[e])
            { 
               for (c = 0; c < 20; c++)
                 {
                   scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], white);
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
                   SDL_FreeSurface(scratch);
                 }
               SDL_UpdateWindowSurface(windowframe);
             
               Advance[e] = 1;
               Score[e] = Score[e] + Addpoints[e];
               Ones_register = Score[e] % 10;
               Tens_register = Score[e] / 10;
               Last_line[e][10] = 32;
               Last_line[e][15] = (char) Tens_register + 48;
               Last_line[e][16] = (char) Ones_register + 48;
               if(Last_line[e][15] == 48){Last_line[e][15] = 32;}
               
               if(Score[e] < Win_score)
                 {
                   Stop[e] = 0;
                   Placeflag[e] = 1;
                   make_add_prob(e);
                 
                   Last_line[e][1] = (char)(Addend1[e])+48;
                   Last_line[e][3] = '+';
                   Last_line[e][5] = (char)(Addend2[e])+48;
                   Last_line[e][7] = '=';
                   Last_line[e][10] = ' ';
                   Last_line[e][11] = ' ';
                  
                   for (c = 0; c < 20; c++)
                     {
                       scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], hue[e]);
                       SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
                       SDL_FreeSurface(scratch);
                     }
                   
                   SDL_UpdateWindowSurface(windowframe);
                 }
                 
                              /* DECLARE A WINNER */
                 
               else
                 {
                   Last_line[e][15] = ' ';
                   Last_line[e][16] = ' ';

                   Operation = 1;
                   win_page();
                   Goaround = 1;
                   return;
                 }              
            }    
              
          else if(Stuans[e] != Answer[e])
            {
              Stop[e] = 0;
              Placeflag[e] = 1;
              Clk[e] = time(t);
              Goofed[e] = 1;
              
              Z[0] = Last_line[e][10];              
              scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
              SDL_BlitSurface(scratch, NULL, glass, &tilem[e][10]);
             
              Z[0] = Last_line[e][11];              
              scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
              SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
              SDL_FreeSurface(scratch);
                 
              Last_line[e][10] = 'X';
              Last_line[e][11] = 'X';
                
              scratch = TTF_RenderText_Solid(fnt40, "XX" , black);
              SDL_BlitSurface(scratch, NULL, glass, &tilem[e][10]);
              SDL_FreeSurface(scratch);
              SDL_UpdateWindowSurface(windowframe);
            }
        }        
    } 
  return;          
}

int
alpha()
{
  SDL_Event event;

  while(1)
    {
      SDL_WaitEvent(&event);    
      if (event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
            {
              case SDLK_0:
                Letter = 0;  
                break;
                
              case SDLK_1:
                Letter = 1;  
                break;
                
              case SDLK_2:
                Letter = 2;  
                break;
                 
              case SDLK_3:
                Letter = 3;  
                break;
                  
              case SDLK_4:
                Letter = 4;  
                break;
                  
              case SDLK_5:
                Letter = 5;  
                break;
                  
              case SDLK_6:
                Letter = 6;  
                break;
                  
              case SDLK_7:
                Letter =7;  
                break;
                   
              case SDLK_8:
                Letter =8;  
                break;
                   
              case SDLK_9:
                Letter =9;  
                break;
                  
              case SDLK_RETURN:
                return Letter;
                break;
            }
        }
    } 
   return Letter;
}

void
bail()
{
  for (e = 1; e < 41; e++)
    {
      Type_linea[e] = (char) (Tline[e] + 48);
    }  

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Type_linea", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Type_linea", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Type_linea", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Type_linea", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Type_linea", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Type_linea", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Type_linea", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Type_linea", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Type_linea", "w");

  fputs(Type_linea, FP);
  fclose(FP);
  return;
}

void
boarda()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

                         /* PRINT 30 PRACTICE LINES */

  Klok = time(t);
  for (e = 1; e <= 30; e++)
    {
      Tta[e] = (int) Tline[e]; 
      Score[e]=0;
      Cursor[e] = 0;
      scratch = TTF_RenderText_Solid(fnt40, TTEXT[Tta[e]], hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &line[e]);
      SDL_FreeSurface(scratch);
    }

  SDL_UpdateWindowSurface(windowframe);

  az=50;
                       /* START POLLING */

  while (1)
    {
      if ((get_char() == 10) || (time(t) - Klok == Time))
        {
          for(e=1; e<=30; e++)
            {
              Block_input[e]=0;
            }
          return;
        }
              
      for (e = 1; e <= 30; e++)
        { 
                       /* BLOCK INPUT DURING MESSAGE TIMEOUT */
            
          if ((Block_input[e] == 1) & (time(t) - Clk[e] < 4))
            { 
              read(Board_file_descriptor[e], ev, sizeof(struct input_event));
              ev[0].code = 0;
              continue;
            }
                 
             /* END MESSAGE TIMEOUT AND PRINT NEXT PRACTICE LINE  */
              
          if ((Block_input[e] == 1) & (time(t) - Clk[e] > 3))
            {
              Block_input[e] = 0;
              Tta[e] = (int) Tline[e]; 
              Score[e]=0;
              Cursor[e] = 0;
                           
              SDL_FillRect(glass, &tagline[e], SDL_MapRGB(glass->format,255,255,255));
              SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
              scratch = TTF_RenderText_Solid(fnt40, TTEXT[Tta[e]], hue[e]);
              SDL_BlitSurface(scratch, NULL, glass, &line[e]);
              SDL_FreeSurface(scratch);
            }
                                 
                            /* CHECK FOR INPUT */
          Ltr = 0;
          Value = 0;
          Code = 0;
          
          read(Board_file_descriptor[e], ev, sizeof(struct input_event));
    
          if (EV_KEY == ev[0].type)
            {
              Value = ev[0].value;
              Code = ev[0].code;
              ev[0].code = 0;

         
              if( (Code == 42) || (Code == 54) )
                {
                  if(Value > 0) Upper_case[e] = 1;
                  else Upper_case[e] = 0;
                  continue;
                } 
            
              if(Value == 0 || Value == 2)
                {
                  Make_or_break[0] = 0; 
                  continue;
                }
            
              if(Upper_case[e] == 0) Ltr = LC[Code];
              else Ltr = UC[Code];           
            }
          
          if(Ltr == 0) continue;

          /* IGNORE ENTER IF NOT AT END OF LINE */
          if((Cursor[e] != 20) & (Ltr == 10)) continue;
               
          /*  IF AT END OF LINE... */
          if (Cursor[e] == 20)
            {
               if (Ltr == 10)          
                 {
                   eval_type();
                   Tline[e] = Tta[e];
                   SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
                   scratch = TTF_RenderText_Solid(fnt40, TTEXT[m], hue[e]);
                   SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                   SDL_FreeSurface(scratch);
                 }
               continue;
            }

                     /* IF AT BEGINNING OF LINE... */
                     
          /* START TIMER.  */
          if (Cursor[e] == 0)
            {
              Clk[e] = time(t);
            }
                         
          /* EVALUATE KEYPRESS.  */
          Z[0] = Ltr;
          if (Ltr ==  TTEXT[Tta[e]][Cursor[e]])
            {
              Score[e]++;
              scratch = TTF_RenderText_Solid(fnt40, &Z[0], yellow);
              SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
              SDL_FreeSurface(scratch);
            } 
                   
          else
            { 
              scratch = TTF_RenderText_Solid(fnt40, "X", black);
              SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
              SDL_FreeSurface(scratch);
            }
                              
          Cursor[e]++;
        }  
      az--;
        if(az==0)
        {          
          SDL_UpdateWindowSurface(windowframe);
          az=50;
        }
    } 
  return;          
}

void
boardb()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

                         /* PRINT 30 PRACTICE LINES */
  for (e = 1; e <= 30; e++)
    {
      Ttb[e] = Line_number; 
      Score[e]=0;
      Cursor[e] = 0;
      scratch = TTF_RenderText_Solid(fnt40, TTEXT[Ttb[e]], hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &line[e]);
      SDL_FreeSurface(scratch);
    }
     
  SDL_UpdateWindowSurface(windowframe);
  
  az=50;  
  while (1)
    {
      if (get_char() == 10)
        {
          for(e=1; e<=30; e++)
            {
              Block_input[e]=0;
            }
          return;
        }
              
      for (e = 1; e <= 30; e++)
        { 
          /* BLOCK INPUT DURING MESSAGE TIMEOUT */
          if ((Block_input[e] == 1) & (time(t) - Clk[e] < 4))
            { 
              read(Board_file_descriptor[e], ev, sizeof(struct input_event));
              ev[0].code = 0;
              continue;
            }
                 
          /* END MESSAGE TIMEOUT AND PRINT NEXT PRACTICE LINE  */
          if ((Block_input[e] == 1) & (time(t) - Clk[e] > 3))
            {
              Block_input[e] = 0;
              Ttb[e] = (int) Tlineb[e]; 
              Score[e]=0;
              Cursor[e] = 0;
                     
              SDL_FillRect(glass, &tagline[e], SDL_MapRGB(glass->format,255,255,255));
              SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
              scratch = TTF_RenderText_Solid(fnt40, TTEXT[Ttb[e]], hue[e]);
              SDL_BlitSurface(scratch, NULL, glass, &line[e]);
              SDL_FreeSurface(scratch);
            }
                                 
                        /* CHECK FOR INPUT */
          Ltr = 0;
          Value = 0;
          Code = 0;
          
          read(Board_file_descriptor[e], ev, sizeof(struct input_event));
          
          if (EV_KEY == ev[0].type)
            {
              Value = ev[0].value;
              Code = ev[0].code;
              ev[0].code = 0;
            }

          if( (Code == 42) || (Code == 54) )
            {
              if (Value > 0) Upper_case[e] = 1;
              else Upper_case[e] = 0;
              continue;
            } 
                          
          if(Value == 0 || Value == 2)
            {
              Make_or_break[0] = 0; 
              continue;
            }
          
          if (Upper_case[e] == 0) Ltr = LC[Code];
          else Ltr = UC[Code];
          
          if(Ltr==0)continue;

          /* IGNORE ENTER IF NOT AT END OF LINE.  */
          if ((Cursor[e] != 20) & (Ltr == 10)) continue;
          
          /*  IF AT END OF LINE... */
          if (Cursor[e] == 20)
            {
              if (Ltr == 10)          
                 {
                   eval_typeb();
                   Tlineb[e] = Ttb[e];

                   SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
                   scratch = TTF_RenderText_Solid(fnt40, TTEXT[m], hue[e]);
                   SDL_BlitSurface(scratch, NULL, glass, &line[e]);

                   if(m==249)
                     {
                       scratch = TTF_RenderText_Solid(fnt40, Rostera[e], hue[e]);
                       SDL_BlitSurface(scratch, NULL, glass, &tagline[e]);
                     }

                   SDL_FreeSurface(scratch);
                 }
               continue;
          }

                    /* IF AT BEGINNING OF LINE... */
          
           /* START TIMER.  */
           if (Cursor[e] == 0)
             {
               Clk[e] = time(t);
             }
                         
           /* EVALUATE KEYPRESS.  */
           Z[0] = Ltr;
                        
           if (Ltr ==  TTEXT[Ttb[e]][Cursor[e]])
             {
               Score[e]++;
               scratch = TTF_RenderText_Solid(fnt40, &Z[0], yellow);
               SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
               SDL_FreeSurface(scratch);
             } 
                   
           else
             { 
               scratch = TTF_RenderText_Solid(fnt40, "X", black);
               SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
               SDL_FreeSurface(scratch);
             }
                              
           Cursor[e]++;
    }
   az--;
   if(az==0)
     {
       SDL_UpdateWindowSurface(windowframe);
       az=50;
     }
  } 
  return;          
}

int
changepage()
{
  SDL_RenderClear(rends); 
  SDL_RenderPresent(rends);

  struct SDL_Rect slots[5];
    
  slots[0].x = 0;
  slots[0].y = 295;
        
  slots[1].x = 0;
  slots[1].y = 395;
      
  slots[2].x = 35;
  slots[2].y = 395;
      
  slots[3].x = 0;
  slots[3].y = 495;
      
  slots[4].x = 0;
  slots[4].y = 595;
      
  scratch = TTF_RenderText_Solid(fnt45, "  NUMBER OF KEYBOARD TO BE ASSIGNED?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);

  char liner[3] = "  ";
  liner[2] = 0;
     
  int x = get_digit();
  liner[0] = (char) (x + 48);
  Boardnumber = x;

  scratch = TTF_RenderText_Solid(fnt45, liner, hue[e]);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
 
  int y = get_digit();
  if (y > 9)
    {
       fetch_name();
       return 0;
    }
  liner[1] = (char)(y + 48);
  Boardnumber = 10 * Boardnumber + y;
  
  scratch = TTF_RenderText_Solid(fnt45, liner, hue[e]);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
   if(Boardnumber > 30)
     {
       scratch = TTF_RenderText_Solid(fnt45, "  YOU DON'T HAVE THAT MANY BOARDS!", black);
       SDL_BlitSurface(scratch, NULL, glass, &slots[4]);
       SDL_FreeSurface(scratch);
       SDL_UpdateWindowSurface(windowframe);
       SDL_Delay(2000);

       detente();
       return 0;
     }

  alpha();
  fetch_name(e);

  return Boardnumber;        
}

void
check()
{
  int d = 0;

  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

                             /* ASK FOR PAGE NUMBER */

  for (e = 1; e <= 30; e++)
    {
       Step[e] = 0;
       scratch = TTF_RenderText_Solid(fnt40, TTEXT[244], hue[e]);
       SDL_BlitSurface(scratch, NULL, glass, &line[e]);
       SDL_FreeSurface(scratch);
       Cursor[e] = 0;
       Page[e] = 0;
    }
  
  SDL_UpdateWindowSurface(windowframe);

                             /* RECEIVE PAGE NUMBER */
  while (1)
    {    
      if (get_char() == 10) 
        {
          return;
        }
       
      for (e = 1; e <= 30; e++)
        {   
          a = 0;
          a = get_event(e);
          if (a == 0) continue;
          Z[0] = a;
          
          if ((Step[e] == 0) && (a > 47) && (a < 58))
            {
              Page[e] = 10 * Page[e] + (int) a - 48;  
           
              /* Start over if there is no such page. */
              jumpstart:
               
              if ((Page[e] > Max_page) || ((int)a == 10))
                {
                  Cursor[e] = 0;
                  Page[e] = 0;
                  
                  SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
            
                  scratch = TTF_RenderText_Solid(fnt40, TTEXT[244], hue[e]);
                  SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                  SDL_FreeSurface(scratch);
                   
                  SDL_UpdateWindowSurface(windowframe);
                  continue;
                }
            
              if (Cursor[e] == 0)
                {
                  SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
                  scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                  SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
                  SDL_FreeSurface(scratch);
                }
                 
              else
                {
                  scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                  SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
                  SDL_FreeSurface(scratch);
                } 
                 
               SDL_UpdateWindowSurface(windowframe);
               Cursor[e]++;  
             }                   
                               /* ASK FOR PROBLEM NUMBER */
               
           if ((Step[e] == 0) && (a == 10))
             {
               if (Page[e] == 0) goto jumpstart;
               Step[e]++;
               Cursor[e] = 0;
               SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
               
               scratch = TTF_RenderText_Solid(fnt40, TTEXT[245], hue[e]);
               SDL_BlitSurface(scratch, NULL, glass, &line[e]);
               SDL_FreeSurface(scratch);
               
               SDL_UpdateWindowSurface(windowframe);
                
               continue; 
             }
           
                       /* RECEIVE PROBLEM NUMBER */		
               
           if ((Step[e] == 1) && (a < 58) && (a > 47))
             {
               Z[0] = (int) a;
               
               if (Cursor[e] == 0)
                 {
                   SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
                 }
               
               scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
               SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
               SDL_FreeSurface(scratch);
               
               SDL_UpdateWindowSurface(windowframe);
               
               Problem[e] = 10 * Problem[e] + (int) a - 48;  
               if (Cursor[e] == 2)
                 {
                   a = 10;
                   Step[e]--;
                   Cursor[e] = 4;
                   goto jumpstart;
                 }
               Cursor[e]++;
             }
                
               /* IF NO PROBLEM REQUESTED, GO BACK AND ASK AGAIN FOR PAGE. */
              
           if ((Step[e] == 1) && (a == 10))
             {
               if (Problem[e] == 0)
                 {
                   Step[e]--;
                   Cursor[e] = 4;
                   goto jumpstart;
                 }
                  
               else
                {
                              /* ASK FOR STUDENT ANSWER */
           
                  Step[e]++;
                  SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
                
                  scratch = TTF_RenderText_Solid(fnt40, TTEXT[246], hue[e]);
                  SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                  SDL_FreeSurface(scratch);
                  
                  SDL_UpdateWindowSurface(windowframe);
                  
                  Cursor[e] = 0;
                  continue;
                }
             }
                  
                         /* RECEIVE STUDENT ANSWER */
                         
           if (Step[e] == 2) 
             {
               if ((a != 10) && (a !=13))
                 {
                   if (Cursor[e] == 0)
                     {
                       SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
                     }
                     
                   scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                   SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
                   SDL_FreeSurface(scratch);
                       
                   SDL_UpdateWindowSurface(windowframe);
                        
                   Guess[e][Cursor[e]] = Z[0];  
                   if (Cursor[e] < 20) Cursor[e]++;  
                 }
                 
                                   /* ANSWER RIGHT OR WRONG? */
                    
               if ((a == 10) || Cursor[e] == 20)
                 {
                   find_answer();
                   SDL_FillRect(glass, &line[e], SDL_MapRGB(glass->format,255,255,255));
                     
                   if (strcmp(Answ[e],Guess[e]) == 0)
                     {
                       scratch = TTF_RenderText_Solid(fnt40, TTEXT[251], hue[e]);
                       SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                       SDL_FreeSurface(scratch);
                       Cursor[e] = 0;
                       Step[e] = 0;
                     }
                     
                   else
                     {
                       scratch = TTF_RenderText_Solid(fnt40, TTEXT[252], hue[e]);
                       SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                       SDL_FreeSurface(scratch);
                       Step[e] = 0;
                     }
                       Problem[e] = 0;
                       SDL_UpdateWindowSurface(windowframe);
                       for (d = 0; d < 20; d++)  Guess[e][d] = 0;
                } 
             }
         }
     }
  return;
}

void
choose_line()
{
  SDL_RenderClear(rends); 
  SDL_RenderPresent(rends);
     
  struct SDL_Rect slots[4];
    
  slots[0].x = 0;
  slots[0].y = 220;
      
  slots[1].x = 0;
  slots[1].y = 320;

  slots[2].x = 25;
  slots[2].y = 320;
    
  slots[3].x = 50;
  slots[3].y = 320;

  scratch = TTF_RenderText_Solid(fnt40, "CHOOSE A LINE BETWEEN 1 AND 252", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);

  Z[0] = 0;
  Z[1] = 0;
  Z[2] = 0;
  Z[3] = 0;

  a = (int)get_digit();

  if( (a < 1) || (a > 9) )
    {
      Panic = 1;
      return;
    }

  Line_number = a;
  Z[0] = (char)a + 48;

  scratch = TTF_RenderText_Solid(fnt40, &Z[0], black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);

  a = (int)get_digit();

  if(a == 10)
    {
      Z[0] = 0;
      Z[1] = 0;
      Z[2] = 0;
      Z[3] = 0;

      detente();
      dump();
      boardb();

      return;
    }

  Line_number = (10 * Line_number) + a;
  Z[1] = (char)a + 48;

  scratch = TTF_RenderText_Solid(fnt40, &Z[1], black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[2]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);

  a = (int)get_digit();

  if(a == 10)
    {
      Z[0] = 0;
      Z[1] = 0;
      Z[2] = 0;
      Z[3] = 0;

      detente();
      dump();
      boardb();

      return;
    }

  Line_number = (10 * Line_number) + a;
  if(Line_number > 252)
    {
      Z[0] = 0;
      Z[1] = 0;
      Z[2] = 0;
      Z[3] = 0;

      Panic = 1;
      return;
    }
  Z[2] = (char)a + 48;

  scratch = TTF_RenderText_Solid(fnt40, &Z[2], black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[3]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);

  Z[0] = 0;
  Z[1] = 0;
  Z[2] = 0;
  Z[3] = 0;

  detente();
  dump();
  boardb();
  return;
}

void
clear_pro()
{
  for (e = 0; e < 41; e++)
    {
       for (a = 0; a < 5002; a++)
         {
           Comp[e][a] = 0;
         }
    }
  file_save();
  return;
}

void
detente()
{  
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
   
  struct SDL_Rect slots[6];
    
  slots[0].x = 0;
  slots[0].y = 200;
    
  slots[1].x = 0;
  slots[1].y = 320;
    
  slots[2].x = 0;
  slots[2].y = 380;
        
  scratch = TTF_RenderText_Solid(fnt50, " PRESS ENTER TO CONTINUE ", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe); 

  int letter = 0;
  while(1)
    {
      letter = get_digit();
      if(letter == 10) break;
    }    
  return;
}

void
divide()
{
  char a;

                          /* OPEN EMPTY WHITE WINDOW */
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

                      /* PUT 30 PROBLEMS ON THE SCREEN */
  for (e = 1; e <= 30; e++)
    {
      Stop[e] = 0;
      Goofed[e] = 0;
      for (b = 0; b < 20; b++) Last_line[e][b] = ' ';
      Last_line[e][15] = ' ';
      Score[e]=0;
      make_div_prob();
    
      Ones_register =  Dividend[e] % 10;
      Tens_register = Dividend[e] / 10;
      Last_line[e][1] = (char) Tens_register + 48;
      Last_line[e][2] = (char) Ones_register + 48;
      Last_line[e][4] = '/';
      Last_line[e][6] = (char) Divisor[e] + 48;
      Last_line[e][8] = '=';
      if (Last_line[e][1] == 48){Last_line[e][1] = 32;}
      
      for (c = 0; c < 20; c++)
        {
          scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], hue[e]);
          SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
          SDL_FreeSurface(scratch);   
        }
    }  
  SDL_UpdateWindowSurface(windowframe);
                
                         /* START KEYBOARD POLLING. */
  while (1)
    {
      if (get_char() == 10)  /* CHECK TEACHER'S KEYBOARD FOR RETURN TO MENU. */
        {
          Goaround = 0;
          return;
        }
      for (e = 1; e <= 30; e++)
        {
          if (Goofed[e] == 1)    /* END OR CONTINUE ERROR TIMEOUT PENALTY. */ 
            {
              if (time(t) - Clk[e] > 3)
                {
                  Goofed[e] = 0;
                  Z[0] = Last_line[e][11];
                  scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
                  SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                  SDL_FreeSurface(scratch);
                  SDL_UpdateWindowSurface(windowframe);
                }    
              else
                {
                  get_event(e);
                  continue;
                } 
            }
            
          if (Goofed[e] == 0)  /* GET STUDENT INPUT. */
            {
              a = get_event(e);        
              if (a == 0) continue;
              if (a == 10) goto evaluatediv;
              if (Stop[e] == 1) continue;

               if (((int) a < 48) || ((int) a > 57)) continue;
               Stuans[e] = (int) a - 48;
               Z[0] = (int) a;
               Last_line[e][11] = (int)a;
                
               scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
               SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
               SDL_FreeSurface(scratch);
               SDL_UpdateWindowSurface(windowframe);
               Stop[e] = 1;
               continue;
            
                                       /* EVALUATE ANSWERS. */
              evaluatediv:
              
                             /* ANSWER IS GOOD. */
              if (Stuans[e] == Answer[e])
                 { 
                   for (c = 0; c < 20; c++)
                     {
                       scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], white);
                       SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
                       SDL_FreeSurface(scratch);
                     }
                   SDL_UpdateWindowSurface(windowframe);
                   
                   Advance[e] = 1;
                   Score[e] = Score[e] + Divpoints[e];
                   Ones_register = Score[e] % 10;
                   Tens_register = Score[e] / 10;
                   Last_line[e][15] = (char) Ones_register + 48;
                   Last_line[e][14] = (char) Tens_register + 48;
                   Last_line[e][11] = 32;
                   if(Last_line[e][14] == 48){Last_line[e][14] = 32;}
                   
                   if(Score[e] < Win_score)
                     {
                       Stop[e] = 0;
                       make_div_prob();
                      
                       Ones_register =  Dividend[e] % 10;
                       Tens_register = Dividend[e] / 10;
                       Last_line[e][1] = (char) Tens_register + 48;
                       Last_line[e][2] = (char) Ones_register + 48;
                       Last_line[e][4] = '/';
                       Last_line[e][6] = (char) Divisor[e] + 48;
                       Last_line[e][8] = '=';
                       Last_line[e][11] = ' ';
                       if (Last_line[e][1] == 48){Last_line[e][1] = 32;}
                      
                       for (c = 0; c < 20; c++)
                         {
                           scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], hue[e]);
                           SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
                           SDL_FreeSurface(scratch);
                         }
                       
                       SDL_UpdateWindowSurface(windowframe);
                     }
                                           
                              /* DECLARE A WINNER. */
                   else
                     {
                       Operation = 4;
                       win_page();
                       Goaround = 1;
                       return;
                     } 
                 }   
                      
                             /* ANSWER IS BAD. */
               else if(Last_line[e][11] != Answer[e])
                 {
                   Clk[e] = time(t);
                   Goofed[e] = 1;
                   Stop[e] = 0;
                    
                   Z[0] = Last_line[e][11];                
                   scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                   
                   Z[0] = 'X';
                   Last_line[e][11] = 'X';
                   scratch = TTF_RenderText_Solid(fnt40, &Z[0], black);
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                   SDL_FreeSurface(scratch);
                   SDL_UpdateWindowSurface(windowframe);
                 }
            }
        }        
    } 
  return;          
}
 
void
double_check()
{    
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
   
  struct SDL_Rect slots[6];
    
  slots[0].x = 0;
  slots[0].y = 200;
    
  slots[1].x = 0;
  slots[1].y = 320;
    
  slots[2].x = 0;
  slots[2].y = 380;
        
  slots[3].x = 0;
  slots[3].y = 440;
        
  scratch = TTF_RenderText_Solid(fnt50, " ARE YOU SURE YOU WANT TO QUIT? ", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
             
  scratch = TTF_RenderText_Solid(fnt50, "       1 - QUIT NOW", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
             
  scratch = TTF_RenderText_Solid(fnt50, "       2 - DON'T QUIT", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[2]);
  SDL_FreeSurface(scratch);
             
  scratch = TTF_RenderText_Solid(fnt50, "       3 - ERASE ALL STUDENT FILES", red);
  SDL_BlitSurface(scratch, NULL, glass, &slots[3]);
  SDL_FreeSurface(scratch);
             
  SDL_UpdateWindowSurface(windowframe); 
        
  SDL_Event event;
  Option = 0;     
  while(1)
    {
      SDL_WaitEvent(&event);    
      if (event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
           {
             case SDLK_1:
               security();
               if (Panic == 1) return;
               exit(0);
             
             case SDLK_2:
               return;   
               
             case SDLK_3:
               new_year();
               return;
           }
        }
    }
  return;
}
  
void
dump()
{
 int zz = 0;
  for (e = 1;  e <= 30; e++)
    {
      for(zz = 0; zz < 200; zz++)
       {
         read(Board_file_descriptor[e], ev, sizeof(struct input_event));
       }
    }
  return;
}
    
void
eval_type()
{  
   Clk[e] = time(t) - Clk[e];
   if (Score[e] < 18) m = 248;
   if (Score[e] > 17)
     {
       if (Clk[e] > 15) m = 250;
       else
         {
           Tta[e]++;
           m = 249;
         }
     }
   Block_input[e] = 1;
   Clk[e] = time(t);
   return;    
}    

void
eval_typeb()
{  
   Clk[e] = time(t) - Clk[e];
   if (Score[e] < 18) m = 248;
   if (Score[e] > 17)
     {
       if (Clk[e] > 15) m = 250;
       else
         {
           Ttb[e]++;
           m = 249;
         }
     }
   Block_input[e] = 1;
   Clk[e] = time(t);
   return;    
}    

void
factor1_A()
{
  for (b = 0; b < 9; b++)
    { 
      At1[b] = (int)Factor[b];
      a = At1[b];
      A_terma[b] = (char)(a);  
    } 
 
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/A_terma", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/A_terma", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/A_terma", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/A_terma", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/A_terma", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/A_terma", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/A_terma", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/A_terma", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/A_terma", "w");
               
  fputs(A_terma, FP);
  fclose(FP);
  for (b = 0; b < 9; b++)
    {
      At1[b] = At1[b] - 48;
    }
  return;
}

void
factor1_D()
{ 

  for (b = 0; b < 9; b++)
    { 
      Dt1[b] = (int)Factor[b];
      a = Dt1[b];
      D_terma[b] = (char)(a);  
    } 

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/D_terma", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/D_terma", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/D_terma", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/D_terma", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/D_terma", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/D_terma", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/D_terma", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/D_terma", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/D_terma", "w");
               
  fputs(D_terma, FP);
  fclose(FP);
  for (b = 0; b < 9; b++)
    {
      Dt1[b] = Dt1[b] - 48;
    }
  return;
}

void
factor1_M()
{ 
  for (b = 0; b < 9; b++)
    { 
      Mt1[b] = (int)Factor[b];
      a = Mt1[b];
      M_terma[b] = (char)(a);  
    } 

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/M_terma", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/M_terma", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/M_terma", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/M_terma", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/M_terma", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/M_terma", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/M_terma", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/M_terma", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/M_terma", "w");
               
  fputs(M_terma, FP);
  fclose(FP);
  for (b = 0; b < 9; b++)
    {
      Mt1[b] = Mt1[b] - 48;
    }
  return;
}

void
factor1_S()
{ 
  for (b = 0; b < 9; b++)
    { 
      St1[b] = (int)Factor[b];
      a = St1[b];
      S_terma[b] = (char)(a);  
    } 

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/S_terma", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/S_terma", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/S_terma", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/S_terma", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/S_terma", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/S_terma", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/S_terma", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/S_terma", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/S_terma", "w");
               
  fputs(S_terma, FP);
  fclose(FP);
  for (b = 0; b < 9; b++)
    {
      St1[b] = St1[b] - 48;
    }
  return;
}

void
factor2_A()
{
  for (b = 0; b < 9; b++)
    { 
      At2[b] = (int)Factor[b];
      a = At2[b];
      A_termb[b] = (char)(a);  
    } 
 
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/A_termb", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/A_termb", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/A_termb", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/A_termb", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/A_termb", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/A_termb", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/A_termb", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/A_termb", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/A_termb", "w");
               
  fputs(A_termb, FP);
  fclose(FP);
  for (b = 0; b < 9; b++)
    {
      At2[b] = At2[b] - 48;
    }
  return;
}

void
factor2_D()
{ 

  for (b = 0; b < 9; b++)
    { 
      Dt2[b] = (int)Factor[b];
      a = Dt2[b];
      D_termb[b] = (char)(a);  
    } 

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/D_termb", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/D_termb", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/D_termb", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/D_termb", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/D_termb", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/D_termb", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/D_termb", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/D_termb", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/D_termb", "w");
               
  fputs(D_termb, FP);
  fclose(FP);
  for (b = 0; b < 9; b++)
    {
      Dt2[b] = Dt2[b] - 48;
    }
  return;
}

void
factor2_M()
{ 
  for (b = 0; b < 9; b++)
    { 
      Mt2[b] = (int)Factor[b];
      a = Mt2[b];
      M_termb[b] = (char)(a);  
    } 

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/M_termb", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/M_termb", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/M_termb", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/M_termb", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/M_termb", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/M_termb", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/M_termb", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/M_termb", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/M_termb", "w");
               
  fputs(M_termb, FP);
  fclose(FP);
  for (b = 0; b < 9; b++)
    {
      Mt2[b] = Mt2[b] - 48;
    }
  return;
}

void
factor2_S()
{ 
  for (b = 0; b < 9; b++)
    { 
      St2[b] = (int)Factor[b];
      a = St2[b];
      S_termb[b] = (char)(a);  
    } 

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/S_termb", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/S_termb", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/S_termb", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/S_termb", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/S_termb", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/S_termb", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/S_termb", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/S_termb", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/S_termb", "w");
               
  fputs(S_termb, FP);
  fclose(FP);
  for (b = 0; b < 9; b++)
    {
      St2[b] = St2[b] - 48;
    }
  return;
}
   
void
fetch_letter()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
  
               /* DIVIDE windowframe INTO TILES. */ 
    
   struct SDL_Rect tile[1000];
   int b = 0;
   for (c = 0;  c < 17; c++)
     {
       for (b = 0; b < 45;  b++)
          {
            tile[b+c*45].x = (b * 30);
            tile[b+c*45].y = c * 44;
          }
     }
   
   scratch = TTF_RenderText_Solid(fnt50, "WHO TEACHES THIS CLASS? ", red);
   SDL_BlitSurface(scratch, NULL, glass, &tile[230]);
   SDL_FreeSurface(scratch);
   SDL_UpdateWindowSurface(windowframe);
    
   for (k = 0; k < 20; k++)
     {
       Teacher[k] = 0;
     }
   k = 0;
   int a = 325;
   char letter[2];
   SDL_Event event;
         
   while(1)
     {
       SDL_WaitEvent(&event);   
         {
            if (event.type == SDL_KEYDOWN)
              {
                if (1 == 1)                 
                  {
                    switch(event.key.keysym.sym)
                      {
                        case SDLK_0:
                          letter[0] =0;
                          break;
                         
                        case SDLK_1:
                          letter[0] ='1';
                          break;
                          
                        case SDLK_2:
                          letter[0] ='2';
                          break;
                          
                        case SDLK_3:
                          letter[0] ='3';
                          break;
                               
                        case SDLK_4:
                          letter[0] ='4';
                          break;
                      
                        case SDLK_5:
                          letter[0] ='5';
                          break;
                      
                        case SDLK_6:
                          letter[0] ='6';
                          break;
                                  
                        case SDLK_7:
                          letter[0] ='7';
                          break;
                                   
                        case SDLK_8:
                          letter[0] ='8';
                          break;
                            
                        case SDLK_9:
                          letter[0] ='9';
                          break;
                             
                        case SDLK_a:
                          letter[0] ='A';
                          break;
                              
                        case SDLK_b:
                          letter[0] ='B';
                          break;
                             
                        case SDLK_c:
                          letter[0] ='C';
                          break;
                               
                        case SDLK_d:
                          letter[0] ='D';
                          break;
                            
                        case SDLK_e:
                          letter[0] ='E';
                          break;
                               
                        case SDLK_f:
                          letter[0] ='F';
                          break;
                                
                        case SDLK_g:
                          letter[0] ='G';
                          break;
                               
                        case SDLK_h:
                          letter[0] ='H';
                          break;
                           
                        case SDLK_i:
                          letter[0] ='I';
                          break;
                             
                        case SDLK_j:
                          letter[0] ='J';
                          break;
                           
                        case SDLK_k:
                          letter[0] ='K';
                          break;
                              
                        case SDLK_l:
                          letter[0] ='L';
                          break;
                            
                        case SDLK_m:
                          letter[0] ='M';
                          break;
                             
                        case SDLK_n:
                          letter[0] ='N';
                          break;
                               
                        case SDLK_o:
                          letter[0] ='O';
                          break;
                                   
                        case SDLK_p:
                          letter[0] ='P';
                          break;
                                 
                        case SDLK_q:
                          letter[0] ='Q';
                          break;
                             
                        case SDLK_r:
                          letter[0] ='R';
                          break;
                              
                        case SDLK_s:
                          letter[0] ='S';
                          break;
                          
                        case SDLK_t:
                          letter[0] ='T';
                          break;
                         
                        case SDLK_u:
                          letter[0] ='U';
                          break;
                          
                        case SDLK_v:
                          letter[0] ='V';
                          break;
                          
                        case SDLK_w:
                          letter[0] ='W';
                          break;
                           
                        case SDLK_x:
                          letter[0] ='X';
                          break;
                               
                        case SDLK_y:
                          letter[0] ='Y';
                          break;
                        
                        case SDLK_z:
                          letter[0] ='Z';
                          break;
                              
                        case SDLK_SPACE:
                          letter[0] =' ';
                          break;
                                         
                        case SDLK_COMMA:
                          letter[0] ='<';
                          break;
                            
                        case SDLK_PERIOD:
                          letter[0] ='>';
                          break;
                           
                        case SDLK_SEMICOLON:
                          letter[0] =':';
                          break;
                           
                        case SDLK_SLASH:
                          letter[0] ='?';
                          break;
                                           
                        case SDLK_MINUS:
                          letter[0] ='_';
                          break;
                          
                        case SDLK_LEFTPAREN:
                          letter[0] ='(';
                          break;
                            
                        case SDLK_RIGHTPAREN:
                          letter[0] =')';
                          break;
                        
                        case SDLK_QUOTEDBL:
                          letter[0] ='"';
                          break;
                             
                        case SDLK_DOLLAR:
                          letter[0] ='$';
                          break;
                            
                        case SDLK_AMPERSAND:
                          letter[0] ='&';
                          break;
                         
                        case SDLK_EXCLAIM:
                          letter[0] ='!';
                          break;
                          
                        case SDLK_PLUS:
                          letter[0] ='+';
                          break;
                          
                        case SDLK_HASH:
                          letter[0] ='#';
                          break;
                          
                        case SDLK_QUESTION:
                          letter[0] ='?';
                          break;
                         
                        case SDLK_COLON:
                          letter[0] =':';
                          break;
                         
                        case SDLK_EQUALS:
                          letter[0] ='=';
                          break;
                             
                        case SDLK_GREATER:
                          letter[0] ='>';
                          break;
                        
                        case SDLK_LEFTBRACKET:
                          letter[0] ='[';
                          break;
                            
                        case SDLK_RIGHTBRACKET:
                          letter[0] =']';
                          break;
                        
                        case SDLK_LESS:
                          letter[0] ='<';
                          break;
                             
                        case SDLK_RETURN:   
         
                          if (Class == 1) FP = fopen("/usr/local/com/class1/names/Teacher", "w");
                          if (Class == 2) FP = fopen("/usr/local/com/class2/names/Teacher", "w");
                          if (Class == 3) FP = fopen("/usr/local/com/class3/names/Teacher", "w");
                          if (Class == 4) FP = fopen("/usr/local/com/class4/names/Teacher", "w");
                          if (Class == 5) FP = fopen("/usr/local/com/class5/names/Teacher", "w");
                          if (Class == 6) FP = fopen("/usr/local/com/class6/names/Teacher", "w");
                          if (Class == 7) FP = fopen("/usr/local/com/class7/names/Teacher", "w");
                          if (Class == 8) FP = fopen("/usr/local/com/class8/names/Teacher", "w");
                          if (Class == 9) FP = fopen("/usr/local/com/class9/names/Teacher", "w");
             
                          fputs(Teacher, FP);
                          fclose(FP);
                          return;
                          break;
                 
                        default:
                          letter[0] = ' ';
                          break;                     
                      }
                   
                    Teacher[k] = letter[0];
                    letter[1] = 0; 
                    k++;
                    if (k > 20)
                      {
                        break;
                      }
                    scratch = TTF_RenderText_Solid(fnt50, letter, black);
                    SDL_BlitSurface(scratch, NULL, glass, &tile[a]);
                    SDL_FreeSurface(scratch);
                    if (a < 675) {a++;}
                    SDL_UpdateWindowSurface(windowframe);
                 }
             }
         }
     }
  alpha();
  return;
}

int
fetch_name()
{  
                         /* OPEN EMPTY WHITE WINDOW. */
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
  
                        /* CREATE RECTANGLE ARRAY. */
  struct SDL_Rect tile[1000];
  int b = 0;
  for (c = 0;  c < 17; c++)
    {
      for (b = 0; b < 45;  b++)
        {
          tile[b+c*45].x = (b * 30);
          tile[b+c*45].y = c * 44;
        }
    }
                         /* PUT MESSAGE ON SCREEN. */
  scratch = TTF_RenderText_Solid(fnt50, "GIMME A NAME! ", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[235]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
   
  int a = 325;
  char letter;
  k = 0;

  for(k = 0; k < 9; k++)
    {
      while(1)
        {
          letter = get_char();         
          if (letter != 0) break;
        }
      if (letter == 10) break; 
      Studint[k] = letter;
      Studint[k+1] = 0;
       
      scratch = TTF_RenderText_Solid(fnt50, &Studint[k], red);
      SDL_BlitSurface(scratch, NULL, glass, &tile[a]);
      SDL_FreeSurface(scratch);
      if (a < 675) {a++;}
      SDL_UpdateWindowSurface(windowframe);
    }

  int g = 0;
  for( g = 0; g < 9; g++)
    {
      Roster[Boardnumber][g] = Studint[g];
    }
  return letter;
}

int
fetch_win_score()
{
  int b = 0;
  int Digit = 0;
      
                       /* CREATE windowframe ON WHICH TO WRITE */
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
  
          
                       /* DIVIDE WHITEBOARD INTO TILES */
  struct SDL_Rect tile[1000];
  for (c = 0;  c < 17; c++)
    {
      for (b = 0; b < 45;  b++)
         {
           tile[b+c*45].x = (b * 30);
           tile[b+c*45].y = c * 44;
         }
    }
  
                               /* ASK FOR SCORE */
  scratch = TTF_RenderText_Solid(fnt40, "WHAT SCORE (between 10 and 99) WINS?", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[316]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
   
  k = 0;         
  char proxy[3] = {0,0,0};

  while((proxy[0] < 49) || (proxy[0] > 57)) 
    {
      proxy[0] = (char) get_digit() + 48;
    }
  scratch = TTF_RenderText_Solid(fnt40, proxy, red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[430]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
    Win_score = (int) proxy[0] - 48;
  
  while((proxy[1] < 48) || (proxy[1] > 57)) 
    {
      proxy[1] = (char) get_digit() + 48;
    } 
  scratch = TTF_RenderText_Solid(fnt40, proxy, red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[430]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe); 
  Win_score = 10 * Win_score + (int) proxy[1] - 48;
  
  alpha();
  
  Ww[0] = (char)(Win_score);
  
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Win_score", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Win_score", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Win_score", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Win_score", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Win_score", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Win_score", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Win_score", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Win_score", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Win_score", "w");
  
  fputs(Ww, FP);
  fclose(FP);
  return Digit;
}
  
void
file_save()
{
  for (e = 1;  e < 41; e++)
    {
      strcpy (Buf, "");
    
      if (Class == 1) strcpy (Buf, "/usr/local/com/class1/files/file");
      if (Class == 2) strcpy (Buf, "/usr/local/com/class2/files/file");
      if (Class == 3) strcpy (Buf, "/usr/local/com/class3/files/file");
      if (Class == 4) strcpy (Buf, "/usr/local/com/class4/files/file");
      if (Class == 5) strcpy (Buf, "/usr/local/com/class5/files/file");
      if (Class == 6) strcpy (Buf, "/usr/local/com/class6/files/file");
      if (Class == 7) strcpy (Buf, "/usr/local/com/class7/files/file");
      if (Class == 8) strcpy (Buf, "/usr/local/com/class8/files/file");
      if (Class == 9) strcpy (Buf, "/usr/local/com/class9/files/file");
      
      if(e<10)
        {
          Buf[32] = 48;
          Buf[33] = e+48;
        }
      else
        {
          Buf[32] = (e/10) + 48;
          Buf[33] = (e % 10) + 48;
        }

      FP = fopen (Buf, "w");
      fputs (Comp[e], FP);
      fclose (FP);
        
    }
  return;
}

void
find_answer()
{
  b = 0;
  c = 0;
  for (a = 0; a < 40000; a++)
    {
      if (Key[a] == 126)
        {
          b++;
          c = 0;
        }
      if (Key[a] == 124) c++;
      if ((b == Page[e]) && (c == Problem[e]))
        {
          a++;
          break;
        }
    }
                           /* CLEAR ANSWER. */

  strncpy(Answ[e], &Dum[0], 40);
  for (c = 0; c < 39; c++)
    {
      if ((Key[a] != 124) && (Key[a] != 0) && (Key[a] != 125))
        {
          Answ[e][c] = Key[a];
          a++;
        }
      else break;
    }
  return;
}

int
get_char()
{
  SDL_Event event;           
  event.type = SDL_KEYUP;
  SDL_PollEvent(&event);    
  if (event.type == SDL_KEYDOWN)
    {
      switch(event.key.keysym.sym)
        {
          case SDLK_0:
            Letra[e] = '0';  
            break;
              
          case SDLK_1:
            Letra[e] = '1';  
            break;
                
          case SDLK_2:
            Letra[e] = '2';  
            break;
                 
          case SDLK_3:
            Letra[e] = '3';  
            break;
                  
          case SDLK_4:
            Letra[e] = '4';  
            break;
                  
          case SDLK_5:
            Letra[e] = '5';  
            break;
                  
          case SDLK_6:
            Letra[e] = '6';  
            break;
                  
          case SDLK_7:
            Letra[e] = '7';  
            break;
                   
          case SDLK_8:
            Letra[e] = '8';  
            break;
                   
           case SDLK_9:
             Letra[e] = '9';  
             break;
                  
           case SDLK_a:
             Letra[e] ='A';
             break;
                           
           case SDLK_b:
             Letra[e] ='B';
             break;
                          
           case SDLK_c:
             Letra[e] ='C';
             break;
                            
           case SDLK_d:
             Letra[e] ='D';
             break;
                         
           case SDLK_e:
             Letra[e] ='E';
             break;
                          
           case SDLK_f:
             Letra[e] ='F';
             break;
                          
           case SDLK_g:
             Letra[e] ='G';
             break;
                           
           case SDLK_h:
             Letra[e] ='H';
             break;
                          
           case SDLK_i:
             Letra[e] ='I';
             break;
                          
           case SDLK_j:
             Letra[e] ='J';
             break;
                           
           case SDLK_k:
             Letra[e] ='K';
             break;
                           
           case SDLK_l:
             Letra[e] ='L';
             break;
                            
           case SDLK_m:
             Letra[e] ='M';
             break;
                       
           case SDLK_n:
             Letra[e] ='N';
             break;
                            
           case SDLK_o:
             Letra[e] ='O';
             break;
                                
           case SDLK_p:
             Letra[e] ='P';
             break;
                              
           case SDLK_q:
             Letra[e] ='Q';
             break;
                        
           case SDLK_r:
             Letra[e] ='R';
             break;
                           
           case SDLK_s:
             Letra[e] ='S';
             break;
                          
           case SDLK_t:
             Letra[e] ='T';
             break;
                         
           case SDLK_u:
             Letra[e] ='U';
             break;
                          
           case SDLK_v:
             Letra[e] ='V';
             break;
                        
           case SDLK_w:
             Letra[e] ='W';
             break;
                        
           case SDLK_x:
             Letra[e] ='X';
             break;
                            
           case SDLK_y:
             Letra[e] ='Y';
             break;
                        
           case SDLK_z:
             Letra[e] ='Z';
             break;

           case SDLK_SPACE:
             Letra[e] =' ';
             break;
                                      
           case SDLK_COMMA:
             Letra[e] ='<';
             break;
                            
           case SDLK_PERIOD:
             Letra[e] ='>';
             break;
                           
           case SDLK_SEMICOLON:
             Letra[e] =':';
             break;
                           
           case SDLK_SLASH:
             Letra[e] ='?';
             break;
                                        
           case SDLK_MINUS:
             Letra[e] ='_';
             break;
                          
           case SDLK_LEFTPAREN:
             Letra[e] ='(';
             break;
                         
           case SDLK_RIGHTPAREN:
             Letra[e] =')';
             break;
                        
           case SDLK_QUOTEDBL:
             Letra[e] ='"';
             break;
                             
           case SDLK_DOLLAR:
             Letra[e] ='$';
             break;
                            
           case SDLK_AMPERSAND:
             Letra[e] ='&';
             break;
                     
           case SDLK_EXCLAIM:
             Letra[e] ='!';
             break;
                          
           case SDLK_PLUS:
             Letra[e] ='+';
             break;
                          
           case SDLK_HASH:
             Letra[e] ='#';
             break;
                          
           case SDLK_QUESTION:
             Letra[e] ='?';
             break;
                         
           case SDLK_COLON:
             Letra[e] =':';
             break;
                         
           case SDLK_EQUALS:
             Letra[e] ='=';
             break;
                             
           case SDLK_GREATER:
             Letra[e] ='>';
             break;
                        
           case SDLK_LEFTBRACKET:
             Letra[e] ='[';
             break;
                            
           case SDLK_RIGHTBRACKET:
             Letra[e] =']';
             break;
                        
           case SDLK_LESS:
             Letra[e] ='<';
             break;
                            
           case SDLK_RETURN:
             Letra[e] = (char) 10;
             return Letra[e];
             break;
                 
           default:
             return 0;
         }
      return Letra[e];
    }
   return 0;
}

void
get_config()
{
  FP = fopen("/usr/local/etc/events/bufa", "a+");
  fgets(BOARDFILE[1], 22, FP);
  fclose(FP);
 
  FP = fopen("/usr/local/etc/events/bufb", "a+");
  fgets(BOARDFILE[2], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufc", "a+");
  fgets(BOARDFILE[3], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufd", "a+");
  fgets(BOARDFILE[4], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufe", "a+");
  fgets(BOARDFILE[5], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/buff", "a+");
  fgets(BOARDFILE[6], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufg", "a+");
  fgets(BOARDFILE[7], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufh", "a+");
  fgets(BOARDFILE[8], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufi", "a+");
  fgets(BOARDFILE[9], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufj", "a+");
  fgets(BOARDFILE[10], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufk", "a+");
  fgets(BOARDFILE[11], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufl", "a+");
  fgets(BOARDFILE[12], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufm", "a+");
  fgets(BOARDFILE[13], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufn", "a+");
  fgets(BOARDFILE[14], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufo", "a+");
  fgets(BOARDFILE[15], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufp", "a+");
  fgets(BOARDFILE[16], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufq", "a+");
  fgets(BOARDFILE[17], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufr", "a+");
  fgets(BOARDFILE[18], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufs", "a+");
  fgets(BOARDFILE[19], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/buft", "a+");
  fgets(BOARDFILE[20], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufu", "a+");
  fgets(BOARDFILE[21], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufv", "a+");
  fgets(BOARDFILE[22], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufw", "a+");
  fgets(BOARDFILE[23], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufx", "a+");
  fgets(BOARDFILE[24], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufy", "a+");
  fgets(BOARDFILE[25], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufz", "a+");
  fgets(BOARDFILE[26], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufaa", "a+");
  fgets(BOARDFILE[27], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufab", "a+");
  fgets(BOARDFILE[28], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufac", "a+");
  fgets(BOARDFILE[29], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufad", "a+");
  fgets(BOARDFILE[30], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufae", "a+");
  fgets(BOARDFILE[31], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufaf", "a+");
  fgets(BOARDFILE[32], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufag", "a+");
  fgets(BOARDFILE[33], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufah", "a+");
  fgets(BOARDFILE[34], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufai", "a+");
  fgets(BOARDFILE[35], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufaj", "a+");
  fgets(BOARDFILE[36], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufak", "a+");
  fgets(BOARDFILE[37], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufal", "a+");
  fgets(BOARDFILE[38], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufam", "a+");
  fgets(BOARDFILE[39], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufan", "a+");
  fgets(BOARDFILE[40], 22, FP);
  fclose(FP);

  return;
} 

int
get_digit()
{
   SDL_Event event;           
   event.type = SDL_KEYUP;
   while(1)
      {
        SDL_WaitEvent(&event);    
        if (event.type == SDL_KEYDOWN)
          {
            switch(event.key.keysym.sym)
               {
                  case SDLK_0:
                  Digit = 0;  
                  break;
                
                  case SDLK_1:
                  Digit = 1;  
                  break;
                
                  case SDLK_2:
                  Digit = 2;  
                  break;
                 
                  case SDLK_3:
                  Digit = 3;  
                  break;
                  
                  case SDLK_4:
                  Digit = 4;  
                  break;
                  
                  case SDLK_5:
                  Digit = 5;  
                  break;
                  
                  case SDLK_6:
                  Digit = 6;  
                  break;
                  
                  case SDLK_7:
                  Digit = 7;  
                  break;
                   
                  case SDLK_8:
                  Digit = 8;  
                  break;
                   
                  case SDLK_9:
                  Digit = 9;  
                  break;
                  
                  case SDLK_RETURN:
                  Digit = 10;
                  return Digit;
                  break;
                 
                  default:
                  Digit = 10;
                  return Digit;
               }
            return Digit;
         }
    } 
}

int
get_event(int e)
{
  Ltr = 0;
  Value = 0;
  Code = 0;
 
  read(Board_file_descriptor[e], ev, sizeof(struct input_event));
 
  if (EV_KEY == ev[0].type)
    {
       Value = ev[0].value;
       Code = ev[0].code;
       ev[0].code = 0;
   
       if((Code == 42) || (Code == 54))
         {
           if (Value > 0) Upper_case[e] = 1;
           else Upper_case[e] = 0;
           return Ltr;
         } 
    
       if(Value == 0 || Value == 2)
         {
           Make_or_break[0] = 0; 
           return 0;
         }
          
       if (Upper_case[e] == 0) Ltr = LC[Code];
       else Ltr = UC[Code];
    }
   return Ltr;
}

void
get_file()
{ 
  int incoming = 0;
  int char_nmbr;
  
                     /* ADDRESS THE CURRENT CLASS. */
  if (Class == 1) strcpy (Buf, "/usr/local/com/class1/files/file");
  if (Class == 2) strcpy (Buf, "/usr/local/com/class2/files/file");
  if (Class == 3) strcpy (Buf, "/usr/local/com/class3/files/file");
  if (Class == 4) strcpy (Buf, "/usr/local/com/class4/files/file");
  if (Class == 5) strcpy (Buf, "/usr/local/com/class5/files/file");
  if (Class == 6) strcpy (Buf, "/usr/local/com/class6/files/file");
  if (Class == 7) strcpy (Buf, "/usr/local/com/class7/files/file");
  if (Class == 8) strcpy (Buf, "/usr/local/com/class8/files/file");
  if (Class == 9) strcpy (Buf, "/usr/local/com/class9/files/file");

                /* OPEN INDIVIDUAL STUDENT FILES. */
  for (e = 1;  e < 41; e++)
    {
      if(e < 10)
        {
          Buf[32] = 48;
          Buf[33] = e + 48;
        }
      else
        {
          Buf[32] = (e / 10) + 48;
          Buf[33] = (e % 10) + 48;
        }

      FP = fopen(Buf,"a+");
      for(char_nmbr = 0; char_nmbr < 5000; char_nmbr++)
        {
          incoming = fgetc(FP);
          if(incoming == -1) break;
          Comp[e][char_nmbr] = (char) incoming;
        }
      fclose(FP);
    }
  return;
}

  /* THIS ROUTINE READS THE STUDENT NAMES FROM THE HARD DRIVE AND PLACES
     THEM IN THE ARRAY Roster[][]. */
void
get_names()
{
                    /*  ADDRESS THE CURRENT CLASS. */ 
  if (Class == 1) strcpy (Buf, "/usr/local/com/class1/names/name");
  if (Class == 2) strcpy (Buf, "/usr/local/com/class2/names/name");
  if (Class == 3) strcpy (Buf, "/usr/local/com/class3/names/name");
  if (Class == 4) strcpy (Buf, "/usr/local/com/class4/names/name");
  if (Class == 5) strcpy (Buf, "/usr/local/com/class5/names/name");
  if (Class == 6) strcpy (Buf, "/usr/local/com/class6/names/name");
  if (Class == 7) strcpy (Buf, "/usr/local/com/class7/names/name");
  if (Class == 8) strcpy (Buf, "/usr/local/com/class8/names/name");
  if (Class == 9) strcpy (Buf, "/usr/local/com/class9/names/name");

           /*  GET INDIVIDUAL STUDENT NAMES. */
  for (e = 1;  e < 41; e++)
    {
      if(e < 10)
        {
          Buf[32] = 48; 
          Buf[33] = e + 48;
        }
      else
        {
          Buf[32] = (e / 10) + 48;
          Buf[33] = (e % 10) + 48;
        }

      FP = fopen(Buf, "a+");
      strcpy (Roster[e], Dumm);  // Clear Roster[e].
      fgets (Roster[e], 10, FP);
      fclose(FP);

      strcpy (Rostera[e], Dumm);  // Clear Rostera[e].
      strcpy(Rostera[e], Roster[e]);
      strcat(Rostera[e],"!");
    }
  return;
}

void
get_nine()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

  struct SDL_Rect tile[1000];
  k = 0;
  int b = 0;
  for (c = 0;  c < 17; c++)
    {
      for (b = 0; b < 45;  b++)
        {
          tile[b+c*45].x = (b * 30);
          tile[b+c*45].y = c * 44;
        }
    }
    
  scratch = TTF_RenderText_Solid(fnt50, "ENTER 9 DIGITS (No zeros)", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[230]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
        
  int a = 325;
  int letter = 0;

  while(k < 9)
    {
      while(1)
        {
          letter = get_digit();
          if((letter > 0) && (letter < 10)) break;
        }    
        
      Factor[k] = (char) letter+48;
      Factor[k+1] = 0;

      scratch = TTF_RenderText_Solid(fnt50, &Factor[k], red);
      SDL_BlitSurface(scratch, NULL, glass, &tile[a]);
      SDL_FreeSurface(scratch);
      if (a < 675) {a++;}
          
      SDL_UpdateWindowSurface(windowframe);
      k++;
    }
  return;
}

int
handicap()
{
  int a = 0;
  int Operation = 0;
  e = select_board();
  if(Panic == 1) return 0;

  a = handicapenterpage();
  Operation = pick_operation();
  
  if (Operation == 1) Addpoints[e] = a ;
  if (Operation == 2) Subpoints[e] = a;
  if (Operation == 3) Mulpoints[e] = a;
  if (Operation == 4) Divpoints[e] = a;

  for (e = 1;  e < 41;  e++){ Hndicap[e] = (char)(Addpoints[e] + 48);}

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Addpoints", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Addpoints", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Addpoints", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Addpoints", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Addpoints", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Addpoints", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Addpoints", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Addpoints", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Addpoints", "w");

  fputs(Hndicap, FP);
  fclose(FP);

  for (e = 1;  e < 41;  e++) Hndicap[e] = (char)(Subpoints[e] + 48);

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Subpoints", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Subpoints", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Subpoints", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Subpoints", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Subpoints", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Subpoints", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Subpoints", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Subpoints", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Subpoints", "w");

  fputs(Hndicap, FP);
  fclose(FP);
  
  for (e = 1;  e < 41;  e++) Hndicap[e] = (char)(Mulpoints[e] + 48);

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Mulpoints", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Mulpoints", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Mulpoints", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Mulpoints", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Mulpoints", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Mulpoints", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Mulpoints", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Mulpoints", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Mulpoints", "w");

  fputs(Hndicap, FP);
  fclose(FP);
  
  for (e = 1;  e < 41;  e++) Hndicap[e] = (char)(Divpoints[e] + 48);

  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Divpoints", "w");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Divpoints", "w");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Divpoints", "w");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Divpoints", "w");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Divpoints", "w");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Divpoints", "w");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Divpoints", "w");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Divpoints", "w");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Divpoints", "w");

  fputs(Hndicap, FP);
  fclose(FP);
  return 0;
}

int
handicapenterpage()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

  struct SDL_Rect slots[3];
    
  slots[0].x = 0;
  slots[0].y = 295;
    
  slots[1].x = 0;
  slots[1].y = 395;
    
  scratch = TTF_RenderText_Solid(fnt50, "          ENTER HANDICAP", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
  
  int letter = 0;
  while(1)
    {
      letter = get_digit() + 48;
      if((letter > 48) && (letter < 58)) break;
    }    
                   
  char proxy[2];
  proxy[0] =  letter;
  letter = letter - 48;
  proxy[1] = 0;
  
  scratch = TTF_RenderText_Solid(fnt50, proxy, black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);           

  return (letter);
}
  
int
pick_operation()
{  
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
  
  struct SDL_Rect slots[6];
    
  slots[0].x = 0;
  slots[0].y = 200;
    
  slots[1].x = 0;
  slots[1].y = 320;
    
  slots[2].x = 0;
  slots[2].y = 380;
    
  slots[3].x = 0;
  slots[3].y = 440;
     
  slots[4].x = 0;
  slots[4].y = 500;
        
  slots[5].x = 0;
  slots[5].y = 560;
        
  scratch  = TTF_RenderText_Solid(fnt50, "      ARE WE TALKING:", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
             
  scratch = TTF_RenderText_Solid(fnt50, "            1 - ADDITION?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
            
  scratch = TTF_RenderText_Solid(fnt50, "            2 - SUBTRACTION?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[2]);
  SDL_FreeSurface(scratch);
             
  scratch = TTF_RenderText_Solid(fnt50, "            3 - MULTIPLICATION?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[3]);
  SDL_FreeSurface(scratch);
            
  scratch = TTF_RenderText_Solid(fnt50, "            4 - DIVISION?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[4]);
  SDL_FreeSurface(scratch);
           
  SDL_UpdateWindowSurface(windowframe); 
        
  while(1)
    {
      Option = get_digit();
      if((Option > 0) && (Option < 5)) break;
    }    
   return Option;
}

int
main(int ARGC, char *ARGV[])
{
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();   
  
  atexit(TTF_Quit);
  atexit(SDL_Quit);
   
  fnt15 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 15);
  fnt20 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 20);
  fnt30 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 30);
  fnt33 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 33);
  fnt35 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 35);
  fnt40 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 40);
  fnt45 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 45);
  fnt50 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 50 );
  fnt90 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 90);
  fnt110 = TTF_OpenFont( "/usr/local/etc/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 110);
  
  windowframe = SDL_CreateWindow("",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  glass = SDL_GetWindowSurface(windowframe);
  rends = SDL_CreateSoftwareRenderer(glass);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  
  int c;
  
  for(a = 1; a < 41;)
    {
      hue[a].r = 0;
      hue[a].g = 0;
      hue[a].b = 0;
           
      hue[a+1].r = 0;
      hue[a+1].g = 0;
      hue[a+1].b = 255;
            
      hue[a+2].r = 0;
      hue[a+2].g = 125;
      hue[a+2].b = 0;
           
      hue[a+3].r = 255;
      hue[a+3].g = 0;
      hue[a+3].b = 0;
           
      hue[a+4].r = 0;
      hue[a+4].g = 0;
      hue[a+4].b = 0;
           
      a = a + 5;
    }
          
  for (e = 1;  e < 6; e++)
    {
      c = e-1;
      for (b = 0; b < 20; b++)
        {
          tile[e][b].x = (b * 24) + 5;
          tile[e][b].y = (c * 46);  // y runs from 0 to 180
                
          tilem[e][b].x = (b * 24) + 5;
          tilem[e][b].y = (c * 46);  // y runs from 0 to 180
        }
          line[e].x = 5 ;
          line[e].y = (c * 46);  // y runs from 0 to 180
          line[e].w = 539;

          tagline[e].x = 125 ;
          tagline[e].y = (c * 46);  // y runs from 0 to 180
          tagline[e].w = 400;
    }
           
  for (e = 6;  e < 11; e++)
    {
      c = e-1;
      for (b = 0; b < 21; b++)
         {
           tile[e][b].x = (b * 24) + 5;  // x's run from 5 to 605
           tile[e][b].y = (c * 46) + 39;  // y runs from 271 to 451
             
           tilem[e][b].x = (b * 24) + 5;  // x's run from 5 to 605
           tilem[e][b].y = (c * 46) + 39;  // y runs from 280 to 451
         }
      line[e].x = 5;
      line[e].y = (c * 46) + 39;  //y runs from 280 to 451
      line[e].w = 539;

      tagline[e].x = 125;
      tagline[e].y = (c * 46) + 39;  //y runs from 280 to 451
      tagline[e].w = 400;
    }
         
  for (e = 11;  e <16; e++)
    {
      c = e-1;
      for (b = 0; b < 21; b++)
        {
          tile[e][b].x = (b * 24) + 5;  // x's run from 5 to 605
          tile[e][b].y = (c * 46) + 78;  // y runs from 542 to 722
               
          tilem[e][b].x = (b * 24) + 5;  // x's run from 5 to 605
          tilem[e][b].y = (c * 46) + 78;  // y runs from 542 to 722
        }
      line[e].x = 5;
      line[e].y = (c * 46) + 78;  // y runs from 542 to 722
      line[e].w = 539;

      tagline[e].x = 125;
      tagline[e].y = (c * 46) + 78;  // y runs from 542 to 722
      tagline[e].w = 400;
    }
                
  for (e = 16;  e < 21; e++)
    {
      c = e-1;
      for (b = 0; b < 21; b++)
        {
          tile[e][b].x = (b * 24) + 540; // x's run from 540
          tile[e][b].y = ((c - 15) * 46);
               
          tilem[e][b].x = (b * 24) + 540; // x's run from 540
          tilem[e][b].y = ((c - 15) * 46);
        }
      line[e].x = 540; // x runs from 540 to 1085
      line[e].y = ((c - 15) * 46);
      line[e].w = 535;

      tagline[e].x = 660; // x runs from 540 to 1085
      tagline[e].y = ((c - 15) * 46);
      tagline[e].w = 400;
    }
              
  for (e = 21;  e < 26; e++)
    {
      c = e-1;
      for (b = 0; b < 21; b++)
        {
          tile[e][b].x = (b * 24) + 540;  // x's run from 540 to 1015
          tile[e][b].y = ((c - 15) * 46) + 39;
           
          tilem[e][b].x = (b * 24) + 540;  // x's run from 540 to 1015
          tilem[e][b].y = ((c - 15) * 46) + 39;
        }
      line[e].x = 540;  // x runs from 540 to 1205
      line[e].y = ((c - 15) * 46) + 39;
      line[e].w = 535;

      tagline[e].x = 660;  // x runs from 540 to 1205
      tagline[e].y = ((c - 15) * 46) + 39;
      tagline[e].w = 400;
    }
        
  for (e = 26;  e < 31; e++)
    {
      c = e-1;
      for (b = 0; b < 21; b++)
        {
          tile[e][b].x = (b * 24) + 540;
          tile[e][b].y = ((c - 15) * 46) + 78;

          tilem[e][b].x = (b * 24) + 540;
          tilem[e][b].y = ((c - 15) * 46) + 78;
        }
      line[e].x = 540;
      line[e].y = ((c - 15) * 46) + 78;
      line[e].w = 535;

      tagline[e].x = 660;
      tagline[e].y = ((c - 15) * 46) + 78;
      tagline[e].w = 400;
    }

  int op = 0;
  int a = 0;
  int b = 0; 
  
  chooseclass:
  
  /*  These download the lists of numbers from which the program 
      randomly draws to create the math problems.  */
        
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/A_terma", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/A_terma", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/A_terma", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/A_terma", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/A_terma", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/A_terma", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/A_terma", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/A_terma", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/A_terma", "a+");
  fgets(A_terma_buffer, 10, FP);
  fclose(FP);
  strcpy(A_terma, A_terma_buffer);
                
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/A_termb", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/A_termb", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/A_termb", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/A_termb", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/A_termb", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/A_termb", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/A_termb", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/A_termb", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/A_termb", "a+");
  fgets(A_termb_buffer, 10, FP);
  fclose(FP);
  strcpy(A_termb, A_termb_buffer);

  /*  These download the lists of numbers from which the program 
      randomly draws to create the math problems.  */
        
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/D_terma", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/D_terma", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/D_terma", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/D_terma", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/D_terma", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/D_terma", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/D_terma", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/D_terma", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/D_terma", "a+");
  fgets(D_terma_buffer, 10, FP);
  fclose(FP);
  strcpy(D_terma, D_terma_buffer);
                
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/D_termb", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/D_termb", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/D_termb", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/D_termb", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/D_termb", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/D_termb", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/D_termb", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/D_termb", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/D_termb", "a+");
  fgets(D_termb_buffer, 10, FP);
  fclose(FP);
  strcpy(D_termb, D_termb_buffer);

  /*  These download the lists of numbers from which the program 
      randomly draws to create the math problems.  */
        
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/M_terma", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/M_terma", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/M_terma", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/M_terma", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/M_terma", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/M_terma", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/M_terma", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/M_terma", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/M_terma", "a+");
  fgets(M_terma_buffer, 10, FP);
  fclose(FP);
  strcpy(M_terma, M_terma_buffer);
                
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/M_termb", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/M_termb", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/M_termb", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/M_termb", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/M_termb", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/M_termb", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/M_termb", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/M_termb", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/M_termb", "a+");
  fgets(M_termb_buffer, 10, FP);
  fclose(FP);
  strcpy(M_termb, M_termb_buffer);

  /*  These download the lists of numbers from which the program 
      randomly draws to create the math problems.  */
        
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/S_terma", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/S_terma", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/S_terma", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/S_terma", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/S_terma", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/S_terma", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/S_terma", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/S_terma", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/S_terma", "a+");
  fgets(S_terma_buffer, 10, FP);
  fclose(FP);
  strcpy(S_terma, S_terma_buffer);
                
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/S_termb", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/S_termb", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/S_termb", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/S_termb", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/S_termb", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/S_termb", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/S_termb", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/S_termb", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/S_termb", "a+");
  fgets(S_termb_buffer, 10, FP);
  fclose(FP);
  strcpy(S_termb, S_termb_buffer);

  /*  Download name of the teacher for this class  */
  if (Class == 1) FP = fopen("/usr/local/com/class1/names/Teacher", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/names/Teacher", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/names/Teacher", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/names/Teacher", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/names/Teacher", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/names/Teacher", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/names/Teacher", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/names/Teacher", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/names/Teacher", "a+");
  fgets(Teacher, 15, FP);
  fclose(FP);
      
  /*  Get the score needed to win math games  */
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Win_score", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Win_score", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Win_score", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Win_score", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Win_score", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Win_score", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Win_score", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Win_score", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Win_score", "a+");
  fgets(Ww, 2, FP);
  fclose(FP);
      
  if(strlen(Ww) == 0)
    {
      Win_score = 90;
    }
  else
    {
      Win_score=(int)Ww[0];
    }
      
  /*  Download a list showing what typing practice line each student is
      working on.  */
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Type_linea", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Type_linea", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Type_linea", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Type_linea", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Type_linea", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Type_linea", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Type_linea", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Type_linea", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Type_linea", "a+");
  fgets(Type_linea, 42, FP);
  fclose(FP);
         
  /*  These sections download the handicaps for each kind of math 
      operation  */
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Addpoints", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Addpoints", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Addpoints", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Addpoints", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Addpoints", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Addpoints", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Addpoints", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Addpoints", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Addpoints", "a+"); 
  fgets(Hndicap, 42, FP);
  fclose(FP);
  for (e = 1;  e < 41;  e++)
    {
      Addpoints[e] = (int)(Hndicap[e]) - 48;
    }
         
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Subpoints", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Subpoints", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Subpoints", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Subpoints", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Subpoints", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Subpoints", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Subpoints", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Subpoints", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Subpoints", "a+");
  fgets(Hndicap, 42, FP);
  fclose(FP);
  for (e = 1;  e < 41;  e++)
    {
      Subpoints[e] = (int)(Hndicap[e]) - 48;
    }
        
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Mulpoints", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Mulpoints", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Mulpoints", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Mulpoints", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Mulpoints", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Mulpoints", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Mulpoints", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Mulpoints", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Mulpoints", "a+");
  fgets(Hndicap, 42, FP);
  fclose(FP);
  for (e = 1;  e < 41;  e++)
    {
      Mulpoints[e] = (int)(Hndicap[e]) - 48;
    }
          
  if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Divpoints", "a+");
  if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Divpoints", "a+");
  if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Divpoints", "a+");
  if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Divpoints", "a+");
  if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Divpoints", "a+");
  if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Divpoints", "a+");
  if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Divpoints", "a+");
  if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Divpoints", "a+");
  if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Divpoints", "a+");
  fgets(Hndicap, 42, FP);
  fclose(FP);
  for (e = 1;  e < 41;  e++)
    {
      Divpoints[e] = (int)(Hndicap[e]) - 48;
    }
         
 /*  These change the numbers which the program randomly draws to
      create math problems from ASCII to binary.  */
  for (a = 0; a < 9; a++)
    {
       At1[a] = (int)(A_terma[a]) - 48;
       At2[a] = (int)(A_termb[a]) - 48;
       Dt1[a] = (int)(D_terma[a]) - 48;
       Dt2[a] = (int)(D_termb[a]) - 48;
       Mt1[a] = (int)(M_terma[a]) - 48;
       Mt2[a] = (int)(M_termb[a]) - 48;
       St1[a] = (int)(S_terma[a]) - 48;
       St2[a] = (int)(S_termb[a]) - 48;
    }
        
  /*  Convert ASCII representation of typing line numbers to binary.  */
  for (e = 1; e < 41; e++)
    {
      Tline[e] = ((int)(Type_linea[e]) - 48);
      Tlineb[e] = ((int)(Type_lineb[e]) - 48);
      
      /*  If bit 7 is set, calculate line number from negative value.  */
      if (Tline[e] < 0)
        {
          Tline[e] = 256 + Tline[e];
          Tlineb[e] = 256 + Tlineb[e];
        }
    }
        
  /*  Eliminate Division by Zero  */
  for (a = 0; a < 9; a++)
    {
      if (Dt1[a] == 0) Dt1[a] = 1;
      if (Dt2[a] == 0) Dt2[a] = 1;
    }
        
  /*  Clear Word Processor Files  */
  for (e = 1; e < 41; e++)
    {
      for (b = 0; b < 5000; b++) Comp[e][b] = 0;
    }
  b = 0;
    
  /*  Set Initial Scores to 0  */
  for (e = 1; e < 42; e++)
    {
      Score[e] = 0;
    }
     
  /*  Recover Files From Hard Drive  */
  get_file();
  get_names();
  get_config();
    
  /* OPEN KEYBOARD FILES AND CREATE FILE DESCRIPTORS FOR EACH.  */  
  if (op == 0)
    {
      opener();
      op = 1;
    }
  
  /* INITIALIZE THE TIMEOUT DATA STRUCTURE. */
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
          
  splash_page();
           
  while (1)
    {
      if (Changeclass == 1)
        {
          Changeclass = 0;
          Class = Classnumber;
          goto chooseclass;
        }
      menu();
    }
  return 0;
}
        
void
make_add_prob()
{ 
  Advance[e] = 0;
  Stuans[e] = 0;
  Addend1[e] = At1[random_num()];
  Addend2[e] = At2[random_num()];
  Answer[e] = Addend1[e] + Addend2[e]; 
  return;
}
         
void
make_div_prob()
{
  int a = random_num();
  int b = random_num();
  Advance[e] = 0;
  Dividend[e] = Dt1[a] * Dt2[b];
  Divisor[e] = Dt1[a];
  Answer[e] = Dt2[b];
  return;
}
      
void
make_mul_prob()
{
  int a = random_num();
  int b = random_num();
  Advance[e] = 0;
  Answer[e] = Mt1[a] * Mt2[b];
  Factor1[e] = Mt1[a];
  Factor2[e] = Mt2[b];
  Stuans[e] = 0;
  return;
}

void
make_sub_prob()
{ 
  int a = random_num();
  int b = random_num();
  Advance[e] = 0;
  Subtrahend[e] = St1[a] + St2[b];
  Minuend[e] = St1[a];
  Stuans[e] = 0;
  Answer[e] = St2[b];
  return;
}
 
int
menu()
{
  starthere:
  Panic = 0;
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
     
  SDL_Event event;
      
  struct SDL_Rect tile[26];
       
  tile[0].x = 0;  /* teacher */
  tile[0].y = 0;
    
  tile[1].x = 130; /* math */
  tile[1].y = 100;
    
  tile[2].x = 380; /* division */
  tile[2].y = 40;
      
  tile[3].x = 380;  /* multiplication */
  tile[3].y = 70;
   
  tile[4].x = 380;  /* subtraction */
  tile[4].y = 100;
    
  tile[5].x = 380;  /* addition */
  tile[5].y = 130;
    
  tile[6].x = 380;  /* check your math */
  tile[6].y = 160;
     
  tile[7].x = 0;  /* blank line */
  tile[7].y = 190;
    
  tile[8].x = 75; /* language */
  tile[8].y = 265;
    
  tile[9].x = 380;  /* keyboarding */
  tile[9].y = 250;
    
  tile[10].x = 380;  /* word processing */
  tile[10].y = 280;
    
  tile[11].x = 100;  /* blank line */
  tile[11].y = 280;
      
  tile[12].x = 50;  /* management */  
  tile[12].y = 480;
    
  tile[13].x = 380;  /* math handicaps */
  tile[13].y = 340;
    
  tile[14].x = 380;  /* win score */
  tile[14].y = 370;
    
  tile[15].x = 380;  /* first factor */
  tile[15].y = 400;
    
  tile[16].x = 380;  /* second factor */
  tile[16].y = 430;
    
  tile[17].x = 380;  /* one typing line */
  tile[17].y = 460;
    
  tile[18].x = 380;  /* all typing lines */
  tile[18].y = 490;
    
  tile[19].x = 380;  /* erase all writing */
  tile[19].y = 520;
    
  tile[20].x = 380;  /* print one */
  tile[20].y = 550;
     
  tile[21].x = 380;  /* print all */
  tile[21].y = 580;
     
  tile[22].x = 380;  /* read roster */
  tile[22].y = 610;
     
  tile[23].x = 380;  /* change roster */
  tile[23].y = 640;
     
  tile[24].x = 380;  /* change classname */
  tile[24].y = 670;
     
  tile[25].x = 380;  /* pick a practice line */
  tile[25].y = 700;
     
  scratch = TTF_RenderText_Solid(fnt30, Teacher, red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[0]);
  SDL_FreeSurface(scratch);
 
  scratch = TTF_RenderText_Solid(fnt30, "MATH", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[1]);
  SDL_FreeSurface(scratch);
   
  scratch = TTF_RenderText_Solid(fnt30, "A - Division", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[2]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "B - Multiplication", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[3]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "C - Subtraction", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[4]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "D - Addition", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[5]);
  SDL_FreeSurface(scratch);
   
  scratch = TTF_RenderText_Solid(fnt30, "E - Check Your Math", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[6]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[7]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "LANGUAGE", green);
  SDL_BlitSurface(scratch, NULL, glass, &tile[8]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "F - Keyboarding", green);
  SDL_BlitSurface(scratch, NULL, glass, &tile[9]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "G - Word Processing", green);
  SDL_BlitSurface(scratch, NULL, glass, &tile[10]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "", red);
  SDL_BlitSurface(scratch, NULL, glass, &tile[11]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "MANAGEMENT", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[12]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "H - Math Facts Handicaps", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[13]);
  SDL_FreeSurface(scratch);
   
  scratch = TTF_RenderText_Solid(fnt30, "I - Set Winning Score", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[14]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "J - First Multiplication Factor", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[15]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "K - Second Multiplication Factor", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[16]);
  SDL_FreeSurface(scratch);
  
  scratch = TTF_RenderText_Solid(fnt30, "L - Set One Student's Typing Line", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[17]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "M - Set Back All Typing Lines", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[18]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "N - Erase All Student Writing", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[19]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "O - Print One Student's Writing", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[20]);
  SDL_FreeSurface(scratch);
   
  scratch = TTF_RenderText_Solid(fnt30, "P - Print All Students' Writing", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[21]);
  SDL_FreeSurface(scratch);
   
  scratch = TTF_RenderText_Solid(fnt30, "Q - Read Roster", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[22]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt30, "R - Change Roster", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[23]);
  SDL_FreeSurface(scratch);
         
  scratch = TTF_RenderText_Solid(fnt30, "S - Change Classname", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[24]);
  SDL_FreeSurface(scratch);
         
  scratch = TTF_RenderText_Solid(fnt30, "T - Choose Practice Line", black);
  SDL_BlitSurface(scratch, NULL, glass, &tile[25]);
  SDL_FreeSurface(scratch);
         
  SDL_UpdateWindowSurface(windowframe);
    
  while(1)
    {
      SDL_WaitEvent(&event);
      if (event.type == SDL_KEYDOWN)
        {
          switch (event.key.keysym.sym)
           {
             /*  DIVISION  */
             case SDLK_a:
                Repeat = 0;
                Goaround = 1;                   
                while(1)
                  {
                    if(Goaround == 1)
                      {
                        dump();
                        divide();
                      }
                    else break;
                  }
                goto starthere;
                break;
               
             /*  MULTIPLICATION  */
             case SDLK_b:
                Repeat = 0;
                Goaround = 1;
                while(1)
                   {
                     if(Goaround == 1)
                       {
                         dump();
                         multiply();
                       }
                     else break;
                   }
                goto starthere;
                break;         
                
             /*  SUBTRACTION  */
             case SDLK_c:
                Repeat = 0;
                Goaround = 1;
                while(1)
                   {
                     if(Goaround == 1)
                       {
                         dump();
                         subtract();
                       }
                     else break;
                   }
                goto starthere;
                break;
          
             /*  ADDITION  */
             case SDLK_d:
                Repeat = 0;
                Goaround = 1;
                while(1)
                  {
                    if(Goaround == 1)
                      {
                        dump();
                        add();
                      }
                    else break;
                  }
                 goto starthere;
                break;
                
             /*  CHECK YOUR MATH  */
             case SDLK_e:
                Repeat = 0;
                select_book();
                dump();
                check();
                goto starthere;
                break;
                
             /*  KEYBOARDING  */
             case SDLK_f:
                Repeat = 0;
                timeit();
                dump();
                boarda();
                bail();
                goto starthere;
                break;
                
             /*  WORD PROCESSOR  */
             case SDLK_g:
                Repeat = 0;
                timeit();
                dump();
                process();
                file_save();
                goto starthere;
                break;
                
             /*  SET HANDICAPS  */
             case SDLK_h:
                Repeat = 0;
                security();
                if (Panic == 1) goto starthere;
                handicap();
                goto starthere;
                break;
                
             /*  SET WINNING SCORE  */
             case SDLK_i:
                Repeat = 0;
                if (Panic == 1) goto starthere;
                fetch_win_score();
                goto starthere;
                break;

             /*  SET FIRST FACTORS  */
             case SDLK_j:
                Repeat = 0;
                if (Panic == 1) goto starthere;
               	Option = pick_operation();
                get_nine();

                if (Option == 1) factor1_A();
                if (Option == 2) factor1_S();
                if (Option == 3) factor1_M();
                if (Option == 4) factor1_D();

               	detente();
                goto starthere;
                break;

             /*  SET SECOND FACTORS  */
             case SDLK_k:
                Repeat = 0;
               	if (Panic == 1) goto starthere;
                Option = pick_operation();

                get_nine();

                if (Option == 1) factor2_A();
                if (Option == 2) factor2_S();
                if (Option == 3) factor2_M();
                if (Option == 4) factor2_D();

                detente();
                goto starthere;
                break;
                
             /*  CHANGE ONE STUDENT'S TYPING LINE  */
             case SDLK_l:
                Repeat = 0;
                select_line_page();
                bail();
                goto starthere;
                break;
                 
             /*  DECREMENT ALL STUDENTS' TYPING LINES  */
             case SDLK_m:
                if (Repeat == 0)
                  {
                    security();
                    if (Panic == 1) goto starthere;
                  }
                Repeat = 1;
                for (e=1;  e<=30; e++)
                  {
                    if (Tline[e] > 1)  Tline[e]=Tline[e] - 1;
                  }      
                bail();
                goto starthere;
                break;
                
             /*  ERASE STUDENT WRITING  */
             case SDLK_n:
                Repeat = 0;
                security();
                if (Panic == 1) goto starthere;
                clear_pro();
                goto starthere;
                break;
                
             /*  PRINT ONE STUDENT'S WRITING  */
             case SDLK_o:
                Length_flag = 0;
                Repeat = 0;
                e=select_board();
                if (Panic == 1) goto starthere;
                print_pro();
                goto starthere;
                break;
                 
             /*  PRINT ALL STUDENTS' WRITING  */
             case SDLK_p:
                Length_flag = 1;
                Repeat = 0;
                for (e = 1;  e <= 30;  e++)
                  {
                     b = 66;
                     print_pro();
                     if (Panic == 1) goto starthere;
                  }
                goto starthere;
                break;
                
             /*  READ ROSTER  */
             case SDLK_q:
                Repeat = 0;
                roster_page();
                goto starthere;
                break; 
                
             /*  CHANGE ROSTER  */
             case SDLK_r:
                Repeat = 0;
                security();
                if (Panic == 1) goto starthere;
                changepage();
              
             for (e = 1; e < 41; e++)
               {
                 strcpy (Buf, "");
                 if (Class == 1) strcpy (Buf, "/usr/local/com/class1/names/name");
                 if (Class == 2) strcpy (Buf, "/usr/local/com/class2/names/name");
                 if (Class == 3) strcpy (Buf, "/usr/local/com/class3/names/name");
                 if (Class == 4) strcpy (Buf, "/usr/local/com/class4/names/name");
                 if (Class == 5) strcpy (Buf, "/usr/local/com/class5/names/name");
                 if (Class == 6) strcpy (Buf, "/usr/local/com/class6/names/name");
                 if (Class == 7) strcpy (Buf, "/usr/local/com/class7/names/name");
                 if (Class == 8) strcpy (Buf, "/usr/local/com/class8/names/name");
                 if (Class == 9) strcpy (Buf, "/usr/local/com/class9/names/name");
                 
                 if(e < 10)
                   {
                     Buf[32] = 48;
                     Buf[33] = e + 48;
                   }
                 else
                   {
                     Buf[32] = (e /10) + 48;
                     Buf[33] = (e % 10) + 48;
                   }

                 FP = fopen (Buf, "w");
                 fputs (Roster[e], FP);
                 fclose (FP);

                 strncpy(Rostera[e], &Dum[0], 20);  // Clear Rostera[e].
                 strncpy(Rostera[e], Roster[e], 20);  // Copy names.
                 strcat(Rostera[e],"!");
               }
                 
             goto starthere;
             break;
                
             /*  SET TEACHER'S NAME  */
             case SDLK_s:
                Repeat = 0;
                security();
                if (Panic == 1) goto starthere;
                fetch_letter();
                goto starthere;
                break;
                          
             /*  CHOOSE PRACTICE LINE  */
             case SDLK_t:
                Repeat = 0;
                Presentation = 1;
                choose_line();
                if (Panic == 1) goto starthere;
                goto starthere;
                break;
                          
             case SDLK_1:
                Classnumber = 1;
                Changeclass = 1;
                return Classnumber;
                break;
                
             case SDLK_2:
                Classnumber = 2;
                Changeclass = 1;
                return Classnumber;
                break;
                  
             case SDLK_3:
                Classnumber = 3;
                Changeclass = 1;
                return Classnumber;
                break;
                
             case SDLK_4:
                Classnumber = 4;
                Changeclass = 1;
                return Classnumber;
                break;
                
             case SDLK_5:
                Classnumber = 5;
                Changeclass = 1;
                return Classnumber;
                break;
                
             case SDLK_6:
                Classnumber = 6;
                Changeclass = 1;
                return Classnumber;
                break;
                
             case SDLK_7:
                Classnumber = 7;
                Changeclass = 1;
                return Classnumber;
                break;
                
             case SDLK_8:
                Classnumber = 8;
                Changeclass = 1;
                return Classnumber;
                break;
                
             case SDLK_9:
                Classnumber = 9;
                Changeclass = 1;
                return Classnumber;
                break;
                
             case SDLK_ESCAPE:
                double_check();
                goto starthere;
                break;
                
             default:
                ;
           }
        }
    }
  return 0;
}

void
multiply()
{
  char a;
                      /* OPEN EMPTY WHITE WINDOW. */
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
      
                      /* PUT 30 PROBLEMS ON THE SCREEN. */   
  for (e = 1; e <= 30; e++)
    {
      Stop[e] = 0;
      Goofed[e] = 0;
        
      for (b = 0; b < 20; b++) Last_line[e][b] = ' ';
      
      Score[e]=0;
      make_mul_prob(e);
      
      Last_line[e][2] = (char) Factor1[e]+48;
      Last_line[e][4] = 'X';    
      Last_line[e][6] = (char) Factor2[e]+48;    
      Last_line[e][8] = '=';    
       
      for (c = 0; c < 20; c++)
        {
           scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], hue[e]);
           SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
           SDL_FreeSurface(scratch);
        }
      Placeflag[e] = 1;
    }
     
  SDL_UpdateWindowSurface(windowframe);

                           /* START KEYBOARD POLLING. */
  while (1)
    {
      if (get_char() == 10)  /* CHECK TEACHER'S KEYBOARD FOR RETURN TO MENU. */
        {
          Last_line[e][16] = ' ';
          Last_line[e][17] = ' ';
          Goaround = 0;
          return;
        }

      for (e = 1; e <= 30; e++)  
        { 
          if (Goofed[e] == 1)  /* END OR CONTINUE ERROR TIMEOUT PENALTY. */
            {
              if (time(t) - Clk[e] > 1)
                {
                  Goofed[e] = 0;
                  Last_line[e][11] = ' ';
                  Last_line[e][12] = ' ';
                       
                  scratch = TTF_RenderText_Solid(fnt40, "XX", white);
                  SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                  SDL_FreeSurface(scratch);
                  SDL_UpdateWindowSurface(windowframe);
                }
                
              else
                {       
                  get_event(e);       
                  continue;
                } 
            }   
                
          if (Goofed[e] == 0)  /* GET STUDENT INPUT. */
            {
              a = (int) get_event(e);        
              if (a == 0) continue;
              if (a == 10) goto evaluatemulb;
              if (Stop[e] == 1) continue;
                            
              if (Placeflag[e] == 1)
                 {       
                   if (((int)a < 48) || ((int)a > 57))  continue;
                   Stuans[e] = (int) a - 48 ;
                   Z[0] = (int) a;  
                   Last_line[e][11] = (int) a;
                   Placeflag[e] = 2;
                      
                   scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                   SDL_FreeSurface(scratch);
                   SDL_UpdateWindowSurface(windowframe);
                       
                   continue;
                 }
                    
              if (Placeflag[e] == 2)
                 {          
                   if (((int)a < 48) || ((int)a > 57)) continue;
                   Z[0] = a;  
                   Last_line[e][12] = (int) a;
                   Stuans[e] = 10 * Stuans[e] + (a - 48);
                             
                   scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);             
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][12]);
                   SDL_FreeSurface(scratch);
                   SDL_UpdateWindowSurface(windowframe);
                       
                   Stop[e] = 1;
                   continue;
                 }       
               continue;
            }
          continue;
                     
          evaluatemulb:
          if (Stuans[e] == Answer[e])
            { 
              for (c = 0; c < 20; c++)
                {
                  scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], white);
                  SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
                  SDL_FreeSurface(scratch);
                }
              SDL_UpdateWindowSurface(windowframe);
                     
              Advance[e] = 1;
              Score[e] = Score[e] + Mulpoints[e];
              Ones_register = Score[e] % 10;
              Tens_register = Score[e] / 10;
              Last_line[e][11] = 32;
              Last_line[e][12] = 32;
              Last_line[e][16] = (char) Tens_register + 48;
              Last_line[e][17] = (char) Ones_register + 48;
              if(Last_line[e][16] == 48){Last_line[e][16] = 32;}
                  
              if(Score[e] < Win_score)
                {
                  Stop[e] = 0;
                  Placeflag[e] = 1;
                  make_mul_prob(e);
                      
                  Last_line[e][1] = ' ';
                  Last_line[e][2] = (char) Factor1[e]+48;
                  Last_line[e][4] = 'X';    
                  Last_line[e][6] = (char) Factor2[e]+48;    
                  Last_line[e][8] = '=';    
                   
                  for (c = 0; c < 20; c++)
                    {
                      scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], hue[e]);
                      SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
                      SDL_FreeSurface(scratch);
                    }
                  Placeflag[e] = 1;
                  SDL_UpdateWindowSurface(windowframe);
                }
                    
              else
                {
                  Last_line[e][16] = ' ';
                  Last_line[e][17] = ' ';
                  Operation = 3;
                  Goaround = 1;
                  win_page();
                  return;
                } 
            }
             
          else if(Stuans[e] != Answer[e])
            {
              Stop[e] = 0;
              Placeflag[e] = 1;
              Clk[e] = time(t);
              Goofed[e] = 1;
                  
              Z[0] = Last_line[e][11];
              scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
              SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                  
              Z[0] = Last_line[e][12];
              scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
              SDL_BlitSurface(scratch, NULL, glass, &tilem[e][12]);
                 
              Last_line[e][11] = 'X';
              Last_line[e][12] = 'X';
                
              scratch = TTF_RenderText_Solid(fnt40, "XX" , black);
              SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
              SDL_FreeSurface(scratch);
              SDL_UpdateWindowSurface(windowframe);
            }
        }
        
    }
  return;          
}

void
new_year()
{
  Panic = 0;
  security();
  if (Panic == 1) return;

  char type_line[41]="11111111111111111111111111111111111111111";
  char addpoints[41]="99999999999999999999999999999999999999999";
  char subpoints[41]="99999999999999999999999999999999999999999";
  char mulpoints[41]="99999999999999999999999999999999999999999";
  char divpoints[41]="99999999999999999999999999999999999999999";


  FP = fopen("/usr/local/com/class1/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class1/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class1/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class1/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class1/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class1/names/n* ");
  system ("rm -f /usr/local/com/class1/files/f* ");

  FP = fopen("/usr/local/com/class2/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class2/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class2/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class2/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class2/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class2/names/n* ");
  system ("rm -f /usr/local/com/class2/files/f* ");

  FP = fopen("/usr/local/com/class3/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class3/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class3/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class3/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class3/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class3/names/n* ");
  system ("rm -f /usr/local/com/class3/files/f* ");

  FP = fopen("/usr/local/com/class4/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class4/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class4/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class4/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class4/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class4/names/n* ");
  system ("rm -f /usr/local/com/class4/files/f* ");

  FP = fopen("/usr/local/com/class5/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class5/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class5/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class5/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class5/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class5/names/n* ");
  system ("rm -f /usr/local/com/class5/files/f* ");

  FP = fopen("/usr/local/com/class6/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class6/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class6/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class6/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class6/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class6/names/n* ");
  system ("rm -f /usr/local/com/class6/files/f* ");

  FP = fopen("/usr/local/com/class7/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class7/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class7/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class7/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class7/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class7/names/n* ");
  system ("rm -f /usr/local/com/class7/files/f* ");

  FP = fopen("/usr/local/com/class8/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class8/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class8/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class8/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class8/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class8/names/n* ");
  system ("rm -f /usr/local/com/class8/files/f* ");

  FP = fopen("/usr/local/com/class9/settings/Type_linea", "w");
  fputs(type_line, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class9/settings/Addpoints", "w");
  fputs(addpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class9/settings/Subpoints", "w");
  fputs(subpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class9/settings/Mulpoints", "w");
  fputs(mulpoints, FP);
  fclose(FP);

  FP = fopen("/usr/local/com/class9/settings/Divpoints", "w");
  fputs(divpoints, FP);
  fclose(FP);

  system ("rm -f /usr/local/com/class9/names/n* ");
  system ("rm -f /usr/local/com/class9/files/f* ");

  exit(0);
}

int
opener()
{
  for(e = 1; e <= 40;  e++)
    {
      Board_file_descriptor[e] = open(BOARDFILE[e], O_RDONLY|O_NONBLOCK);
      ioctl(Board_file_descriptor[e], EVIOCGRAB, 1);
    }
  return 0;
}

int
print_pro()
{
  int ones = 0;
  int tens = 0;
  int screen_pos[42];

  struct SDL_Rect slots[3];
          
  slots[0].x = 0;
  slots[0].y = 295;
        
  slots[1].x = 45;
  slots[1].y = 395;
       
  slots[2].x = 80;
  slots[2].y = 395;

                      /* PUT LINE LENGTH INTO INTEGER b. */
      
  if (Length_flag == 0)
    {
      SDL_RenderClear(rends);
      SDL_RenderPresent(rends);
      SDL_UpdateWindowSurface(windowframe);
                
      scratch = TTF_RenderText_Solid(fnt40, " ENTER LINE LENGTH (Between 10 and 70)", black);
      SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
      SDL_FreeSurface(scratch);
      SDL_UpdateWindowSurface(windowframe); 
            
      Z[1] = 0;
            
      tens = get_digit();
      if((tens < 1) || (tens > 7))
        {
          return 0;
        }
      Z[0] = (char) tens + 48;
      scratch = TTF_RenderText_Solid(fnt40, &Z[0], black);
      SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
      SDL_FreeSurface(scratch);
      SDL_UpdateWindowSurface(windowframe); 
      tens = tens * 10;
       
      ones = get_digit();
      Z[0] = (char) ones + 48;
      scratch = TTF_RenderText_Solid(fnt40, &Z[0], black);
      SDL_BlitSurface(scratch, NULL, glass, &slots[2]);
      SDL_FreeSurface(scratch);
      SDL_UpdateWindowSurface(windowframe); 
      b = tens + ones;
    }
  
           /*  CLEAR PRINT BUFFER.  */
   
  for (a = 0; a < 4995; a++)
    {
      Proof[a] = 0;
    }
   
          /*  IF STUDENT COMPOSITION IS BLANK, RETURN WITHOUT PRINTING.  */
   
  if (Comp[e][0] == 0) return 0;  
  
                 /*  PRINT STUDENT ROSTER NUMBER AND NAME.  */
   
  Proof[0] = 10; // These are line feeds to create top margin.
  Proof[1] = 10;
  Proof[2] = 10;
  Proof[3] = 10;
  Proof[4] = 13; // Insert carriage return.
  Proof[5] = 9; // Insert tab.
  Proof[6] = (char) ((e) / 10 + 48);  // Calculate and insert tens of student number.
  if (Proof[6] == 48) Proof[6] = 32;  // Put a space if tens == 0.
  Proof[7] = (char) (((e) % 10) + 48); // Calculate and insert ones.
  Proof[8] = (char) 32; // Insert a space between roster number and following name.
  
  a = 0;
  while(Roster[e][a] != 0)  // Add student name to first line.
    {
      Proof[9+a] = Roster[e][a];
      a++;
    }

  a=a+9;
  Proof[a] = (char) 10;
  a++;
  Proof[a]=13;           
  a++;
  Proof[a]=9;
  a++;

/* ********************** BREAK TEXT INTO PRINTABLE LINES ************************ */
     
  Text_pos[e] = 0;     // Set pointer to beginning of student e's file.     
  screen_pos[e] = a;  // Set pointer to next character of Proof (display string).

  while(Comp[e][Text_pos[e]+c] != 0)  // DO UNTIL EOF.
    {
      for(c=0; c<b; c++) // Do b characters at a time. Default = 66.
        {
          Proof[screen_pos[e]+c] = Comp[e][Text_pos[e]+c]; // Copy student text into Proof.
        
          if(Proof[screen_pos[e]+c]==10) 
            {
              screen_pos[e]++;
              Proof[screen_pos[e]+c]=13;
              screen_pos[e]++;
              Proof[screen_pos[e]+c]=9;
              screen_pos[e]=screen_pos[e]+c; // screen_pos is at the tab.
              Text_pos[e]=Text_pos[e]+c; // Text_pos[e] is at 1st char of next line
              c = 0; // c starts the next line.
            }
        }
        
/* For each line of chars, add CR, LF, & tab. Set text & screen pointers to next line.*/

      for(c=c;  c>b-15; c--)
        {
          if(Proof[screen_pos[e]+c] == 32) break;
        }
      Proof[screen_pos[e]+c]=10;
      screen_pos[e]=screen_pos[e]+c+1;
      Proof[screen_pos[e]]=13;
      screen_pos[e]++;
      Proof[screen_pos[e]]=9;
      screen_pos[e]++;
      Text_pos[e]=Text_pos[e]+c+1;
      c=0;
    }

    /* SAVE FORMATTED FILE AS 'output'. */
     
  FP =  fopen("/usr/local/com/class1/files/output", "w");
  for(b = 0; Proof[b]!= 0; b++)
    {
      fputc(Proof[b], FP);  
    }
  fclose(FP);
    
    /* GO PRINT output, THEN RETURN TO MENU. */
    
  system("lpr   /usr/local/com/class1/files/output");
  Text_pos[e]=0;
  return 0;
}

void
process()
{
  int b = 0;
  Klok = time(t);  
  SDL_RenderClear(rends);
  
            /*  Text_pos[] IS THE OFFSET FROM THE BEGINNING OF STUDENT COMPOSITION TO
            THE FIRST LETTER OF DISPLAYED LINE (Display_line[][]).  Cursor[] IS THE 
            OFFSET FROM Text_pos[] TO THE CURRENT CHARACTER. */
    
           /* COPY OPENING LINES OF 30 STUDENTS TO THE SCREEN. */
  for (e = 1; e <= 30; e++)
    {
      Cursor[e] = 0;
      for (a = 0; a < 20; a++)
       {
         Display_line[e][a] = Comp[e][Text_pos[e] + a];
       }      
          scratch = TTF_RenderText_Solid(fnt40, "_", hue[e]);
          SDL_BlitSurface(scratch, NULL, glass, &tile[e][0]);
          SDL_FreeSurface(scratch);
          
          for (b = 0; b < 20; b++)
            {
              scratch = TTF_RenderText_Solid(fnt40, &Display_line[e][b] , hue[e]);
              SDL_BlitSurface(scratch, NULL, glass, &tile[e][b]);
              SDL_FreeSurface(scratch);
            } 
    }   
     
  SDL_UpdateWindowSurface(windowframe);

  az=50;
  while (1)
   {         
       /*  GO BACK TO MAIN MENU IF MASTER KEYBOARD RETURNS NEWLINE. */ 
     if ((get_char() == 10) || (time(t) - Klok == Time)) 
        {
          return;
        }       


                   /* START POLLING 30 KEYBOARDS. */

         for (e = 1; e <= 30; e++)
           {
             a = get_event(e);
             if ((int)a == 0) continue;              
             Z[0] = a;
               
             /* MAKE SPACE FOR INSERTING A CHARACTER, OR... */
             if ((int)a == 129)   
               {
                 /* First, erase the whole line.  */
                 scratch = TTF_RenderText_Solid(fnt40, Display_line[e], white);
                 SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                 SDL_FreeSurface(scratch);
      
                 /* Second, insert the space into the text file.  */
                 for (b = 4999; b > Text_pos[e] + Cursor[e]; b--)
                   {
                     Comp[e][b] = Comp[e][b - 1];
                   }
                 Comp[e][b] = (char)(32);
                 for(b = 0; b < 20; b++)
                   {
                     Display_line[e][b] = Comp[e][Text_pos[e] + b];
                   }
                 
                 /* Finally, reprint the line with the space inserted.  */
                 scratch = TTF_RenderText_Solid(fnt40, Display_line[e], hue[e]);
                 SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                 SDL_FreeSurface(scratch);
              
                 continue;  
              }
              
             /* DELETE A CHARACTER AND CLOSE SPACE WHERE IT WAS, OR .... */
             if (a == 127)
              {
                /* First, erase the whole line.  */
                scratch = TTF_RenderText_Solid(fnt40, Display_line[e], white);
                SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                SDL_FreeSurface(scratch);
              
                /* Second, delete the character from the text file.  */
                for (a = Text_pos[e] + Cursor[e]; a < 4999; a++)
                  {
                    Comp[e][a] = Comp[e][a + 1];
                  }
                for (a = 0; a < 20; a++)
                  {
                    Display_line[e][a] = Comp[e][Text_pos[e] + a];
                  }
               
                /* Finally, reprint the file segment sans target character.  */                 
                scratch = TTF_RenderText_Solid(fnt40, Display_line[e], hue[e]);
                SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                SDL_FreeSurface(scratch);
                
                continue;  
              }
             
             /* DO NOTHING,(Ignore pipe symbol) OR... */
             if (a == 124) continue;
             
             /* SCROLL TEXT TO RIGHT ON LEFT CURSOR IF Cursor[e] IS ZERO, OR... */
             if ((Cursor[e] == 0) && (Text_pos[e] > 0) && (a == 130))
              {
                /* First, erase the line.  */
                scratch = TTF_RenderText_Solid(fnt40, Display_line[e], white);
                SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                SDL_FreeSurface(scratch);
            
                /* Second, select the adjacent (to the left) line segment.  */ 
                Text_pos[e]--;
                for (b = 0; b < 20; b++)
                  {
                    Display_line[e][b] = Comp[e][Text_pos[e] + b];
                  } 
                 
                /* Third, reprint the new line segment.  */
                scratch = TTF_RenderText_Solid(fnt40, Display_line[e], hue[e]);
                SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                SDL_FreeSurface(scratch);
              
                /*  Finally, restore cursor to the first character.  */
                if(Text_pos[e] == 0)
                 {
                   scratch = TTF_RenderText_Solid(fnt40, "_", hue[e]);
                   SDL_BlitSurface(scratch, NULL, glass, tile[e]);
                   SDL_FreeSurface(scratch);
               }
                 
                 continue;
             }
             
             /* MOVE CURSOR TO THE RIGHT, OR ... */
             if (a == 131)
               {
                 /* STOP IF END OF FILE. */
                 if (Comp[e][Text_pos[e] + Cursor[e]] == 0) continue;
                       
                 /* SCROLL TEXT TO LEFT IF AT END OF DISPLAYED LINE. */
                 if (Cursor[e] == 19)
                   {
                     /* First, erase the displayed line.  */
                     scratch = TTF_RenderText_Solid(fnt40, Display_line[e], white);
                     SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                     SDL_FreeSurface(scratch);
                   
                      /* Second, select the adjacent (right hand) line segment.  */        
                      Text_pos[e]++;
                      for (b = 0; b < 20; b++) 
                        {
                          Display_line[e][b] = Comp[e][Text_pos[e] + b];
                        }
                     
                      /* Third, display the new line segment.  */
                      scratch = TTF_RenderText_Solid(fnt40, Display_line[e], hue[e]);
                      SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                      SDL_FreeSurface(scratch);
           
                      continue;
               }
               
             /* MOVE CURSOR ONE SPACE TO RIGHT IF NOT AT END OF DISPLAYED LINE. */
             else
               {
                 /* First, erase cursor by painting over.  */
                 scratch = TTF_RenderText_Solid(fnt40, "_", white);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
                 SDL_FreeSurface(scratch);
             
                        /* Second, reprint the original letter.  */ 
                 Z[0] = Display_line[e][Cursor[e]];
                 scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);         
                 SDL_FreeSurface(scratch);

                 /* Third, increment the cursor and reprint.  */                 
                 Cursor[e]++;
                 scratch = TTF_RenderText_Solid(fnt40, "_", hue[e]);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);         
                 SDL_FreeSurface(scratch);
                 
                 continue;
               }
           } 
               
             /* MOVE CURSOR LEFT, OR... */
             if ((a == 130) && (Cursor[e] > 0))
               {
                 /* First, erase cursor. */               
                 scratch = TTF_RenderText_Solid(fnt40, "_", white);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);         
                 SDL_FreeSurface(scratch);
             
                 /* Second, reprint original character.  */
                 Z[0] = Display_line[e][Cursor[e]];
                 scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);         
                 SDL_FreeSurface(scratch);
                
                 /* Third, decrement cursor and reprint.  */
                 Cursor[e]--;
                 scratch = TTF_RenderText_Solid(fnt40, "_", hue[e]);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);         
                 SDL_FreeSurface(scratch);
             
                 continue;
               }
             
             /* PRINT INPUT IF CURSOR IS BETWEEN 0 AND 18, OR... */
             if ((a != 130) && (Cursor[e] < 19))
               {
                 /* First, erase any character in destination space.  */
                 Z[0] = Display_line[e][Cursor[e]];
                 scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
                 SDL_FreeSurface(scratch);
                 
                 /* Second, erase cursor in desination space.  */             
                 scratch = TTF_RenderText_Solid(fnt40, "_", white);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
                 SDL_FreeSurface(scratch);
                 
                 /* Third, put the new character in text file.  */
                 Z[0] = a;
                 Display_line[e][Cursor[e]] = Z[0];
                 Comp[e][Text_pos[e] + Cursor[e]] = Z[0];
                 
                 /* Fourth, put the new character on the screen.  */
                 scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
                 SDL_FreeSurface(scratch);
                 
                 /* Finally, increment and reprint the cursor.  */
                 Cursor[e]++;
                 scratch = TTF_RenderText_Solid(fnt40, "_", hue[e]);
                 SDL_BlitSurface(scratch, NULL, glass, &tile[e][Cursor[e]]);
                 SDL_FreeSurface(scratch);
                 
                 continue;
               }
               
             /* SCROLL TEXT TO LEFT AND PRINT INPUT CHARACTER IF CURSOR IS 19. */
             if (Cursor[e] == 19)
               {
                 /* First, erase existing line.  */
                 scratch = TTF_RenderText_Solid(fnt40, Display_line[e], white);
                 SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                 SDL_FreeSurface(scratch);
                 
                 /* Second, add new character to text file.  */
                 Z[0] = a;
                 Display_line[e][Cursor[e]] = Z[0];
                 Comp[e][Text_pos[e] + Cursor[e]] = Z[0];
                   
                 /* Third, select adjacent (right hand) text segment.  */
                 if (Text_pos[e] < 4970) Text_pos[e]++;
                 for (b = 0; b < 20; b++) 
                   {
                     Display_line[e][b] = Comp[e][Text_pos[e] + b];
                   }
                 
                 /* Fourth, print the new text segment.  */               
                 scratch = TTF_RenderText_Solid(fnt40, Display_line[e], hue[e]);
                 SDL_BlitSurface(scratch, NULL, glass, &line[e]);
                 SDL_FreeSurface(scratch);
            }
         }
     az--;
     if(az==0)
       {
         SDL_UpdateWindowSurface(windowframe);
         az=50;
       }
   }
  return;
}

int
random_num()
{
  while(1)
    {
      a = rand() / 230000000;
      if(a<9) break;
    }
  return a;
}

void
roster_page()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
  
  for (e = 1;  e < 6; e++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[e][b].x = (b * 30) + 5;
            slots[e][b].y = (e * 32);  // y runs from 0 to 128
          }
     }

  for (e = 6;  e < 11; e++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[e][b].x = (b * 30) + 5;
            slots[e][b].y = (e * 32 + 20);  // y runs from 0 to 530
          }
     }

  for (e = 11;  e < 16; e++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[e][b].x = (b * 30) + 5;
            slots[e][b].y = (e * 32 + 40);  // y runs from 0 to 530
          }
     }

  for (e = 16;  e < 21; e++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[e][b].x = (b * 30) + 5;
            slots[e][b].y = (e * 32 + 60);  // y runs from 0 to 530
          }
     }

   
   for (e = 21;  e < 26; e++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[e][b].x = (b * 30) + 520; // x's run from 520 to 1090
            slots[e][b].y = ((e - 20) * 32);
          }
     }

   for (e = 26;  e < 31; e++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[e][b].x = (b * 30) + 520; // x's run from 520 to 1090
            slots[e][b].y = ((e - 20) * 32) + 20;
          }
     }
   for (e = 31;  e < 36; e++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[e][b].x = (b * 30) + 520; // x's run from 520 to 1090
            slots[e][b].y = ((e - 20) * 32) + 40;
          }
     }
   for (e = 36;  e < 41; e++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[e][b].x = (b * 30) + 520; // x's run from 520 to 1090
            slots[e][b].y = ((e - 20) * 32) + 60;
          }
     }
     
  scratch = TTF_RenderText_Solid(fnt20, "               TYPE  ADD  SUB  MUL DIV                      TYPE  ADD  SUB  MUL  DIV", black);
  SDL_BlitSurface(scratch, NULL, glass, slots[0]);
  SDL_FreeSurface(scratch);
     
  char ttline[4];
  ttline[3] = 0; 
   
  char apoints[2];
  apoints[1] = 0;
  
  char spoints[2];
  spoints[1] = 0;
  
  char dpoints[2];
  dpoints[1] = 0;
   
  char mpoints[2];
  mpoints[1] = 0;

  for (e = 1; e < 21; e++)
    {  
      scratch = TTF_RenderText_Solid(fnt33, Roster[e],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, slots[e]);
      SDL_FreeSurface(scratch);
      
      ttline[0] = (char) ((Tline[e] / 100) + 48);
      if(ttline[0] == '0') {ttline[0] = ' ';}
       
      ttline[1] = (char) ((Tline[e] % 100) / 10) + 48;
      if((ttline[0] == ' ') & (ttline[1] == '0')) {ttline[1] = ' ';}
        
      ttline[2] = (char) (Tline[e] % 10) + 48;
      
      apoints[0] =  (char) Addpoints[e]+48;
      spoints[0] =  (char) Subpoints[e]+48;
      mpoints[0] =  (char) Mulpoints[e]+48;
      dpoints[0] =  (char) Divpoints[e]+48;
      
      scratch = TTF_RenderText_Solid(fnt33, &ttline[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][5]);
      SDL_FreeSurface(scratch);
      
      scratch = TTF_RenderText_Solid(fnt33, &apoints[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][8]);
      SDL_FreeSurface(scratch);
       
      scratch = TTF_RenderText_Solid(fnt33, &spoints[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][10]);
      SDL_FreeSurface(scratch);
      
      scratch = TTF_RenderText_Solid(fnt33, &mpoints[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][12]);
      SDL_FreeSurface(scratch);
      
      scratch = TTF_RenderText_Solid(fnt33, &dpoints[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][14]);
      SDL_FreeSurface(scratch);
    }
     
  for (e = 21; e < 31; e++)
    {  
      scratch = TTF_RenderText_Solid(fnt33, Roster[e],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, slots[e]);
      SDL_FreeSurface(scratch);
          
      ttline[0] = (char) ((Tline[e] / 100) + 48);
      if(ttline[0] == '0') {ttline[0] = ' ';}
        
      ttline[1] = (char) ((Tline[e] % 100) / 10) + 48;
      if((ttline[0] == ' ') & (ttline[1] == '0')) {ttline[1] = ' ';}
          
      ttline[2] = (char) (Tline[e] % 10) + 48;
         
      apoints[0] =  (char) Addpoints[e]+48;
      spoints[0] =  (char) Subpoints[e]+48;
      mpoints[0] =  (char) Mulpoints[e]+48;
      dpoints[0] =  (char) Divpoints[e]+48;
          
      scratch = TTF_RenderText_Solid(fnt33, &ttline[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][6]);
      SDL_FreeSurface(scratch);
          
      scratch = TTF_RenderText_Solid(fnt33, &apoints[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][9]);
      SDL_FreeSurface(scratch);
          
      scratch = TTF_RenderText_Solid(fnt33, &spoints[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][11]);
      SDL_FreeSurface(scratch);
          
      scratch = TTF_RenderText_Solid(fnt33, &mpoints[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][13]);
      SDL_FreeSurface(scratch);
          
      scratch = TTF_RenderText_Solid(fnt33, &dpoints[0],  hue[e]);
      SDL_BlitSurface(scratch, NULL, glass, &slots[e][15]);
      SDL_FreeSurface(scratch);
    }
      
  SDL_UpdateWindowSurface(windowframe); 
  alpha();
  return;
}

void
security()
{
  int a = 0;
  Panic = 0;
  
  SDL_RenderClear(rends); 
  SDL_RenderPresent(rends);
     
  struct SDL_Rect slots[2];
    
  slots[0].x = 0;
  slots[0].y = 220;
      
  slots[1].x = 0;
  slots[1].y = 320;
    
  scratch = TTF_RenderText_Solid(fnt40, "     ENTER YOUR PASSWORD", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
  
  Z[0] = (char) get_digit() + 48;
  if((Z[0] < 48) || (Z[0] > 57))
    {
      Z[0] = 0;
      Panic = 1;
      return;
    }
  
  scratch = TTF_RenderText_Solid(fnt40, "*", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
  
  Z[1] = (char) get_digit() + 48;
  if((Z[1] < 48) || (Z[1] > 57))
    {
      Z[1] = 0;
      Panic = 1;
      return;
    }
  
  scratch = TTF_RenderText_Solid(fnt40, "**", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe); 
  
  Z[2] = (char) get_digit() + 48;
  if((Z[2] < 48) || (Z[2] > 57))
    {
      Z[2] = 0;
      Panic = 1;
      return;
    }
  
  scratch = TTF_RenderText_Solid(fnt40, "***", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe); 
  
  Z[3] = (char) get_digit() + 48;
  if((Z[3] < 48) || (Z[3] > 57))
    {
      Z[3] = 0;
      Panic = 1;
      return;
    }
  
  scratch = TTF_RenderText_Solid(fnt40, "****", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe); 
  alpha();
     
  for (a = 0; a < 4; a++)
    {
      if (Z[a] != PASSWORD[a])
         {
           for (b = 0; b < 4; b++)
             {
               Z[b] = 0;
             }
           Panic = 1;
           return;
         }
      Z[a] = 0;
    }
  return;
}
  
int
select_board()
{
  char proxy[3] = {0,0,0};
  
  Panic = 0;
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
     
  struct SDL_Rect slots[3];
      
  slots[0].x = 0;
  slots[0].y = 295;
      
  slots[1].x = 0;
  slots[1].y = 395;
  
  slots[2].x = 0;
  slots[2].y = 495;
        
  scratch = TTF_RenderText_Solid(fnt50, "        WHICH KEYBOARD?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
  
  Ones_register = get_digit();
  e = Ones_register ;
  proxy[0]= Ones_register + 48;
   
  scratch = TTF_RenderText_Solid(fnt50, proxy, black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
       
  Tens_register = get_digit();
  if(Tens_register == 10)
    {
      return e;
    }
  e = 10 * e + Tens_register;
  proxy[1]= Tens_register + 48;
       
  scratch = TTF_RenderText_Solid(fnt50, proxy, black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
  
  if(e > 30)
    {  
      scratch = TTF_RenderText_Solid(fnt50, " YOU DON'T HAVE THAT MANY BOARDS!", black);
      SDL_BlitSurface(scratch, NULL, glass, &slots[2]);
      SDL_FreeSurface(scratch);
      SDL_UpdateWindowSurface(windowframe);
      SDL_Delay(2000);
      Panic = 1;
      return e;
    }
  
  alpha();
  return e;
}

int
select_book()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
   
  struct SDL_Rect slots[6];
    
  slots[0].x = 0;
  slots[0].y = 200;
    
  slots[1].x = 0;
  slots[1].y = 320;
    
  slots[2].x = 0;
  slots[2].y = 380;
    
  slots[3].x = 0;
  slots[3].y = 440;
     
  slots[4].x = 0;
  slots[4].y = 500;
        
  slots[5].x = 0;
  slots[5].y = 560;
        
  scratch = TTF_RenderText_Solid(fnt40, "      WHICH BOOK?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
             
  scratch = TTF_RenderText_Solid(fnt40, "         1 - Class Activities", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
             
  scratch = TTF_RenderText_Solid(fnt40, "         2 - Homework", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[2]);
  SDL_FreeSurface(scratch);
             
  scratch = TTF_RenderText_Solid(fnt40, "         3 - Text 3", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[3]);
  SDL_FreeSurface(scratch);
            
  scratch = TTF_RenderText_Solid(fnt40, "         4 - Text 4", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[4]);
  SDL_FreeSurface(scratch);
           
  SDL_UpdateWindowSurface(windowframe); 
        
  SDL_Event event;
     
  while(1)
    {
      SDL_WaitEvent(&event);    
      if (event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
           {
             case SDLK_1:
             FP = fopen("/usr/local/etc/answerkeys/text1", "a+");
             fgets(Key, 40000, FP);
             fclose(FP);
             booklength();
             return 0;
             break;
                  
             case SDLK_2:
             FP = fopen("/usr/local/etc/answerkeys/text2", "a+");
             fgets(Key, 40000, FP);
             fclose(FP);
             booklength();
             return 0;
             break;
                  
             case SDLK_3:
             FP = fopen("/usr/local/etc/answerkeys/text3", "a+");
             fgets(Key, 40000, FP);
             fclose(FP);
             booklength();
             return 0;
             break;
                  
             case SDLK_4:
             FP = fopen("/usr/local/etc/answerkeys/text4", "a+");
             fgets(Key, 40000, FP);
             fclose(FP);
             booklength();
             return 0;
             break;
           }
        }
    }                        
  return 0;
}

void
booklength()
{ 
  Max_page = 0;
  for (a = 0;  Key[a] != 64;  a++)
    {
      if (Key[a] == 126) Max_page++;
    }  
  return;
}

int
select_line_page()
{
  char proxy[4] = {0,0,0,0};
  e = select_board();
  if(e > 30)
    {
      detente();
      return 0;
    }
    
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
    
  struct SDL_Rect slots[2];
     
     slots[0].x = 0;
     slots[0].y = 295;
      
     slots[1].x = 0;
     slots[1].y = 395;
     
  scratch = TTF_RenderText_Solid(fnt50, "             WHICH LINE?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
     
  Ones_register = get_digit();
  if((Ones_register == 10)||(Ones_register == 0))
    {
      return 0;
    }
  Tline[e] = Ones_register ;
  proxy[0]= Ones_register + 48;
    
  scratch = TTF_RenderText_Solid(fnt50, proxy, black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe); 
     
  Tens_register = get_digit();
  if(Tens_register == 10)
    {
      return 0;
    }
  Tline[e] = 10 * Tline[e] + Tens_register ;
  proxy[1]= Tens_register + 48;
    
  scratch = TTF_RenderText_Solid(fnt50, proxy, black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe); 
      
  Hundreds_register = get_digit();
  if(Hundreds_register == 10)
    {
      return 0;
    }
  Tline[e] = 10 * Tline[e] + Hundreds_register ;
  if(Tline[e] > 240)
    {
      Tline[e] = 240;
      return 0;
    }
  proxy[2]= Hundreds_register + 48;
    
  scratch = TTF_RenderText_Solid(fnt50, proxy, black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe); 
  alpha();
  return e;
} 
     
void
splash_page()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);
  
  lines[0].x = 160;
  lines[0].y = 200;
    
  lines[1].x = 305;
  lines[1].y = 400;
      
  lines[2].x = 230;
  lines[2].y = 450;
   
  lines[3].x = 260;
  lines[3].y = 500;
    
  lines[4].x = 170;
  lines[4].y = 550;
  
  lines[5].x = 230;
  lines[5].y = 600;
     
  scratch = TTF_RenderText_Solid(fnt90, " CLASSNET-02 ", red);
  SDL_BlitSurface(scratch, NULL, glass, &lines[0]);
  SDL_FreeSurface(scratch);
     
  scratch = TTF_RenderText_Solid(fnt35, "DISTRIBUTABLE UNDER", black);
  SDL_BlitSurface(scratch, NULL, glass, &lines[1]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt35, "GNU GENERAL PUBLIC LICENSE", black);
  SDL_BlitSurface(scratch, NULL, glass, &lines[2]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt35, "FREE SOFTWARE FOUNDATION", black);
  SDL_BlitSurface(scratch, NULL, glass, &lines[3]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt35, "51 FRANKLIN STREET, FIFTH FLOOR", black);
  SDL_BlitSurface(scratch, NULL, glass, &lines[4]);
  SDL_FreeSurface(scratch);
    
  scratch = TTF_RenderText_Solid(fnt35, "BOSTON, MA  02110-1301, USA", black);
  SDL_BlitSurface(scratch, NULL, glass, &lines[5]);
  SDL_FreeSurface(scratch);
      
  SDL_UpdateWindowSurface(windowframe);
    
  SDL_Delay(5000);
  return;
}

void
subtract()
{
  char a;

                          /* OPEN EMPTY WHITE WINDOW. */

  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

                          /* PUT 30 PROBLEMS ON THE SCREEN. */

  for (e = 1; e <= 30; e++)
    {
      for(b = 0; b < 20; b++) Last_line[e][b] = ' ';
      Stop[e] = 0;
      Goofed[e] = 0;
      Score[e]=0;
      make_sub_prob(e);
      
      Ones_register =  Subtrahend[e] % 10;
      Tens_register = Subtrahend[e] / 10;
      Last_line[e][2] =  Ones_register + 48;
      Last_line[e][1] = (char) Tens_register + 48;
      Last_line[e][4] = '-';
      Last_line[e][6] = (char) Minuend[e] + 48;
      Last_line[e][8] = '=';
      Last_line[e][11] = 32;
      if(Last_line[e][1] == 48){Last_line[e][1] = 32;}
      
      for (c = 0; c < 20; c++)
        {
          scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], hue[e]);
          SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
          SDL_FreeSurface(scratch);
        }
    }
     
  SDL_UpdateWindowSurface(windowframe);
   
                      /* START KEYBOARD POLLING. */
   
  while (1)
    {
            /* CHECK TEACHER'S KEYBOARD FOR RETURN TO MENU. */
      if (get_char() == 10)
        {
          Goaround = 0;
          return;
        }
      for (e = 1; e <= 30; e++)
        {
               /* END OR CONTINUE ERROR TIMEOUT PENALTY. */
          if (Goofed[e] == 1)
            {
              if (time(t) - Clk[e] > 3)
                {
                  Goofed[e] = 0;
                  Z[0] = Last_line[e][11];
                  scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
                  SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                  SDL_FreeSurface(scratch);
                  SDL_UpdateWindowSurface(windowframe);
                }
              else
                {              
                  get_event(e);
                  continue;
                } 
            }
            
                    /* GET STUDENT INPUT. */
           if (Goofed[e] == 0)
             {
               a = get_event(e);
               if (a == 0) continue;
               if (a == 10) goto evaluatesub;
               if (Stop[e] == 1) continue;

               if (((int) a < 48) || ((int) a > 57)) continue;
               Stuans[e] = (int) a - 48;
               Z[0] = (int) a;
               Last_line[e][11] = (int) a;
               
               scratch = TTF_RenderText_Solid(fnt40, &Z[0], hue[e]);
               SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
               SDL_FreeSurface(scratch);
               SDL_UpdateWindowSurface(windowframe);

               Stop[e] = 1;
               continue;
                                 /* EVALUATE ANSWERS. */	
               evaluatesub:
               if (Stuans[e] == Answer[e])
                 { 
                   for (c = 0; c < 20; c++)
                      {
                         scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], white);
                         SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
                         SDL_FreeSurface(scratch);
                      }
                   
                   SDL_UpdateWindowSurface(windowframe);
                   
                   Advance[e] = 1;
                   Score[e] = Score[e] + Subpoints[e];
                   Ones_register = Score[e] % 10;
                   Tens_register = Score[e] / 10;
                   Last_line[e][15] = (char) Ones_register + 48;
                   Last_line[e][14] = (char) Tens_register + 48;
                   Last_line[e][11] = 32;
                   if(Last_line[e][14] == 48){Last_line[e][14] = 32;}
                   
                                 /* ANSWER GOOD */
                    
                   if(Score[e] < Win_score)
                      {
                        Stop[e] = 0;
                        make_sub_prob(e);
                        
                        Ones_register =  Subtrahend[e] % 10;
                        Tens_register = Subtrahend[e] / 10;
                        Last_line[e][2] =  Ones_register + 48;
                        Last_line[e][1] = (char) Tens_register + 48;
                        Last_line[e][4] = '-';
                        Last_line[e][6] = (char) Minuend[e] + 48;
                        Last_line[e][8] = '=';
                        if(Last_line[e][1] == 48){Last_line[e][1] = 32;}
                        
                        for (c = 0; c < 20; c++)
                          {
                            scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][c], hue[e]);
                            SDL_BlitSurface(scratch, NULL, glass, &tilem[e][c]);
                            SDL_FreeSurface(scratch);
                          }
                          
                        SDL_UpdateWindowSurface(windowframe);
                      }
                   
                   else
                      {
                        Operation = 2;
                        Goaround = 1;
                        win_page();
                        return;
                      } 
                 }

                              /* ANSWER BAD */
                            
               else if(Last_line[e][11] != Answer[e])
                 {
                   Clk[e] = time(t);
                   Stop[e] = 0;
                   Goofed[e] = 1;
                   
                   Z[0] = Last_line[e][11];
                   scratch = TTF_RenderText_Solid(fnt40, &Z[0], white);
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                   
                   Last_line[e][11] = 'X';
                   scratch = TTF_RenderText_Solid(fnt40, &Last_line[e][11], black);
                   SDL_BlitSurface(scratch, NULL, glass, &tilem[e][11]);
                   SDL_FreeSurface(scratch);
                   SDL_UpdateWindowSurface(windowframe);
                 }
             }
        }        
    } 
  return;          
}

void
timeit()
{
  Time = 20000;

  Panic = 0;
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(windowframe);

  struct SDL_Rect slots[3];

  slots[0].x = 0;
  slots[0].y = 295;

  scratch = TTF_RenderText_Solid(fnt50,"HOW MANY MINUTES THIS SESSION?", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
  SDL_UpdateWindowSurface(windowframe);
  Ones_register = get_digit();
  if(Ones_register==10)return;
  Time = Ones_register * 60;

  return;
}

void
wedge(int c)
{
  int a = 0;
  for (a = 4999; a > c; a--) Proof[a] = Proof[a - 1];
  return;
}

void
win_page()
{
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  
  struct SDL_Rect slots[3];
     
  slots[0].x = 0;
  slots[0].y = 225;
  
  slots[1].x = 100;
  slots[1].y = 400;      
  
  slots[2].x = 0;
  slots[2].y = 700;
   
  scratch = TTF_RenderText_Solid(fnt45, " THE WINNER IS:  ", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[0]);
  SDL_FreeSurface(scratch);
   
  scratch = TTF_RenderText_Solid(fnt110, Roster[e], red);
  SDL_BlitSurface(scratch, NULL, glass, &slots[1]);
  SDL_FreeSurface(scratch);
   
  scratch = TTF_RenderText_Solid(fnt15, " Make life harder?  ", black);
  SDL_BlitSurface(scratch, NULL, glass, &slots[2]);
  SDL_FreeSurface(scratch);
   
  SDL_UpdateWindowSurface(windowframe);
   
  unsigned long az = 0;
   
  for (az = 0; az < 2000000; az++)
    {
      SDL_Event event;
      event.type = SDL_KEYUP;    
      SDL_PollEvent(&event);    
      if (event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
            {
              case SDLK_y:
                if (Operation == 1) Addpoints[e]--;
                if (Operation == 2) Subpoints[e]--;
                if (Operation == 3) Mulpoints[e]--;
                if (Operation == 4) Divpoints[e]--;
                 
                for (e = 1;  e < 41;  e++){ Hndicap[e] = (char)(Addpoints[e] + 48);}
                if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Addpoints", "w");
                if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Addpoints", "w");
                if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Addpoints", "w");
                if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Addpoints", "w");
                if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Addpoints", "w");
                if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Addpoints", "w");
                if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Addpoints", "w");
                if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Addpoints", "w");
                if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Addpoints", "w");
                
                fputs(Hndicap, FP);
                fclose(FP);
                
                for (e = 1;  e < 41;  e++) Hndicap[e] = (char)(Subpoints[e] + 48);
                if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Subpoints", "w");
                if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Subpoints", "w");
                if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Subpoints", "w");
                if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Subpoints", "w");
                if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Subpoints", "w");
                if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Subpoints", "w");
                if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Subpoints", "w");
                if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Subpoints", "w");
                if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Subpoints", "w");
                   
                fputs(Hndicap, FP);
                fclose(FP);
                
                for (e = 1;  e < 41;  e++) Hndicap[e] = (char)(Mulpoints[e] + 48);
                if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Mulpoints", "w");
                if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Mulpoints", "w");
                if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Mulpoints", "w");
                if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Mulpoints", "w");
                if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Mulpoints", "w");
                if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Mulpoints", "w");
                if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Mulpoints", "w");
                if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Mulpoints", "w");
                if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Mulpoints", "w");
                  
                fputs(Hndicap, FP);
                fclose(FP);
                 
                for (e = 1;  e < 41;  e++) Hndicap[e] = (char)(Divpoints[e] + 48);
                if (Class == 1) FP = fopen("/usr/local/com/class1/settings/Divpoints", "w");
                if (Class == 2) FP = fopen("/usr/local/com/class2/settings/Divpoints", "w");
                if (Class == 3) FP = fopen("/usr/local/com/class3/settings/Divpoints", "w");
                if (Class == 4) FP = fopen("/usr/local/com/class4/settings/Divpoints", "w");
                if (Class == 5) FP = fopen("/usr/local/com/class5/settings/Divpoints", "w");
                if (Class == 6) FP = fopen("/usr/local/com/class6/settings/Divpoints", "w");
                if (Class == 7) FP = fopen("/usr/local/com/class7/settings/Divpoints", "w");
                if (Class == 8) FP = fopen("/usr/local/com/class8/settings/Divpoints", "w");
                if (Class == 9) FP = fopen("/usr/local/com/class9/settings/Divpoints", "w");
                
                fputs(Hndicap, FP);
                fclose(FP);
                break;
                
              case SDLK_RETURN:
                Score[e] = 0;
                Advance[e] = 1;
                Goaround = 0;
                return;
             } 
        }
    }
  Score[e] = 0;
  Advance[e] = 1;
  return;
}
