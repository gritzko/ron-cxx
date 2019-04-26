%%{

machine UTF8;

alphtype unsigned char;

action string_cont { cp = (cp<<6) | (fc&0x3f); }
action string_lead1 { cp = fc; }
action string_lead2 { cp = fc&0x1f; }
action string_lead3 { cp = fc&0xf; }
action string_lead4 { cp = fc&7; }

CONT =  0x80..0xbf @string_cont;
LEAD1 = 0x00..0x7f @string_lead1;
LEAD2 = 0xc0..0xdf @string_lead2;
LEAD3 = 0xe0..0xef @string_lead3;
LEAD4 = 0xf0..0xf7 @string_lead4;

CODEPOINT =  ( LEAD1 | LEAD2 CONT | LEAD3 CONT CONT | LEAD4 CONT CONT CONT );

UTF8 = CODEPOINT *;



}%%
