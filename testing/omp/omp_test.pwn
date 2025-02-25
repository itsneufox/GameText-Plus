#include <open.mp>
#define OVERRIDE_NATIVE_GAMETEXT
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

#define GAMETEXT_STYLE_COUNT 17

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
            GameTextForPlayer(playerid, "~r~Test already running!", 3000, 6);

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
            GameTextForPlayer(playerid, "~r~Use /test first!", 3000, 6);

            return true;
        }
        
        gCurrentStyle[playerid]++;
        if (gCurrentStyle[playerid] >= GAMETEXT_STYLE_COUNT)
        {
            GameTextForPlayer(playerid, "~g~Test Complete!", 3000, 6);
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
            GameTextForPlayer(playerid, "~r~No test running!", 3000, 6);

            return true;
        }
        
        gTestActive[playerid] = 0;
        gPlayerShowingStyles[playerid] = false;
        GameTextForPlayer(playerid, "~y~Test Stopped", 3000, 6);

        return true;
    }
    
    return false;
}

ShowCurrentStyle(playerid)
{

    new Float:health, Float:armor, pname[MAX_PLAYER_NAME + 1];

    GetPlayerHealth(playerid, health);
    GetPlayerArmour(playerid, armor);
    GetPlayerName(playerid, pname, sizeof(pname));

    switch (gCurrentStyle[playerid])
    {
        case 0: GameTextForPlayer(playerid, "~g~MISSION PASSED!~n~~w~Respect +%d", 3000, 0, 10);
        case 1: GameTextForPlayer(playerid, "~y~Player: %s~n~~w~Weapons Unlocked", 3000, 1, pname);
        case 2: GameTextForPlayer(playerid, "~r~WASTED~n~$%d lost", 3000, 2, 500);
        case 3: GameTextForPlayer(playerid, "~b~Player ID: %d~n~~w~Health: %.1f", 3000, 3, playerid, health);
        case 4: GameTextForPlayer(playerid, "~p~Style %d~n~~w~Armor: %.1f", 3000, 4, 4, armor);
        case 5: GameTextForPlayer(playerid, "~y~Score: %d~n~~w~Position: %d/%d", 3000, 5, GetPlayerScore(playerid), playerid + 1, GetMaxPlayers());
        case 6: GameTextForPlayer(playerid, "~b~Current Time: %02d:%02d~n~~w~Game Date: %02d/%02d/%d", 5000, 6, gettime() % 60, gettime() / 60 % 60, getdate() % 30, getdate() % 12, getdate() / 12 + 2000);
        case 7: GameTextForPlayer(playerid, "Vehicle ID: %d", 5000, 7, 522); // Vehicle name style with ID
        case 8: GameTextForPlayer(playerid, "Zone: %s", 5000, 8, "Los Santos"); // Location style
        case 9: GameTextForPlayer(playerid, "Radio: %s", 5000, 9, "Los Santos"); // Radio name
        case 10: GameTextForPlayer(playerid, "Finding: %s...", 5000, 10, "Station"); // Radio switch
        case 11: GameTextForPlayer(playerid, "+$%d", 5000, 11, 100000); // Positive money
        case 12: GameTextForPlayer(playerid, "-$%d", 5000, 12, 25000); // Negative money
        case 13: GameTextForPlayer(playerid, "%s STUNT BONUS: $%d", 5000, 13, "INSANE", 500); // Stunt bonus
        case 14: GameTextForPlayer(playerid, "%02d:%02d", 5000, 14, gettime() / 60 % 60, gettime() % 60); // Clock style
        case 15: GameTextForPlayer(playerid, "~y~CONTROLS~n~~w~%s: Help Menu~n~%s: Options~n~%s: Statistics~n~%s: Quit", 5000, 15, "F1", "F2", "F3", "ESC"); // Popup style
        case 16: GameTextForPlayer(playerid, "~g~TIPS~n~~w~Use %s for commands~n~Press %s to enter vehicles~n~Press %s to accept offers", 5000, 16, "/help", "H", "Y"); // Lower popup style
    }
    
    // Show info about current style
    new string[128];
    format(string, sizeof(string), "Showing style %d of %d - Use /next to continue", 
        gCurrentStyle[playerid], GAMETEXT_STYLE_COUNT-1);
    SendClientMessage(playerid, -1, string);
}