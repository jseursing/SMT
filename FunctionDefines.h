#pragma once
#include <string>
#include <vector>

enum FunctionIdEnum
{
  NO_FUNCTION,
  LOGIN_CHAR,
  LOGOUT,
  CHANGE_PRESET,
  CHANGE_CHANNEL,
  GUILD_ATTENDANCE,
  COLLECT_ACHIEVEMENTS,
  COLLECT_MAIL,
  DAILY_DUNGEON,
  CHAOS_DAILY_DUNGEON,
  ELITE_DUNGEON,
  MU_LUNG,
  QUEST,
  CLICK,
  MOUSE_DOWN,
  MOUSE_UP,
  SWIPE,
  DRAG,
  DELAY,
  REPEAT,
  MOVE,
  SKILL,
  QUICKSLOT,
  IFPIXEL,
  EXTEND_DELAYS,
  RUN_SCRIPT,
  SNIPE_ALL,

  // Begin commanded functions
  GETPIXEL,
  SKIP_FUNCITON,
  SET_FUNCTION,
  EXPERIMENTAL
};

struct FunctionSet
{
  unsigned int function;
  std::vector<int> params;
  unsigned int counter;
  unsigned int errors;
  unsigned int lastState;
  int reliantFunction;
  std::string syntax;

  FunctionSet() : 
    function(0), counter(0), errors(0), lastState(0), reliantFunction(0), syntax("") {}
};
