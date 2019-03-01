%%{

machine UTF8;

alphtype unsigned char;

CONT =  0x80..0xbf;
LEAD1 = 0x00..0x7f;
LEAD2 = 0xc0..0xdf;
LEAD3 = 0xe0..0xef;
LEAD4 = 0xf0..0xf7;

CODEPOINT =  ( LEAD1 | LEAD2 CONT | LEAD3 CONT CONT | LEAD4 CONT CONT CONT );

UTF8 = CODEPOINT *;



}%%
