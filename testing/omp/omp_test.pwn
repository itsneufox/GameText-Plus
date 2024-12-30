#include <open.mp>
#include <gametext_plus>

main()
{
	printf(" ");
    printf("    _______  _______  __   __  _______  _______  _______  __   __  _______    _   ");
    printf("   |    ___||   _   ||  |_|  ||    ___||_     _||    ___||  |_|  ||_     _| _| |_ ");
    printf("   |   | __ |  |_|  ||       ||   |___   |   |  |   |___ |       |  |   |  |_   _|");
    printf("   |   ||  ||       ||       ||    ___|  |   |  |    ___| |     |   |   |    |_|  ");
    printf("   |   |_| ||   _   || ||_|| ||   |___   |   |  |   |___ |   _   |  |   |         ");
    printf("   |_______||__| |__||_|   |_||_______|  |___|  |_______||__| |__|  |___|         ");
    printf("   open.mp gamemode test");
    printf(" ");
}

#define GAMETEXT_STYLE_COUNT 16

new 
    gCurrentStyle[MAX_PLAYERS],
    gTestActive[MAX_PLAYERS],
    bool:gPlayerShowingStyles[MAX_PLAYERS]
;

public OnGameModeInit()
{
	SetGameModeText("GameText+");
	return true;
}

public OnPlayerConnect(playerid)
{
	gCurrentStyle[playerid] = 0;
    gTestActive[playerid] = 0;
    gPlayerShowingStyles[playerid] = false;
   
    SendClientMessage(playerid, -1, "Welcome! Use these commands to test GameText styles:");
    SendClientMessage(playerid, -1, "  {FFFF00}/test{FFFFFF} - Start the GameText style test");
    SendClientMessage(playerid, -1, "  {FFFF00}/next{FFFFFF} - Show the next style");
    SendClientMessage(playerid, -1, "  {FFFF00}/stop{FFFFFF} - Stop the test");

    TogglePlayerSpectating(playerid, true);

	return true;
}

public OnPlayerDisconnect(playerid, reason)
{
	gPlayerShowingStyles[playerid] = false;

	return true;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/test", true))
    {
        if (gTestActive[playerid])
        {
            GameTextForPlayer(playerid, "~r~Test already running!", 3000, 0);

            return true;
        }
        
        gTestActive[playerid] = 1;
        gCurrentStyle[playerid] = 0;
        gPlayerShowingStyles[playerid] = true;
        
        SendClientMessage(playerid, -1, "Starting GameText style test...");
        ShowCurrentStyle(playerid);

        return true;
    }
    
    if (!strcmp(cmdtext, "/next", true))
    {
        if (!gTestActive[playerid])
        {
            GameTextForPlayer(playerid, "~r~Use /test first!", 3000, 0);

            return true;
        }
        
        gCurrentStyle[playerid]++;
        if (gCurrentStyle[playerid] >= GAMETEXT_STYLE_COUNT)
        {
            GameTextForPlayer(playerid, "~g~Test Complete!", 3000, 2);
            SendClientMessage(playerid, -1, "You've seen all available styles!");
            gTestActive[playerid] = 0;
            gPlayerShowingStyles[playerid] = false;

            return true;
        }
        
        ShowCurrentStyle(playerid);

        return true;
    }
    
    if (!strcmp(cmdtext, "/stop", true))
    {
        if (!gTestActive[playerid])
        {
            GameTextForPlayer(playerid, "~r~No test running!", 3000, 0);

            return true;
        }
        
        gTestActive[playerid] = 0;
        gPlayerShowingStyles[playerid] = false;
        GameTextForPlayer(playerid, "~y~Test Stopped", 3000, 2);

        return true;
    }
    
    return false;
}

ShowCurrentStyle(playerid)
{
    // Show appropriate test text for each style
    switch (gCurrentStyle[playerid])
    {
        case 0: GameTextForPlayer(playerid, "mission passed!~n~~w~respect +", 3000, 0);
        case 1: GameTextForPlayer(playerid, "Nines and AK's", 3000, 1);
        case 2: GameTextForPlayer(playerid, "wasted", 3000, 2);
        case 3: GameTextForPlayer(playerid, "macaco", 3000, 3);
        case 4: GameTextForPlayer(playerid, "even more macaco", 3000, 4);
        case 5: GameTextForPlayer(playerid, "even more macaco again", 3000, 5);
        case 6: GameTextForPlayer(playerid, "fine dunno anymore", 5000, 6);
        case 7: GameTextForPlayer(playerid, "NRG-500", 5000, 7); // Vehicle name style
        case 8: GameTextForPlayer(playerid, "Grove Street", 5000, 8); // Location style
        case 9: GameTextForPlayer(playerid, "K-Rose", 5000, 9); // Radio name
        case 10: GameTextForPlayer(playerid, "K-Rose", 5000, 10); // Radio switch
        case 11: GameTextForPlayer(playerid, "+$50,000", 5000, 11); // Positive money
        case 12: GameTextForPlayer(playerid, "-$50,000", 5000, 12); // Negative money
        case 13: GameTextForPlayer(playerid, "DOUBLE INSANE STUNT BONUS: $125", 5000, 13); // Stunt bonus
        case 14: GameTextForPlayer(playerid, "09:41", 5000, 14); // Clock style
        case 15: GameTextForPlayer(playerid, "LSHIFT Increase Wager~n~LMB Decrease Wager~n~SPACE Proceed~n~RETURN Quit", 5000, 15); // Popup style
    }
    
    // Show info about current style
    new string[128];
    format(string, sizeof(string), "Showing style %d of %d - Use /next to continue", 
        gCurrentStyle[playerid], GAMETEXT_STYLE_COUNT-1);
    SendClientMessage(playerid, -1, string);
}
