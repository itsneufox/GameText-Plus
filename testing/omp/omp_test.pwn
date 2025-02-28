/*
 * GameText+ Test Gamemode
 * 
 * This gamemode demonstrates all available gametext styles from the GameText+ include.
 * It provides an interactive way to test different text effects, styles and formatting options.
 */

#include <open.mp>


// * |=================================================================================|
// * |  When defined, GameText+ will replace the default SA-MP gametext styles (0-6)   |
// * |  with enhanced versions that support formatting and fade effects.               |
// * |  Comment this line if you want to keep the original behavior for styles 0-6.    |
// * |                                                                                 |
                             #define OVERRIDE_NATIVE_GAMETEXT
// * |                                                                                 |
// * |=================================================================================|


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

#define DIALOG_TEST_MENU 1000

new 
    gCurrentStyle[MAX_PLAYERS],
    gTestActive[MAX_PLAYERS],
    bool:gPlayerShowingStyles[MAX_PLAYERS],
    gTestType[MAX_PLAYERS]
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
    gTestType[playerid] = 0;
   
    SendClientMessage(playerid, -1, "Welcome! Use these commands to test GameText styles:");
    SendClientMessage(playerid, -1, "  {FFFF00}/test{FFFFFF} - Open the GameText test menu");
    SendClientMessage(playerid, -1, "  {FFFF00}/stop{FFFFFF} - Stop the active test");

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
        
        ShowTestDialog(playerid);
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
        KillTimer(gTestActive[playerid]);
        GameTextForPlayer(playerid, "~y~Test Stopped", 3000, 6);
        return true;
    }
    
    return false;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_TEST_MENU:
        {
            if (!response) return true;
            
            gTestType[playerid] = listitem;
            gCurrentStyle[playerid] = 0;
            gPlayerShowingStyles[playerid] = true;
            
            new string[128];
            format(string, sizeof(string), "Starting GameText test with %s...", 
                (listitem == 0) ? "normal GameTextForPlayer" : 
                (listitem == 1) ? "formatted GameTextForPlayer" : 
                (listitem == 2) ? "normal GameTextForAll" : 
                "formatted GameTextForAll");
                
            SendClientMessage(playerid, -1, string);
            
            StartStyleTest(playerid);
            return true;
        }
    }
    return false;
}

ShowTestDialog(playerid)
{
    ShowPlayerDialog(playerid, DIALOG_TEST_MENU, DIALOG_STYLE_LIST, "GameText+ Test Menu", 
        "Normal GameTextForPlayer\nFormatted GameTextForPlayer\nNormal GameTextForAll\nFormatted GameTextForAll", 
        "Select", "Cancel");
    return true;
}

StartStyleTest(playerid)
{
    gTestActive[playerid] = 1;
    ShowCurrentStyle(playerid);
    return true;
}

forward ContinueStyleTest(playerid);
public ContinueStyleTest(playerid)
{
    if (!gPlayerShowingStyles[playerid]) return false;
    
    gCurrentStyle[playerid]++;
    if (gCurrentStyle[playerid] > GAMETEXT_STYLE_MAX)
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


ShowCurrentStyle(playerid)
{
    new Float:health, Float:armor, pname[MAX_PLAYER_NAME + 1];
    new score = GetPlayerScore(playerid);
    
    GetPlayerHealth(playerid, health);
    GetPlayerArmour(playerid, armor);
    GetPlayerName(playerid, pname, sizeof(pname));
    
    // Show info about current style
    new string[128];
    format(string, sizeof(string), "Showing style %d of %d", 
        gCurrentStyle[playerid], GAMETEXT_STYLE_MAX);
    SendClientMessage(playerid, -1, string);
    
    if (gTestType[playerid] == 0) // Normal GameText for Player (about the player)
    {
        switch(gCurrentStyle[playerid])
        {
            case 0: GameTextForPlayer(playerid, "~g~MISSION PASSED!~n~~w~Respect +100", 5000, 0);
            case 1: GameTextForPlayer(playerid, "~y~Player Connected~n~~w~Weapons Available", 5000, 1);
            case 2: GameTextForPlayer(playerid, "~r~WASTED $500 lost", 5000, 2);
            case 3: GameTextForPlayer(playerid, "~b~Your Health~n~~w~HP: 100.0", 5000, 3);
            case 4: GameTextForPlayer(playerid, "~p~Your Armor~n~~w~AP: 0.0", 5000, 4);
            case 5: GameTextForPlayer(playerid, "~y~Your Score~n~~w~Position: 1/50", 5000, 5);
            case 6: GameTextForPlayer(playerid, "~b~Your Location~n~~w~Los Santos", 5000, 6);
            case 7: GameTextForPlayer(playerid, "NRG-500", 5000, 7); // Vehicle name style
            case 8: GameTextForPlayer(playerid, "Grove Street", 5000, 8); // Location style
            case 9: GameTextForPlayer(playerid, "Radio: Los Santos", 5000, 9); // Radio name
            case 10: GameTextForPlayer(playerid, "Finding: Station...", 5000, 10); // Radio switch
            case 11: GameTextForPlayer(playerid, "+$1000", 5000, 11); // Positive money
            case 12: GameTextForPlayer(playerid, "-$250", 5000, 12); // Negative money
            case 13: GameTextForPlayer(playerid, "INSANE STUNT BONUS: $500", 5000, 13); // Stunt bonus
            case 14: GameTextForPlayer(playerid, "12:34", 5000, 14); // Clock style
            case 15: GameTextForPlayer(playerid, "~y~YOUR CONTROLS~n~~w~F1: Help Menu~n~F2: Options~n~F3: Statistics~n~ESC: Quit", 5000, 15); // Popup style
            case 16: GameTextForPlayer(playerid, "~g~YOUR TIPS~n~~w~Use /help for commands~n~Press H to enter vehicles~n~Press Y to accept offers", 5000, 16); // Lower popup style
            case 17: GameTextForPlayer(playerid, "~w~You have completed the mission, return to base for reward", 5000, 17); // Subtitles style
        }
    }
    else if (gTestType[playerid] == 1) // Formatted GameText for Player (about the player)
    {
        switch(gCurrentStyle[playerid])
        {
            case 0: GameTextForPlayer(playerid, "~g~MISSION PASSED!~n~~w~Respect +%d", 5000, 0, 100);
            case 1: GameTextForPlayer(playerid, "~y~%s Connected~n~~w~Level: %d", 5000, 1, pname, score);
            case 2: GameTextForPlayer(playerid, "~r~WASTED $%d lost", 5000, 2, 500);
            case 3: GameTextForPlayer(playerid, "~b~Your Health~n~~w~HP: %.1f", 5000, 3, health);
            case 4: GameTextForPlayer(playerid, "~p~Your Armor~n~~w~AP: %.1f", 5000, 4, armor);
            case 5: GameTextForPlayer(playerid, "~y~Your Score: %d~n~~w~Position: %d/%d", 5000, 5, score, 1, GetPlayerCount());
            case 6: GameTextForPlayer(playerid, "~b~Your Location~n~~w~%s", 5000, 6, "Los Santos");
            case 7: GameTextForPlayer(playerid, "Vehicle: %s", 5000, 7, "NRG-500"); // Vehicle name style
            case 8: GameTextForPlayer(playerid, "Area: %s", 5000, 8, "Grove Street"); // Location style
            case 9: GameTextForPlayer(playerid, "Radio: %s", 5000, 9, "Los Santos"); // Radio name
            case 10: GameTextForPlayer(playerid, "Finding: %s...", 5000, 10, "Station"); // Radio switch
            case 11: GameTextForPlayer(playerid, "+$%d", 5000, 11, 1000); // Positive money
            case 12: GameTextForPlayer(playerid, "-$%d", 5000, 12, 250); // Negative money
            case 13: GameTextForPlayer(playerid, "%s STUNT BONUS: $%d", 5000, 13, "INSANE", 500); // Stunt bonus
            case 14: GameTextForPlayer(playerid, "%02d:%02d", 5000, 14, gettime() / 60 % 60, gettime() % 60); // Clock style
            case 15: GameTextForPlayer(playerid, "~y~YOUR CONTROLS~n~~w~%s: Help Menu~n~%s: Options~n~%s: Statistics~n~%s: Quit", 5000, 15, "F1", "F2", "F3", "ESC"); // Popup style
            case 16: GameTextForPlayer(playerid, "~g~YOUR TIPS~n~~w~Current Score: %d~n~Health: %.1f~n~Armor: %.1f", 5000, 16, score, health, armor); // Lower popup style
            case 17: GameTextForPlayer(playerid, "~w~%s, your mission is complete the reward is $%d", 5000, 17, pname, 1000); // Subtitles style
        }
    }
    else if (gTestType[playerid] == 2) // Normal GameText for All (general info)
    {
        switch(gCurrentStyle[playerid])
        {
            case 0: GameTextForAll("~g~EVENT STARTED!~n~~w~Join Now", 5000, 0);
            case 1: GameTextForAll("~y~Server Update~n~~w~New Features Available", 5000, 1);
            case 2: GameTextForAll("~r~SERVER RESTART~n~in 5 minutes", 5000, 2);
            case 3: GameTextForAll("~b~RACE EVENT~n~~w~Starting Soon", 5000, 3);
            case 4: GameTextForAll("~p~ADMIN ANNOUNCEMENT~n~~w~Rules Updated", 5000, 4);
            case 5: GameTextForAll("~y~SERVER STATUS~n~~w~Online: 50/100", 5000, 5);
            case 6: GameTextForAll("~b~WEATHER UPDATE~n~~w~Sunny", 5000, 6);
            case 7: GameTextForAll("New Vehicles Available", 5000, 7); // Vehicle name style
            case 8: GameTextForAll("Los Santos Derby", 5000, 8); // Location style
            case 9: GameTextForAll("Radio: Server FM", 5000, 9); // Radio name
            case 10: GameTextForAll("Changing: Weather...", 5000, 10); // Radio switch
            case 11: GameTextForAll("+$10000 Prize Pool", 5000, 11); // Positive money
            case 12: GameTextForAll("-$5000 Entry Fee", 5000, 12); // Negative money
            case 13: GameTextForAll("SERVER EVENT: $1000 PRIZE", 5000, 13); // Stunt bonus
            case 14: GameTextForAll("12:34", 5000, 14); // Clock style
            case 15: GameTextForAll("~y~SERVER RULES~n~~w~No Cheating~n~No Spamming~n~Be Respectful~n~Have Fun", 5000, 15); // Popup style
            case 16: GameTextForAll("~g~SERVER NEWS~n~~w~New weapons added~n~Map updated~n~Events every hour", 5000, 16); // Lower popup style
            case 17: GameTextForAll("~w~Upcoming server maintenance, please save your progress", 5000, 17); // Subtitles style
        }
    }
    else if (gTestType[playerid] == 3) // Formatted GameText for All (general info)
    {
        switch(gCurrentStyle[playerid])
        {
            case 0: GameTextForAll("~g~EVENT STARTED!~n~~w~%d minutes remaining", 5000, 0, 15);
            case 1: GameTextForAll("~y~Server Update~n~~w~Version %d.%d", 5000, 1, 2, 5);
            case 2: GameTextForAll("~r~SERVER RESTART~n~in %d minutes", 5000, 2, 5);
            case 3: GameTextForAll("~b~RACE EVENT~n~~w~Starting in %d seconds", 5000, 3, 30);
            case 4: GameTextForAll("~p~ADMIN ANNOUNCEMENT~n~~w~%s updated", 5000, 4, "Server Rules");
            case 5: GameTextForAll("~y~SERVER STATUS~n~~w~Online: %d/%d", 5000, 5, GetPlayerCount(), GetMaxPlayers());
            case 6: GameTextForAll("~b~WEATHER UPDATE~n~~w~%s", 5000, 6, "Thunderstorm");
            case 7: GameTextForAll("%s now available", 5000, 7, "New Vehicles"); // Vehicle name style
            case 8: GameTextForAll("%s Event Area", 5000, 8, "Los Santos"); // Location style
            case 9: GameTextForAll("Radio: %s", 5000, 9, "Server FM"); // Radio name
            case 10: GameTextForAll("Changing: %s...", 5000, 10, "Server Time"); // Radio switch
            case 11: GameTextForAll("+$%d Prize Pool", 5000, 11, 10000); // Positive money
            case 12: GameTextForAll("-$%d Entry Fee", 5000, 12, 5000); // Negative money
            case 13: GameTextForAll("%s EVENT: $%d PRIZE", 5000, 13, "SERVER", 1000); // Stunt bonus
            case 14: GameTextForAll("%02d:%02d", 5000, 14, gettime() / 60 % 60, gettime() % 60); // Clock style
            case 15: GameTextForAll("~y~SERVER INFO~n~~w~Online: %d~n~Events: %d/day~n~Updates: Weekly", 5000, 15, GetPlayerCount(), 5); // Popup style
            case 16: GameTextForAll("~g~SERVER NEWS~n~~w~New %s added~n~Map %s~n~Next event: %d min", 5000, 16, "weapons", "updated", 30); // Lower popup style
            case 17: GameTextForAll("~w~%s Event starting, the pize pool is $%d", 5000, 17, "Stunt", 5000); // Subtitles style
        }
    }
    
    // Schedule next style to be shown after 2.5 seconds
    gTestActive[playerid] = SetTimerEx("ContinueStyleTest", 2500, false, "d", playerid);
}

stock GetPlayerCount()
{
    new count = 0;
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i)) count++;
    }
    return count;
}