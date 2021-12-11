#define BLANK  {0,0,0,0,0,0,0,0}
static const uint8_t font[][8] = {
  // 20h ' '
  BLANK,
  // 21h '!'
  {4,14,14,4,4,0,4,0},
  // 22h '"'
  {10,10,0,0,0,0,0,0},
  // 23h '#'
  {0,10,31,10,10,31,10,0},
  // 24h '$'
  {4,30,5,14,20,15,4,0},
  // 25h '%'
  {3,19,8,4,2,25,24,0},
  // 26h '&'
  {2,5,5,2,21,9,22,0},
  // 27h '''
  {6,4,2,0,0,0,0,0},
  // 28h '('
  {8,4,2,2,2,4,8,0},
  // 29h ')'
  {2,4,8,8,8,4,2,0},
  // 2ah '*'
  {0,4,21,14,21,4,0,0},
  // 2bh '+'
  {0,4,4,31,4,4,0,0},
  // 2ch ','
  {0,0,0,0,6,4,2,0},
  // 2dh '-'
  {0,0,0,31,0,0,0,0},
  // 2eh '.'
  {0,0,0,0,0,6,6,0},
  // 2fh '/'
  {0,16,8,4,2,1,0,0},
  // 30h '0'
  {14,17,25,21,19,17,14,0},
  // 31h '1'
  {4,6,4,4,4,4,14,0},
  // 32h '2'
  {14,17,16,12,2,1,31,0},
  // 33h '3'
  {31,8,4,8,16,17,14,0},
  // 34h '4'
  {8,12,10,9,31,8,8,0},
  // 35h '5'
  {31,1,15,16,16,17,14,0},
  // 36h '6'
  {12,2,1,15,17,17,14,0},
  // 37h '7'
  {31,16,8,4,2,2,2,0},
  // 38h '8'
  {14,17,17,14,17,17,14,0},
  // 39h '9'
  {14,17,17,30,16,8,6,0},
  // 3ah ':'
  {0,6,6,0,6,6,0,0},
  // 3bh ';'
  {0,6,6,0,6,4,2,0},
  // 3ch '<'
  {8,4,2,1,2,4,8,0},
  // 3dh '='
  {0,0,31,0,31,0,0,0},
  // 3eh '>'
  {2,4,8,16,8,4,2,0},
  // 3fh '?'
  {14,17,16,12,4,0,4,0},
  // 40h '@'
  {14,17,29,21,29,1,14,0},
  // 41h 'A'
  {14,17,17,17,31,17,17,0},
  // 42h 'B'
  {15,17,17,15,17,17,15,0},
  // 43h 'C'
  {14,17,1,1,1,17,14,0},
  // 44h 'D'
  {7,9,17,17,17,9,7,0},
  // 45h 'E'
  {31,1,1,15,1,1,31,0},
  // 46h 'F'
  {31,1,1,15,1,1,1,0},
  // 47h 'G'
  {14,17,1,29,17,17,30,0},
  // 48h 'H'
  {17,17,17,31,17,17,17,0},
  // 49h 'I'
  {14,4,4,4,4,4,14,0},
  // 4ah 'J'
  {16,16,16,16,17,17,14,0},
  // 4bh 'K'
  {17,9,5,3,5,9,17,0},
  // 4ch 'L'
  {1,1,1,1,1,1,31,0},
  // 4dh 'M'
  {17,27,21,21,17,17,17,0},
  // 4eh 'N'
  {17,19,21,25,17,17,17,0},
  // 4fh 'O'
  {14,17,17,17,17,17,14,0},
  // 50h 'P'
  {15,17,17,15,1,1,1,0},
  // 51h 'Q'
  {14,17,17,17,21,9,22,0},
  // 52h 'R'
  {15,17,17,15,5,9,17,0},
  // 53h 'S'
  {30,1,1,14,16,16,15,0},
  // 54h 'T'
  {31,4,4,4,4,4,4,0},
  // 55h 'U'
  {17,17,17,17,17,17,14,0},
  // 56h 'V'
  {17,17,17,17,17,10,4,0},
  // 57h 'W'
  {17,17,21,21,21,21,10,0},
  // 58h 'X'
  {17,17,10,4,10,17,17,0},
  // 59h 'Y'
  {17,17,10,4,4,4,4,0},
  // 5ah 'Z'
  {31,16,8,4,2,1,31,0},
  // 5bh '['
  {14,2,2,2,2,2,14,0},
  // 5ch '\'
  {0,1,2,4,8,16,0,0},
  // 5dh ']'
  {14,8,8,8,8,8,14,0},
  // 5eh '^'
  {4,10,17,0,0,0,0,0},
  // 5fh '_'
  {0,0,0,0,0,0,31,0},
  // 60h '`'
  {2,4,0,0,0,0,0,0},
  // 61h 'a'
  {0,0,14,16,30,17,30,0},
  // 62h 'b'
  {1,1,15,17,17,17,15,0},
  // 63h 'c'
  {0,0,14,1,1,17,14,0},
  // 64h 'd'
  {16,16,30,17,17,17,30,0},
  // 65h 'e'
  {0,0,14,17,31,1,14,0},
  // 66h 'f'
  {12,18,2,15,2,2,2,0},
  // 67h 'g'
  {0,0,30,17,30,16,14,0},
  // 68h 'h'
  {1,1,13,19,17,17,17,0},
  // 69h 'i'
  {4,0,6,4,4,4,14,0},
  // 6ah 'j'
  {8,0,12,8,8,9,6,0},
  // 6bh 'k'
  {1,1,9,5,3,5,9,0},
  // 6ch 'l'
  {6,4,4,4,4,4,14,0},
  // 6dh 'm'
  {0,0,11,21,21,17,17,0},
  // 6eh 'n'
  {0,0,13,19,17,17,17,0},
  // 6fh 'o'
  {0,0,14,17,17,17,14,0},
  // 70h 'p'
  {0,0,15,17,17,15,1,0},
  // 71h 'q'
  {0,0,22,25,17,30,16,0},
  // 72h 'r'
  {0,0,13,18,2,2,7,0},
  // 73h 's'
  {0,0,14,1,14,16,15,0},
  // 74h 't'
  {2,2,15,2,2,18,12,0},
  // 75h 'u'
  {0,0,17,17,17,25,22,0},
  // 76h 'v'
  {0,0,17,17,17,10,4,0},
  // 77h 'w'
  {0,0,17,21,21,21,10,0},
  // 78h 'x'
  {0,0,17,10,4,10,17,0},
  // 79h 'y'
  {0,0,17,17,18,12,7,0},
  // 7ah 'z'
  {0,0,31,8,4,2,31,0},
  // 7bh '{'
  {12,2,2,1,2,2,12,0},
  // 7ch '|'
  {4,4,4,0,4,4,4,0},
  // 7dh '}'
  {6,8,8,16,8,8,6,0},
  // 7eh '~'
  {0,22,13,0,0,0,0,0},
  // 7fh '⌂'
  BLANK,
  // 80h
  BLANK,
  // 81h
  BLANK,
  // 82h
  BLANK,
  // 83h
  BLANK,
  // 84h
  BLANK,
  // 85h
  BLANK,
  // 86h
  BLANK,
  // 87h
  BLANK,
  // 88h
  BLANK,
  // 89h
  BLANK,
  // 8ah
  BLANK,
  // 8bh
  BLANK,
  // 8ch
  BLANK,
  // 8dh
  BLANK,
  // 8eh
  BLANK,
  // 8fh
  BLANK,
  // 90h
  BLANK,
  // 91h
  BLANK,
  // 92h
  BLANK,
  // 93h
  BLANK,
  // 94h
  BLANK,
  // 95h
  BLANK,
  // 96h
  BLANK,
  // 97h
  BLANK,
  // 98h
  BLANK,
  // 99h
  BLANK,
  // 9ah
  BLANK,
  // 9bh
  BLANK,
  // 9ch
  BLANK,
  // 9dh
  BLANK,
  // 9eh
  BLANK,
  // 9fh
  BLANK,
  // a0h
  BLANK,
  // a1h
  {4,0,4,4,14,14,4,0},
  // a2h
  {4,14,5,5,21,14,4,0},
  // a3h
  {12,2,7,2,18,31,0,0},
  // a4h
  {17,14,10,14,17,0,0,0},
  // a5h
  {17,10,4,31,4,31,4,0},
  // a6h
  {4,4,4,0,4,4,4,0},
  // a7h
  {14,1,14,17,14,16,14,0},
  // a8h
  {10,0,0,0,0,0,0,0},
  // a9h
  {14,17,21,19,21,17,14,0},
  // aah
  {14,16,30,17,30,0,0,0},
  // abh
  {0,0,20,10,5,10,20,0},
  // ach
  {0,0,31,16,16,0,0,0},
  // adh
  {0,0,0,30,0,0,0,0},
  // aeh
  {14,17,23,23,27,17,14,0},
  // afh
  {31,0,0,0,0,0,0,0},
  // b0h
  {6,9,9,6,0,0,0,0},
  // b1h
  {0,4,4,31,4,4,31,0},
  // b2h
  {6,8,4,2,14,0,0,0},
  // b3h
  {6,8,4,8,6,0,0,0},
  // b4h
  {8,4,2,0,0,0,0,0},
  // b5h
  {0,17,17,17,19,13,1,0},
  // b6h
  {30,23,23,22,20,20,20,0},
  // b7h
  {0,0,14,14,14,0,0,0},
  // b8h
  {0,0,0,0,4,8,6,0},
  // b9h
  {4,6,4,4,14,0,0,0},
  // bah
  {4,10,10,10,4,0,0,0},
  // bbh
  {0,0,5,10,20,10,5,0},
  // bch
  {2,19,10,22,20,30,17,0},
  // bdh
  {2,19,10,30,20,10,29,0},
  // beh
  {3,20,10,20,23,30,17,0},
  // bfh
  {4,0,4,6,1,17,14,0},
  // c0h
  BLANK,
  // c1h
  BLANK,
  // c2h
  BLANK,
  // c3h
  BLANK,
  // c4h
  BLANK,
  // c5h
  BLANK,
  // c6h
  BLANK,
  // c7h
  BLANK,
  // c8h
  BLANK,
  // c9h
  BLANK,
  // cah
  BLANK,
  // cbh
  BLANK,
  // cch
  BLANK,
  // cdh
  BLANK,
  // ceh
  BLANK,
  // cfh
  BLANK,
  // d0h
  BLANK,
  // d1h
  BLANK,
  // d2h
  BLANK,
  // d3h
  BLANK,
  // d4h
  BLANK,
  // d5h
  BLANK,
  // d6h
  BLANK,
  // d7h
  {0,17,10,4,10,17,0,0},
  // d8h
  BLANK,
  // d9h
  BLANK,
  // dah
  BLANK,
  // dbh
  BLANK,
  // dch
  BLANK,
  // ddh
  BLANK,
  // deh
  BLANK,
  // dfh
  BLANK,
  // e0h
  BLANK,
  // e1h
  BLANK,
  // e2h
  BLANK,
  // e3h
  BLANK,
  // e4h
  BLANK,
  // e5h
  BLANK,
  // e6h
  BLANK,
  // e7h
  BLANK,
  // e8h
  BLANK,
  // e9h
  BLANK,
  // eah
  BLANK,
  // ebh
  BLANK,
  // ech
  BLANK,
  // edh
  BLANK,
  // eeh
  BLANK,
  // efh
  BLANK,
  // f0h
  {12,18,2,26,18,18,28,0},
  // f1h
  {7,9,9,7,9,9,7,0},
  // f2h
  BLANK,
  // f3h
  BLANK,
  // f4h
  BLANK,
  // f5h
  BLANK,
  // f6h
  BLANK,
  // f7h
  {0,4,0,31,0,4,0,0},
  // f8h
  BLANK,
  // f9h
  BLANK,
  // fah
  BLANK,
  // fbh
  BLANK,
  // fch
  BLANK,
  // fdh
  BLANK,
  // feh
  BLANK,
  // ffh
  BLANK,
};
#undef BLANK
