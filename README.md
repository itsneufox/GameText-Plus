# GameText+

[![SA-MP](https://img.shields.io/badge/SA--MP-✔-green.svg)](https://www.sa-mp.mp/)
[![open.mp](https://img.shields.io/badge/open.mp-✔-green.svg)](https://www.open.mp/)
[![fixes.inc](https://img.shields.io/badge/fixes.inc-✖-red.svg)](https://github.com/pawn-lang/sa-mp-fixes)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

GameText+ is an include for both SA-MP and open.mp that improves the native GameText. 

It introduces enhanced styling options, improved functionality, and seamless integration with existing scripts.

## Key Features

- Drop-in replacement for native GameText functions
- Fader functionality integrated
- 
- Full SA-MP and open.mp compatibility

## Installation

1. Download the latest version of `gametext_plus.inc`
2. Place the file in your project's includes directory
3. Add the following line to your script:

```pawn
#include <gametext_plus>
```
4. OPTIONAL: If you want the include to override the native GameText (Style 1 -> 6), add the following line:
```pawn
#define OVERRIDE_NATIVE_GAMETEXT // This line should be added before including GameText+!
#include <gametext_plus>
```

## Platform Compatibility

- **SA-MP**: Fully compatible
- **open.mp**: Fully compatible
- **fixes.inc**: Replaces fixes.inc, it's incompatible and needs to be removed.

## Usage Examples

GameText+ seamlessly replaces native functions while providing enhanced functionality, this means no code changes are needed!
Some examples:

```pawn
// Show GameText to all players using location style
GameTextForAll("Grove Street", 5000, 8);

// Show money earned with positive money style
GameTextForPlayer(playerid, "+$50,000", 3000, 11);

// Display radio station with radio style
GameTextForPlayer(playerid, "K-Rose", 3000, 9);

```

## Style Reference Guide

[Check the wiki here](https://github.com/itsneufox/GameText-Plus/wiki)

## Testing Tools

The package includes comprehensive testing scripts for both platforms:

### SA-MP Test (`samp_test.pwn`)
### open.mp Test (`omp_test.pwn`)

### Test Commands

| Command | Description | Usage |
|---------|-------------|--------|
| `/test` | Initiate style showcase | Shows all styles sequentially |
| `/next` | View next style | Advances to next style in showcase |
| `/stop` | End demonstration | Terminates current showcase |

## Special Thanks

- @AmyrAhmady - For the idea and the opportunity.
- @edgyaf - for the textdraw's fader functionality
- @imshooter - for the creation of Style 16
- Y_Less - Original fixes.inc

## License

This project is licensed under the MIT License, making it freely available for use in both personal and commercial projects. See the LICENSE file for full details.

