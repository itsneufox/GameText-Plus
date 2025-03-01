# GameText+

[![SA-MP](https://img.shields.io/badge/SA--MP-✔-green.svg)](https://www.sa-mp.mp/)
[![open.mp](https://img.shields.io/badge/open.mp-✔-green.svg)](https://www.open.mp/)
[![fixes.inc](https://img.shields.io/badge/fixes.inc-✖-red.svg)](https://github.com/pawn-lang/sa-mp-fixes)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

GameText+ is an include for both SA-MP and open.mp that improves the native GameText. 

It introduces enhanced styling options, improved functionality, and seamless integration with existing scripts.

## Key Features

- **18 Total Styles**: Native styles (0-6) plus 11 additional custom styles (7-17)
- **Fade Effects**: Smooth fade-in and fade-out animations for text displays
- **Format Support**: Use var args in all GameText functions
- **Drop-in Replacement**: Works as a direct replacement for native GameText functions
- **Full Compatibility**: Works with both SA-MP and open.mp

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

## Compatibility

- **SA-MP**: Fully compatible
- **open.mp**: Fully compatible
- **fixes.inc**: Replaces fixes.inc, it's incompatible and needs to be removed.

## Usage Examples

GameText+ seamlessly replaces native functions while providing enhanced functionality, this means no code changes are needed!
Some examples:

```pawn
// Normal GameText

GameTextForAll("Infernus", 3000, 7);

GameTextForAll("Los Santos", 3000, 8);

GameTextForPlayer(playerid, "+$50,000", 3000, 11);

GameTextForPlayer(playerid, "-$25,000", 3000, 12);


// With var args

GameTextForAll("Mission Passed!~n~Everyone, get out!", 5000, 15);

GameTextForAll("Follow the damn train, %s!", 5000, 17, "CJ");

GameTextForPlayer(playerid, "Money: $%d", 3000, 13, 999999);

GameTextForPlayer(playerid, "Mission Passed!~n~Respect +%d!", 5000, 15, 1000);

```

## Style Reference Guide

[Check the wiki here](https://github.com/itsneufox/GameText-Plus/wiki)

## Testing Tools

The package includes a comprehensive testing script:

### open.mp Test (`omp_test.pwn`)

### Test Commands

| Command | Description | Usage |
|---------|-------------|--------|
| `/test` | Initiate style showcase | Shows all styles sequentially |
| `/stop` | End demonstration | Terminates current showcase |

## Special Thanks

- @AmyrAhmady - For the idea and the opportunity and also the updated Assembly code
- @Sreyas-Sreelal - For the Assembly code
- @edgyaf - for the textdraw's fader functionality
- @imshooter - for the creation of Style 16
- @Vince0789 - for the creation of Style 17
- @Y_Less - Original fixes.inc

## License

This project is licensed under the MIT License, making it freely available for use in both personal and commercial projects. See the LICENSE file for full details.

