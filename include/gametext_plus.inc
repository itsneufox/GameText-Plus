/*
 *   _______  _______  __   __  _______  _______  _______  __   __  _______    _
 *  |    ___||   _   ||  |_|  ||    ___||_     _||    ___||  |_|  ||_     _| _| |_
 *  |   | __ |  | |  ||       ||   |___   |   |  |   |___ |       |  |   |  |_   _|
 *  |   ||  ||  |_|  ||       ||    ___|  |   |  |    ___| |     |   |   |    |_|
 *  |   |_| ||   _   || ||_|| ||   |___   |   |  |   |___ |   _   |  |   |
 *  |_______||__| |__||_|   |_||_______|  |___|  |_______||__| |__|  |___|
 *
 *  GameText+ created and maintained by itsneufox (v1.2.0)

 *  Textdraws originally created by Y_Less for fixes.inc
 *  Fader functionality created by edgyaf
 *  Style 16 created by imshooter
 *  Assembly code created by __SyS__ and iAmir
 *  Style 17 created by Vince0789
 *  All the improvements on the include by Nexius

 *  Thank you to y'all! <3
 */

//==========================================================================
//                         HEADER GUARDS & CHECKS
//==========================================================================

#if defined _INC_gametext_plus
    #endinput
#endif

#define _INC_gametext_plus

#if defined _INC_SAMP_Community_fixes
    #error "Incompatibility detected: 'fixes.inc'. Please remove it from your project. All GameText functionalities are now provided by GameText+ include."
#endif

#if !defined _INC_open_mp
    #if !defined TEXT_DRAW_ALIGN
        #define TEXT_DRAW_ALIGN: _:
    #endif

    #if !defined TEXT_DRAW_FONT
        #define TEXT_DRAW_FONT: _:
    #endif

    #if !defined PlayerTextDrawBoxColour
        #define PlayerTextDrawBoxColour PlayerTextDrawBoxColor
    #endif

    #if !defined PlayerTextDrawBackgroundColour
        #define PlayerTextDrawBackgroundColour PlayerTextDrawBackgroundColor
    #endif

    #if !defined PlayerTextDrawColour
        #define PlayerTextDrawColour PlayerTextDrawColor
    #endif

    #if !defined TextDrawBoxColour
        #define TextDrawBoxColour TextDrawBoxColor
    #endif

    #if !defined TextDrawBackgroundColour
        #define TextDrawBackgroundColour TextDrawBackgroundColor
    #endif

    #if !defined TextDrawColour
        #define TextDrawColour TextDrawColor
    #endif
#else // defined _INC_open_mp
    #if __namemax > 31
        #define PlayerTextDrawGetBackgroundCol PlayerTextDrawGetBackgroundColour
    #endif
#endif

//! WILD COMPILER appeared! Compiler used WARNINGS about const correctness!
#if !defined _INC_open_mp && defined __PawnBuild
    #if __PawnBuild > 4
        #pragma warning push
        #pragma warning disable 239
    #endif
#endif
//! Developer used PRAGMA! It's super effective, compiler's warning level fell sharply!

//==========================================================================
//                              MAIN CONSTANTS
//==========================================================================

#if defined OVERRIDE_NATIVE_GAMETEXT
    #define MIN_GAMETEXT_STYLE      0
#else
    #define MIN_GAMETEXT_STYLE      7
#endif

#define     MAX_GAMETEXT_STYLE      17
#define     GAMETEXT_LEGACY         6

#define     MAX_GAMETEXT_LENGTH     256

#define     FADE_INTERVAL           50
#define     FADE_ALPHA_STEP         0x12

#define     GTP_LEGACY_BG_ALPHA     0xAA
#define     GTP_FIXED_BG_ALPHA      0xFF
#define     GTP_LEGACY_BOX_ALPHA    0x80
#define     GTP_FIXED_BOX_ALPHA     0xDD

//==========================================================================
//                              DATA STRUCTURES
//==========================================================================

enum e_STYLE_DATA
{
    Float:e_pos[2],
    Float:e_lettersize[2],
    Float:e_textsize[2],
    e_color,
    e_bgalpha,
    e_boxalpha,
    e_shadow,
    e_outline,
    TEXT_DRAW_ALIGN:e_alignment,
    TEXT_DRAW_FONT:e_font,
    bool:e_proportional,
    bool:e_fadein,
    bool:e_fadeout
};

static const gs_StyleList[MAX_GAMETEXT_STYLE + 1][e_STYLE_DATA] =
{
    // Native Styles (0-6)

    // Style 0 (Mission Passed)
    {{320.0, 214.0}, {1.3, 3.6}, {200.0, 620.0}, 0x906210FF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:3, true, true, true},
    // Style 1 (Mission Name)
    {{620.0, 310.0}, {1.0, 2.6}, {10.00, 200.0}, 0x906210FF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:3, TEXT_DRAW_FONT:3, true, false, true},
    // Style 2 (Wasted Style)
    {{320.0, 156.0}, {2.1, 4.2}, {200.0, 620.0}, 0xE1E1E1FF, 0x000000AA, 0x00, 0, 3, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:0, true, true, true},
    // Style 3 (Native Variations)
    {{320.0, 154.5}, {0.6, 2.75}, {200.0, 620.0}, 0x906210FF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:2, true, false, false},
    // Style 4 (Native Variations)
    {{320.0, 115.5}, {0.6, 2.75}, {200.0, 620.0}, 0x906210FF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:2, true, false, false},
    // Style 5 (Native Variations)
    {{320.0, 217.0}, {0.6, 2.75}, {200.0, 620.0}, 0xE1E1E1FF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:2, true, false, false},
    // Style 6 (Native Variations)
    {{320.0, 60.0}, {1.0, 3.6}, {200.0, 620.0}, 0xACCBF1FF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:3, true, false, false},

    // Enhanced Styles (7-17)

    // Style 7 (Vehicle Name)
    {{608.0, 344.0}, {1.0, 3.0}, {10.0, 200.0}, 0x36682CFF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:3, TEXT_DRAW_FONT:2, true, true, true},
    // Style 8 (Location Name)
    {{608.0, 385.8}, {1.2, 3.8}, {10.0, 200.0}, 0xACCBF1FF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:3, TEXT_DRAW_FONT:0, true, true, true},
    // Style 9 (Radio Station Name)
    {{320.0, 22.0}, {0.6, 1.8}, {200.0, 620.0}, 0x906210FF, 0x000000AA, 0x00, 0, 1, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:2, true, false, false},
    // Style 10 (Radio Station Name While Switching)
    {{320.0, 22.0}, {0.6, 1.8}, {200.0, 620.0}, 0x969696FF, 0x000000AA, 0x00, 0, 1, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:2, true, false, false},
    // Style 11 (Positive Money)
    {{608.0, 77.0}, {0.55, 2.2}, {10.0, 200.0}, 0x36682CFF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:3, TEXT_DRAW_FONT:3, false, false, false},
    // Style 12 (Negative Money)
    {{608.0, 77.0}, {0.55, 2.2}, {10.0, 200.0}, 0xB4191DFF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:3, TEXT_DRAW_FONT:3, false, false, false},
    // Style 13 (Stunt Bonus)
    {{380.0, 341.15}, {0.58, 2.42}, {40.0, 460.0}, 0xE1E1E1FF, 0x000000AA, 0x00, 2, 0, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:1, true, false, false},
    // Style 14 (In-Game Clock)
    {{608.0, 22.0}, {0.55, 2.2}, {400.0, 20.0}, 0xE1E1E1FF, 0x000000AA, 0x00, 0, 2, TEXT_DRAW_ALIGN:3, TEXT_DRAW_FONT:3, false, false, false},
    // Style 15 (Notification Popup)
    {{34.0, 28.0}, {0.52, 2.2}, {230.5, 200.0}, 0xFFFFFF96, 0x00, 0x00000080, 0, 0, TEXT_DRAW_ALIGN:1, TEXT_DRAW_FONT:1, true, false, true},
    // Style 16 (Lower Notification)
    {{28.0, 150.0}, {0.42, 1.8}, {190.0, 200.0}, 0xFFFFFF96, 0x00, 0x00000080, 0, 0, TEXT_DRAW_ALIGN:1, TEXT_DRAW_FONT:1, true, false, true},
    // Style 17 (Subtitles)
    {{320.0, 368.1}, {0.58, 2.38}, {40.0, 460.0}, 0xE1E1E1FF, 0x000000AA, 0x00, 2, 0, TEXT_DRAW_ALIGN:2, TEXT_DRAW_FONT:1, true, false, false}
};

//==========================================================================
//                                 VARIABLES
//==========================================================================

static
    Text:gs_GameTextStyle[MAX_GAMETEXT_STYLE + 1],
    PlayerText:gs_PlayerGameTextStyle[MAX_PLAYERS][MAX_GAMETEXT_STYLE + 1],
    gs_GameTextTimer[MAX_GAMETEXT_STYLE + 1][MAX_PLAYERS + 1],
    gs_GameTextFadeOutTimer[MAX_GAMETEXT_STYLE + 1][MAX_PLAYERS + 1],
    gs_GameTextFadeInTimer[MAX_GAMETEXT_STYLE + 1][MAX_PLAYERS + 1],
    gs_GameTextOriginalColor[MAX_GAMETEXT_STYLE + 1][MAX_PLAYERS + 1],
    gs_GameTextOriginalBGColor[MAX_GAMETEXT_STYLE + 1][MAX_PLAYERS + 1],
    gs_GameTextOriginalBoxColor[MAX_GAMETEXT_STYLE + 1][MAX_PLAYERS + 1],
    bool:gs_GameTextIsFadingOut[MAX_GAMETEXT_STYLE + 1][MAX_PLAYERS + 1],
    bool:gs_GameTextIsFadingIn[MAX_GAMETEXT_STYLE + 1][MAX_PLAYERS + 1],
    bool:gs_GameTextNeedsAlphaFix[MAX_PLAYERS]
;

//==========================================================================
//                            FUNCTION REMAPPING
//==========================================================================

#if defined _ALS_GameTextForPlayer
    #error _ALS_GameTextForPlayer defined
#endif
native ORIG_GameTextForPlayer(playerid, const string[], time, style) = GameTextForPlayer;

#if defined _ALS_GameTextForAll
    #error _ALS_GameTextForAll defined
#endif
native ORIG_GameTextForAll(const string[], time, style) = GameTextForAll;

//==========================================================================
//                          TEXTDRAW COLOR STORAGE
//==========================================================================

/*
 * Used for SA-MP compatibility to store and retrieve TextDraw colors since
 * SA-MP doesn't provide native getter functions for TextDraw colors
 */
#if !defined _INC_open_mp
    static
        gs_TextDrawColors[MAX_TEXT_DRAWS],
        gs_TextDrawBGColors[MAX_TEXT_DRAWS],
        gs_TextDrawBoxColors[MAX_TEXT_DRAWS],
        gs_PlayerTextDrawColors[MAX_PLAYERS][MAX_PLAYER_TEXT_DRAWS],
        gs_PlayerTextDrawBGColors[MAX_PLAYERS][MAX_PLAYER_TEXT_DRAWS],
        gs_PlayerTextDrawBoxColors[MAX_PLAYERS][MAX_PLAYER_TEXT_DRAWS]
    ;

    stock SaveTextDrawColour(Text:text, color)
    {
        gs_TextDrawColors[_:text] = color;
        return color;
    }

    stock SaveTextDrawBackgroundColour(Text:text, color)
    {
        gs_TextDrawBGColors[_:text] = color;
        return color;
    }

    stock SaveTextDrawBoxColour(Text:text, color)
    {
        gs_TextDrawBoxColors[_:text] = color;
        return color;
    }

    stock SavePlayerTextDrawColour(playerid, PlayerText:text, color)
    {
        gs_PlayerTextDrawColors[playerid][_:text] = color;
        return color;
    }

    stock SavePlayerTextDrawBackgroundCol(playerid, PlayerText:text, color)
    {
        gs_PlayerTextDrawBGColors[playerid][_:text] = color;
        return color;
    }

    stock SavePlayerTextDrawBoxColour(playerid, PlayerText:text, color)
    {
        gs_PlayerTextDrawBoxColors[playerid][_:text] = color;
        return color;
    }

    stock TextDrawGetColour(Text:text)
    {
        return gs_TextDrawColors[_:text];
    }

    stock TextDrawGetBackgroundColour(Text:text)
    {
        return gs_TextDrawBGColors[_:text];
    }

    stock TextDrawGetBoxColour(Text:text)
    {
        return gs_TextDrawBoxColors[_:text];
    }

    stock PlayerTextDrawGetColour(playerid, PlayerText:text)
    {
        return gs_PlayerTextDrawColors[playerid][_:text];
    }

    stock PlayerTextDrawGetBackgroundCol(playerid, PlayerText:text)
    {
        return gs_PlayerTextDrawBGColors[playerid][_:text];
    }

    stock PlayerTextDrawGetBoxColour(playerid, PlayerText:text)
    {
        return gs_PlayerTextDrawBoxColors[playerid][_:text];
    }
#endif

//==========================================================================
//                              CORE FUNCTIONS
//==========================================================================

/*
 * Converts known legacy alpha values to newer client equivalents
 * @param alpha - Alpha channel value (0-255)
 */
static stock GTP_FixLegacyAlphaValue(alpha)
{
    switch (alpha)
    {
        case GTP_LEGACY_BG_ALPHA: return GTP_FIXED_BG_ALPHA;
        case GTP_LEGACY_BOX_ALPHA: return GTP_FIXED_BOX_ALPHA;
    }
    return alpha;
}

/*
 * Updates only the alpha channel in a color if it matches a legacy value
 * @param colour - RGBA color value
 */
static stock GTP_FixLegacyAlphaColour(colour)
{
    return (colour & ~0xFF) | GTP_FixLegacyAlphaValue(colour & 0xFF);
}

/*
 * Applies alpha compatibility fix for player textdraws
 * @param playerid - Target player
 * @param text - PlayerTextDraw handle
 */
static stock GTP_ApplyPlayerAlphaFix(playerid, PlayerText:text)
{
    if (!gs_GameTextNeedsAlphaFix[playerid]) return false;

    new colour = PlayerTextDrawGetBackgroundCol(playerid, text);
    new fixed = GTP_FixLegacyAlphaColour(colour);
    if (fixed != colour)
    {
        #if !defined _INC_open_mp
            SavePlayerTextDrawBackgroundCol(playerid, text, fixed);
        #endif
        PlayerTextDrawBackgroundColour(playerid, text, fixed);
    }

    colour = PlayerTextDrawGetBoxColour(playerid, text);
    fixed = GTP_FixLegacyAlphaColour(colour);
    if (fixed != colour)
    {
        #if !defined _INC_open_mp
            SavePlayerTextDrawBoxColour(playerid, text, fixed);
        #endif
        PlayerTextDrawBoxColour(playerid, text, fixed);
    }

    return true;
}

/*
 * Creates TextDraw styles either globally or for a specific player
 * @param playerid - INVALID_PLAYER_ID for global styles, player ID for player-specific styles
 */
static stock GameTextEx(playerid)
{
    if (playerid == INVALID_PLAYER_ID)
    {
        for (new style = MIN_GAMETEXT_STYLE, Text:t; style <= MAX_GAMETEXT_STYLE; style++)
        {
            gs_GameTextStyle[style] = TextDrawCreate(gs_StyleList[style][e_pos][0], gs_StyleList[style][e_pos][1], " ");
            t = gs_GameTextStyle[style];

            TextDrawLetterSize(t, gs_StyleList[style][e_lettersize][0], gs_StyleList[style][e_lettersize][1]);
            TextDrawAlignment(t, gs_StyleList[style][e_alignment]);
            #if !defined _INC_open_mp
                TextDrawColour(t, SaveTextDrawColour(t, gs_StyleList[style][e_color]));
                TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, gs_StyleList[style][e_bgalpha]));
                TextDrawBoxColour(t, SaveTextDrawBoxColour(t, gs_StyleList[style][e_boxalpha]));
            #else
                TextDrawColour(t, gs_StyleList[style][e_color]);
                TextDrawBackgroundColour(t, gs_StyleList[style][e_bgalpha]);
                TextDrawBoxColour(t, gs_StyleList[style][e_boxalpha]);
            #endif
            TextDrawSetShadow(t, gs_StyleList[style][e_shadow]);
            TextDrawSetOutline(t, gs_StyleList[style][e_outline]);
            TextDrawFont(t, gs_StyleList[style][e_font]);
            TextDrawSetProportional(t, gs_StyleList[style][e_proportional]);
            TextDrawTextSize(t, gs_StyleList[style][e_textsize][0], gs_StyleList[style][e_textsize][1]);
            TextDrawUseBox(t, true);
        }
    }
    else
    {
        for (new style = MIN_GAMETEXT_STYLE, PlayerText:pt; style <= MAX_GAMETEXT_STYLE; style++)
        {
            gs_PlayerGameTextStyle[playerid][style] = CreatePlayerTextDraw(playerid, gs_StyleList[style][e_pos][0], gs_StyleList[style][e_pos][1], " ");
            pt = gs_PlayerGameTextStyle[playerid][style];

            PlayerTextDrawLetterSize(playerid, pt, gs_StyleList[style][e_lettersize][0], gs_StyleList[style][e_lettersize][1]);
            PlayerTextDrawAlignment(playerid, pt, gs_StyleList[style][e_alignment]);
            #if !defined _INC_open_mp
                PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, gs_StyleList[style][e_color]));
                PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundCol(playerid, pt, gs_StyleList[style][e_bgalpha]));
                PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, gs_StyleList[style][e_boxalpha]));
            #else
                PlayerTextDrawColour(playerid, pt, gs_StyleList[style][e_color]);
                PlayerTextDrawBackgroundColour(playerid, pt, gs_StyleList[style][e_bgalpha]);
                PlayerTextDrawBoxColour(playerid, pt, gs_StyleList[style][e_boxalpha]);
            #endif
            PlayerTextDrawSetShadow(playerid, pt, gs_StyleList[style][e_shadow]);
            PlayerTextDrawSetOutline(playerid, pt, gs_StyleList[style][e_outline]);
            PlayerTextDrawFont(playerid, pt, gs_StyleList[style][e_font]);
            PlayerTextDrawSetProportional(playerid, pt, gs_StyleList[style][e_proportional]);
            PlayerTextDrawTextSize(playerid, pt, gs_StyleList[style][e_textsize][0], gs_StyleList[style][e_textsize][1]);
            PlayerTextDrawUseBox(playerid, pt, true);
        }
    }
    return true;
}

/*
 * Timer callback to hide gametext after specified duration
 * @param playerid - Player ID (MAX_PLAYERS for global TextDraws)
 * @param styleid - Style ID of the gametext to hide
 */
forward OnHideGameText(playerid, styleid);
public OnHideGameText(playerid, styleid)
{
    if (!gs_StyleList[styleid][e_fadeout])
    {
        if (playerid == MAX_PLAYERS)
        {
            TextDrawHideForAll(gs_GameTextStyle[styleid]);
            gs_GameTextTimer[styleid][MAX_PLAYERS] = 0;
        }
        else
        {
            PlayerTextDrawHide(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
            gs_GameTextTimer[styleid][playerid] = 0;
        }
        return 1;
    }

    if (!gs_GameTextIsFadingOut[styleid][playerid])
    {
        if (playerid == MAX_PLAYERS)
        {
            gs_GameTextOriginalColor[styleid][MAX_PLAYERS] = TextDrawGetColour(gs_GameTextStyle[styleid]);
            gs_GameTextOriginalBGColor[styleid][MAX_PLAYERS] = TextDrawGetBackgroundColour(gs_GameTextStyle[styleid]);
            gs_GameTextOriginalBoxColor[styleid][MAX_PLAYERS] = TextDrawGetBoxColour(gs_GameTextStyle[styleid]);
        }
        else
        {
            gs_GameTextOriginalColor[styleid][playerid] = PlayerTextDrawGetColour(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
            gs_GameTextOriginalBGColor[styleid][playerid] = PlayerTextDrawGetBackgroundCol(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
            gs_GameTextOriginalBoxColor[styleid][playerid] = PlayerTextDrawGetBoxColour(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
        }
        gs_GameTextIsFadingOut[styleid][playerid] = true;
    }

    OnFadeOutGameText(playerid, styleid,
        gs_GameTextOriginalColor[styleid][playerid] & 0xFF,
        gs_GameTextOriginalBGColor[styleid][playerid] & 0xFF,
        gs_GameTextOriginalBoxColor[styleid][playerid] & 0xFF
    );

    return 1;
}

/*
 * Destroys TextDraw styles either globally or for a specific player
 * @param playerid - INVALID_PLAYER_ID for global styles, player ID for player-specific styles
 */
static stock OnDestroyGameText(playerid)
{
    if (playerid == INVALID_PLAYER_ID)
    {
        for (new style = MIN_GAMETEXT_STYLE; style <= MAX_GAMETEXT_STYLE; style++)
        {
            TextDrawDestroy(gs_GameTextStyle[style]);
        }
    }
    else
    {
        for (new style = MIN_GAMETEXT_STYLE; style <= MAX_GAMETEXT_STYLE; style++)
        {
            PlayerTextDrawDestroy(playerid, gs_PlayerGameTextStyle[playerid][style]);
        }
    }
    return true;
}

/*
 * Enhanced GameTextForPlayer with support for extended styles
 * @param playerid - Target player
 * @param text - Text to display
 * @param time - Duration in milliseconds (0 for permanent)
 * @param style - Style ID (0-6 for native, 7-17 for extended)
 */
stock GTP_GameTextForPlayer(playerid, const text[], time, style, {Float, _}:...)
{
    if (!IsPlayerConnected(playerid)) return false;

    #if defined OVERRIDE_NATIVE_GAMETEXT
        if (!(MIN_GAMETEXT_STYLE <= style <= MAX_GAMETEXT_STYLE)) return false;
    #else
        // Allow native styles 0-6 to pass through to native functions
        if (!(0 <= style <= MAX_GAMETEXT_STYLE)) return false;
    #endif

    const static_args = 4;
    const args_diff = (3 - static_args) * 4; // format's static args end at index 3, current function's end at static_arg_count. Also * 4 because cells are 4 bytes

    static args, temp_string[MAX_GAMETEXT_LENGTH];
    const string_size = sizeof(temp_string);

    args = numargs();

    #if !defined OVERRIDE_NATIVE_GAMETEXT
        if (style <= GAMETEXT_LEGACY)
        {
            if (args > static_args)
            {
                while (--args >= 4)
                {
                    #emit LCTRL 5
                    #emit LOAD.alt args
                    #emit SHL.C.alt 2
                    #emit ADD.C 12
                    #emit ADD
                    #emit LOAD.I
                    #emit PUSH.pri
                }

                #emit PUSH.S text
                #emit PUSH.C string_size
                #emit PUSH.C temp_string
                #emit LOAD.S.pri 8
                #emit ADD.C args_diff
                #emit PUSH.pri
                #emit SYSREQ.C format
                #emit LCTRL 5
                #emit SCTRL 4

                return ORIG_GameTextForPlayer(playerid, temp_string, time, style);
            }
            else
            {
                return ORIG_GameTextForPlayer(playerid, text, time, style);
            }
        }
    #else
        #pragma unused temp_string
    #endif

    if (gs_GameTextTimer[style][playerid])
    {
        KillTimer(gs_GameTextTimer[style][playerid]);
        gs_GameTextTimer[style][playerid] = 0;
    }

    if (gs_GameTextFadeInTimer[style][playerid])
    {
        KillTimer(gs_GameTextFadeInTimer[style][playerid]);
        gs_GameTextFadeInTimer[style][playerid] = 0;
    }

    if (gs_GameTextFadeOutTimer[style][playerid])
    {
        KillTimer(gs_GameTextFadeOutTimer[style][playerid]);
        gs_GameTextFadeOutTimer[style][playerid] = 0;
    }

    GTP_ApplyPlayerAlphaFix(playerid, gs_PlayerGameTextStyle[playerid][style]);

    if (args > static_args)
    {
        static string[MAX_GAMETEXT_LENGTH];
        const string_size = sizeof(string);

        while (--args >= 4)
        {
            #emit LCTRL 5
            #emit LOAD.alt args
            #emit SHL.C.alt 2
            #emit ADD.C 12
            #emit ADD
            #emit LOAD.I
            #emit PUSH.pri
        }

        #emit PUSH.S text
        #emit PUSH.C string_size
        #emit PUSH.C string
        #emit LOAD.S.pri 8
        #emit ADD.C args_diff
        #emit PUSH.pri
        #emit SYSREQ.C format
        #emit LCTRL 5
        #emit SCTRL 4

        PlayerTextDrawSetString(playerid, gs_PlayerGameTextStyle[playerid][style], string);
    }
    else
    {
        static string[MAX_GAMETEXT_LENGTH];
        format(string, sizeof(string), "%s", text);
        PlayerTextDrawSetString(playerid, gs_PlayerGameTextStyle[playerid][style], string);
    }

    if (!gs_StyleList[style][e_fadein])
    {
        if (gs_StyleList[style][e_fadeout] && gs_GameTextIsFadingOut[style][playerid])
        {
            PlayerTextDrawColour(playerid, gs_PlayerGameTextStyle[playerid][style], gs_GameTextOriginalColor[style][playerid]);
            PlayerTextDrawBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][style], gs_GameTextOriginalBGColor[style][playerid]);
            PlayerTextDrawBoxColour(playerid, gs_PlayerGameTextStyle[playerid][style], gs_GameTextOriginalBoxColor[style][playerid]);
        }

        gs_GameTextIsFadingOut[style][playerid] = false;
        gs_GameTextIsFadingIn[style][playerid] = false;

        PlayerTextDrawShow(playerid, gs_PlayerGameTextStyle[playerid][style]);
    }
    else
    {
        gs_GameTextOriginalColor[style][playerid] = PlayerTextDrawGetColour(playerid, gs_PlayerGameTextStyle[playerid][style]);
        gs_GameTextOriginalBGColor[style][playerid] = PlayerTextDrawGetBackgroundCol(playerid, gs_PlayerGameTextStyle[playerid][style]);
        gs_GameTextOriginalBoxColor[style][playerid] = PlayerTextDrawGetBoxColour(playerid, gs_PlayerGameTextStyle[playerid][style]);

        gs_GameTextIsFadingOut[style][playerid] = false;
        gs_GameTextIsFadingIn[style][playerid] = true;

        OnFadeInGameText(playerid, style, 0, 0, 0);
    }

    if (time > 0)
    {
        gs_GameTextTimer[style][playerid] = SetTimerEx("OnHideGameText", time, false, "dd", playerid, style);
    }

    return true;
}

/*
 * Enhanced GameTextForAll with support for extended styles
 * @param text - Text to display
 * @param time - Duration in milliseconds (0 for permanent)
 * @param style - Style ID (0-6 for native, 7-17 for extended)
 */
stock GTP_GameTextForAll(const text[], time, style, {Float, _}:...)
{
    #if defined OVERRIDE_NATIVE_GAMETEXT
        if (!(MIN_GAMETEXT_STYLE <= style <= MAX_GAMETEXT_STYLE)) return false;
    #else
        // Allow native styles 0-6 to pass through to native functions
        if (!(0 <= style <= MAX_GAMETEXT_STYLE)) return false;
    #endif

    const static_args = 3;
    const args_diff = (3 - static_args) * 4; // format's static args end at index 3, current function's end at static_arg_count. Also * 4 because cells are 4 bytes

    static args, temp_string[MAX_GAMETEXT_LENGTH];
    const string_size = sizeof(temp_string);

    args = numargs();

    #if !defined OVERRIDE_NATIVE_GAMETEXT
        if (style <= GAMETEXT_LEGACY)
        {
            if (args > static_args)
            {
                while (--args >= 3)
                {
                    #emit LCTRL 5
                    #emit LOAD.alt args
                    #emit SHL.C.alt 2
                    #emit ADD.C 12
                    #emit ADD
                    #emit LOAD.I
                    #emit PUSH.pri
                }

                #emit PUSH.S text
                #emit PUSH.C string_size
                #emit PUSH.C temp_string
                #emit LOAD.S.pri 8
                #emit ADD.C args_diff
                #emit PUSH.pri
                #emit SYSREQ.C format
                #emit LCTRL 5
                #emit SCTRL 4

                return ORIG_GameTextForAll(temp_string, time, style);
            }
            else
            {
                return ORIG_GameTextForAll(text, time, style);
            }
        }
    #else
        #pragma unused temp_string
    #endif

    if (gs_GameTextTimer[style][MAX_PLAYERS])
    {
        KillTimer(gs_GameTextTimer[style][MAX_PLAYERS]);
        gs_GameTextTimer[style][MAX_PLAYERS] = 0;
    }

    if (gs_GameTextFadeInTimer[style][MAX_PLAYERS])
    {
        KillTimer(gs_GameTextFadeInTimer[style][MAX_PLAYERS]);
        gs_GameTextFadeInTimer[style][MAX_PLAYERS] = 0;
    }

    if (gs_GameTextFadeOutTimer[style][MAX_PLAYERS])
    {
        KillTimer(gs_GameTextFadeOutTimer[style][MAX_PLAYERS]);
        gs_GameTextFadeOutTimer[style][MAX_PLAYERS] = 0;
    }

    for (new playerid; playerid < MAX_PLAYERS; playerid++)
    {
        if (!IsPlayerConnected(playerid) || !gs_GameTextNeedsAlphaFix[playerid]) continue;

        TextDrawHideForAll(gs_GameTextStyle[style]);

        if (args > static_args)
        {
            static string[256];
            const string_size = sizeof(string);

            while (--args >= 3)
            {
                #emit LCTRL 5
                #emit LOAD.alt args
                #emit SHL.C.alt 2
                #emit ADD.C 12
                #emit ADD
                #emit LOAD.I
                #emit PUSH.pri
            }

            #emit PUSH.S text
            #emit PUSH.C string_size
            #emit PUSH.C string
            #emit LOAD.S.pri 8
            #emit ADD.C args_diff
            #emit PUSH.pri
            #emit SYSREQ.C format
            #emit LCTRL 5
            #emit SCTRL 4

            for (new i; i < MAX_PLAYERS; i++)
            {
                if (!IsPlayerConnected(i)) continue;
                GTP_GameTextForPlayer(i, string, time, style);
            }
        }
        else
        {
            static string[MAX_GAMETEXT_LENGTH];
            format(string, sizeof(string), "%s", text);

            for (new i; i < MAX_PLAYERS; i++)
            {
                if (!IsPlayerConnected(i)) continue;
                GTP_GameTextForPlayer(i, string, time, style);
            }
        }
        return true;
    }

    if (args > static_args)
    {
        static string[MAX_GAMETEXT_LENGTH];
        const string_size = sizeof(string);

        while (--args >= 3)
        {
            #emit LCTRL 5
            #emit LOAD.alt args
            #emit SHL.C.alt 2
            #emit ADD.C 12
            #emit ADD
            #emit LOAD.I
            #emit PUSH.pri
        }

        #emit PUSH.S text
        #emit PUSH.C string_size
        #emit PUSH.C string
        #emit LOAD.S.pri 8
        #emit ADD.C args_diff
        #emit PUSH.pri
        #emit SYSREQ.C format
        #emit LCTRL 5
        #emit SCTRL 4

        TextDrawSetString(gs_GameTextStyle[style], string);
    }
    else
    {
        static string[MAX_GAMETEXT_LENGTH];
        format(string, sizeof(string), "%s", text);
        TextDrawSetString(gs_GameTextStyle[style], string);
    }

    if (!gs_StyleList[style][e_fadein])
    {
        if (gs_StyleList[style][e_fadeout] && gs_GameTextIsFadingOut[style][MAX_PLAYERS])
        {
            TextDrawColour(gs_GameTextStyle[style], gs_GameTextOriginalColor[style][MAX_PLAYERS]);
            TextDrawBackgroundColour(gs_GameTextStyle[style], gs_GameTextOriginalBGColor[style][MAX_PLAYERS]);
            TextDrawBoxColour(gs_GameTextStyle[style], gs_GameTextOriginalBoxColor[style][MAX_PLAYERS]);
        }

        gs_GameTextIsFadingOut[style][MAX_PLAYERS] = false;
        gs_GameTextIsFadingIn[style][MAX_PLAYERS] = false;

        TextDrawShowForAll(gs_GameTextStyle[style]);
    }
    else
    {
        gs_GameTextOriginalColor[style][MAX_PLAYERS] = TextDrawGetColour(gs_GameTextStyle[style]);
        gs_GameTextOriginalBGColor[style][MAX_PLAYERS] = TextDrawGetBackgroundColour(gs_GameTextStyle[style]);
        gs_GameTextOriginalBoxColor[style][MAX_PLAYERS] = TextDrawGetBoxColour(gs_GameTextStyle[style]);

        gs_GameTextIsFadingOut[style][MAX_PLAYERS] = false;
        gs_GameTextIsFadingIn[style][MAX_PLAYERS] = true;

        OnFadeInGameText(MAX_PLAYERS, style, 0, 0, 0);
    }

    if (time > 0)
    {
        gs_GameTextTimer[style][MAX_PLAYERS] = SetTimerEx("OnHideGameText", time, false, "dd", MAX_PLAYERS, style);
    }

    return true;
}

/*
 * Timer callback to fade in gametext with a smooth animation
 * @param playerid - Player ID (MAX_PLAYERS for global TextDraws)
 * @param styleid - Style ID of the gametext
 * @param colourAlpha - Current alpha value of the text color
 * @param backgroundColourAlpha - Current alpha value of the background color
 * @param boxColourAlpha - Current alpha value of the box color
 */
forward OnFadeInGameText(playerid, styleid, colourAlpha, backgroundColourAlpha, boxColourAlpha);
public OnFadeInGameText(playerid, styleid, colourAlpha, backgroundColourAlpha, boxColourAlpha)
{
    new targetAlpha = (gs_GameTextOriginalColor[styleid][playerid] & 0xFF);

    if (colourAlpha < targetAlpha)
    {
        colourAlpha += clamp(targetAlpha / FADE_ALPHA_STEP, 1, targetAlpha - colourAlpha);
    }

    targetAlpha = (gs_GameTextOriginalBGColor[styleid][playerid] & 0xFF);
    if (backgroundColourAlpha < targetAlpha)
    {
        backgroundColourAlpha += clamp(targetAlpha / FADE_ALPHA_STEP, 1, targetAlpha - backgroundColourAlpha);
    }

    targetAlpha = (gs_GameTextOriginalBoxColor[styleid][playerid] & 0xFF);
    if (boxColourAlpha < targetAlpha)
    {
        boxColourAlpha += clamp(targetAlpha / FADE_ALPHA_STEP, 1, targetAlpha - boxColourAlpha);
    }

    if (colourAlpha < (gs_GameTextOriginalColor[styleid][playerid] & 0xFF) ||
        backgroundColourAlpha < (gs_GameTextOriginalBGColor[styleid][playerid] & 0xFF) ||
        boxColourAlpha < (gs_GameTextOriginalBoxColor[styleid][playerid] & 0xFF))
    {
        if (playerid == MAX_PLAYERS)
        {
            TextDrawColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalColor[styleid][MAX_PLAYERS] & ~0xFF) | colourAlpha);
            TextDrawBackgroundColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalBGColor[styleid][MAX_PLAYERS] & ~0xFF) | backgroundColourAlpha);
            if (gs_StyleList[styleid][e_fadein])
            {
                TextDrawBoxColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalBoxColor[styleid][MAX_PLAYERS] & ~0xFF) | boxColourAlpha);
            }
            TextDrawShowForAll(gs_GameTextStyle[styleid]);
        }
        else
        {
            PlayerTextDrawColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalColor[styleid][playerid] & ~0xFF) | colourAlpha);
            PlayerTextDrawBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBGColor[styleid][playerid] & ~0xFF) | backgroundColourAlpha);
            if (gs_StyleList[styleid][e_fadein])
            {
                PlayerTextDrawBoxColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBoxColor[styleid][playerid] & ~0xFF) | boxColourAlpha);
            }
            PlayerTextDrawShow(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
        }

        gs_GameTextFadeInTimer[styleid][playerid] = SetTimerEx("OnFadeInGameText", FADE_INTERVAL, false, "ddddd", playerid, styleid, colourAlpha, backgroundColourAlpha, boxColourAlpha);
    }
    else
    {
        gs_GameTextIsFadingIn[styleid][playerid] = false;
        gs_GameTextFadeInTimer[styleid][playerid] = 0;
    }
    return 1;
}

/*
 * Timer callback to fade out gametext with a smooth animation
 * @param playerid - Player ID (MAX_PLAYERS for global TextDraws)
 * @param styleid - Style ID of the gametext
 * @param colourAlpha - Current alpha value of the text color
 * @param backgroundColourAlpha - Current alpha value of the background color
 * @param boxColourAlpha - Current alpha value of the box color
 */
forward OnFadeOutGameText(playerid, styleid, colourAlpha, backgroundColourAlpha, boxColourAlpha);
public OnFadeOutGameText(playerid, styleid, colourAlpha, backgroundColourAlpha, boxColourAlpha)
{
    if (colourAlpha > 0)
    {
        colourAlpha -= clamp((gs_GameTextOriginalColor[styleid][playerid] & 0xFF) / FADE_ALPHA_STEP, 1, colourAlpha);
    }
    if (backgroundColourAlpha > 0)
    {
        backgroundColourAlpha -= clamp((gs_GameTextOriginalBGColor[styleid][playerid] & 0xFF) / FADE_ALPHA_STEP, 1, backgroundColourAlpha);
    }
    if (boxColourAlpha > 0)
    {
        boxColourAlpha -= clamp((gs_GameTextOriginalBoxColor[styleid][playerid] & 0xFF) / FADE_ALPHA_STEP, 1, boxColourAlpha);
    }

    if (colourAlpha > 0 || backgroundColourAlpha > 0 || boxColourAlpha > 0)
    {
        if (playerid == MAX_PLAYERS)
        {
            TextDrawColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalColor[styleid][MAX_PLAYERS] & ~0xFF) | colourAlpha);
            TextDrawBackgroundColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalBGColor[styleid][MAX_PLAYERS] & ~0xFF) | backgroundColourAlpha);
            if (gs_StyleList[styleid][e_fadeout])
            {
                TextDrawBoxColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalBoxColor[styleid][MAX_PLAYERS] & ~0xFF) | boxColourAlpha);
            }
            TextDrawShowForAll(gs_GameTextStyle[styleid]);
        }
        else
        {
            PlayerTextDrawColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalColor[styleid][playerid] & ~0xFF) | colourAlpha);
            PlayerTextDrawBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBGColor[styleid][playerid] & ~0xFF) | backgroundColourAlpha);
            if (gs_StyleList[styleid][e_fadeout])
            {
                PlayerTextDrawBoxColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBoxColor[styleid][playerid] & ~0xFF) | boxColourAlpha);
            }
            PlayerTextDrawShow(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
        }

        gs_GameTextFadeOutTimer[styleid][playerid] = SetTimerEx("OnFadeOutGameText", FADE_INTERVAL, false, "ddddd", playerid, styleid, colourAlpha, backgroundColourAlpha, boxColourAlpha);
    }
    else
    {
        new
            origColorAlpha = (gs_GameTextOriginalColor[styleid][playerid] & 0xFF),
            origBGAlpha = (gs_GameTextOriginalBGColor[styleid][playerid] & 0xFF),
            origBoxAlpha = (gs_GameTextOriginalBoxColor[styleid][playerid] & 0xFF)
        ;

        if (playerid == MAX_PLAYERS)
        {
            TextDrawColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalColor[styleid][MAX_PLAYERS] & ~0xFF) | origColorAlpha);
            TextDrawBackgroundColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalBGColor[styleid][MAX_PLAYERS] & ~0xFF) | origBGAlpha);
            if (gs_StyleList[styleid][e_fadeout])
            {
                TextDrawBoxColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalBoxColor[styleid][MAX_PLAYERS] & ~0xFF) | origBoxAlpha);
            }
            TextDrawHideForAll(gs_GameTextStyle[styleid]);
        }
        else
        {
            PlayerTextDrawColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalColor[styleid][playerid] & ~0xFF) | origColorAlpha);
            PlayerTextDrawBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBGColor[styleid][playerid] & ~0xFF) | origBGAlpha);
            if (gs_StyleList[styleid][e_fadeout])
            {
                PlayerTextDrawBoxColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBoxColor[styleid][playerid] & ~0xFF) | origBoxAlpha);
            }
            PlayerTextDrawHide(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
        }

        gs_GameTextIsFadingOut[styleid][playerid] = false;
        gs_GameTextFadeOutTimer[styleid][playerid] = 0;
    }
    return 1;
}

//==========================================================================
//                            HOOKS & CALLBACKS
//==========================================================================

#define _ALS_GameTextForPlayer
#define GameTextForPlayer GTP_GameTextForPlayer

#define _ALS_GameTextForAll
#define GameTextForAll GTP_GameTextForAll

public OnPlayerConnect(playerid)
{
    GameTextEx(playerid);

    new version[24];
    gs_GameTextNeedsAlphaFix[playerid] = false;
    if (GetPlayerVersion(playerid, version, sizeof(version)))
    {
        gs_GameTextNeedsAlphaFix[playerid] =
            strcmp(version, "0.3.7") != 0 &&
            strcmp(version, "0.3.7-R2") != 0
        ;
    }

    #if defined GTP_OnPlayerConnect
        return GTP_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect GTP_OnPlayerConnect
#if defined GTP_OnPlayerConnect
    forward GTP_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    gs_GameTextNeedsAlphaFix[playerid] = false;

    for (new style = MIN_GAMETEXT_STYLE; style <= MAX_GAMETEXT_STYLE; style++)
    {
        if (gs_GameTextFadeOutTimer[style][playerid])
        {
            KillTimer(gs_GameTextFadeOutTimer[style][playerid]);
            gs_GameTextFadeOutTimer[style][playerid] = 0;
        }
        if (gs_GameTextFadeInTimer[style][playerid])
        {
            KillTimer(gs_GameTextFadeInTimer[style][playerid]);
            gs_GameTextFadeInTimer[style][playerid] = 0;
        }
        gs_GameTextIsFadingOut[style][playerid] = false;
        gs_GameTextIsFadingIn[style][playerid] = false;
    }

    OnDestroyGameText(playerid);

    #if defined GTP_OnPlayerDisconnect
        return GTP_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect GTP_OnPlayerDisconnect
#if defined GTP_OnPlayerDisconnect
    forward GTP_OnPlayerDisconnect(playerid, reason);
#endif

public OnGameModeInit()
{
    GameTextEx(INVALID_PLAYER_ID);

    #if defined GTP_OnGameModeInit
        return GTP_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit GTP_OnGameModeInit
#if defined GTP_OnGameModeInit
    forward GTP_OnGameModeInit();
#endif

public OnGameModeExit()
{
    OnDestroyGameText(INVALID_PLAYER_ID);

    #if defined GTP_OnGameModeExit
        return GTP_OnGameModeExit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit GTP_OnGameModeExit
#if defined GTP_OnGameModeExit
    forward GTP_OnGameModeExit();
#endif

//! Developer's PRAGMA wore off!
#if !defined _INC_open_mp && defined __PawnBuild
    #if __PawnBuild > 4
        #pragma warning pop
    #endif
#endif
//! Compiler awakens! Its power is restored to its previous state!
