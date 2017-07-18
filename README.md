# Shroud

*Doesn't work*

This is a C source code obfuscator, copied from
https://sourceforge.net/projects/cshroud/ in hope I'd be able to use it for
obfuscating the C code.

Unfortunately this code was written for MS-DOS, and the C grammar from
`clex.l` is written in an old version of LEX for DOS. Also it appeared like
the C grammar wasn't up-to-date, so it's likely to cause trouble for new
(post 2000) ANSI C code.
