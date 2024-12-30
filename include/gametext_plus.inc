/*
 *   _______  _______  __   __  _______  _______  _______  __   __  _______    _    
 *  |    ___||   _   ||  |_|  ||    ___||_     _||    ___||  |_|  ||_     _| _| |_  
 *  |   | __ |  | |  ||       ||   |___   |   |  |   |___ |       |  |   |  |_   _| 
 *  |   ||  ||  |_|  ||       ||    ___|  |   |  |    ___| |     |   |   |    |_|   
 *  |   |_| ||   _   || ||_|| ||   |___   |   |  |   |___ |   _   |  |   |  
 *  |_______||__| |__||_|   |_||_______|  |___|  |_______||__| |__|  |___|  
 *  
 *  GameText+ by itsneufox (v1.0.1)
 *  Textdraws originally created by Y_Less for fixes.inc - https://github.com/pawn-lang/sa-mp-fixes
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

//==========================================================================
//                          CONSTANTS & VARIABLES
//==========================================================================

#define MAX_GAMETEXT_LENGTH   256
#define GAMETEXT_LEGACY       6 
#define GAMETEXT_STYLE_MIN    7 
#define GAMETEXT_STYLE_MAX    15

static 
    Text:gs_GameTextStyle[GAMETEXT_STYLE_MAX + 1],
    PlayerText:gs_PlayerGameTextStyle[MAX_PLAYERS][GAMETEXT_STYLE_MAX + 1],
    gs_GameTextTimer[GAMETEXT_STYLE_MAX + 1][MAX_PLAYERS + 1]
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
//                          CORE FUNCTIONS
//==========================================================================

/*
 * Creates TextDraw styles either globally or for a specific player
 * @param playerid - INVALID_PLAYER_ID for global styles, player ID for player-specific styles
 */
static stock GameTextEx(playerid)
{
    if (playerid == INVALID_PLAYER_ID)
    {
        new 
            Text:t
        ;

        // Style 7 (Vehicle Name Style)
        t = gs_GameTextStyle[7] = TextDrawCreate(608.0, 344.0, " ");
        TextDrawLetterSize(t, 1.0, 3.0);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        TextDrawColour(t, 0x36682CFF);
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:2);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawBoxColour(t, 0x00000000);
        TextDrawTextSize(t, 10.0, 200.0);

        // Style 8 (Location Name Style)
        t = gs_GameTextStyle[8] = TextDrawCreate(608.0, 385.8, " ");
        TextDrawLetterSize(t, 1.2, 3.8);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        TextDrawColour(t, 0xACCBF1FF);
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:0);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawBoxColour(t, 0x00000000);
        TextDrawTextSize(t, 10.0, 200.0);

        // Style 9 (Radio Station Name Style)
        t = gs_GameTextStyle[9] = TextDrawCreate(320.0, 22.0, " ");
        TextDrawLetterSize(t, 0.6, 1.8);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
        TextDrawColour(t, 0x906210FF);
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 1);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:2);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawBoxColour(t, 0x00000000);
        TextDrawTextSize(t, 200.0, 620.0);

        // Style 10 (Radio Station Name While Switching Style)
        t = gs_GameTextStyle[10] = TextDrawCreate(320.0, 22.0, " ");
        TextDrawLetterSize(t, 0.6, 1.8);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
        TextDrawColour(t, 0x969696FF);
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 1);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:2);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawBoxColour(t, 0x00000000);
        TextDrawTextSize(t, 200.0, 620.0);

        // Style 11 (Positive Money Style)
        t = gs_GameTextStyle[11] = TextDrawCreate(608.0, 77.0, " ");
        TextDrawLetterSize(t, 0.55, 2.2);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        TextDrawColour(t, 0x36682CFF);
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:3);
        TextDrawSetProportional(t, false);
        TextDrawUseBox(t, true);
        TextDrawBoxColour(t, 0x00000000);
        TextDrawTextSize(t, 10.0, 200.0);

        // Style 12 (Negative Money Style)
        t = gs_GameTextStyle[12] = TextDrawCreate(608.0, 77.0, " ");
        TextDrawLetterSize(t, 0.55, 2.2);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        TextDrawColour(t, 0xB4191DFF);
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:3);
        TextDrawSetProportional(t, false);
        TextDrawUseBox(t, true);
        TextDrawBoxColour(t, 0x00000000);
        TextDrawTextSize(t, 10.0, 200.0);

        // Style 13 (Stunt Bonus Style)
        t = gs_GameTextStyle[13] = TextDrawCreate(380.0, 341.15, " ");
        TextDrawLetterSize(t, 0.58, 2.42);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:2);
        TextDrawColour(t, 0xDDDDDBFF);
        TextDrawSetShadow(t, 2);
        TextDrawSetOutline(t, 0);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:1);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawBoxColour(t, 0x00000000);
        TextDrawTextSize(t, 40.0, 460.0);

        // Style 14 (In-Game Clock Style)
        t = gs_GameTextStyle[14] = TextDrawCreate(608.0, 22.0, " ");
        TextDrawLetterSize(t, 0.55, 2.2);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:3);
        TextDrawColour(t, 0xC3C3C3FF);
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 2);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:3);
        TextDrawSetProportional(t, false);
        TextDrawUseBox(t, false);
        TextDrawBoxColour(t, 0x00000000);
        TextDrawTextSize(t, 400.0, 20.0);

        // Style 15 (Notification Popup Style)
        t = gs_GameTextStyle[15] = TextDrawCreate(34.0, 28.0, " ");
        TextDrawLetterSize(t, 0.52, 2.2);
        TextDrawAlignment(t, TEXT_DRAW_ALIGN:1);
        TextDrawColour(t, 0xFFFFFF96);
        TextDrawSetShadow(t, 0);
        TextDrawSetOutline(t, 0);
        TextDrawBackgroundColour(t, 0x000000AA);
        TextDrawFont(t, TEXT_DRAW_FONT:1);
        TextDrawSetProportional(t, true);
        TextDrawUseBox(t, true);
        TextDrawBoxColour(t, 0x00000080);
        TextDrawTextSize(t, 230.5, 200.0);
    }
    else
    {
        new 
            PlayerText:pt
        ;

        // Style 7 (Vehicle Name Style)
        pt = gs_PlayerGameTextStyle[playerid][7] = CreatePlayerTextDraw(playerid, 608.0, 344.0, " ");
        PlayerTextDrawLetterSize(playerid, pt, 1.0, 3.0);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        PlayerTextDrawColour(playerid, pt, 0x36682CFF);
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

        // Style 8 (Location Name Style)
        pt = gs_PlayerGameTextStyle[playerid][8] = CreatePlayerTextDraw(playerid, 608.0, 385.8, " ");
        PlayerTextDrawLetterSize(playerid, pt, 1.2, 3.8);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        PlayerTextDrawColour(playerid, pt, 0xACCBF1FF);
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:0);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

        // Style 9 (Radio Station Name Style)
        pt = gs_PlayerGameTextStyle[playerid][9] = CreatePlayerTextDraw(playerid, 320.0, 22.0, " ");
        PlayerTextDrawLetterSize(playerid, pt, 0.6, 1.8);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
        PlayerTextDrawColour(playerid, pt, 0x906210FF);
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 1);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

        // Style 10 (Radio Station Name While Switching Style)
        pt = gs_PlayerGameTextStyle[playerid][10] = CreatePlayerTextDraw(playerid, 320.0, 22.0, " ");
        PlayerTextDrawLetterSize(playerid, pt, 0.6, 1.8);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
        PlayerTextDrawColour(playerid, pt, 0x969696FF);
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 1);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:2);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        PlayerTextDrawTextSize(playerid, pt, 200.0, 620.0);

        // Style 11 (Positive Money Style)
        pt = gs_PlayerGameTextStyle[playerid][11] = CreatePlayerTextDraw(playerid, 608.0, 77.0, " ");
        PlayerTextDrawLetterSize(playerid, pt, 0.55, 2.2);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        PlayerTextDrawColour(playerid, pt, 0x36682CFF);
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
        PlayerTextDrawSetProportional(playerid, pt, false);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

        // Style 12 (Negative Money Style)
        pt = gs_PlayerGameTextStyle[playerid][12] = CreatePlayerTextDraw(playerid, 608.0, 77.0, " ");
        PlayerTextDrawLetterSize(playerid, pt, 0.55, 2.2);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        PlayerTextDrawColour(playerid, pt, 0xB4191DFF);
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
        PlayerTextDrawSetProportional(playerid, pt, false);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        PlayerTextDrawTextSize(playerid, pt, 10.0, 200.0);

        // Style 13 (Stunt Bonus Style)
        pt = gs_PlayerGameTextStyle[playerid][13] = CreatePlayerTextDraw(playerid, 380.0, 341.15, " ");
        PlayerTextDrawLetterSize(playerid, pt, 0.58, 2.42);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:2);
        PlayerTextDrawColour(playerid, pt, 0xDDDDDBFF);
        PlayerTextDrawSetShadow(playerid, pt, 2);
        PlayerTextDrawSetOutline(playerid, pt, 0);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:1);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        PlayerTextDrawTextSize(playerid, pt, 40.0, 460.0);

        // Style 14 (In-Game Clock Style)
        pt = gs_PlayerGameTextStyle[playerid][14] = CreatePlayerTextDraw(playerid, 608.0, 22.0, " ");
        PlayerTextDrawLetterSize(playerid, pt, 0.55, 2.2);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:3);
        PlayerTextDrawColour(playerid, pt, 0xC3C3C3FF);
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 2);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:3);
        PlayerTextDrawSetProportional(playerid, pt, false);
        PlayerTextDrawUseBox(playerid, pt, false);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000000);
        PlayerTextDrawTextSize(playerid, pt, 400.0, 20.0);

        // Style 15 (Notification Popup Style)
        pt = gs_PlayerGameTextStyle[playerid][15] = CreatePlayerTextDraw(playerid, 34.0, 28.0, " ");
        PlayerTextDrawLetterSize(playerid, pt, 0.52, 2.2);
        PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN:1);
        PlayerTextDrawColour(playerid, pt, 0xFFFFFF96);
        PlayerTextDrawSetShadow(playerid, pt, 0);
        PlayerTextDrawSetOutline(playerid, pt, 0);
        PlayerTextDrawBackgroundColour(playerid, pt, 0x000000AA);
        PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT:1);
        PlayerTextDrawSetProportional(playerid, pt, true);
        PlayerTextDrawUseBox(playerid, pt, true);
        PlayerTextDrawBoxColour(playerid, pt, 0x00000080);
        PlayerTextDrawTextSize(playerid, pt, 230.5, 200.0);
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
    return true;
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
    
    if (style <= GAMETEXT_LEGACY)
    {
        return GameTextForPlayer(playerid, text, time, style);
    }
    
    if (!(GAMETEXT_STYLE_MIN <= style <= GAMETEXT_STYLE_MAX)) return false;
    
    if (gs_GameTextTimer[style][playerid])
    {
        KillTimer(gs_GameTextTimer[style][playerid]);
        gs_GameTextTimer[style][playerid] = 0;
    }
    
    #if defined _INC_open_mp
        PlayerTextDrawSetString(playerid, gs_PlayerGameTextStyle[playerid][style], text);
    #else
        new 
            string[MAX_GAMETEXT_LENGTH]
        ;

        format(string, sizeof(string), "%s", text);
        PlayerTextDrawSetString(playerid, gs_PlayerGameTextStyle[playerid][style], string);
    #endif

    PlayerTextDrawShow(playerid, gs_PlayerGameTextStyle[playerid][style]);
    
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
    if (style <= GAMETEXT_LEGACY)
    {
        return GameTextForAll(text, time, style);
    }
    
    if (!(GAMETEXT_STYLE_MIN <= style <= GAMETEXT_STYLE_MAX)) return false;
    
    if (gs_GameTextTimer[style][MAX_PLAYERS])
    {
        KillTimer(gs_GameTextTimer[style][MAX_PLAYERS]);
        gs_GameTextTimer[style][MAX_PLAYERS] = 0;
    }
    
    #if defined _INC_open_mp
        TextDrawSetString(gs_GameTextStyle[style], text);
    #else
        new 
            string[MAX_GAMETEXT_LENGTH]
        ;
        
        format(string, sizeof(string), "%s", text);
        TextDrawSetString(gs_GameTextStyle[style], string);
    #endif

    TextDrawShowForAll(gs_GameTextStyle[style]);
    
    if (time > 0)
    {
        gs_GameTextTimer[style][MAX_PLAYERS] = SetTimerEx("OnHideGameText", time, false, "dd", MAX_PLAYERS, style);
    }
    
    return true;
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