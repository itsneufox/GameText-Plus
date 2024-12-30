# neufox's GameText+ (v1.0.0)

[![SA-MP Version](https://img.shields.io/badge/SA--MP-✓-green.svg)](https://www.sa-mp.mp/)
[![open.mp](https://img.shields.io/badge/open.mp-✓-green.svg)](https://www.open.mp/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

GameText+ is an include for both SA-MP and open.mp that improves the native GameText. 
It introduces enhanced styling options, improved functionality, and seamless integration with existing scripts. 
By incorporating fixes previously handled by fixes.inc, it provides a comprehensive solution for all your in-game GameText display needs.

## Key Features

- Drop-in replacement for native GameText functions
- Full SA-MP and open.mp compatibility

## Installation

1. Download the latest version of `gametext_plus.inc`
2. Place the file in your project's includes directory
3. Add the following line to your script:

```pawn
#include <gametext_plus>
```

## Platform Compatibility

- **SA-MP**: Fully compatible
- **open.mp**: Fully compatible
- **fixes.inc**: Replaces fixes.inc, so incompatible.

## Usage Examples

GameText+ seamlessly replaces native functions while providing enhanced functionality:

```pawn
// Show GameText to all players using location style
GameTextForAll("Grove Street", 5000, 8);

// Show money earned with positive money style
GameTextForPlayer(playerid, "+$50,000", 3000, 11);

// Display radio station with radio style
GameTextForPlayer(playerid, "K-Rose", 3000, 9);

```

## Style Reference Guide

| Style ID | Description | Example Usage |
|----------|-------------|---------------|
| 0-6 | Native SA-MP styles | Mission passed, basic messages |
| 7 | Vehicle Name | Vehicle names, model displays |
| 8 | Location Name | Area names, territory notifications |
| 9 | Radio Station | Radio station names |
| 10 | Radio Switch | Station change |
| 11 | Positive Money | Money gained, rewards |
| 12 | Negative Money | Money lost, fines |
| 13 | Stunt Bonus | Stunt rewards, bonus points |
| 14 | Clock | Time display |
| 15 | Notification | General UI notifications |

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

- AmyrAhmady
- edgyaf
- Y_Less - Original fixes.inc

## License

This project is licensed under the MIT License, making it freely available for use in both personal and commercial projects. See the LICENSE file for full details.

## Support

For issues, feature requests, or general support:
- Open an issue on GitHub
- Join the Discord community
- Check the wiki for additional documentation

Remember to star the repository if you find it useful!
