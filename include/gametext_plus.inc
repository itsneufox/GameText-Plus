/*
 *   _______  _______  __   __  _______  _______  _______  __   __  _______    _    
 *  |    ___||   _   ||  |_|  ||    ___||_     _||    ___||  |_|  ||_     _| _| |_  
 *  |   | __ |  | |  ||       ||   |___   |   |  |   |___ |       |  |   |  |_   _| 
 *  |   ||  ||  |_|  ||       ||    ___|  |   |  |    ___| |     |   |   |    |_|   
 *  |   |_| ||   _   || ||_|| ||   |___   |   |  |   |___ |   _   |  |   |  
 *  |_______||__| |__||_|   |_||_______|  |___|  |_______||__| |__|  |___|  
 *  
 *  GameText+ by itsneufox (v1.1.0)
 *  Textdraws originally created by Y_Less for fixes.inc
 *  Fader functionality by edgyaf
 *  Style 16 created by imshooter
 */

//==========================================================================
//                          HEADER GUARDS & CHECKS
//==========================================================================

#if defined _INC_gametext_plus
    #endinput
#endif

#define _INC_gametext_plus

#if defined _INC_SAMP_Community_fixes
    #error "Incompatibility detected: 'fixes.inc' is deprecated in open.mp. Please remove it from your project. All GameText functionalities are now provided by GameText+ include."
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
    
#endif

//! WILD COMPILER appeared! Compiler used WARNINGS about const correctness!
#if !defined _INC_open_mp
    #pragma warning disable 239
#endif
//! Developer used PRAGMA! It's super effective, compiler's warning level fell sharply!

//==========================================================================
//                          CONSTANTS & VARIABLES
//==========================================================================

#if defined OVERRIDE_NATIVE_GAMETEXT
    #define GAMETEXT_STYLE_MIN      0
#else
    #define GAMETEXT_STYLE_MIN      7
#endif

#define     MAX_GAMETEXT_LENGTH     256
#define     GAMETEXT_LEGACY         6 
#define     GAMETEXT_STYLE_MAX      16
#define     FADE_INTERVAL           50
#define     FADE_ALPHA_STEP         0x12

static 
    Text:gs_GameTextStyle[GAMETEXT_STYLE_MAX + 1],
    PlayerText:gs_PlayerGameTextStyle[MAX_PLAYERS][GAMETEXT_STYLE_MAX + 1],
    gs_GameTextTimer[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1],
    gs_GameTextFadeOutTimer[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1],
    gs_GameTextFadeInTimer[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1],
    gs_GameTextOriginalColor[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1],
    gs_GameTextOriginalBGColor[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1],
    gs_GameTextOriginalBoxColor[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1],
    bool:gs_GameTextIsFadingOut[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1],
    bool:gs_GameTextIsFadingIn[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1]
;

//==========================================================================
//                          FUNCTION REMAPPING
//==========================================================================

#if defined _ALS_GameTextForPlayer 
    #error _ALS_GameTextForPlayer defined
#endif
native BAD_GameTextForPlayer(playerid, const string[], time, style) = GameTextForPlayer;

#if defined _ALS_GameTextForAll
    #error _ALS_GameTextForAll defined
#endif
native BAD_GameTextForAll(const string[], time, style) = GameTextForAll;

//==========================================================================
//                       TEXTDRAW COLOR STORAGE
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

    stock SaveTextDrawColour(Text:text, color) {
        gs_TextDrawColors[_:text] = color;
        return color;
    }

    stock SaveTextDrawBackgroundColour(Text:text, color) {
        gs_TextDrawBGColors[_:text] = color;
        return color;
    }

    stock SaveTextDrawBoxColour(Text:text, color) {
        gs_TextDrawBoxColors[_:text] = color;
        return color;
    }

    stock SavePlayerTextDrawColour(playerid, PlayerText:text, color) {
        gs_PlayerTextDrawColors[playerid][_:text] = color;
        return color;
    }

    stock SavePlayerTextDrawBackgroundColour(playerid, PlayerText:text, color) {
        gs_PlayerTextDrawBGColors[playerid][_:text] = color;
        return color;
    }

    stock SavePlayerTextDrawBoxColour(playerid, PlayerText:text, color) {
        gs_PlayerTextDrawBoxColors[playerid][_:text] = color;
        return color;
    }

    stock TextDrawGetColour(Text:text) {
        return gs_TextDrawColors[_:text];
    }

    stock TextDrawGetBackgroundColour(Text:text) {
        return gs_TextDrawBGColors[_:text];
    }

    stock TextDrawGetBoxColour(Text:text) {
        return gs_TextDrawBoxColors[_:text];
    }

    stock PlayerTextDrawGetColour(playerid, PlayerText:text) {
        return gs_PlayerTextDrawColors[playerid][_:text];
    }

    stock PlayerTextDrawGetBackgroundColour(playerid, PlayerText:text) {
        return gs_PlayerTextDrawBGColors[playerid][_:text];
    }

    stock PlayerTextDrawGetBoxColour(playerid, PlayerText:text) {
        return gs_PlayerTextDrawBoxColors[playerid][_:text];
    }
#endif

//==========================================================================
//                          CORE FUNCTIONS
//==========================================================================

/*
 * Creates TextDraw styles either globally or for a specific player
 * @param playerid - INVALID_PLAYER_ID for global styles, player ID for player-specific styles
 */
static stock GameTextEx(playerid)
{
    new
        EMPTY_STRING[2] = " "
    ;
    
    if (playerid == INVALID_PLAYER_ID)
    {
        new 
            Text:t
        ;
         #if defined OVERRIDE_NATIVE_GAMETEXT
            // Native style 0
            t = gs_GameTextStyle[0] = TextDrawCreate(320.0, 214.0, EMPTY_STRING);
            TextDrawLetterSize(t, 1.3, 3.6);
            TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                TextDrawColour(t, SaveTextDrawColour(t, 0x906210FF));
                TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
                TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
            #else
                TextDrawColour(t, 0x906210FF);
                TextDrawBackgroundColour(t, 0x000000AA);
                TextDrawBoxColour(t, 0x00000000);
            #endif
            TextDrawSetShadow(t, 0);
            TextDrawSetOutline(t, 2);
            TextDrawFont(t, TEXT_DRAW_FONT:3);
            TextDrawSetProportional(t, true);
            TextDrawUseBox(t, true);
            TextDrawTextSize(t, 200.0, 620.0);

            // Native style 1
            t = gs_GameTextStyle[1] = TextDrawCreate(620.0, 310.0, EMPTY_STRING);
            TextDrawLetterSize(t, 1.0, 2.6);
            TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
            #if !defined _INC_open_mp
                TextDrawColour(t, SaveTextDrawColour(t, 0x906210FF));
                TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
                TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
            #else
                TextDrawColour(t, 0x906210FF);
                TextDrawBackgroundColour(t, 0x000000AA);
                TextDrawBoxColour(t, 0x00000000);
            #endif
            TextDrawSetShadow(t, 0);
            TextDrawSetOutline(t, 2);
            TextDrawFont(t, TEXT_DRAW_FONT:3);
            TextDrawSetProportional(t, true);
            TextDrawUseBox(t, true);
            TextDrawTextSize(t, 10.0, 200.0);

            // Native style 2
            t = gs_GameTextStyle[2] = TextDrawCreate(320.0, 156.0, EMPTY_STRING);
            TextDrawLetterSize(t, 2.1, 4.2);
            TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                TextDrawColour(t, SaveTextDrawColour(t, 0xE1E1E1FF));
                TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
                TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
            #else
                TextDrawColour(t, 0xE1E1E1FF);
                TextDrawBackgroundColour(t, 0x000000AA);
                TextDrawBoxColour(t, 0x00000000);
            #endif
            TextDrawSetShadow(t, 0);
            TextDrawSetOutline(t, 3);
            TextDrawFont(t, TEXT_DRAW_FONT:0);
            TextDrawSetProportional(t, true);
            TextDrawUseBox(t, true);
            TextDrawTextSize(t, 200.0, 620.0);

            // Native style 3
            t = gs_GameTextStyle[3] = TextDrawCreate(320.0, 154.5, EMPTY_STRING);
            TextDrawLetterSize(t, 0.6, 2.75);
            TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                TextDrawColour(t, SaveTextDrawColour(t, 0x906210FF));
                TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
                TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
            #else
                TextDrawColour(t, 0x906210FF);
                TextDrawBackgroundColour(t, 0x000000AA);
                TextDrawBoxColour(t, 0x00000000);
            #endif
            TextDrawSetShadow(t, 0);
            TextDrawSetOutline(t, 2);
            TextDrawFont(t, TEXT_DRAW_FONT:2);
            TextDrawSetProportional(t, true);
            TextDrawUseBox(t, true);
            TextDrawTextSize(t, 200.0, 620.0);

            // Native style 4
            t = gs_GameTextStyle[4] = TextDrawCreate(320.0, 115.5, EMPTY_STRING);
            TextDrawLetterSize(t, 0.6, 2.75);
            TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                TextDrawColour(t, SaveTextDrawColour(t, 0x906210FF));
                TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
                TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
            #else
                TextDrawColour(t, 0x906210FF);
                TextDrawBackgroundColour(t, 0x000000AA);
                TextDrawBoxColour(t, 0x00000000);
            #endif
            TextDrawSetShadow(t, 0);
            TextDrawSetOutline(t, 2);
            TextDrawFont(t, TEXT_DRAW_FONT:2);
            TextDrawSetProportional(t, true);
            TextDrawUseBox(t, true);
            TextDrawTextSize(t, 200.0, 620.0);

            // Native style 5
            t = gs_GameTextStyle[5] = TextDrawCreate(320.0, 217.0, EMPTY_STRING);
            TextDrawLetterSize(t, 0.6, 2.75);
            TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                TextDrawColour(t, SaveTextDrawColour(t, 0xE1E1E1FF));
                TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
                TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
            #else
                TextDrawColour(t, 0xE1E1E1FF);
                TextDrawBackgroundColour(t, 0x000000AA);
                TextDrawBoxColour(t, 0x00000000);
            #endif
            TextDrawSetShadow(t, 0);
            TextDrawSetOutline(t, 2);
            TextDrawFont(t, TEXT_DRAW_FONT:2);
            TextDrawSetProportional(t, true);
            TextDrawUseBox(t, true);
            TextDrawTextSize(t, 200.0, 620.0);

            // Native style 6
            t = gs_GameTextStyle[6] = TextDrawCreate(320.0, 60.0, EMPTY_STRING);
            TextDrawLetterSize(t, 1.0, 3.6);
            TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                TextDrawColour(t, SaveTextDrawColour(t, 0xACCBF1FF));
                TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
                TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
            #else
                TextDrawColour(t, 0xACCBF1FF);
                TextDrawBackgroundColour(t, 0x000000AA);
                TextDrawBoxColour(t, 0x00000000);
            #endif
            TextDrawSetShadow(t, 0);
            TextDrawSetOutline(t, 2);
            TextDrawFont(t, TEXT_DRAW_FONT:3);
            TextDrawSetProportional(t, true);
            TextDrawUseBox(t, true);
            TextDrawTextSize(t, 200.0, 620.0);
        #endif

        // Style 7 (Vehicle Name Style)
        t = gs_GameTextStyle[7] = TextDrawCreate(608.0, 344.0, EMPTY_STRING);
        TextDrawLetterSize(t, 1.0, 3.0);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0x36682CFF));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
        #else
            TextDrawColour(t, 0x36682CFF);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x00000000);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawFont(t, TEXT_DRAW_FONT:2);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 10.0, 200.0);

        // Style 8 (Location Name Style)
        t = gs_GameTextStyle[8] = TextDrawCreate(608.0, 385.8, EMPTY_STRING);
        TextDrawLetterSize(t, 1.2, 3.8);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0xACCBF1FF));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
        #else
            TextDrawColour(t, 0xACCBF1FF);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x00000000);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawFont(t, TEXT_DRAW_FONT:0);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 10.0, 200.0);

        // Style 9 (Radio Station Name Style)
        t = gs_GameTextStyle[9] = TextDrawCreate(320.0, 22.0, EMPTY_STRING);
        TextDrawLetterSize(t, 0.6, 1.8);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0x906210FF));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
        #else
            TextDrawColour(t, 0x906210FF);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x00000000);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 1);
        TextDrawFont(t, TEXT_DRAW_FONT:2);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 200.0, 620.0);

        // Style 10 (Radio Station Name While Switching Style)
        t = gs_GameTextStyle[10] = TextDrawCreate(320.0, 22.0, EMPTY_STRING);
        TextDrawLetterSize(t, 0.6, 1.8);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0x969696FF));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
        #else
            TextDrawColour(t, 0x969696FF);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x00000000);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 1);
        TextDrawFont(t, TEXT_DRAW_FONT:2);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 200.0, 620.0);

        // Style 11 (Positive Money Style)
        t = gs_GameTextStyle[11] = TextDrawCreate(608.0, 77.0, EMPTY_STRING);
        TextDrawLetterSize(t, 0.55, 2.2);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0x36682CFF));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
        #else
            TextDrawColour(t, 0x36682CFF);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x00000000);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawFont(t, TEXT_DRAW_FONT:3);
        TextDrawSetProportional(t, false);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 10.0, 200.0);

        // Style 12 (Negative Money Style)
        t = gs_GameTextStyle[12] = TextDrawCreate(608.0, 77.0, EMPTY_STRING);
        TextDrawLetterSize(t, 0.55, 2.2);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0xB4191DFF));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
        #else
            TextDrawColour(t, 0xB4191DFF);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x00000000);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawFont(t, TEXT_DRAW_FONT:3);
        TextDrawSetProportional(t, false);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 10.0, 200.0);

        // Style 13 (Stunt Bonus Style)
        t = gs_GameTextStyle[13] = TextDrawCreate(380.0, 341.15, EMPTY_STRING);
        TextDrawLetterSize(t, 0.58, 2.42);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0xDDDDDBFF));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
        #else
            TextDrawColour(t, 0xDDDDDBFF);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x00000000);
        #endif
        TextDrawSetShadow(t, 2);
        TextDrawSetOutline(t, 0);
        TextDrawFont(t, TEXT_DRAW_FONT:1);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 40.0, 460.0);

        // Style 14 (In-Game Clock Style)
        t = gs_GameTextStyle[14] = TextDrawCreate(608.0, 22.0, EMPTY_STRING);
        TextDrawLetterSize(t, 0.55, 2.2);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0xC3C3C3FF));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x00000000));
        #else
            TextDrawColour(t, 0xC3C3C3FF);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x00000000);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawFont(t, TEXT_DRAW_FONT:3);
        TextDrawSetProportional(t, false);
        TextDrawUseBox(t, false);
        TextDrawTextSize(t, 400.0, 20.0);

        // Style 15 (Notification Popup Style)
        t = gs_GameTextStyle[15] = TextDrawCreate(34.0, 28.0, EMPTY_STRING);
        TextDrawLetterSize(t, 0.52, 2.2);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:1);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0xFFFFFF96));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x000000DD));
        #else
            TextDrawColour(t, 0xFFFFFF96);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x000000DD);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 0);
        TextDrawFont(t, TEXT_DRAW_FONT:1);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 230.5, 200.0);

        // Style 16 (Lower Notification Style)
        t = gs_GameTextStyle[16] = TextDrawCreate(28.0, 150.0, EMPTY_STRING);
        TextDrawLetterSize(t, 0.416, 1.760);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:1);
        #if !defined _INC_open_mp
            TextDrawColour(t, SaveTextDrawColour(t, 0xFFFFFF96));
            TextDrawBackgroundColour(t, SaveTextDrawBackgroundColour(t, 0x000000AA));
            TextDrawBoxColour(t, SaveTextDrawBoxColour(t, 0x000000DD));
        #else
            TextDrawColour(t, 0xFFFFFF96);
            TextDrawBackgroundColour(t, 0x000000AA);
            TextDrawBoxColour(t, 0x000000DD);
        #endif
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 0);
        TextDrawFont(t, TEXT_DRAW_FONT:1);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawTextSize(t, 212.0, 200.0);
    }
    else
    {
        new PlayerText:pt;

        #if defined OVERRIDE_NATIVE_GAMETEXT
            // Native style 0
            pt = gs_PlayerGameTextStyle[playerid][0] = CreatePlayerTextDraw(playerid, 320.0, 214.0, EMPTY_STRING);
            PlayerTextDrawLetterSize(playerid, pt, 1.3, 3.6);
            PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0x906210FF));
                PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
                PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
            #else
                PlayerTextDrawColour(playerid, pt, 0x906210FF);
                PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
                PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
            #endif
            PlayerTextDrawSetShadow(playerid, pt, 0);
            PlayerTextDrawSetOutline(playerid, pt, 2);
            PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
            PlayerTextDrawSetProportional(playerid, pt, true);
            PlayerTextDrawUseBox(playerid, pt, true);
            PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

            // Native style 1
            pt = gs_PlayerGameTextStyle[playerid][1] = CreatePlayerTextDraw(playerid, 620.0, 310.0, EMPTY_STRING);
            PlayerTextDrawLetterSize(playerid, pt, 1.0, 2.6);
            PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
            #if !defined _INC_open_mp
                PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0x906210FF));
                PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
                PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
            #else
                PlayerTextDrawColour(playerid, pt, 0x906210FF);
                PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
                PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
            #endif
            PlayerTextDrawSetShadow(playerid, pt, 0);
            PlayerTextDrawSetOutline(playerid, pt, 2);
            PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
            PlayerTextDrawSetProportional(playerid, pt, true);
            PlayerTextDrawUseBox(playerid, pt, true);
            PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

            // Native style 2
            pt = gs_PlayerGameTextStyle[playerid][2] = CreatePlayerTextDraw(playerid, 320.0, 156.0, EMPTY_STRING);
            PlayerTextDrawLetterSize(playerid, pt, 2.1, 4.2);
            PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xE1E1E1FF));
                PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
                PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
            #else
                PlayerTextDrawColour(playerid, pt, 0xE1E1E1FF);
                PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
                PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
            #endif
            PlayerTextDrawSetShadow(playerid, pt, 0);
            PlayerTextDrawSetOutline(playerid, pt, 3);
            PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:0);
            PlayerTextDrawSetProportional(playerid, pt, true);
            PlayerTextDrawUseBox(playerid, pt, true);
            PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

            // Native style 3
            pt = gs_PlayerGameTextStyle[playerid][3] = CreatePlayerTextDraw(playerid, 320.0, 154.5, EMPTY_STRING);
            PlayerTextDrawLetterSize(playerid, pt, 0.6, 2.75);
            PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0x906210FF));
                PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
                PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
            #else
                PlayerTextDrawColour(playerid, pt, 0x906210FF);
                PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
                PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
            #endif
            PlayerTextDrawSetShadow(playerid, pt, 0);
            PlayerTextDrawSetOutline(playerid, pt, 2);
            PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
            PlayerTextDrawSetProportional(playerid, pt, true);
            PlayerTextDrawUseBox(playerid, pt, true);
            PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

            // Native style 4
            pt = gs_PlayerGameTextStyle[playerid][4] = CreatePlayerTextDraw(playerid, 320.0, 115.5, EMPTY_STRING);
            PlayerTextDrawLetterSize(playerid, pt, 0.6, 2.75);
            PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0x906210FF));
                PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
                PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
            #else
                PlayerTextDrawColour(playerid, pt, 0x906210FF);
                PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
                PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
            #endif
            PlayerTextDrawSetShadow(playerid, pt, 0);
            PlayerTextDrawSetOutline(playerid, pt, 2);
            PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
            PlayerTextDrawSetProportional(playerid, pt, true);
            PlayerTextDrawUseBox(playerid, pt, true);
            PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

            // Native style 5
            pt = gs_PlayerGameTextStyle[playerid][5] = CreatePlayerTextDraw(playerid, 320.0, 217.0, EMPTY_STRING);
            PlayerTextDrawLetterSize(playerid, pt, 0.6, 2.75);
            PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xE1E1E1FF));
                PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
                PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
            #else
                PlayerTextDrawColour(playerid, pt, 0xE1E1E1FF);
                PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
                PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
            #endif
            PlayerTextDrawSetShadow(playerid, pt, 0);
            PlayerTextDrawSetOutline(playerid, pt, 2);
            PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
            PlayerTextDrawSetProportional(playerid, pt, true);
            PlayerTextDrawUseBox(playerid, pt, true);
            PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

            // Native style 6
            pt = gs_PlayerGameTextStyle[playerid][6] = CreatePlayerTextDraw(playerid, 320.0, 60.0, EMPTY_STRING);
            PlayerTextDrawLetterSize(playerid, pt, 1.0, 3.6);
            PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
            #if !defined _INC_open_mp
                PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xACCBF1FF));
                PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
                PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
            #else
                PlayerTextDrawColour(playerid, pt, 0xACCBF1FF);
                PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
                PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
            #endif
            PlayerTextDrawSetShadow(playerid, pt, 0);
            PlayerTextDrawSetOutline(playerid, pt, 2);
            PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
            PlayerTextDrawSetProportional(playerid, pt, true);
            PlayerTextDrawUseBox(playerid, pt, true);
            PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);
        #endif

        // Style 7 (Vehicle Name Style)
        pt = gs_PlayerGameTextStyle[playerid][7] = CreatePlayerTextDraw(playerid, 608.0, 344.0, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 1.0, 3.0);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0x36682CFF));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
        #else
            PlayerTextDrawColour(playerid, pt, 0x36682CFF);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

        // Style 8 (Location Name Style)
        pt = gs_PlayerGameTextStyle[playerid][8] = CreatePlayerTextDraw(playerid, 608.0, 385.8, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 1.2, 3.8);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xACCBF1FF));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
        #else
            PlayerTextDrawColour(playerid, pt, 0xACCBF1FF);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:0);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

        // Style 9 (Radio Station Name Style)
        pt = gs_PlayerGameTextStyle[playerid][9] = CreatePlayerTextDraw(playerid, 320.0, 22.0, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 0.6, 1.8);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0x906210FF));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
        #else
            PlayerTextDrawColour(playerid, pt, 0x906210FF);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 1);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

        // Style 10 (Radio Station Name While Switching Style)
        pt = gs_PlayerGameTextStyle[playerid][10] = CreatePlayerTextDraw(playerid, 320.0, 22.0, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 0.6, 1.8);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0x969696FF));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
        #else
            PlayerTextDrawColour(playerid, pt, 0x969696FF);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 1);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

        // Style 11 (Positive Money Style)
        pt = gs_PlayerGameTextStyle[playerid][11] = CreatePlayerTextDraw(playerid, 608.0, 77.0, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 0.55, 2.2);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0x36682CFF));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
        #else
            PlayerTextDrawColour(playerid, pt, 0x36682CFF);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
        PlayerTextDrawSetProportional(playerid, pt, false);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

        // Style 12 (Negative Money Style)
        pt = gs_PlayerGameTextStyle[playerid][12] = CreatePlayerTextDraw(playerid, 608.0, 77.0, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 0.55, 2.2);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xB4191DFF));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
        #else
            PlayerTextDrawColour(playerid, pt, 0xB4191DFF);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
        PlayerTextDrawSetProportional(playerid, pt, false);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

        // Style 13 (Stunt Bonus Style)
        pt = gs_PlayerGameTextStyle[playerid][13] = CreatePlayerTextDraw(playerid, 380.0, 341.15, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 0.58, 2.42);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xDDDDDBFF));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
        #else
            PlayerTextDrawColour(playerid, pt, 0xDDDDDBFF);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 2);
        PlayerTextDrawSetOutline(playerid, pt, 0);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:1);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 40.0, 460.0);

        // Style 14 (In-Game Clock Style)
        pt = gs_PlayerGameTextStyle[playerid][14] = CreatePlayerTextDraw(playerid, 608.0, 22.0, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 0.55, 2.2);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xC3C3C3FF));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x00000000));
        #else
            PlayerTextDrawColour(playerid, pt, 0xC3C3C3FF);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
        PlayerTextDrawSetProportional(playerid, pt, false);
        PlayerTextDrawUseBox(playerid, pt, false);
        PlayerTextDrawTextSize(playerid, pt, 400.0, 20.0);

        // Style 15 (Notification Popup Style)
        pt = gs_PlayerGameTextStyle[playerid][15] = CreatePlayerTextDraw(playerid, 34.0, 28.0, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 0.52, 2.2);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:1);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xFFFFFF96));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x000000DD));
        #else
            PlayerTextDrawColour(playerid, pt, 0xFFFFFF96);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x000000DD);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 0);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:1);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 230.5, 200.0);

        // Style 16 (Lower Notification Style)
        pt = gs_PlayerGameTextStyle[playerid][16] = CreatePlayerTextDraw(playerid, 28.0, 150.0, EMPTY_STRING);
        PlayerTextDrawLetterSize(playerid, pt, 0.416, 1.760);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:1);
        #if !defined _INC_open_mp
            PlayerTextDrawColour(playerid, pt, SavePlayerTextDrawColour(playerid, pt, 0xFFFFFF96));
            PlayerTextDrawBackgroundColour(playerid, pt, SavePlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA));
            PlayerTextDrawBoxColour(playerid, pt, SavePlayerTextDrawBoxColour(playerid, pt, 0x000000DD));
        #else
            PlayerTextDrawColour(playerid, pt, 0xFFFFFF96);
            PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
            PlayerTextDrawBoxColour(playerid, pt, 0x000000DD);
        #endif
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 0);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:1);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawTextSize(playerid, pt, 212.0, 200.0);
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
    if (styleid == 14 || styleid == 9 || styleid == 10)
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
            gs_GameTextOriginalBGColor[styleid][playerid] = PlayerTextDrawGetBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
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
        for (new style = GAMETEXT_STYLE_MIN; style <= GAMETEXT_STYLE_MAX; style++)
        {
            TextDrawDestroy(gs_GameTextStyle[style]);
        }
    }
    else
    {
        for (new style = GAMETEXT_STYLE_MIN; style <= GAMETEXT_STYLE_MAX; style++)
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
 * @param style - Style ID (0-6 for native, 7-15 for extended)
 */
stock FIXES_GameTextForPlayer(playerid, const text[], time, style)
{
    if (!IsPlayerConnected(playerid)) return false;
    
    #if !defined OVERRIDE_NATIVE_GAMETEXT
        if (style <= GAMETEXT_LEGACY)
        {
            return GameTextForPlayer(playerid, text, time, style);
        }
    #endif
    
    if (!(GAMETEXT_STYLE_MIN <= style <= GAMETEXT_STYLE_MAX)) return false;
    
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
    
    gs_GameTextIsFadingOut[style][playerid] = false;
    gs_GameTextIsFadingIn[style][playerid] = false;
    
    #if defined _INC_open_mp
        PlayerTextDrawSetString(playerid, gs_PlayerGameTextStyle[playerid][style], text);
    #else
        new string[MAX_GAMETEXT_LENGTH];
        format(string, sizeof(string), "%s", text);
        PlayerTextDrawSetString(playerid, gs_PlayerGameTextStyle[playerid][style], string);
    #endif

    if (style == 14 || style == 9 || style == 10)
    {
        PlayerTextDrawShow(playerid, gs_PlayerGameTextStyle[playerid][style]);
    }
    else
    {
        gs_GameTextOriginalColor[style][playerid] = PlayerTextDrawGetColour(playerid, gs_PlayerGameTextStyle[playerid][style]);
        gs_GameTextOriginalBGColor[style][playerid] = PlayerTextDrawGetBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][style]);
        gs_GameTextOriginalBoxColor[style][playerid] = PlayerTextDrawGetBoxColour(playerid, gs_PlayerGameTextStyle[playerid][style]);
        
        gs_GameTextIsFadingIn[style][playerid] = true;
        
        PlayerTextDrawColour(playerid, gs_PlayerGameTextStyle[playerid][style], gs_GameTextOriginalColor[style][playerid] & 0xFFFFFF00);
        PlayerTextDrawBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][style], gs_GameTextOriginalBGColor[style][playerid] & 0xFFFFFF00);
        PlayerTextDrawBoxColour(playerid, gs_PlayerGameTextStyle[playerid][style], gs_GameTextOriginalBoxColor[style][playerid] & 0xFFFFFF00);
        
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
 * @param style - Style ID (0-6 for native, 7-15 for extended)
 */
stock FIXES_GameTextForAll(const text[], time, style)
{
    #if !defined OVERRIDE_NATIVE_GAMETEXT
        if (style <= GAMETEXT_LEGACY)
        {
            return GameTextForAll(text, time, style);
        }
    #endif
    
    if (!(GAMETEXT_STYLE_MIN <= style <= GAMETEXT_STYLE_MAX)) return false;
    
    // Kill all existing timers
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
    
    // Reset fade states
    gs_GameTextIsFadingOut[style][MAX_PLAYERS] = false;
    gs_GameTextIsFadingIn[style][MAX_PLAYERS] = false;
    
    #if defined _INC_open_mp
        TextDrawSetString(gs_GameTextStyle[style], text);
    #else
        new string[MAX_GAMETEXT_LENGTH];
        format(string, sizeof(string), "%s", text);
        TextDrawSetString(gs_GameTextStyle[style], string);
    #endif

    if (style == 14 || style == 9 || style == 10)
    {
        TextDrawShowForAll(gs_GameTextStyle[style]);
    }
    else
    {
        // Get current TextDraw colors (preserves any custom colors set by developer)
        gs_GameTextOriginalColor[style][MAX_PLAYERS] = TextDrawGetColour(gs_GameTextStyle[style]);
        gs_GameTextOriginalBGColor[style][MAX_PLAYERS] = TextDrawGetBackgroundColour(gs_GameTextStyle[style]);
        gs_GameTextOriginalBoxColor[style][MAX_PLAYERS] = TextDrawGetBoxColour(gs_GameTextStyle[style]);
        
        // Start new fade
        gs_GameTextIsFadingIn[style][MAX_PLAYERS] = true;
        
        // Reset all alpha channels to 0 for fade-in
        TextDrawColour(gs_GameTextStyle[style], gs_GameTextOriginalColor[style][MAX_PLAYERS] & 0xFFFFFF00);
        TextDrawBackgroundColour(gs_GameTextStyle[style], gs_GameTextOriginalBGColor[style][MAX_PLAYERS] & 0xFFFFFF00);
        TextDrawBoxColour(gs_GameTextStyle[style], gs_GameTextOriginalBoxColor[style][MAX_PLAYERS] & 0xFFFFFF00);
        
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
            if (styleid != 14 && styleid != 9 && styleid != 10)
            {
                TextDrawBoxColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalBoxColor[styleid][MAX_PLAYERS] & ~0xFF) | boxColourAlpha);
            }
            TextDrawShowForAll(gs_GameTextStyle[styleid]);
        }
        else
        {
            PlayerTextDrawColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalColor[styleid][playerid] & ~0xFF) | colourAlpha);
            PlayerTextDrawBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBGColor[styleid][playerid] & ~0xFF) | backgroundColourAlpha);
            if (styleid != 14 && styleid != 9 && styleid != 10)
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
            if (styleid != 14 && styleid != 9 && styleid != 10)
            {
                TextDrawBoxColour(gs_GameTextStyle[styleid], (gs_GameTextOriginalBoxColor[styleid][MAX_PLAYERS] & ~0xFF) | boxColourAlpha);
            }
            TextDrawShowForAll(gs_GameTextStyle[styleid]);
        }
        else
        {
            PlayerTextDrawColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalColor[styleid][playerid] & ~0xFF) | colourAlpha);
            PlayerTextDrawBackgroundColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBGColor[styleid][playerid] & ~0xFF) | backgroundColourAlpha);
            if (styleid != 14 && styleid != 9 && styleid != 10)
            {
                PlayerTextDrawBoxColour(playerid, gs_PlayerGameTextStyle[playerid][styleid], (gs_GameTextOriginalBoxColor[styleid][playerid] & ~0xFF) | boxColourAlpha);
            }
            PlayerTextDrawShow(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
        }

        gs_GameTextFadeOutTimer[styleid][playerid] = SetTimerEx("OnFadeOutGameText", FADE_INTERVAL, false, "ddddd", playerid, styleid, colourAlpha, backgroundColourAlpha, boxColourAlpha);
    }
    else
    {
        gs_GameTextIsFadingOut[styleid][playerid] = false;
        gs_GameTextFadeOutTimer[styleid][playerid] = 0;

        if (playerid == MAX_PLAYERS)
        {
            TextDrawHideForAll(gs_GameTextStyle[styleid]);
        }
        else
        {
            PlayerTextDrawHide(playerid, gs_PlayerGameTextStyle[playerid][styleid]);
        }
    }
    return 1;
}

//==========================================================================
//                          HOOKS & CALLBACKS
//==========================================================================

#define _ALS_GameTextForPlayer
#define GameTextForPlayer FIXES_GameTextForPlayer

#define _ALS_GameTextForAll  
#define GameTextForAll FIXES_GameTextForAll

public OnPlayerConnect(playerid)
{
    GameTextEx(playerid);
    
    #if defined GTP_OnPlayerConnect
        return GTP_OnPlayerConnect(playerid);
    #else
        return true;
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
    for (new style = GAMETEXT_STYLE_MIN; style <= GAMETEXT_STYLE_MAX; style++)
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
        return true;
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
        return true;
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
        return true;
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
#if !defined _INC_open_mp
    #pragma warning enable 239
#endif
//! Compiler awakens! Its power is restored!