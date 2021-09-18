#pragma once
#include "PxXY.h"
#include <string>

namespace MSMConstants
{
  ///////////////////////////////////////////////
  enum ActionEnum
  {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ACTION_JUMP,
    ACTION_ATTACK,
    ACTION_SKILL0,
    ACTION_SKILL1,
    ACTION_SKILL2,
    ACTION_SKILL3,
    ACTION_QUICK0,
    ACTION_QUICK1,
    ACTION_QUICK2,
    ACTION_QUICK3,
    ACTION_MAX
  };

  static const PxXY ActionPxXY_1280x720[ACTION_MAX] =
  {
    PxXY(145, 445, 0, true),  // MOVE_UP
    PxXY(145, 650, 0, true),  // MOVE_DOWN
    PxXY(45, 545, 0, true),   // MOVE_LEFT
    PxXY(250, 545, 0, true),  // MOVE_RIGHT
    PxXY(1200, 600, 0, true), // ACTION_JUMP
    PxXY(1070, 600, 0, true), // ACTION_ATTACK
    PxXY(950, 640, 0, true),  // ACTION_SKILL0
    PxXY(950, 530, 0, true),  // ACTION_SKILL1
    PxXY(1030, 470, 0, true), // ACTION_SKILL2
    PxXY(1150, 470, 0, true), // ACTION_SKILL3
    PxXY(1000, 350, 0, true), // ACTION_QUICK0
    PxXY(1065, 350, 0, true), // ACTION_QUICK1
    PxXY(1150, 350, 0, true), // ACTION_QUICK2
    PxXY(1230, 350, 0, true)  // ACTION_QUICK3
  };

  static const PxXY ActionPxXY_960x540[ACTION_MAX] =
  {
    PxXY(110, 335, 0, true), // MOVE_UP
    PxXY(110, 485, 0, true), // MOVE_DOWN
    PxXY(35, 405, 0, true),  // MOVE_LEFT
    PxXY(185, 405, 0, true), // MOVE_RIGHT
    PxXY(905, 450, 0, true), // ACTION_JUMP
    PxXY(805, 450, 0, true), // ACTION_ATTACK
    PxXY(710, 485, 0, true), // ACTION_SKILL0
    PxXY(710, 405, 0, true), // ACTION_SKILL1
    PxXY(775, 350, 0, true), // ACTION_SKILL2
    PxXY(855, 350, 0, true), // ACTION_SKILL3
    PxXY(745, 265, 0, true), // ACTION_QUICK0
    PxXY(800, 265, 0, true), // ACTION_QUICK1
    PxXY(865, 265, 0, true), // ACTION_QUICK2
    PxXY(925, 265, 0, true)  // ACTION_QUICK3
  };

  ///////////////////////////////////////////////
  enum InterfaceEnum
  {
    INGAME_AD,
    ELITE_BATTLE_AD0,
    ELITE_BATTLE_AD1,
    LOADING_SCREEN,
    START_MENU_AD,
    WELCOME_BACK_DIALOG,
    ATTENDANCE_DIALOG,
    SELECT_SERVER,
    SELECT_CHAR,
    START_MENU,
    OPTIONS_DIALOG,
    INFO_DIALOG,
    LOGGED_IN,
    SUBMENU_OPEN,
    AUTOBATTLE_DIALOG,
    DUNGEON_DIALOG,
    ELITE_DUNGEON_DIALOG,
    START_ELITE_DUNGEON,
    ELITE_DUNGEON_IN_PROGRESS,
    ELITE_DUNGEON_DEATH,
    COMPLETE_ELITE_DUNGEON,
    MULUNG_DIALOG,
    MULUNG_WEEKLY_DIALOG,
    ENTER_MULUNG_DIALOG,
    COMPLETE_MULUNG,
    MULUNG_IN_PROGESS,
    CHAOS_DAILY_DUNGEON_DIALOG,
    CHAOS_DAILY_DUNGEON_TICKET,
    CHAOS_DAILY_DUNGEON_IN_PROGRESS,
    COMPLETE_CHAOS_DAILY_DUNGEON,
    DAILY_DUNGEON_DIALOG,
    DAILY_DUNGEON_TICKET,
    DAILY_DUNGEON_IN_PROGRESS,
    DAILY_DUNGEON_DEATH,
    COMPLETE_DAILY_DUNGEON,
    MAIL_OPEN,
    GET_MAIL,
    CONFIRM_MAIL,
    GUILD_CLAIM,
    GET_ALL_DAILY_MISSION,
    CONFIRM_DAILY_MISSION,
    CONFIRM_DAILY_MISSION_EXT,
    CLAIM_WEEKLY_REWARD,
    QUEST_DIALOG,
    QUEST_DIALOG_ACCEPT,
    QUEST_CLAIM_REWARD,
    QUEST_MULTI_AVAILABLE,
    QUEST_IN_PROGRESS,
    DIALOG_OPEN,
    MAP_DIALOG_OPEN,
    CHANNEL_DIALOG_OPEN,
    CAPTCHA_M_UPPER,
    CAPTCHA_M_LOWER,
    CAPTCHA_A_UPPER,
    CAPTCHA_A_LOWER,
    CAPTCHA_P_UPPER,
    CAPTCHA_P_LOWER,
    CAPTCHA_L_UPPER,
    CAPTCHA_L_LOWER,
    CAPTCHA_E_UPPER,
    CAPTCHA_E_LOWER,
    CAPTCHA_SUBMIT,
    TRADE_STATION_IDLE,
    TRADE_STATION_CLOSED,
    TRADE_STATION_FILTER0,
    TRADE_STATION_FILTER1,
    SELECT_WEAPON,
    SELECT_ARMOR,
    SELECT_CATEGORY_0,
    SELECT_CATEGORY_1,
    SELECT_CATEGORY_2,
    SELECT_CATEGORY_3,
    SELECT_CATEGORY_4,
    SELECT_CATEGORY_5,
    SELECT_CATEGORY_6,
    SELECT_CATEGORY_7,
    SELECT_TS_ITEM,
    SELECT_BUY,
    CONFIRM_BUY,
    INTERFACE_MAX
  };

  static const PxXY InterfacePxXY_1280x720[INTERFACE_MAX] =
  {
    PxXY(1143, 71, 165, 165, 165, true),  // INGAME_AD
    PxXY(1143, 71, 165, 165, 165, true),  // ELITE_BATTLE_AD0
    PxXY(1055, 100, 255, 255, 255, true), // ELITE_BATTLE_AD1
    PxXY(325, 350, 0, 0, 0),              // LOADING_SCREEN.
    PxXY(1220, 45, 16, 16, 18, true),     // START_MENU_AD.
    PxXY(1154, 56, 255, 255, 255, true),  // WELCOME_BACK_DIALOG.
    PxXY(1245, 50, 81, 95, 110, true),    // ATTENDANCE_DIALOG.
    PxXY(650, 255, 255, 255, 255, true),  // SELECT_SERVER.
    PxXY(975, 615, 140, 181, 24),         // SELECT_CHAR.
    PxXY(180, 685, 232, 187, 0),          // START_MENU.
    PxXY(1245, 665, 84, 143, 186),        // OPTIONS_DIALOG
    PxXY(660, 240, 84, 143, 186, true),   // INFO_DIALOG
    PxXY(25, 89, 255, 255, 255),          // LOGGED_IN.
    PxXY(183, 23, 255, 255, 255),         // SUBMENU_OPEN.
    PxXY(642, 554, 255, 123, 80, true),   // AUTOBATTLE_DIALOG.
    PxXY(1010, 100, 255, 123, 80),        // DUNGEON_DIALOG.
    PxXY(1150, 250, 81, 95, 110),         // ELITE_DUNGEON_DIALOG.
    PxXY(258, 209, 255, 123, 80, true),   // START_ELITE_DUNGEON.
    PxXY(1244, 216, 255, 236, 50),        // ELITE_DUNGEON_IN_PROGRESS.
    PxXY(450, 520, 84, 143, 186, true),   // ELITE_DUNGEON_DEATH?
    PxXY(375, 637, 84, 143, 186, true),   // COMPLETE_ELITE_DUNGEON.
    PxXY(40, 390, 89, 176, 168),          // MULUNG_DIALOG.
    PxXY(640, 660, 255, 123, 80, true),   // MULUNG_WEEKLY_DIALOG
    PxXY(785, 625, 255, 123, 80, true),   // ENTER_MULUNG_DIALOG.
    PxXY(640, 490, 196, 0, 0),            // COMPLETE_MULUNG.
    PxXY(646, 11, 166, 104, 17),          // MULUNG_IN_PROGESS.
    PxXY(1150, 275, 26, 15, 15),          // CHAOS_DAILY_DUNGEON_DIALOG
    PxXY(785, 410, 255, 255, 255),        // CHAOS_DAILY_DUNGEON_TICKET
    PxXY(1243, 206, 255, 236, 48),        // CHAOS_DAILY_DUNGEON_IN_PROGRESS
    PxXY(445, 600, 84, 143, 186, true),   // COMPLETE_CHAOS_DAILY_DUNGEON
    PxXY(1180, 600, 255, 215, 65),        // DAILY_DUNGEON_DIALOG.
    PxXY(785, 295, 255, 255, 255),        // DAILY_DUNGEON_TICKET.
    PxXY(1244, 216, 255, 236, 50),        // DAILY_DUNGEON_IN_PROGRESS.
    PxXY(431, 356, 84, 143, 186, true),   // DAILY_DUNGEON_DEATH
    PxXY(445, 600, 84, 143, 186, true),   // COMPLETE_DAILY_DUNGEON.
    PxXY(1035, 60, 255, 255, 255),        // MAIL_OPEN.
    PxXY(965, 620, 255, 123, 80, true),   // GET_MAIL.
    PxXY(640, 515, 255, 123, 80, true),   // CONFIRM_MAIL.
    PxXY(1145, 245, 89, 176, 168, true),  // GUILD_CLAIM.
    PxXY(1150, 665, 255, 123, 80, true),  // GET_ALL_DAILY_MISSION.
    PxXY(640, 535, 255, 123, 80, true),   // CONFIRM_DAILY_MISSION.
    PxXY(640, 600, 255, 123, 80, true),   // CONFIRM_DAILY_MISSION_EXT
    PxXY(1142, 467, 158, 221, 238, true), // CLAIM_WEEKLY_REWARD.
    PxXY(1210, 500, 255, 255, 255, true), // QUEST_DIALOG.
    PxXY(1035, 420, 255, 123, 80, true),  // QUEST_DIALOG_ACCEPT.
    PxXY(640, 625, 255, 123, 80, true),   // QUEST_CLAIM_REWARD
    PxXY(355, 570, 255, 123, 80, true),   // QUEST_MULTI_AVAILABLE?
    PxXY(378, 656, 243, 243, 243),        // QUEST_IN_PROGRESS.
    PxXY(1243, 40, 255, 255, 255),        // DIALOG_OPEN.
    PxXY(1170, 40, 255, 123, 80, true),   // MAP_DIALOG_OPEN.
    PxXY(635, 665, 255, 123, 80),         // CHANNEL_DIALOG_OPEN.
    PxXY(440, 420, 255, 255, 255, true),  // CAPTCHA_M_UPPER
    PxXY(440, 525, 255, 255, 255, true),  // CAPTCHA_M_LOWER
    PxXY(540, 413, 255, 255, 255, true),  // CAPTCHA_A_UPPER
    PxXY(540, 518, 255, 255, 255, true),  // CAPTCHA_A_LOWER
    PxXY(640, 409, 255, 255, 255, true),  // CAPTCHA_P_UPPER
    PxXY(640, 514, 255, 255, 255, true),  // CAPTCHA_P_LOWER
    PxXY(740, 423, 255, 255, 255, true),  // CAPTCHA_L_UPPER
    PxXY(740, 528, 255, 255, 255, true),  // CAPTCHA_L_LOWER
    PxXY(840, 405, 255, 255, 255, true),  // CAPTCHA_E_UPPER
    PxXY(840, 510, 255, 255, 255, true),  // CAPTCHA_E_LOWER
    PxXY(639, 663, 255, 123, 82, true),   // CAPTCHA_SUBMIT
    PxXY(30, 138, 255, 255, 255),         // TRADE_STATION_IDLE
    PxXY(477, 657, 255, 255, 255, true),  // TRADE_STATION_CLOSED
    PxXY(160, 190, 255, 123, 80),         // TRADE_STATION_FILTER0
    PxXY(1200, 200, 255, 123, 80),        // TRADE_STATION_FILTER1
    PxXY(190, 275, 68, 81, 95),           // SELECT_WEAPON
    PxXY(190, 355, 68, 81, 95),           // SELECT_ARMOR
    PxXY(430, 280, 68, 81, 95),           // SELCT_CATEGORY_0
    PxXY(430, 335, 68, 81, 95),           // SELCT_CATEGORY_1
    PxXY(430, 390, 68, 81, 95),           // SELCT_CATEGORY_2
    PxXY(430, 445, 68, 81, 95),           // SELCT_CATEGORY_3
    PxXY(430, 500, 68, 81, 95),           // SELCT_CATEGORY_4
    PxXY(430, 555, 68, 81, 95),           // SELCT_CATEGORY_5
    PxXY(430, 610, 68, 81, 95),           // SELCT_CATEGORY_6
    PxXY(430, 665, 68, 81, 95),           // SELCT_CATEGORY_7
    PxXY(840, 340, 255, 255, 255, true),  // SELECT_TS_ITEM
    PxXY(1035, 645, 255, 123, 80, true),  // SELECT_BUY
    PxXY(710, 555, 255, 123, 80, true)    // CONFIRM_BUY
  };

  static const PxXY InterfacePxXY_960x540[INTERFACE_MAX] =
  {
    PxXY(855, 55, 165, 165, 165, true),   // INGAME_AD
    PxXY(790, 75, 255, 255, 255, true),   // ELITE_BATTLE_AD0
    PxXY(795, 75, 255, 255, 255, true),   // ELITE_BATTLE_AD1
    PxXY(250, 270, 0, 0, 0),              // LOADING_SCREEN
    PxXY(925, 25, 16, 16, 18, true),      // START_MENU_AD
    PxXY(865, 42, 255, 255, 255, true),   // WELCOME_BACK_DIALOG
    PxXY(925, 32, 81, 95, 110, true),     // ATTENDANCE_DIALOG
    PxXY(480, 170, 255, 255, 255, true),  // SELECT_SERVER
    PxXY(735, 460, 145, 186, 21),         // SELECT_CHAR
    PxXY(130, 500, 232, 187, 0),          // START_MENU
    PxXY(915, 500, 84, 143, 186),         // OPTIONS_DIALOG
    PxXY(500, 180, 84, 143, 186, true),   // INFO_DIALOG
    PxXY(18, 66, 255, 255, 255),          // LOGGED_IN
    PxXY(135, 35, 255, 255, 255),         // SUBMENU_OPEN
    PxXY(480, 385, 255, 123, 80, true),   // AUTOBATTLE_DIALOG
    PxXY(760, 75, 255, 123, 80),          // DUNGEON_DIALOG
    PxXY(855, 180, 81, 95, 110),          // ELITE_DUNGEON_DIALOG
    PxXY(160, 155, 255, 123, 80, true),   // START_ELITE_DUNGEON
    PxXY(280, 502, 212, 212, 211),        // ELITE_DUNGEON_IN_PROGRESS
    PxXY(300, 390, 84, 143, 186, true),   // ELITE_DUNGEON_DEATH
    PxXY(285, 480, 84, 143, 186, true),   // COMPLETE_ELITE_DUNGEON
    PxXY(30, 300, 89, 176, 168),          // MULUNG_DIALOG
    PxXY(480, 500, 255, 123, 80, true),   // MULUNG_WEEKLY_DIALOG
    PxXY(535, 480, 255, 123, 80, true),   // ENTER_MULUNG_DIALOG
    PxXY(476, 370, 196, 0, 0),            // COMPLETE_MULUNG
    PxXY(478, 9, 166, 104, 17),           // MULUNG_IN_PROGESS
    PxXY(862, 205, 26, 16, 16),           // CHAOS_DAILY_DUNGEON_DIALOG
    PxXY(588, 308, 255, 255, 255),        // CHAOS_DAILY_DUNGEON_TICKET
    PxXY(280, 502, 212, 212, 211),        // CHAOS_DAILY_DUNGEON_IN_PROGRESS
    PxXY(365, 440, 84, 143, 186, true),   // COMPLETE_CHAOS_DAILY_DUNGEON?
    PxXY(900, 435, 255, 215, 65),         // DAILY_DUNGEON_DIALOG
    PxXY(590, 220, 255, 255, 255),        // DAILY_DUNGEON_TICKET
    PxXY(280, 502, 212, 212, 211),        // DAILY_DUNGEON_IN_PROGRESS
    PxXY(340, 270, 84, 143, 186, true),   // DAILY_DUNGEON_DEATH
    PxXY(365, 440, 84, 143, 186, true),   // COMPLETE_DAILY_DUNGEON
    PxXY(775, 45, 255, 255, 255),         // MAIL_OPEN
    PxXY(725, 465, 255, 123, 80, true),   // GET_MAIL
    PxXY(480, 380, 255, 123, 80, true),   // CONFIRM_MAIL
    PxXY(890, 170, 89, 176, 168, true),   // GUILD_CLAIM
    PxXY(900, 485, 255, 123, 80, true),   // GET_ALL_DAILY_MISSION
    PxXY(480, 365, 255, 123, 80, true),   // CONFIRM_DAILY_MISSION
    PxXY(480, 410, 255, 123, 80, true),   // CONFIRM_DAILY_MISSION_EXT
    PxXY(855, 350, 147, 218, 235, true),  // CLAIM_WEEKLY_REWARD
    PxXY(915, 375, 254, 254, 254, true),  // QUEST_DIALOG
    PxXY(880, 315, 255, 123, 80, true),   // QUEST_DIALOG_ACCEPT
    PxXY(480, 495, 255, 123, 80, true),   // QUEST_CLAIM_REWARD
    PxXY(305, 440, 255, 123, 80, true),   // QUEST_MULTI_AVAILABLE
    PxXY(284, 495, 166, 166, 164),        // QUEST_IN_PROGRESS
    PxXY(930, 30, 255, 255, 255),         // DIALOG_OPEN
    PxXY(850, 20, 255, 123, 80, true),    // MAP_DIALOG_OPEN
    PxXY(475, 465, 255, 123, 80),         // CHANNEL_DIALOG_OPEN
    PxXY(329, 314, 255, 255, 255, true),  // CAPTCHA_M_UPPER.
    PxXY(329, 395, 255, 255, 255, true),  // CAPTCHA_M_LOWER.
    PxXY(405, 308, 255, 255, 255, true),  // CAPTCHA_A_UPPER.
    PxXY(404, 388, 255, 255, 255, true),  // CAPTCHA_A_LOWER.
    PxXY(474, 315, 255, 255, 255, true),  // CAPTCHA_P_UPPER
    PxXY(474, 395, 255, 255, 255, true),  // CAPTCHA_P_LOWER.
    PxXY(550, 315, 255, 255, 255, true),  // CAPTCHA_L_UPPER.
    PxXY(550, 396, 255, 255, 255, true),  // CAPTCHA_L_LOWER.
    PxXY(630, 316, 255, 255, 255, true),  // CAPTCHA_E_UPPER.
    PxXY(630, 396, 255, 255, 255, true),  // CAPTCHA_E_LOWER.
    PxXY(482, 498, 255, 123, 80, true),   // CAPTCHA_SUBMIT
    PxXY(30, 138, 255, 255, 255),         // TRADE_STATION_IDLE - TODO
    PxXY(477, 657, 255, 255, 255, true),  // TRADE_STATION_CLOSED
    PxXY(119, 143, 255, 123, 80),         // TRADE_STATION_FILTER0
    PxXY(900, 145, 255, 123, 80),         // TRADE_STATION_FILTER1
    PxXY(145, 205, 68, 81, 95),           // SELECT_WEAPON
    PxXY(145, 265, 68, 81, 95),           // SELECT_ARMOR
    PxXY(325, 210, 68, 81, 95),           // SELCT_CATEGORY_0
    PxXY(325, 250, 68, 81, 95),           // SELCT_CATEGORY_1
    PxXY(325, 290, 68, 81, 95),           // SELCT_CATEGORY_2
    PxXY(325, 330, 68, 81, 95),           // SELCT_CATEGORY_3
    PxXY(325, 375, 68, 81, 95),           // SELCT_CATEGORY_4
    PxXY(325, 420, 68, 81, 95),           // SELCT_CATEGORY_5
    PxXY(325, 460, 68, 81, 95),           // SELCT_CATEGORY_6
    PxXY(325, 500, 68, 81, 95),           // SELCT_CATEGORY_7
    PxXY(655, 255, 255, 255, 255, true),  // SELECT_TS_ITEM
    PxXY(770, 485, 255, 123, 80, true),   // SELECT_BUY
    PxXY(515, 415, 255, 123, 80, true)    // CONFIRM_BUY
  };

  static const std::string InterfaceStrings[INTERFACE_MAX + 1] =
  {
    "Closing in-game Ad",
    "Closing in-game Ad",
    "Closing in-game Ad",
    "Loading",
    "Start Menu Ad",
    "Welcome Back Dialog",
    "Attendance Dialog",
    "Select Server",
    "Select Character",
    "Start Menu",
    "Options Dialog",
    "Information Dialog",
    "Logged in",
    "Submenu Open",
    "AutoBattle Dialog",
    "Dungeon Dialog Open",
    "Elite Dungeon Dialog",
    "Starting Elite Dungeon",
    "Elite Dungeon in Progress",
    "Elite Dungeon Failure",
    "Completing Elite Dungeon",
    "Mu Lung Dialog",
    "Mu Lung Weekly Reward",
    "Entering Mu Lung",
    "Exiting Mu Lung",
    "Mu Lung in Progress",
    "Chaos Daily Dungeon Dialog",
    "Entering Chaos Daily Dungeon",
    "Chaos Daily Dungeon in Progress",
    "Completing Chaos Daily Dungeon",
    "Daily Dungeon Dialog",
    "Entering Daily Dungeon",
    "Daily Dungeon in Progress",
    "Daily Dungeon Failure",
    "Completing Daily Dungeon",
    "Mail Open",
    "Getting Mail",
    "Confirming Mail",
    "Claim Guild Attendance",
    "Claim Daily Rewards",
    "Confirm Daily Rewards",
    "Confirm Daily Rewards",
    "Claim Weekly Rewards",
    "Quest Dialog",
    "Accepting Quest",
    "Claiming Quest Reward",
    "Accepting Quest",
    "Quest In Progress",
    "Dialog Open",
    "Map Dialog Open",
    "Channel Dialog Open",
    "Captcha M",
    "Captcha M",
    "Captcha A",
    "Captcha A",
    "Captcha P",
    "Captcha P",
    "Captcha L",
    "Captcha L",
    "Captcha E",
    "Captcha E",
    "Submit Captcha",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Enabling Trade Station Filter",
    "Hiding Trade Station Unlisted Items",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Scanning Trade Station",
    "Item Found",
    "Attempting Purchase",
    "Confirming Purchase",
    "Waiting..."
  };

  enum ValidationEnum
  {
    MENU_CHECK,
    ATTENDANCE_CHECK,
    BUSY,
    SET_PRESET,
    QUEST_EXPANDED,
    DAILY_DUNGEON_DEATH_CHECK,
    CAPTCHA_M,
    CAPTCHA_A,
    CAPTCHA_P,
    CAPTCHA_L,
    CAPTCHA_E,
    MAX_VALIDATION
  };

  static const PxXY ValidatePxXY_1280x720[MAX_VALIDATION] =
  {
    PxXY(1250, 35, 255, 255, 255),  // MENU_CHECK
    PxXY(1245, 115, 84, 143, 186),  // ATTENDANCE_CHECK
    PxXY(30, 170, 255, 255, 255),   // BUSY
    PxXY(1230, 365, 255, 255, 255), // SET_PRESET
    PxXY(23, 180, 255, 255, 255),   // QUEST_EXPANDED
    PxXY(885, 380, 255, 123, 80),   // DAILY_DUNGEON_DEATH_CHECK
    PxXY(440, 260, 255, 123, 82),   // CAPTCHA_M
    PxXY(540, 260, 255, 123, 82),   // CAPTCHA_A
    PxXY(640, 260, 255, 123, 82),   // CAPTCHA_P
    PxXY(740, 260, 255, 123, 82),   // CAPTCHA_L
    PxXY(840, 260, 255, 123, 82)    // CAPTCHA_E
  };

  static const PxXY ValidatePxXY_960x540[MAX_VALIDATION] =
  {
    PxXY(916, 26, 255, 255, 255),   // MENU_CHECK
    PxXY(930, 90, 84, 143, 186),    // ATTENDANCE_CHECK
    PxXY(25, 130, 255, 255, 255),   // BUSY
    PxXY(922, 275, 255, 255, 255),  // SET_PRESET
    PxXY(17, 136, 255, 255, 255),   // QUEST_EXPANDED
    PxXY(665, 285, 255, 123, 80),   // DAILY_DUNGEON_DEATH_CHECK
    PxXY(330, 195, 255, 123, 82),   // CAPTCHA_M
    PxXY(405, 195, 255, 123, 82),   // CAPTCHA_A
    PxXY(480, 195, 255, 123, 82),   // CAPTCHA_P
    PxXY(555, 195, 255, 123, 82),   // CAPTCHA_L
    PxXY(630, 195, 255, 123, 82)    // CAPTCHA_E
  };

  ///////////////////////////////////////////////
  enum InterfaceActionEnum
  {
    SCREEN_CENTER,
    SELECT_CHAR_0,
    SELECT_CHAR_1,
    SELECT_CHAR_2,
    SELECT_CHAR_3,
    SELECT_CHAR_4,
    SELECT_CHAR_5,
    SELECT_CHAR_6,
	  OPEN_SUB_MENU,
    OPEN_GUILD_MENU,
    OPEN_MAIL,
    OPEN_OPTIONS,
    SELECT_DEFAULT_MAIL,
    SELECT_PERSONAL_MAIL,
    OPEN_CHALLENGE,
    OPEN_ACHIEVEMENTS,
    CONFIRM_ACHIEVEMENTS,
    SELECT_WEEKLY_MISSION,
    SELECT_CONSUME,
    SELECT_ITEM,
    SELECT_FORGE,
    SELECT_HUNT,
    SELECT_D_DUNGEON,
    SELECT_E_DUNGEON,
    SELECT_M_DUNGEON,
    SELECT_DUNGEON,
    SELECT_DAILY_DUNGEON,
    REMOVE_DAILY_DUNGEON_TICKET,
    REMOVE_CHAOS_DAILY_DUNGEON_TICKET,
    SELECT_ELITE_DUNGEON,
    SELECT_MU_LUNG,
    ENTER_DAILY_DUNGEON,
    CREATE_ELITE_DUNGEON,
    ENTER_MU_LUNG,
    EXIT_MU_LUNG,
    DAILY_DUNGEON_CHAOS,
    DAILY_DUNGEON_HELL,
    DAILY_DUNGEON_VHARD,
    DAILY_DUNGEON_HARD,
    DAILY_DUNGEON_NORMAL,
    DAILY_DUNGEON_EASY,
    CONFIRM_DAILY_DUNGEON,
    ENTER_CHAOS_DAILY_DUNGEON,
    CONFIRM_CHAOS_DAILY_DUNGEON,
    SELECT_INFO,
    SELECT_QUEST,
    SELECT_LOCATION,
    SELECT_CHANNEL_1,
    SELECT_CHANNEL_2,
    SELECT_CHANNEL_3,
    SELECT_CHANNEL_4,
    SELECT_CHANNEL_5,
    SELECT_CHANNEL_6,
    SELECT_CHANNEL_7,
    SELECT_CHANNEL_8,
    SELECT_CHANNEL_9,
    SELECT_CHANNEL_10,
    SELECT_CHANNEL_11,
    SELECT_CHANNEL_12,
    SELECT_CHANNEL_13,
    SELECT_CHANNEL_14,
    SELECT_CHANNEL_15,
    SELECT_CHANNEL_16,
    SCROLL_TRADE_STATION,
    INTERFACE_ACTION_MAX
  };

  static const PxXY InterfaceActionPxXY_1280x720[INTERFACE_ACTION_MAX] =
  {
    PxXY(650, 300, 0, true),  // SCREEN_CENTER
    PxXY(275, 235, 0, true),  // SELECT_CHAR_0
    PxXY(475, 235, 0, true),  // SELECT_CHAR_1
    PxXY(650, 235, 0, true),  // SELECT_CHAR_2
    PxXY(175, 525, 0, true),  // SELECT_CHAR_3
    PxXY(375, 525, 0, true),  // SELECT_CHAR_4
    PxXY(555, 525, 0, true),  // SELECT_CHAR_5
    PxXY(755, 525, 0, true),  // SELECT_CHAR_6
	  PxXY(1240, 50, 0, true),  // OPEN_SUB_MENU
    PxXY(795, 650, 0, true),  // OPEN_GUILD_MENU
    PxXY(910, 45, 0, true),   // OPEN_MAIL
    PxXY(1110, 645, 0, true), // OPEN_OPTIONS
    PxXY(430, 155, 0, true),  // SELECT_DEFAULT_MAIL
    PxXY(845, 155, 0, true),  // SELECT_PERSONAL_MAIL
    PxXY(365, 50, 0, true),   // OPEN_CHALLENGE
    PxXY(360, 160, 0, true),  // OPEN_ACHIEVEMENTS
    PxXY(640, 510, 0, true),  // CONFIRM_ACHIEVEMENTS
    PxXY(100, 310, 0, true),  // SELECT_WEEKLY_MISSION
    PxXY(295, 240, 0, true),  // SELECT_CONSUME
    PxXY(435, 240, 0, true),  // SELECT_ITEM
    PxXY(580, 240, 0, true),  // SELECT_FORGE
    PxXY(720, 240, 0, true),  // SELECT_HUNT
    PxXY(860, 240, 0, true),  // SELECT_D_DUNGEON
    PxXY(295, 415, 0, true),  // SELECT_E_DUNGEON
    PxXY(720, 415, 0, true),  // SELECT_M_DUNGEON
    PxXY(1093, 60, 0, true),  // SELECT_DUNGEON
    PxXY(125, 300, 0, true),  // SELECT_DAILY_DUNGEON 
    PxXY(571, 297, 0, true),  // REMOVE_DAILY_DUNGEON_TICKET
    PxXY(575, 410, 0, true),  // REMOVE_CHAOS_DAILY_DUNGEON_TICKET
    PxXY(125, 575, 0, true),  // SELECT_ELITE_DUNGEON
    PxXY(350, 300, 0, true),  // SELECT_MU_LUNG
    PxXY(1100, 650, 0, true), // ENTER_DAILY_DUNGEON
    PxXY(825, 650, 0, true),  // CREATE_ELITE_DUNGEON
    PxXY(1100, 650, 0, true), // ENTER_MU_LUNG
    PxXY(470, 640, 0, true),  // EXIT_MU_LUNG
    PxXY(100, 615, 0, true),  // DAILY_DUNGEON_CHAOS
    PxXY(100, 550, 0, true),  // DAILY_DUNGEON_HELL
    PxXY(100, 475, 0, true),  // DAILY_DUNGEON_VHARD
    PxXY(100, 400, 0, true),  // DAILY_DUNGEON_HARD
    PxXY(100, 340, 0, true),  // DAILY_DUNGEON_NORMAL
    PxXY(100, 265, 0, true),  // DAILY_DUNGEON_EASY
    PxXY(775, 640, 0, true),  // CONFIRM_DAILY_DUNGEON
    PxXY(1120, 660, 0, true), // ENTER_CHAOS_DAILY_DUNGEON
    PxXY(785, 525, 0, true),  // CONFIRM_CHAOS_DAILY_DUNGEON
    PxXY(75, 400, 0, true),   // SELECT_INFO
    PxXY(115, 245, 0, true),  // SELECT_QUEST
    PxXY(25, 140, 0, true),   // SELECT_LOCATION
    PxXY(330, 225, 0, true),  // SELECT_CHANNEL_1
    PxXY(535, 225, 0, true),  // SELECT_CHANNEL_2
    PxXY(740, 225, 0, true),  // SELECT_CHANNEL_3
    PxXY(945, 225, 0, true),  // SELECT_CHANNEL_4
    PxXY(330, 335, 0, true),  // SELECT_CHANNEL_5
    PxXY(535, 335, 0, true),  // SELECT_CHANNEL_6
    PxXY(750, 335, 0, true),  // SELECT_CHANNEL_7
    PxXY(945, 335, 0, true),  // SELECT_CHANNEL_8
    PxXY(330, 445, 0, true),  // SELECT_CHANNEL_9
    PxXY(535, 445, 0, true),  // SELECT_CHANNEL_10
    PxXY(750, 445, 0, true),  // SELECT_CHANNEL_11
    PxXY(945, 445, 0, true),  // SELECT_CHANNEL_12
    PxXY(330, 555, 0, true),  // SELECT_CHANNEL_13
    PxXY(535, 555, 0, true),  // SELECT_CHANNEL_14
    PxXY(750, 555, 0, true),  // SELECT_CHANNEL_15
    PxXY(945, 555, 0, true),  // SELECT_CHANNEL_16
    PxXY(430, 175, 0, true)   // SCROLL_TRADE_STATION
  };

  static const PxXY InterfaceActionPxXY_960x540[INTERFACE_ACTION_MAX] =
  {
    PxXY(480, 270, 0, true),  // SCREEN_CENTER
    PxXY(205, 175, 0, true),  // SELECT_CHAR_0
    PxXY(355, 175, 0, true),  // SELECT_CHAR_1
    PxXY(505, 175, 0, true),  // SELECT_CHAR_2
    PxXY(125, 395, 0, true),  // SELECT_CHAR_3
    PxXY(275, 395, 0, true),  // SELECT_CHAR_4
    PxXY(425, 395, 0, true),  // SELECT_CHAR_5
    PxXY(575, 395, 0, true),  // SELECT_CHAR_6
    PxXY(930, 35, 0, true),   // OPEN_SUB_MENU
    PxXY(600, 485, 0, true),  // OPEN_GUILD_MENU
    PxXY(680, 45, 0, true),   // OPEN_MAIL
    PxXY(835, 490, 0, true),  // OPEN_OPTIONS
    PxXY(340, 110, 0, true),  // SELECT_DEFAULT_MAIL
    PxXY(635, 110, 0, true),  // SELECT_PERSONAL_MAIL
    PxXY(275, 45, 0, true),   // OPEN_CHALLENGE
    PxXY(275, 120, 0, true),  // OPEN_ACHIEVEMENTS
    PxXY(485, 365, 0, true),  // CONFIRM_ACHIEVEMENTS
    PxXY(80, 235, 0, true),   // SELECT_WEEKLY_MISSION
    PxXY(220, 195, 0, true),  // SELECT_CONSUME
    PxXY(325, 195, 0, true),  // SELECT_ITEM
    PxXY(435, 195, 0, true),  // SELECT_FORGE
    PxXY(545, 195, 0, true),  // SELECT_HUNT
    PxXY(645, 195, 0, true),  // SELECT_D_DUNGEON
    PxXY(220, 320, 0, true),  // SELECT_E_DUNGEON
    PxXY(545, 320, 0, true),  // SELECT_M_DUNGEON
    PxXY(820, 45, 0, true),   // SELECT_DUNGEON
    PxXY(95, 225, 0, true),   // SELECT_DAILY_DUNGEON
    PxXY(425, 220, 0, true),  // REMOVE_DAILY_DUNGEON_TICKET
    PxXY(430, 310, 0, true),  // REMOVE_CHAOS_DAILY_DUNGEON_TICKET
    PxXY(95, 435, 0, true),   // SELECT_ELITE_DUNGEON
    PxXY(260, 225, 0, true),  // SELECT_MU_LUNG
    PxXY(845, 495, 0, true),  // ENTER_DAILY_DUNGEON
    PxXY(625, 495, 0, true),  // CREATE_ELITE_DUNGEON
    PxXY(840, 490, 0, true),  // ENTER_MU_LUNG
    PxXY(335, 480, 0, true),  // EXIT_MU_LUNG
    PxXY(75, 460, 0, true),   // DAILY_DUNGEON_CHAOS
    PxXY(75, 405, 0, true),   // DAILY_DUNGEON_HELL
    PxXY(75, 355, 0, true),   // DAILY_DUNGEON_VHARD
    PxXY(75, 300, 0, true),   // DAILY_DUNGEON_HARD
    PxXY(75, 245, 0, true),   // DAILY_DUNGEON_NORMAL
    PxXY(75, 200, 0, true),   // DAILY_DUNGEON_EASY
    PxXY(585, 480, 0, true),  // CONFIRM_DAILY_DUNGEON
    PxXY(840, 495, 0, true),  // ENTER_CHAOS_DAILY_DUNGEON
    PxXY(585, 395, 0, true),  // CONFIRM_CHAOS_DAILY_DUNGEON
    PxXY(55, 300, 0, true),   // SELECT_INFO
    PxXY(85, 180, 0, true),   // SELECT_QUEST
    PxXY(20, 100, 0, true),   // SELECT_LOCATION
    PxXY(250, 155, 0, true),  // SELECT_CHANNEL_1
    PxXY(405, 155, 0, true),  // SELECT_CHANNEL_2
    PxXY(565, 155, 0, true),  // SELECT_CHANNEL_3
    PxXY(715, 155, 0, true),  // SELECT_CHANNEL_4
    PxXY(250, 235, 0, true),  // SELECT_CHANNEL_5
    PxXY(405, 235, 0, true),  // SELECT_CHANNEL_6
    PxXY(565, 235, 0, true),  // SELECT_CHANNEL_7
    PxXY(715, 235, 0, true),  // SELECT_CHANNEL_8
    PxXY(250, 315, 0, true),  // SELECT_CHANNEL_9
    PxXY(405, 315, 0, true),  // SELECT_CHANNEL_10
    PxXY(565, 315, 0, true),  // SELECT_CHANNEL_11
    PxXY(715, 315, 0, true),  // SELECT_CHANNEL_12
    PxXY(250, 395, 0, true),  // SELECT_CHANNEL_13
    PxXY(405, 395, 0, true),  // SELECT_CHANNEL_14
    PxXY(565, 395, 0, true),  // SELECT_CHANNEL_15
    PxXY(715, 395, 0, true),  // SELECT_CHANNEL_16
    PxXY(325, 135, 0, true)   // SCROLL_TRADE_STATION
  };
};