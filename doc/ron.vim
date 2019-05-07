" vim syntax file
" Language: Replicated Object Notation 2.1 (open)
"
if exists("b:current_syntax")
    finish
endif

syn match COMMENT_ID /@\~/
hi COMMENT_ID ctermfg=Grey cterm=bold

syn match ERROR_ATOM /@\~\~\~\~\~\~\~\~\~\~/
hi ERROR_ATOM ctermfg=Red cterm=bold

syn match ANNOTATION /@\s*[0-9A-Za-z_\~]\{1,10}\>/
hi ANNOTATION ctermfg=Grey cterm=italic

syn match INT_ATOM /\(=\s*\)\=[+-]\=[0-9]\{1,19}\>/
hi INT_ATOM ctermfg=LightGreen

syn match NAME_ATOM /\<\(>\s*\)\=[A-Za-z_\~][0-9A-Za-z_\~]\{0,9}\>/
hi NAME_ATOM ctermfg=DarkGreen cterm=underline

syn match CODEWORD_ATOM /\<\(>\s*\)\=[a-z_][0-9a-z_]\{0,9}\>/
hi CODEWORD_ATOM ctermfg=DarkGreen cterm=bold

syn match VERSION_NAME_ATOM /\<\(>\s*\)\=[A-Z\~][0-9A-Z\~]\{0,9}\>/
hi VERSION_NAME_ATOM ctermfg=DarkGreen

syn match VARIETY_NAME_ATOM /\<\(>\s*\)\=\<\([0-9A-F][/]\)[0-9A-Za-z\~_]\{1,10}/
hi VARIETY_NAME_ATOM ctermfg=DarkGreen cterm=underline

syn match FLOAT_ATOM /\(\^\s*\)\=[+-]\{0,1}[0-9]\{0,19}[.][0-9]\{1,19}\([Ee][+-]\{0,1}[0-9]\{1,3}\)\{0,1}\>/
hi FLOAT_ATOM ctermfg=LightGreen

syn match UUID_ATOM /\(>\s*\)\=\<\([0-9A-F][/]\)\=[0-9A-Za-z_\~]\{1,10}[-+$%][0-9A-Za-z_\~]\{1,10}\>/
hi UUID_ATOM ctermfg=DarkGreen cterm=underline

syn match SPAN /(\s*\('\(\\u[0-9a-fA-F]\{4}\|\\[^\n\r]\|[^'\\\n\r]\)*'\)\=\s*[0-9]\{1,10}\s*)/
hi SPAN ctermfg=White

"syn match STRING_CHAIN /"\(\\u[0-9a-fA-F]\{4}\|\\[^\n\r]\|[^'\\\n\r]\)*"/
"hi STRING_CHAIN ctermfg=Green cterm=bold

syn match STRING_ATOM /'\(\\u[0-9a-fA-F]\{4}\|\\[^\n\r]\|[^'\\\n\r]\)*'/
hi STRING_ATOM ctermfg=Green

syn match OP_DERIVED_ID  /@\s*\([0-9A-F][/]\)\=[0-9A-Za-z_\~]\{1,10}[-][0-9A-Za-z_\~]\{1,10}\>/
hi OP_DERIVED_ID ctermfg=Cyan

syn match OP_ID  /@\s*\([0-9A-F][/]\)\=[0-9A-Za-z_\~]\{1,10}[+][0-9A-Za-z_\~]\{1,10}\>/
hi OP_ID ctermfg=Blue

syn match NAME_REF /\:\s*[0-9A-Za-z_\~]\{1,10}\>/
hi NAME_REF ctermfg=LightGrey cterm=bold

syn match REF_ID /:\s*\([0-9A-F][/]\)\=[0-9A-Za-z_\~]\{1,10}+[0-9A-Za-z_\~]\{1,10}\>/
hi REF_ID ctermfg=DarkBlue

syn match DERIVED_REF_ID /:\s*\([0-9A-F][/]\)\=[0-9A-Za-z_\~]\{1,10}[-][0-9A-Za-z_\~]\{1,10}\>/
hi DERIVED_REF_ID ctermfg=DarkCyan

syn match OPTERM /[!?,;]/
hi OPTERM ctermfg=Brown

syn match FRAMETERM /[.]\n/
hi FRAMETERM ctermfg=Red

let b:current_syntax = "ron"
