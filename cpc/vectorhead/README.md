# The Vector Head

A wireframe head digitized by hand off a P.M. Computerheft article in 1985,
rewritten in Z80 assembler, animated, and given a voice through the SP0256-AL2.

- `head.dsk` - for an emulator, or a real drive
- `head.hfe` - the same disc, for a Gotek or HxC

## Running it

You need a CPC with a disc drive and an **Amstrad SSA-1**, or **LambdaSpeak**
in SSA-1 mode. Then:

```
RUN"VH
```

**`RUN"VH`, not `RUN"HEAD`.** `VH.BAS` is what loads the letter-to-sound engine
that the typed-text speech needs; started any other way there is no prompt and
only the one sentence built into the program can be spoken. The program says so
on screen if that happens.

The head draws in about a fifth of a second. It introduces itself, and then you
can type any sentence - up to 96 characters - and have it spoken, with the lips,
the jaw, the chin and the eyes moving on the allophones. The allophones the
rules chose are listed under what you typed.

- `,` and `.` on their own change the speech rate (slow / medium / quick)
- `QUIT` on its own leaves

In an emulator:

```
mame cpc6128 -flop1 head.dsk -autoboot_delay 3 -autoboot_command 'RUN"VH\n'
```

The source is not public yet.
