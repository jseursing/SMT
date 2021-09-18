#include "BotController.h"
#include "MSMManager.h"
#include <fstream>
#include <sstream>

//
// Static declarations
//
std::mutex MSMManager::Lock;

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: SplitSyntax
//
////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> MSMManager::SplitSyntax(std::string text, const char* delim)
{
  std::vector<std::string> tokens;

  unsigned int base = 0;
  unsigned int next = text.find_first_of(delim, base);
  while (std::string::npos != next)
  {
    if ((base != next) && (0 != text.substr(base, next - base).compare("\n")))
    {
      tokens.push_back(text.substr(base, next - base));
    }

    base = next + 1;
    next = text.find_first_of(delim, base);
  }

  // Add remainder of string if base is not at the end..
  if (base < text.length())
  {
    tokens.push_back(text.substr(base));
  }

  return tokens;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: StringReplace
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::StringReplace(std::string& text, const char* delim, const char* ch)
{
  unsigned int base = 0;
  unsigned int next = text.find_first_of(delim, base);
  while (std::string::npos != next)
  {
    text = text.substr(base, next) +
           std::string(ch) +
           text.substr(next + strlen(delim));
    base = 0;
    next = text.find_first_of(delim, base);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: BuildScript
//
////////////////////////////////////////////////////////////////////////////////////////
std::string MSMManager::BuildScript(std::vector<std::string> syntax,
                                    std::vector<FunctionSet>& script)
{
  int relFunction = -1;
  std::string error_str;
  for (unsigned int i = 0; i < syntax.size(); ++i)
  {
    // Remove all spaces
    std::string line = syntax[i];
    StringReplace(line, " ", "");

    // Check for comments before continuing
    if (2 >= line.length())
    {
      syntax.erase(syntax.begin() + i);
      --i;
      continue;
    }
    
    if (('/' == line[0]) && ('/' == line[1]))
    {
      syntax.erase(syntax.begin() + i);
      --i;
      continue;
    }
      
    // Tokenize the string
    std::vector<std::string> tokens = SplitSyntax(syntax[i], "( ,)");
    if (0 == tokens.size())
    {
      // endif will have no tokens...
      if (0 == syntax[i].compare("endif"))
      {
        relFunction = -1;
        continue;
      }

      line = "Invalid syntax: " + line;
      break;
    }

    // Parse each function definition
    FunctionSet function;
    unsigned int requiredParams = 0;
    unsigned int maxOptionParams = 0;

    if (0 == tokens[0].compare("loginchar"))
    {
      function.function = FunctionIdEnum::LOGIN_CHAR;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("logout"))
    {
      function.function = FunctionIdEnum::LOGOUT;
      requiredParams = 0;
    }
    else if (0 == tokens[0].compare("changepreset"))
    {
      function.function = FunctionIdEnum::CHANGE_PRESET;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("changechannel"))
    {
      function.function = FunctionIdEnum::CHANGE_CHANNEL;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("guildattendance"))
    {
      function.function = FunctionIdEnum::GUILD_ATTENDANCE;
      requiredParams = 0;
    }
    else if (0 == tokens[0].compare("achievements"))
    {
      function.function = FunctionIdEnum::COLLECT_ACHIEVEMENTS;
      requiredParams = 1;
      maxOptionParams = 7;
    }
    else if (0 == tokens[0].compare("collectmail"))
    {
      function.function = FunctionIdEnum::COLLECT_MAIL;
      requiredParams = 1;
      maxOptionParams = 2;
    }
    else if (0 == tokens[0].compare("dailydungeon"))
    {
      function.function = FunctionIdEnum::DAILY_DUNGEON;
      requiredParams = 2;
    }
    else if (0 == tokens[0].compare("chaosdailydungeon"))
    {
      function.function = FunctionIdEnum::CHAOS_DAILY_DUNGEON;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("elitedungeon"))
    {
      function.function = FunctionIdEnum::ELITE_DUNGEON;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("mulung"))
    {
      function.function = FunctionIdEnum::MU_LUNG;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("move"))
    {
      function.function = FunctionIdEnum::MOVE;
      requiredParams = 0;
      maxOptionParams = 1;
    }
    else if (0 == tokens[0].compare("skill"))
    {
      function.function = FunctionIdEnum::SKILL;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("quickslot"))
    {
      function.function = FunctionIdEnum::QUICKSLOT;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("quest"))
    {
      function.function = FunctionIdEnum::QUEST;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("click"))
    {
      function.function = FunctionIdEnum::CLICK;
      requiredParams = 2;
      maxOptionParams = 3;
    }
    else if (0 == tokens[0].compare("mousedown"))
    {
      function.function = FunctionIdEnum::MOUSE_DOWN;
      requiredParams = 2;
    }
    else if (0 == tokens[0].compare("mouseup"))
    {
      function.function = FunctionIdEnum::MOUSE_UP;
      requiredParams = 2;
    }
    else if (0 == tokens[0].compare("swipe"))
    {
      function.function = FunctionIdEnum::SWIPE;
      requiredParams = 3;
    }
    else if (0 == tokens[0].compare("drag"))
    {
      function.function = FunctionIdEnum::DRAG;
      requiredParams = 4;
    }
    else if (0 == tokens[0].compare("delay"))
    {
      function.function = FunctionIdEnum::DELAY;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("repeat"))
    {
      function.function = FunctionIdEnum::REPEAT;
      requiredParams = 2;
    }
    else if (0 == tokens[0].compare("ifpixel"))
    {
      function.function = FunctionIdEnum::IFPIXEL;
      relFunction = script.size();
      requiredParams = 5;
    }
    else if (0 == tokens[0].compare("extenddelays"))
    {
      function.function = FunctionIdEnum::EXTEND_DELAYS;
      requiredParams = 1;
    }
    else if (0 == tokens[0].compare("runscript"))
    {
      // Do not parse parameters..
      function.function = FunctionIdEnum::RUN_SCRIPT;  
      function.syntax = syntax[i];
      script.push_back(function);
      continue;
    }
    else if (0 == tokens[0].compare("snipeall"))
    {
      // Do not parse parameters..
      function.function = FunctionIdEnum::SNIPE_ALL;
      requiredParams = 1;
    }
    else 
    {
      error_str = "Invalid command: " + line;
    }

    // Fill parameters
    unsigned int param = 0;
    for (unsigned int i = 1; i < tokens.size(); ++i)
    {
      param = MapParameter(tokens[i]);
      if (0xFFFFFFFF == param) // Exit if invalid parameter
      {
        error_str = "Invalid parameter: " + line;
        return error_str;
      }

      // For the following function types, we need to subtract one
      // so the user doesn't identify first index as 0.
      if ((FunctionIdEnum::LOGIN_CHAR == function.function) ||
          (FunctionIdEnum::CHANGE_PRESET == function.function) ||
          (FunctionIdEnum::SKILL == function.function) ||
          (FunctionIdEnum::QUICKSLOT == function.function) ||
          (FunctionIdEnum::REPEAT == function.function))
      {
        --param;
      }

      // Add the parameter and exit if we meet/exceed the required
      // parameter count.
      function.params.push_back(param);
      if ((function.params.size() >= requiredParams) &&
          (function.params.size() >= maxOptionParams))
      {
        break;
      }
    }

    // If we didn't get the right amount of parameters, exit
    if (function.params.size() < requiredParams)
    {
      error_str = "Not enough parameters: " + line;
      break;
    }

    // Assign reliant function for ifpixel
    function.reliantFunction = relFunction;

    // Assign syntax
    function.syntax = syntax[i];
    if (function.syntax[function.syntax.length() - 1] == '\r')
    {
      function.syntax[function.syntax.length() - 1] = '\0';
    }

    // Reset counter and errors
    function.errors = 0;
    function.counter = 0;

    // Add function
    script.push_back(function);
  }

  // If all functions were generated, erase the error string.
  if (script.size() == syntax.size())
  {
    error_str.clear();
  }

  return error_str;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ReadScriptFile
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ReadScriptFile(std::string path, std::vector<std::string>& syntax)
{
  std::ifstream inFile(path, std::ios::binary);
  if (false == inFile.is_open())
  {
    MessageBoxA(0, std::string("Unable to open script file " + path).c_str(), 
                "Error", MB_OK);
    return false;
  }

  unsigned int dataLength = 0;
  unsigned int dataBegin = inFile.tellg();
  inFile.seekg(0, std::ios::end);
  dataLength = static_cast<unsigned int>(inFile.tellg()) - dataBegin;
  inFile.seekg(0, std::ios::beg);

  // Read the file into a string buffer.
  std::string data;
  data.resize(dataLength);
  inFile.read(&data[0], dataLength);

  // Close the file handle
  inFile.close();

  // If there isn't a new line at the end, add it
  if (data[data.length() - 1] != '\n')
  {
    data += "\n";
  }

  // Split the text file into lines
  syntax = SplitSyntax(data, "\r\n\b");

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: CalibrateLogin
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::CalibrateLogin()
{
  PxXY pixels;
  int response = 0;
  BotController* bController = reinterpret_cast<BotController*>(Controller);

  // Begin prep
  MessageBoxA(0, "Logout, Click OK at the Start Menu.", "Calibration", MB_OK);

  // START_MENU_AD
  response = MessageBoxA(0, "Is the Start Menu AD open?", "Calibration", MB_YESNO);
  if (IDYES == response)
  {
    bController->Delay(1000);
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::START_MENU_AD]);
    memcpy(&InterfacePxXY[MSMConstants::START_MENU_AD], &pixels, sizeof(PxXY));
    bController->LeftClick(InterfacePxXY[MSMConstants::START_MENU_AD]);
    MessageBoxA(0, "Click OK when the AD is closed", "Calibration", MB_OK);
  }

  // START_MENU
  bController->Delay(1000);
  pixels = bController->GetPX(InterfacePxXY[MSMConstants::START_MENU]);
  memcpy(&InterfacePxXY[MSMConstants::START_MENU], &pixels, sizeof(PxXY));
  bController->LeftClick(ActionPxXY[MSMConstants::SCREEN_CENTER]);
  MessageBoxA(0, "Click OK when Server selection is visible", "Calibration", MB_OK);

  // SELECT_SERVER
  bController->Delay(1000);
  pixels = bController->GetPX(InterfacePxXY[MSMConstants::SELECT_SERVER]);
  memcpy(&InterfacePxXY[MSMConstants::SELECT_SERVER], &pixels, sizeof(PxXY));
  bController->LeftClick(InterfacePxXY[MSMConstants::SELECT_SERVER]);
  MessageBoxA(0, "Click OK when Character selection is visible", "Calibration", MB_OK);

  // SELECT_CHAR
  bController->Delay(1000);
  pixels = bController->GetPX(InterfacePxXY[MSMConstants::SELECT_CHAR]);
  memcpy(&InterfacePxXY[MSMConstants::SELECT_CHAR], &pixels, sizeof(PxXY));
  bController->LeftClick(InterfacePxXY[MSMConstants::SELECT_CHAR]);
  MessageBoxA(0, "Click OK when logged in", "Calibration", MB_OK);

  // INGAME_AD
  response = MessageBoxA(0, "Is the In-Game AD open?", "Calibration", MB_YESNO);
  if (IDYES == response)
  {
    bController->Delay(1000);
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::INGAME_AD]);
    memcpy(&InterfacePxXY[MSMConstants::INGAME_AD], &pixels, sizeof(PxXY));
    bController->LeftClick(InterfacePxXY[MSMConstants::INGAME_AD]);
  }

  // LOGGED_IN
  MessageBoxA(0, "Click OK when all Dialogs and ADs are closed", "Calibration", MB_OK);
  bController->Delay(1000);
  pixels = bController->GetPX(InterfacePxXY[MSMConstants::LOGGED_IN]);
  memcpy(&InterfacePxXY[MSMConstants::LOGGED_IN], &pixels, sizeof(PxXY));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: CalibrateQuest
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::CalibrateQuest()
{
  PxXY pixels;
  int response = 0;
  BotController* bController = reinterpret_cast<BotController*>(Controller);

  // Begin prep
  MessageBoxA(0, "Start a NEW Quest. Once a Quest dialog is visible, click OK", 
                 "Calibration", MB_OK);

  // QUEST_DIALOG
  response = MessageBoxA(0, "Do you see two right arrows in the dialog?", 
                            "Calibration", MB_YESNO);
  if (IDYES == response)
  {
    bController->Delay(1000);
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::QUEST_DIALOG]);
    memcpy(&InterfacePxXY[MSMConstants::QUEST_DIALOG], &pixels, sizeof(PxXY));
    bController->LeftClick(InterfacePxXY[MSMConstants::QUEST_DIALOG]);
  }

  // QUEST_ACCEPT
  MessageBoxA(0, "Click OK when you see the ACCEPT button", "Calibration", MB_OK);
  bController->Delay(1000);
  pixels = bController->GetPX(InterfacePxXY[MSMConstants::QUEST_DIALOG_ACCEPT]);
  memcpy(&InterfacePxXY[MSMConstants::QUEST_DIALOG_ACCEPT], &pixels, sizeof(PxXY));
  bController->LeftClick(InterfacePxXY[MSMConstants::QUEST_DIALOG_ACCEPT]);
  MessageBoxA(0, "Click OK when you are auto battling for the Quest. "
                 "Verify the screen does not dim at any point.",
                 "Calibration", MB_OK);

  // QUEST_IN_PROGRESS
  bController->Delay(1000);
  pixels = bController->GetPX(InterfacePxXY[MSMConstants::QUEST_IN_PROGRESS]);
  memcpy(&InterfacePxXY[MSMConstants::QUEST_IN_PROGRESS], &pixels, sizeof(PxXY));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: GeneratePixelData
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::GeneratePixelData(unsigned int w, unsigned int h)
{
  char current_dir[MAX_PATH] = { 0 };
  GetCurrentDirectoryA(MAX_PATH, current_dir);
  sprintf_s(current_dir, "%s\\corrections%dx%d.cpx", current_dir, w, h);

  std::ofstream outFile(current_dir, std::ios::binary);
  if (false == outFile.is_open())
  {
    MessageBoxA(0, "Failed generating corrections file.", "Error", MB_OK);
    return;
  }

  char temp_line[256] = { 0 };
  for (unsigned int i = MSMConstants::INGAME_AD; i < MSMConstants::INTERFACE_MAX; ++i)
  {
    sprintf_s(temp_line,
              "%d, %d, %d, %d, %d //%s\n",
              InterfacePxXY[i].X,
              InterfacePxXY[i].Y,
              InterfacePxXY[i].R,
              InterfacePxXY[i].G,
              InterfacePxXY[i].B,
              MSMConstants::InterfaceStrings[i].c_str());
    outFile.write(temp_line, strlen(temp_line));
  }

  for (unsigned int i = MSMConstants::MENU_CHECK; i < MSMConstants::MAX_VALIDATION; ++i)
  {
    sprintf_s(temp_line,
              "%d, %d, %d, %d, %d \n",
              ValidatePxXY[i].X,
              ValidatePxXY[i].Y,
              ValidatePxXY[i].R,
              ValidatePxXY[i].G,
              ValidatePxXY[i].B);
    outFile.write(temp_line, strlen(temp_line));
  }

  outFile.close();

  MessageBoxA(0, "Corrections file generated. Click the mushroom to apply changes. "
                 "To Revert changes, delete the corrections file in the bot folder "
                 "and restart the bot.", 
                 "Calibration", MB_OK);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: CorrectPixelData
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::CorrectPixelData(std::string path)
{
  std::ifstream inFile(path.c_str(), std::ios::binary);
  if (false == inFile.is_open())
  {
    return false;
  }

  unsigned int dataLength = 0;
  unsigned int dataBegin = inFile.tellg();
  inFile.seekg(0, std::ios::end);
  dataLength = static_cast<unsigned int>(inFile.tellg()) - dataBegin;
  inFile.seekg(0, std::ios::beg);

  // Read the file into a string buffer.
  std::string data;
  data.resize(dataLength);
  inFile.read(&data[0], dataLength);

  // Close the file handle
  inFile.close();

  // Separate each line into a vector of strings
  std::vector<std::string> lines = SplitSyntax(data, "\n");
  if (lines.size() < (MSMConstants::INTERFACE_MAX + MSMConstants::MAX_VALIDATION - 1))
  {
    MessageBoxA(0, "Data length mismatch", "Corrections", MB_OK);
    return false;
  }

  // Tokenize each line and replace pixel data
  bool updated = true;

  Lock.lock();

  for (unsigned int i = 0; i < lines.size(); ++i)
  {
    std::vector<std::string> tokens = SplitSyntax(lines[i], "( ,)");
    if (5 > tokens.size())
    {
      char error[256] = { 0 };
      sprintf_s(error, "Error on Line:%d \"%s\"", i, tokens[i].c_str());
      MessageBoxA(0, error, "Corrections", MB_OK);
      updated = false;
      break;
    }

    if (i < MSMConstants::INTERFACE_MAX)
    {
      InterfacePxXY[i].X = atoi(tokens[0].c_str());
      InterfacePxXY[i].Y = atoi(tokens[1].c_str());
      InterfacePxXY[i].R = atoi(tokens[2].c_str());
      InterfacePxXY[i].G = atoi(tokens[3].c_str());
      InterfacePxXY[i].B = atoi(tokens[4].c_str());
    }
    else
    {
      unsigned int relI = i - MSMConstants::INTERFACE_MAX;
      if (relI < MSMConstants::MAX_VALIDATION)
      {
        ValidatePxXY[relI].X = atoi(tokens[0].c_str());
        ValidatePxXY[relI].Y = atoi(tokens[1].c_str());
        ValidatePxXY[relI].R = atoi(tokens[2].c_str());
        ValidatePxXY[relI].G = atoi(tokens[3].c_str());
        ValidatePxXY[relI].B = atoi(tokens[4].c_str());
      }
    }
  }

  Lock.unlock();

  return updated;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: MapParameter
//
////////////////////////////////////////////////////////////////////////////////////////
unsigned int MSMManager::MapParameter(std::string token)
{
  // Daily dungeon parameters
  if (0 == token.compare("hell"))
  {
    return MSMConstants::DAILY_DUNGEON_HELL;
  }
  if (0 == token.compare("veryhard"))
  {
    return MSMConstants::DAILY_DUNGEON_VHARD;
  }
  if (0 == token.compare("hard"))
  {
    return MSMConstants::DAILY_DUNGEON_HARD;
  }
  if (0 == token.compare("normal"))
  {
    return MSMConstants::DAILY_DUNGEON_NORMAL;
  }
  if (0 == token.compare("easy"))
  {
    return MSMConstants::DAILY_DUNGEON_EASY;
  }

  // Collect Achievements parameters
  if (0 == token.compare("consume"))
  {
    return MSMConstants::SELECT_CONSUME;
  }
  if (0 == token.compare("item"))
  {
    return MSMConstants::SELECT_ITEM;
  }
  if (0 == token.compare("forge"))
  {
    return MSMConstants::SELECT_FORGE;
  }
  if (0 == token.compare("hunt"))
  {
    return MSMConstants::SELECT_HUNT;
  }
  if (0 == token.compare("daily"))
  {
    return MSMConstants::SELECT_D_DUNGEON;
  }
  if (0 == token.compare("elite"))
  {
    return MSMConstants::SELECT_E_DUNGEON;
  }
  if (0 == token.compare("ml"))
  {
    return MSMConstants::SELECT_M_DUNGEON;
  }

  // Movement parameters
  if (0 == token.compare("up"))
  {
    return MSMConstants::MOVE_UP;
  }
  if (0 == token.compare("down"))
  {
    return MSMConstants::MOVE_DOWN;
  }
  if (0 == token.compare("left"))
  {
    return MSMConstants::MOVE_LEFT;
  }
  if (0 == token.compare("right"))
  {
    return MSMConstants::MOVE_RIGHT;
  }
  if (0 == token.compare("jump"))
  {
    return MSMConstants::ACTION_JUMP;
  }
  if (0 == token.compare("attack"))
  {
    return MSMConstants::ACTION_ATTACK;
  }

  // Mail parameters
  if (0 == token.compare("default"))
  {
    return MSMConstants::SELECT_DEFAULT_MAIL;
  }
  if (0 == token.compare("personal"))
  {
    return MSMConstants::SELECT_PERSONAL_MAIL;
  }

  // If the parameter has any non-numerical numbers, do not process
  bool hasAscii = false;
  for (unsigned int i = 0; i < token.length(); ++i)
  {
    if ((0x30 > token[i]) || (0x39 < token[i]))
    {
      hasAscii = true;
      break;
    }
  }

  if (false == hasAscii)
  {
    return atoi(token.c_str());
  }

  return 0xFFFFFFFF;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessMSMFunction
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessMSMFunction(FunctionSet* function, 
                                    MSMConstants::InterfaceEnum& state)
{
  bool functionComplete = false;
  BotController* bController = reinterpret_cast<BotController*>(Controller);

  // Update the current character slot if the function is login
  if (FunctionIdEnum::LOGIN_CHAR == function->function)
  {
    CurrentCharSlot = function->params[0];
  }

  // Search for the in-game ad before continuing.
  if (true == ProcessAds(state))
  {
    return false;
  }

  // Search for captcha before continuing.
  ProcessCaptcha();
  
  // Check for the auto battle dialog, close if it necessary.
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::AUTOBATTLE_DIALOG]);
  if (pixels == InterfacePxXY[MSMConstants::AUTOBATTLE_DIALOG])
  {
    bController->LeftClick(InterfacePxXY[MSMConstants::AUTOBATTLE_DIALOG]);
    bController->Delay(2000);
  }

  // The following functions will not rely on logged in/out state.
  switch (function->function)
  {
  // This function requires the trade station dialog to be already open.
  case FunctionIdEnum::SNIPE_ALL:
    CurrentState = MSMManager::SNIPE_ALL;
    break;
  }

  // Begin finite state machine
  switch (CurrentState)
  {
  case MSMManager::INITIAL:
    // If the MSM State is INITIAL, we need to find out if we are logged
    // in or not. Update the bot controller state separately.
    bController->RefreshPixels();
    functionComplete = ProcessInitialState(function, state);
    break;
  case MSMManager::LOGGING_IN:
    bController->RefreshPixels();
    functionComplete = ProcessLoggingInState(function, state);
    break;
  case MSMManager::LOGGED_IN:
    bController->RefreshPixels();
    functionComplete = ProcessLoggedInState(function, state);
    break;
  case MSMManager::LOGGING_OUT:
    bController->RefreshPixels();
    functionComplete = ProcessLoggingOutState(function, state);
    break;
  case MSMManager::CHANGING_CHANNEL:
    bController->RefreshPixels();
    functionComplete = ProcessChangingChannelState(function, state);
    break;
  case MSMManager::GUILD:
    bController->RefreshPixels();
    functionComplete = ProcessGuildState(function, state);
    break;
  case MSMManager::ACHIEVEMENTS:
    bController->RefreshPixels();
    functionComplete = ProcessAchievementsState(function, state);
    break;
  case MSMManager::MAIL:
    if (FunctionIdEnum::COLLECT_MAIL == function->function)
    {
      bController->RefreshPixels();
      functionComplete = ProcessMailState(function, state);
    }

    bController->LeftClick(InterfacePxXY[MSMConstants::MAIL_OPEN]);
    break;
  case MSMManager::ELITE_DUNGEON:
    bController->RefreshPixels();
    functionComplete = ProcessEliteDungeonState(function, state);
    break;
  case MSMManager::DAILY_DUNGEON:
    bController->RefreshPixels();
    functionComplete = ProcessDailyDungeonState(function, state);
    break;
  case MSMManager::CHAOS_DAILY_DUNGEON:
    bController->RefreshPixels();
    functionComplete = ProcessChaosDailyDungeonState(function, state);
    break;  
  case MSMManager::MU_LUNG:
    bController->RefreshPixels();
    functionComplete = ProcessMuLungState(function, state);
    break;
  case MSMManager::QUEST:
    bController->RefreshPixels();
    functionComplete = ProcessQuestState(function, state);
    break;
  case MSMManager::SNIPE_ALL:
    bController->RefreshPixels();
    functionComplete = ProcessSnipeAllState(function, state);
    break;
  }

  // Do not increase the counter (Do not proceed to next function).
  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ResetCurrentState
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ResetCurrentState()
{
  CurrentState = MSMManager::INITIAL;
  CurrentCharSlot = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: GetCurrentState
//
////////////////////////////////////////////////////////////////////////////////////////
MSMManager::MSMState MSMManager::GetCurrentState() const
{
  return CurrentState;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: GetCurrentStateString
//
////////////////////////////////////////////////////////////////////////////////////////
const char* MSMManager::GetCurrentStateString() const
{
  return MSMStateStrings[CurrentState].c_str();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: Constructor
//
////////////////////////////////////////////////////////////////////////////////////////
MSMManager::MSMManager(void* controller, unsigned int w, unsigned int h) :
  Controller(controller),
  CurrentState(MSMManager::INITIAL),
  CurrentCharSlot(0),
  ActionPxXY(0),
  InterfacePxXY(0),
  InterfaceActionPxXY(0),
  ValidatePxXY(0)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);

  if ((NOXConstants::ReqWindowWidth == w) && 
      (NOXConstants::ReqWindowHeight == h))
  {
    ActionPxXY = const_cast<PxXY*>(MSMConstants::ActionPxXY_1280x720);
    InterfacePxXY = const_cast<PxXY*>(MSMConstants::InterfacePxXY_1280x720);
    InterfaceActionPxXY = const_cast<PxXY*>(MSMConstants::InterfaceActionPxXY_1280x720);
    ValidatePxXY = const_cast<PxXY*>(MSMConstants::ValidatePxXY_1280x720);
    bController->SendStatus("[!] Coordinates Set for 1280x720");
  }
  else
  {
    if ((NOXConstants::ReqWindowWidthOpt == w) &&
        (NOXConstants::ReqWindowHeightOpt == h))
    {
      ActionPxXY = const_cast<PxXY*>(MSMConstants::ActionPxXY_960x540);
      InterfacePxXY = const_cast<PxXY*>(MSMConstants::InterfacePxXY_960x540);
      InterfaceActionPxXY = const_cast<PxXY*>(MSMConstants::InterfaceActionPxXY_960x540);
      ValidatePxXY = const_cast<PxXY*>(MSMConstants::ValidatePxXY_960x540);
      bController->SendStatus("[!] Coordinates Set for 960x540");
    }
    else
    {
      std::string error_status = "[!] Unsupported resolution " +
                                 std::to_string(w) + "x" + std::to_string(h);
      bController->SendStatus(error_status);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: Destructor
//
////////////////////////////////////////////////////////////////////////////////////////
MSMManager::~MSMManager()
{

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessInitialState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessInitialState(FunctionSet*& func, 
                                     MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // First, if any dialog is open, close it...
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
  if (pixels == InterfacePxXY[MSMConstants::DIALOG_OPEN])
  {
    bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
    bController->Delay(2000);
  }

  // Found our current state.
  bool functionComplete = false;
  for (unsigned int i = MSMConstants::LOADING_SCREEN;
       i < MSMConstants::INTERFACE_MAX;
       ++i)
  {
    pixels = bController->GetPX(InterfacePxXY[i], false);
    if (pixels == InterfacePxXY[i])
    {
      state = static_cast<MSMConstants::InterfaceEnum>(i);
      bController->SendStatus(state, func->syntax);
      switch (state)
      {
      case MSMConstants::START_MENU_AD:
      case MSMConstants::START_MENU:
      case MSMConstants::SELECT_SERVER:
      case MSMConstants::SELECT_CHAR:
      case MSMConstants::WELCOME_BACK_DIALOG:
      case MSMConstants::ATTENDANCE_DIALOG:
        pixels = reinterpret_cast<BotController*>(Controller)->GetPX
                 (ValidatePxXY[MSMConstants::MENU_CHECK]);
        if (pixels != ValidatePxXY[MSMConstants::MENU_CHECK])
        {
          CurrentState = LOGGING_IN;
          break;
        }
      default:
        CurrentState = LOGGED_IN;
        if (FunctionIdEnum::LOGIN_CHAR == func->function)
        {
          functionComplete = true;
        }
      }
      break;
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessLoggingInState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessLoggingInState(FunctionSet*& func,
                                       MSMConstants::InterfaceEnum& state)
{
  bool functionComplete = false;
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // If the MSM State is LOGGING_IN, we only check for every
  // state leading up to logged in.
  for (unsigned int i = MSMConstants::LOADING_SCREEN;
       i <= MSMConstants::SUBMENU_OPEN;
       ++i)
  {
    PxXY pixels = bController->GetPX(InterfacePxXY[i], false);
    if (pixels == InterfacePxXY[i])
    {
      state = static_cast<MSMConstants::InterfaceEnum>(i);
      bController->SendStatus(state, func->syntax);
      switch (state)
      {
      case MSMConstants::LOADING_SCREEN:
        bController->Delay(3000); // Wait a bit.
        break;
      case MSMConstants::LOGGED_IN:
        CurrentState = MSMManager::LOGGED_IN;

        // When auto-questing, we may end up here, do not
        // complete the function if QUEST is the current state.
        if (FunctionIdEnum::QUEST != func->function)
        {
          bController->SendStatus("[" + func->syntax + "] Logged in");
          functionComplete = true;
        }
        break;
      case MSMConstants::SELECT_SERVER:
        // We may mistakenly think we are logged out, fix this by checking for
        // the sub menu.
        pixels = reinterpret_cast<BotController*>(Controller)->GetPX
                 (ValidatePxXY[MSMConstants::MENU_CHECK]);
        if (pixels == ValidatePxXY[MSMConstants::MENU_CHECK])
        {
          state = MSMConstants::DIALOG_OPEN;
          CurrentState = MSMManager::LOGGED_IN;
          break;
        }

        ProcessLogin(state);
        break;
      case MSMConstants::SUBMENU_OPEN:
        CurrentState = MSMManager::LOGGED_IN;
        bController->LeftClick(InterfaceActionPxXY[state]);
        bController->Delay(1000);
        break;
      default:
        ProcessLogin(state);
      }

      break;
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessLoggedInState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessLoggedInState(FunctionSet*& func, 
                                      MSMConstants::InterfaceEnum& state)
{
  bool functionComplete = false;
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // Check to see if we are logged out, update current state if so.
  if (true == IsLoggedOut())
  {
    CurrentState = MSMManager::LOGGING_IN;
    state = MSMConstants::START_MENU;
    return functionComplete;
  }

  // Check for leftover dialogs we didn't catch...
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::CONFIRM_MAIL]);
  if (pixels == InterfacePxXY[MSMConstants::CONFIRM_MAIL])
  {
    bController->LeftClick(InterfacePxXY[MSMConstants::CONFIRM_MAIL]);
    bController->Delay(1000);
  }

  pixels = bController->GetPX(InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION]);
  if (pixels == InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION])
  {
    bController->LeftClick(InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION]);
    bController->Delay(1000);
  }

  pixels = bController->GetPX(InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION_EXT]);
  if (pixels == InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION_EXT])
  {
    bController->LeftClick(InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION_EXT]);
    bController->Delay(1000);
  }

  for (unsigned int i = MSMConstants::LOGGED_IN;
       i <= MSMConstants::DIALOG_OPEN;
       ++i)
  {
    pixels = bController->GetPX(InterfacePxXY[i], false);
    if (pixels == InterfacePxXY[i])
    {
      state = static_cast<MSMConstants::InterfaceEnum>(i);
      bController->SendStatus(state, func->syntax);
      switch (state)
      {
      case MSMConstants::LOGGED_IN:

        // React according to function type
        switch (func->function)
        {
        case FunctionIdEnum::LOGIN_CHAR:
          bController->SendStatus("[" + func->syntax + "] Already logged in");
          functionComplete = true;
          break;
        case FunctionIdEnum::QUEST:
          CurrentState = MSMManager::QUEST;
          break;
        case FunctionIdEnum::CHANGE_CHANNEL:
          CurrentState = MSMManager::CHANGING_CHANNEL;
          break;
        case FunctionIdEnum::MOVE:
        case FunctionIdEnum::SKILL:
        case FunctionIdEnum::QUICKSLOT:
          ProcessManualActions(func, state);
          functionComplete = true;
          break;
        case FunctionIdEnum::DAILY_DUNGEON:
        case FunctionIdEnum::ELITE_DUNGEON:
        case FunctionIdEnum::CHAOS_DAILY_DUNGEON:
        case FunctionIdEnum::MU_LUNG:
        case FunctionIdEnum::GUILD_ATTENDANCE:
        case FunctionIdEnum::COLLECT_ACHIEVEMENTS:
        case FunctionIdEnum::COLLECT_MAIL:
        case FunctionIdEnum::LOGOUT:
          // Function types which require the sub menu
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_SUB_MENU]);
          bController->Delay(2000);
          break;
        case FunctionIdEnum::CHANGE_PRESET:
          // The quick slot area only has four pages to swipe through...
          for (unsigned int page = 0; page < 4; ++page)
          {
            pixels = bController->GetPX(ValidatePxXY[MSMConstants::SET_PRESET]);
            if (pixels == ValidatePxXY[MSMConstants::SET_PRESET])
            {
              break;
            }

            bController->Swipe(ValidatePxXY[MSMConstants::SET_PRESET], 
                                            MSMConstants::MOVE_LEFT);
            bController->Delay(1000);
          }

          // Click the specified preset.
          if (pixels == ValidatePxXY[MSMConstants::SET_PRESET])
          {
            bController->LeftClick(ActionPxXY[MSMConstants::ACTION_QUICK0 + 
                                   func->params[0]]);
            functionComplete = true;
          }
          break;
        }

        break;
      case MSMConstants::SUBMENU_OPEN:
        switch (func->function)
        {
        case FunctionIdEnum::DAILY_DUNGEON: // Open dungeon window
        case FunctionIdEnum::ELITE_DUNGEON:
        case FunctionIdEnum::CHAOS_DAILY_DUNGEON:
        case FunctionIdEnum::MU_LUNG:
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_DUNGEON]);
          bController->Delay(3000);
          break;
        case FunctionIdEnum::GUILD_ATTENDANCE: // Open guild window
          CurrentState = MSMManager::GUILD;
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_GUILD_MENU]);
          bController->Delay(3000);
          break;
        case FunctionIdEnum::COLLECT_ACHIEVEMENTS: // Open achievements
          CurrentState = MSMManager::ACHIEVEMENTS;
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_CHALLENGE]);
          bController->Delay(2000);
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_ACHIEVEMENTS]);
          bController->Delay(3000);
          break;
        case FunctionIdEnum::COLLECT_MAIL: // Open Mail
          CurrentState = MSMManager::MAIL;
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_MAIL]);
          bController->Delay(3000);
          break;
        case FunctionIdEnum::LOGOUT: // Open Options
          CurrentState = MSMManager::LOGGING_OUT;
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_OPTIONS]);
          bController->Delay(3000);
          break;
        default:
          // Close the submenu
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
        }
        break;
      case MSMConstants::MAIL_OPEN:
      case MSMConstants::GET_ALL_DAILY_MISSION:
      case MSMConstants::CONFIRM_DAILY_MISSION:
      case MSMConstants::CONFIRM_DAILY_MISSION_EXT:
      case MSMConstants::CLAIM_WEEKLY_REWARD:
      case MSMConstants::CONFIRM_MAIL:
        // If we reach this case while status=LOGGED_IN close it because
        // the actual state processing function should handle our case.
        bController->LeftClick(InterfacePxXY[state]);
        break;
      case MSMConstants::DUNGEON_DIALOG:
        switch (func->function)
        {
        case FunctionIdEnum::DAILY_DUNGEON:
        case FunctionIdEnum::CHAOS_DAILY_DUNGEON:
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_DAILY_DUNGEON]);
          bController->Delay(2000);
          break;
        case FunctionIdEnum::ELITE_DUNGEON:
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_ELITE_DUNGEON]);
          bController->Delay(2000);
          break;
        case FunctionIdEnum::MU_LUNG:
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_MU_LUNG]);
          bController->Delay(2000);
          break;
        default: // Close the dialog
          bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
          bController->Delay(3000);
        }
        break;
      case MSMConstants::DIALOG_OPEN: // Close the dialog        
        bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
        bController->Delay(2000);
        break;
      case MSMConstants::ELITE_DUNGEON_DIALOG:
        if (FunctionIdEnum::ELITE_DUNGEON == func->function)
        {
          CurrentState = MSMManager::ELITE_DUNGEON;
          break;
        }

        // Close the dialog if we don't intend on being here...
        bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
        break;
      case MSMConstants::DAILY_DUNGEON_DIALOG:
        if (FunctionIdEnum::DAILY_DUNGEON == func->function)
        {
          CurrentState = MSMManager::DAILY_DUNGEON;
          break;
        }
        else if (FunctionIdEnum::CHAOS_DAILY_DUNGEON == func->function)
        {
          // Select the correct dungeon
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::DAILY_DUNGEON_CHAOS]);
        }
        else
        {
          // Close the dialog if we don't intend on being here...
          bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
        }
        break;
      case MSMConstants::MULUNG_DIALOG:
        if (FunctionIdEnum::MU_LUNG == func->function)
        {
          CurrentState = MSMManager::MU_LUNG;
          break;
        }

        // Close the dialog if we don't intend on being here...
        bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
        break;
      case MSMConstants::CHAOS_DAILY_DUNGEON_DIALOG:
        if (FunctionIdEnum::CHAOS_DAILY_DUNGEON == func->function)
        {
          CurrentState = MSMManager::CHAOS_DAILY_DUNGEON;
        }
        else if (FunctionIdEnum::DAILY_DUNGEON == func->function)
        {
          // Select the correct dungeon
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::DAILY_DUNGEON_HELL]);
        }
        else
        {
          // Close the dialog if we don't intend on being here...
          bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
        }
        break;
      case MSMConstants::QUEST_DIALOG_ACCEPT:
      case MSMConstants::QUEST_CLAIM_REWARD:
      case MSMConstants::QUEST_DIALOG:
      case MSMConstants::QUEST_MULTI_AVAILABLE:
      case MSMConstants::QUEST_IN_PROGRESS:
        if (FunctionIdEnum::QUEST == func->function)
        {
          CurrentState = MSMManager::QUEST;
        }
        break;
      case MSMConstants::MULUNG_IN_PROGESS:
        // Set state to MU_LUNG regardless so the bot can
        // exit the dungeon.
        CurrentState = MSMManager::MU_LUNG;
        break;
      case MSMConstants::ELITE_DUNGEON_IN_PROGRESS:
        // Set state to ELITE_DUNGEON regardless so the bot can
        // exit the dungeon.
        CurrentState = MSMManager::ELITE_DUNGEON;
        break;
      case MSMConstants::DAILY_DUNGEON_IN_PROGRESS:
        // Set state to DAILY_DUNGEON_IN_PROGRESS regardless so the bot can
        // exit the dungeon.
        CurrentState = MSMManager::DAILY_DUNGEON;
        break;
      case MSMConstants::CHAOS_DAILY_DUNGEON_IN_PROGRESS:
        // Set state to CHAOS_DAILY_DUNGEON_IN_PROGRESS regardless so the bot can
        // exit the dungeon.
        CurrentState = MSMManager::CHAOS_DAILY_DUNGEON;
        break;
      default:
        // Catch-all for all mistaken states (when menus are open)
        bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
        break;
      }

      break; // Exit for loop
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessLoggingOutState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessLoggingOutState(FunctionSet*& func,
                                        MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // Check to see if we are logged out, update current state if so.
  if (true == IsLoggedOut())
  {
    CurrentState = MSMManager::LOGGING_IN;
    state = MSMConstants::START_MENU;
    return true;
  }

  for (unsigned int i = MSMConstants::OPTIONS_DIALOG;
       i <= MSMConstants::SUBMENU_OPEN;
       ++i)
  {
    PxXY pixels = bController->GetPX(InterfacePxXY[i], false);
    if (pixels == InterfacePxXY[i])
    {
      state = static_cast<MSMConstants::InterfaceEnum>(i);
      bController->SendStatus(state, func->syntax);
      switch (state)
      {
      case MSMConstants::OPTIONS_DIALOG:
        bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_INFO]);
        bController->Delay(2000);
        break;
      case MSMConstants::INFO_DIALOG:
        bController->LeftClick(InterfacePxXY[MSMConstants::INFO_DIALOG]);
        bController->Delay(2000);
        break;
      case MSMConstants::SUBMENU_OPEN:
        bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_OPTIONS]);
        bController->Delay(2000);
        break;
      }

      break;
    }
  }

  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessChangingChannelState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessChangingChannelState(FunctionSet*& func,
                                             MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // If we are just logged in, open the map dialog
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::LOGGED_IN]);
  if (pixels == InterfacePxXY[MSMConstants::LOGGED_IN])
  {
    bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_LOCATION]);
    CurrentState = MSMManager::CHANGING_CHANNEL;
    bController->Delay(3000);
  }

  // Check for the rest of states
  bool functionComplete = false;
  for (unsigned int i = MSMConstants::MAP_DIALOG_OPEN;
       i <= MSMConstants::CHANNEL_DIALOG_OPEN;
       ++i)
  {
    pixels = bController->GetPX(InterfacePxXY[i]);
    if (pixels == InterfacePxXY[i])
    {
      state = static_cast<MSMConstants::InterfaceEnum>(i);
      bController->SendStatus(state, func->syntax);
      switch (state)
      {
      case MSMConstants::MAP_DIALOG_OPEN:
        bController->LeftClick(InterfacePxXY[i]);
        bController->Delay(3000);
        break;
      case MSMConstants::CHANNEL_DIALOG_OPEN:
        if (0 == func->params[0])
        {
          bController->LeftClick(InterfaceActionPxXY[BotController::Random(
                                                     MSMConstants::SELECT_CHANNEL_1,
                                                     MSMConstants::SELECT_CHANNEL_16)]);
        }
        else
        {
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_CHANNEL_1 +
                                                     func->params[0]]);
        }

        bController->Delay(2000);
        bController->LeftClick(InterfacePxXY[i]);

        if (FunctionIdEnum::CHANGE_CHANNEL == func->function)
        {
          functionComplete = true;
        }
        break;
      }

      break; // Exit for loop
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessGuildState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessGuildState(FunctionSet*& func,
                                   MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  bool functionComplete = false;

  // If the submenu isn't open, open it.
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
  if (pixels != InterfacePxXY[MSMConstants::SUBMENU_OPEN])
  {
    bController->LeftClick(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
    bController->Delay(3000);
  }

  // If the guild dialog still isn't open, open it.
  if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
  {
    bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_GUILD_MENU]);
    bController->Delay(3000);
  }

  // If the dialog is now open, claim attendance.
  pixels = bController->GetPX(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
  if (pixels == InterfacePxXY[MSMConstants::DIALOG_OPEN])
  {
    if (true == bController->LClickUntilChange(&InterfacePxXY[MSMConstants::GUILD_CLAIM],
                                               &InterfacePxXY[MSMConstants::GUILD_CLAIM],
                                               2000))
    {
      bController->SendStatus("[" + func->syntax + "] Claimed");
    }

    bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
    CurrentState = MSMManager::LOGGED_IN;
    functionComplete = true;
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessAchievementsState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessAchievementsState(FunctionSet*& func,
                                          MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  bool functionComplete = false;
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
  if (pixels == InterfacePxXY[MSMConstants::DIALOG_OPEN])
  {
    // Confirm daily rewards (twice for auto battle)
    for (unsigned int i = 0; i < 2; ++i)
    {
      pixels = bController->GetPX(InterfacePxXY[MSMConstants::GET_ALL_DAILY_MISSION]);
      if (pixels == InterfacePxXY[MSMConstants::GET_ALL_DAILY_MISSION])
      {
        // Click Get All
        bController->LClickUntilChange
          (&InterfacePxXY[MSMConstants::GET_ALL_DAILY_MISSION],
           &InterfacePxXY[MSMConstants::GET_ALL_DAILY_MISSION],
           3000);

        // Confirm daily rewards
        bController->LClickUntilChange
          (&InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION],
           &InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION],
           3000);

        bController->LClickUntilChange
          (&InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION_EXT],
           &InterfacePxXY[MSMConstants::CONFIRM_DAILY_MISSION_EXT],
           3000);

        bController->SendStatus("[" + func->syntax + "] Claimed");
      }
    }

    bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_WEEKLY_MISSION]);
    bController->Delay(3000);

    for (unsigned int i = 0; i < func->params.size(); ++i)
    {
      bController->LeftClick(InterfaceActionPxXY
       [static_cast<MSMConstants::InterfaceActionEnum>(func->params[i])]);
      bController->Delay(2000);

      // Claim each item without verification...
      pixels = bController->GetPX(InterfacePxXY[MSMConstants::CLAIM_WEEKLY_REWARD]);
      if (pixels != InterfacePxXY[MSMConstants::CLAIM_WEEKLY_REWARD])
      {
        bController->LeftClick(InterfacePxXY[MSMConstants::CLAIM_WEEKLY_REWARD]);
        bController->Delay(2000);
      }
    }

    // Close the dialog
    bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
    functionComplete = true;
  }

  // Allow the bot to restart this procedure.
  CurrentState = MSMManager::LOGGED_IN;
  state = MSMConstants::LOGGED_IN;

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessMailState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessMailState(FunctionSet*& func,
                                  MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // If the submenu is open, click the mail icon
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
  if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
  {
    bController->LeftClick(InterfaceActionPxXY[MSMConstants::OPEN_MAIL]);
    bController->Delay(2000);
  }

  // Look for other states
  bool functionComplete = true;
  for (unsigned int type : func->params)
  {
    for (unsigned int i = MSMConstants::MAIL_OPEN;
         i <= MSMConstants::CONFIRM_MAIL;
         ++i)
    {
      pixels = bController->GetPX(InterfacePxXY[i]);
      if (pixels == InterfacePxXY[i])
      {
        state = static_cast<MSMConstants::InterfaceEnum>(i);
        bController->SendStatus(state, func->syntax);
        switch (state)
        {
        case MSMConstants::MAIL_OPEN:
          bController->LeftClick(InterfaceActionPxXY[type]);
          bController->Delay(1000);
          // No break here to speed up the process
        case MSMConstants::GET_MAIL:
          bController->LeftClick(InterfacePxXY[MSMConstants::GET_MAIL]);
          bController->Delay(2000);
          // No break here to speed up the process
        case MSMConstants::CONFIRM_MAIL:
          bController->LeftClick(InterfacePxXY[MSMConstants::CONFIRM_MAIL]);
          bController->Delay(2000);
          break;
        default: // Invalid state, start over
          state = MSMConstants::LOGGED_IN;
          functionComplete = false;
        }
      }
    }
  }

  CurrentState = MSMManager::LOGGED_IN;

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessEliteDungeonState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessEliteDungeonState(FunctionSet*& func,
                                          MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  std::string status;
  bool functionComplete = false;
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::COMPLETE_ELITE_DUNGEON]);

  // If the completion dialog is visible, click the button and update current state.
  if (pixels == InterfacePxXY[MSMConstants::COMPLETE_ELITE_DUNGEON])
  {
    bController->SendStatus(MSMConstants::COMPLETE_ELITE_DUNGEON, func->syntax);
    bController->LClickUntilChange
      (&InterfacePxXY[MSMConstants::COMPLETE_ELITE_DUNGEON],
       &InterfacePxXY[MSMConstants::COMPLETE_ELITE_DUNGEON],
       3000);

    if (FunctionIdEnum::ELITE_DUNGEON == func->function)
    {
      ++func->counter;
      if (func->counter >= func->params[0])
      {
        functionComplete = true;

        status = "[" + func->syntax + "] Complete";
        bController->SendStatus(status);
      }
      else
      {
        status = "[" + func->syntax + "] " +
                 std::to_string(func->params[0] - func->counter) +
                 " remaining";
        bController->SendStatus(status);
      }
    }

    CurrentState = MSMManager::LOGGED_IN;
    state = MSMConstants::LOGGED_IN;
    bController->Delay(3000);
  }
  else
  {
    // Check to see if we died in elite dungeon, if so, complete the function
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::ELITE_DUNGEON_DEATH]);
    if (pixels == InterfacePxXY[MSMConstants::ELITE_DUNGEON_DEATH])
    {
      bController->LeftClick(InterfacePxXY[MSMConstants::ELITE_DUNGEON_DEATH]);

      CurrentState = MSMManager::LOGGED_IN;
      state = MSMConstants::LOGGED_IN;
      return true;
    }

    // Once we are in dungeon state, the sub menu shouldn't be open.
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
    if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
    {
      bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
      bController->Delay(2000);
    }

    // Check for other states...
    bool processed = false;
    for (unsigned int i = MSMConstants::ELITE_DUNGEON_DIALOG;
         i <= MSMConstants::COMPLETE_ELITE_DUNGEON;
         ++i)
    {
      pixels = bController->GetPX(InterfacePxXY[i]);
      if (pixels == InterfacePxXY[i])
      {
        state = static_cast<MSMConstants::InterfaceEnum>(i);
        ProcessEliteDungeon(func, state);
        processed = true;
        break;
      }
    }

    if (false == processed)
    {
      if (true == IsLoggedInBusy())
      {
        state = static_cast<MSMConstants::InterfaceEnum>
                (MSMConstants::ELITE_DUNGEON_IN_PROGRESS);
        bController->SendStatus(state, func->syntax);
        bController->Delay(1000);
      }
      else // We reached an erroneous state.. end the function
      {
        functionComplete = true;
      }
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessDailyDungeonState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessDailyDungeonState(FunctionSet*& func,
                                          MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);
  
  std::string status;
  bool functionComplete = false;
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::COMPLETE_DAILY_DUNGEON]);

  // If the completion dialog is visible, click the button and update current state.
  if (pixels == InterfacePxXY[MSMConstants::COMPLETE_DAILY_DUNGEON])
  {
    bController->SendStatus(MSMConstants::COMPLETE_DAILY_DUNGEON, func->syntax);
    bController->LClickUntilChange
      (&InterfacePxXY[MSMConstants::COMPLETE_DAILY_DUNGEON],
       &InterfacePxXY[MSMConstants::COMPLETE_DAILY_DUNGEON],
       3000);

    CurrentState = MSMManager::LOGGED_IN;
    state = MSMConstants::LOGGED_IN;

    if (FunctionIdEnum::DAILY_DUNGEON == func->function)
    {
      functionComplete = true;
      status = "[" + func->syntax + "] Complete";
      bController->SendStatus(status);
    }

    bController->Delay(3000);
  }
  else
  {
    // Check to see if we died in daily dungeon, if so, complete the function
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::DAILY_DUNGEON_DEATH]);
    if (pixels == InterfacePxXY[MSMConstants::DAILY_DUNGEON_DEATH])
    {
      // We may mistakenly think we died, validate this
      pixels = bController->GetPX(ValidatePxXY[MSMConstants::DAILY_DUNGEON_DEATH_CHECK]);
      if (pixels == ValidatePxXY[MSMConstants::DAILY_DUNGEON_DEATH_CHECK])
      {
        bController->LeftClick(InterfacePxXY[MSMConstants::DAILY_DUNGEON_DEATH]);
        CurrentState = MSMManager::LOGGED_IN;
        state = MSMConstants::LOGGED_IN;
        return true;
      }
    }

    // Once we are in dungeon state, the sub menu shouldn't be open.
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
    if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
    {
      bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
      bController->Delay(2000);
    }

    // Check for other states.
    bool processed = false;
    for (unsigned int i = MSMConstants::DAILY_DUNGEON_DIALOG;
         i <= MSMConstants::COMPLETE_DAILY_DUNGEON;
         ++i)
    {
      pixels = bController->GetPX(InterfacePxXY[i]);
      if (pixels == InterfacePxXY[i])
      {
        state = static_cast<MSMConstants::InterfaceEnum>(i);
        bController->SendStatus(state, func->syntax);
        ProcessDailyDungeon(func, state);
        processed = true;
        break;
      }
    }

    if (false == processed)
    {
      if (true == IsLoggedInBusy())
      {
        state = static_cast<MSMConstants::InterfaceEnum>
                (MSMConstants::DAILY_DUNGEON_IN_PROGRESS);
        bController->SendStatus(state, func->syntax);
        bController->Delay(1000);
      }
      else // We reached an erroneous state.. end the function
      {
        functionComplete = true;
      }
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessChaosDailyDungeonState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessChaosDailyDungeonState(FunctionSet*& func,
                                               MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  std::string status;
  bool functionComplete = false;
  PxXY pixels = bController->GetPX(InterfacePxXY
                                   [MSMConstants::COMPLETE_CHAOS_DAILY_DUNGEON]);

  // If the completion dialog is visible, click the button and update current state.
  if (pixels == InterfacePxXY[MSMConstants::COMPLETE_CHAOS_DAILY_DUNGEON])
  {
    bController->SendStatus(MSMConstants::COMPLETE_CHAOS_DAILY_DUNGEON, func->syntax);
    bController->LClickUntilChange
      (&InterfacePxXY[MSMConstants::COMPLETE_CHAOS_DAILY_DUNGEON],
       &InterfacePxXY[MSMConstants::COMPLETE_CHAOS_DAILY_DUNGEON],
       3000);

    CurrentState = MSMManager::LOGGED_IN;
    state = MSMConstants::LOGGED_IN;

    if (FunctionIdEnum::CHAOS_DAILY_DUNGEON == func->function)
    {
      functionComplete = true;
      status = "[" + func->syntax + "] Complete";
      bController->SendStatus(status);
    }

    bController->Delay(3000);
  }
  else
  {
    // Once we are in dungeon state, the sub menu shouldn't be open.
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
    if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
    {
      bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
      bController->Delay(2000);
    }

    bool processed = false;
    for (unsigned int i = MSMConstants::CHAOS_DAILY_DUNGEON_DIALOG;
         i <= MSMConstants::COMPLETE_CHAOS_DAILY_DUNGEON;
         ++i)
    {
      pixels = bController->GetPX(InterfacePxXY[i]);
      if (pixels == InterfacePxXY[i])
      {
        state = static_cast<MSMConstants::InterfaceEnum>(i);
        bController->SendStatus(state, func->syntax);
        ProcessChaosDailyDungeon(func, state);
        processed = true;
        break;
      }
    }

    if (false == processed)
    {
      if (true == IsLoggedInBusy())
      {
        state = static_cast<MSMConstants::InterfaceEnum>
                (MSMConstants::CHAOS_DAILY_DUNGEON_IN_PROGRESS);
        bController->SendStatus(state, func->syntax);
        bController->Delay(1000);
      }
      else // We reached an erroneous state.. end the function
      {
        functionComplete = true;
      }
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessMuLungState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessMuLungState(FunctionSet*& func,
                                    MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  std::string status;
  bool functionComplete = false;
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::COMPLETE_MULUNG]);

  // If the completion dialog is visible, click the button and update current state.
  if (pixels == InterfacePxXY[MSMConstants::COMPLETE_MULUNG])
  {
    bController->SendStatus(MSMConstants::COMPLETE_MULUNG, func->syntax);
    bController->LClickUntilChange
      (&InterfacePxXY[MSMConstants::COMPLETE_MULUNG],
       &InterfaceActionPxXY[MSMConstants::EXIT_MU_LUNG],
       3000);

    CurrentState = MSMManager::LOGGED_IN;
    state = MSMConstants::LOGGED_IN;

    if (FunctionIdEnum::MU_LUNG == func->function)
    {
      ++func->counter;
      if (func->counter >= func->params[0])
      {
        functionComplete = true;
        status = "[" + func->syntax + "] Complete";
        bController->SendStatus(status);
      }
      else
      {
        status = "[" + func->syntax + "] " +
                  std::to_string(func->params[0] - func->counter) +
                  " remaining";
        bController->SendStatus(status);
      }
    }

    bController->Delay(3000);
  }
  else
  {
    // Once we are in dungeon state, the sub menu shouldn't be open.
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
    if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
    {
      bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
      bController->Delay(2000);
    }

    bool processed = false;
    for (unsigned int i = MSMConstants::MULUNG_DIALOG;
         i <= MSMConstants::COMPLETE_MULUNG;
         ++i)
    {
      pixels = bController->GetPX(InterfacePxXY[i]);
      if (pixels == InterfacePxXY[i])
      {
        state = static_cast<MSMConstants::InterfaceEnum>(i);
        bController->SendStatus(state, func->syntax);
        ProcessMuLung(func, state);
        processed = true;
        break;
      }
    }

    if (false == processed)
    {
      if (true == IsLoggedInBusy())
      {
        state = static_cast<MSMConstants::InterfaceEnum>
                (MSMConstants::MULUNG_IN_PROGESS);
        bController->SendStatus(state, func->syntax);
        bController->Delay(1000);
      }
      else // We reached an erroneous state.. end the function
      {
        functionComplete = true;
      }
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessQuestState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessQuestState(FunctionSet*& func,
                                   MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  PxXY pixels;
  std::string status;
  bool functionComplete = false;

  bool checkLoginState = true;
  for (unsigned int i = MSMConstants::QUEST_DIALOG;
        i <= MSMConstants::QUEST_IN_PROGRESS;
        ++i)
  {
    pixels = bController->GetPX(InterfacePxXY[i]);
    if (pixels == InterfacePxXY[i])
    {
      func->errors = 0;
      checkLoginState = false;

      state = static_cast<MSMConstants::InterfaceEnum>(i);
      bController->SendStatus(state, func->syntax);
      switch (state)
      {
      case MSMConstants::QUEST_IN_PROGRESS:
        bController->Delay(3000);
        break;
      case MSMConstants::QUEST_DIALOG:
      case MSMConstants::QUEST_DIALOG_ACCEPT:
        ProcessQuest(func, state);
        break;
      case MSMConstants::QUEST_CLAIM_REWARD:
        ProcessQuest(func, state);

        // Increase quest counter, exit this function if 
        // the counter exceeds count.
        ++func->counter;
        if (0 != func->params[0])
        {
          if (func->counter > func->params[0])
          {
            func->counter = 0;
            functionComplete = true;
            status = "[" + func->syntax + "] Complete";
            bController->SendStatus(status);
          }
          else
          {
            status = "[" + func->syntax + "] " +
                     std::to_string(func->params[0] - func->counter) +
                     " remaining";
            bController->SendStatus(status);
          }
        }
        break;
      default: // Perform login check
        ProcessQuest(func, state);
      }

      break; // Exit for loop
    }
  }

  // Check login state if no valid state was found
  if (true == checkLoginState)
  {
    // Check logged in state
    pixels = bController->GetPX(InterfacePxXY[MSMConstants::LOGGED_IN]);
    if (pixels == InterfacePxXY[MSMConstants::LOGGED_IN])
    {
      state = MSMConstants::LOGGED_IN;
      bController->SendStatus(state, func->syntax);

      ++func->errors;
      if (3 <= func->errors)
      {
        // Check to see if the quest is not expanded..
        pixels = bController->GetPX(ValidatePxXY[MSMConstants::QUEST_EXPANDED]);
        if (pixels == ValidatePxXY[MSMConstants::QUEST_EXPANDED])
        {
          bController->LeftClick(ValidatePxXY[MSMConstants::QUEST_EXPANDED]);
          bController->Delay(1000);
        }
        else
        {
          bController->LeftClick(InterfaceActionPxXY[MSMConstants::SELECT_QUEST]);
          bController->Delay(3000);
        }
      }

      if (10 <= func->errors)
      {
        if (10 == func->errors)
        {
          // If we are stuck in this state 10x, attempt
          // to change channels. 
          CurrentState = MSMManager::CHANGING_CHANNEL;
          bController->SendStatus("[" + func->syntax + "] Changing Channel");
        }
        else if (15 <= func->errors)
        {
          // We changed channels by now, exit this function
          func->errors = 0;
          functionComplete = true;
          bController->SendStatus("[" + func->syntax + "] Timeout error");
        }
      }
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessSnipeAllState
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessSnipeAllState(FunctionSet*& func,
                                      MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  std::string status;
  bool functionComplete = false;
  
  // Open the submenu if we are just logged in
  bController->LClickUntilChange
    (&InterfacePxXY[MSMConstants::TRADE_STATION_IDLE],
     &InterfaceActionPxXY[MSMConstants::OPEN_SUB_MENU],
     3000);

  // If the submenu is open, select the trade station
  bController->LClickUntilChange
    (&InterfacePxXY[MSMConstants::TRADE_STATION_CLOSED],
     &InterfacePxXY[MSMConstants::TRADE_STATION_CLOSED],
     3000);

  // If the filter isn't already set, click it
  bController->LClickUntilChange
    (&InterfacePxXY[MSMConstants::TRADE_STATION_FILTER0],
     &InterfacePxXY[MSMConstants::TRADE_STATION_FILTER0],
     3000,
     false);

  // If the "Display items with no.." is set, click it
  bController->LClickUntilChange
    (&InterfacePxXY[MSMConstants::TRADE_STATION_FILTER1],
     &InterfacePxXY[MSMConstants::TRADE_STATION_FILTER1],
     3000);

  // Begin looking for other states.
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::CONFIRM_BUY]);
  if (pixels == InterfacePxXY[MSMConstants::CONFIRM_BUY])
  {
    if (true == bController->LClickUntilChange(&InterfacePxXY[MSMConstants::CONFIRM_BUY],
                                               &InterfacePxXY[MSMConstants::CONFIRM_BUY],
                                               2000))
    {
      status = "[" + func->syntax + "] Item sniped";
      bController->SendStatus(status);
      ++func->counter;
    }

    // Check to see if the function is complete
    if (0 != func->params[0])
    {
      if (func->counter >= func->params[0])
      {
        functionComplete = true;
      }
    }
  }
  else
  {
    bool processed = false;
    for (unsigned int i = MSMConstants::SELECT_CATEGORY_0;
         i <= MSMConstants::SELECT_CATEGORY_7;
         ++i)
    {
      pixels = bController->GetPX(InterfacePxXY[i]);
      if (pixels == InterfacePxXY[i])
      {
        state = static_cast<MSMConstants::InterfaceEnum>(i);
        ProcessSnipeAll(func, state);
        processed = true;
        break;
      }
    }

    // If no state is found, click the first item...
    if (false == processed)
    {
      state = MSMConstants::TRADE_STATION_IDLE;
      ProcessSnipeAll(func, state);
    }
  }

  return functionComplete;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessLogin
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessLogin(MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, "loginchar");

  // If the state has response on, click the coordinate.
  if (true == InterfacePxXY[state].Response)
  {
    bController->LeftClick(InterfacePxXY[state]);
    bController->Delay(5000);
    return;
  }

  // We need to find the specific state and react accordingly.
  std::vector<PxXY> responses;
  switch (state)
  {
  case MSMConstants::START_MENU:
    responses.push_back(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
    break;
  case MSMConstants::SELECT_CHAR:
    responses.push_back(InterfaceActionPxXY[MSMConstants::SELECT_CHAR_0 +
                                            CurrentCharSlot]);
    responses.push_back(InterfaceActionPxXY[MSMConstants::SELECT_CHAR_0 +
                                            CurrentCharSlot]);
    responses.push_back(InterfaceActionPxXY[MSMConstants::SELECT_CHAR_0 +
                                            CurrentCharSlot]);
    responses.push_back(InterfacePxXY[MSMConstants::SELECT_CHAR]);
    break;
  }

  for (PxXY response : responses)
  {
    bController->LeftClick(response);
    bController->Delay(2000);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessEliteDungeon
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessEliteDungeon(FunctionSet* func,
                                     MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // Check to see if we are logged out, update current state if so.
  if (true == IsLoggedOut())
  {
    CurrentState = MSMManager::LOGGING_IN;
    state = MSMConstants::START_MENU;
    return;
  }

  // If the state has response on, click the coordinate.
  if (true == InterfacePxXY[state].Response)
  {
    bController->LeftClick(InterfacePxXY[state]);
    return;
  }

  // If the submenu is open, close it
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
  if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
  {
    bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
  }

  // We need to find the specific state and react accordingly.
  std::vector<PxXY> responses;
  switch (state)
  {
  case MSMConstants::ELITE_DUNGEON_DIALOG:
    responses.push_back(InterfaceActionPxXY[MSMConstants::CREATE_ELITE_DUNGEON]);
    break;
  case MSMConstants::START_ELITE_DUNGEON:
    responses.push_back(InterfacePxXY[MSMConstants::START_ELITE_DUNGEON]);
    break;
  case MSMConstants::ELITE_DUNGEON_IN_PROGRESS:
    bController->Delay(1000);
    break;
  }

  for (PxXY response : responses)
  {
    bController->LeftClick(response);
    bController->Delay(3000);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessDailyDungeon
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessDailyDungeon(FunctionSet* func,
                                     MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // Check to see if we are logged out, update current state if so.
  if (true == IsLoggedOut())
  {
    CurrentState = MSMManager::LOGGING_IN;
    state = MSMConstants::START_MENU;
    return;
  }

  // If the state has response on, click the coordinate.
  if (true == InterfacePxXY[state].Response)
  {
    bController->LeftClick(InterfacePxXY[state]);
    return;
  }

  // If the submenu is open, close it
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
  if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
  {
    bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
  }

  // We need to find the specific state and react accordingly.
  std::vector<PxXY> responses;
  switch (state)
  {
  case MSMConstants::CHAOS_DAILY_DUNGEON_DIALOG: // In case chaos is shown
    responses.push_back(InterfaceActionPxXY[func->params[0]]);
    break;
  case MSMConstants::DAILY_DUNGEON_DIALOG:
    responses.push_back(InterfaceActionPxXY[func->params[0]]);
    responses.push_back(InterfaceActionPxXY[MSMConstants::ENTER_DAILY_DUNGEON]);
    break;
  case MSMConstants::DAILY_DUNGEON_TICKET:
    // Remove all tickets
    for (unsigned int i = 0; i < 5; ++i)
    {
      responses.push_back(InterfaceActionPxXY[MSMConstants::REMOVE_DAILY_DUNGEON_TICKET]);
    }

    // Add the specified amount of tickets
    for (unsigned int i = 0; i < func->params[1]; ++i)
    {
      responses.push_back(InterfacePxXY[MSMConstants::DAILY_DUNGEON_TICKET]);
    }

    // Confirm
    responses.push_back(InterfaceActionPxXY[MSMConstants::CONFIRM_DAILY_DUNGEON]);
    break;
  case MSMConstants::DAILY_DUNGEON_IN_PROGRESS:
    bController->Delay(1000);
    break;
  }

  for (PxXY response : responses)
  {
    bController->LeftClick(response);
    bController->Delay(1000);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessChaosDailyDungeon
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessChaosDailyDungeon(FunctionSet* func, 
                                          MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // Check to see if we are logged out, update current state if so.
  if (true == IsLoggedOut())
  {
    CurrentState = MSMManager::LOGGING_IN;
    state = MSMConstants::START_MENU;
    return;
  }

  // If the state has response on, click the coordinate.
  if (true == InterfacePxXY[state].Response)
  {
    bController->LeftClick(InterfacePxXY[state]);
    return;
  }

  // If the submenu is open, close it
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
  if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
  {
    bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
  }

  // We need to find the specific state and react accordingly.
  std::vector<PxXY> responses;
  switch (state)
  {
  case MSMConstants::DAILY_DUNGEON_DIALOG: // In case chaos is show
    responses.push_back(InterfaceActionPxXY[MSMConstants::DAILY_DUNGEON_CHAOS]);
    break;
  case MSMConstants::CHAOS_DAILY_DUNGEON_DIALOG:
    responses.push_back(InterfaceActionPxXY[MSMConstants::ENTER_DAILY_DUNGEON]);
    break;
  case MSMConstants::CHAOS_DAILY_DUNGEON_TICKET:
    // Remove all tickets
    for (unsigned int i = 0; i < 5; ++i)
    {
      responses.push_back(InterfaceActionPxXY
                          [MSMConstants::REMOVE_CHAOS_DAILY_DUNGEON_TICKET]);
    }

    // Add the specified amount of tickets
    for (unsigned int i = 0; i < func->params[0]; ++i)
    {
      responses.push_back(InterfacePxXY
                          [MSMConstants::CHAOS_DAILY_DUNGEON_TICKET]);
    }

    responses.push_back(InterfaceActionPxXY
                        [MSMConstants::CONFIRM_CHAOS_DAILY_DUNGEON]);
    break;
  case MSMConstants::CHAOS_DAILY_DUNGEON_IN_PROGRESS:
    bController->Delay(1000);
    break;
  }

  for (PxXY response : responses)
  {
    bController->LeftClick(response);
    bController->Delay(1000);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessMuLung
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessMuLung(FunctionSet* func,
                               MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // Check to see if we are logged out, update current state if so.
  if (true == IsLoggedOut())
  {
    CurrentState = MSMManager::LOGGING_IN;
    state = MSMConstants::START_MENU;
    return;
  }

  // If the state has response on, click the coordinate.
  if (true == InterfacePxXY[state].Response)
  {
    bController->LeftClick(InterfacePxXY[state]);
    return;
  }

  // If the submenu is open, close it
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::SUBMENU_OPEN]);
  if (pixels == InterfacePxXY[MSMConstants::SUBMENU_OPEN])
  {
    bController->LeftClick(InterfaceActionPxXY[MSMConstants::SCREEN_CENTER]);
  }

  // We need to find the specific state and react accordingly.
  std::vector<PxXY> responses;
  switch (state)
  {
  case MSMConstants::MULUNG_DIALOG:
    responses.push_back(InterfaceActionPxXY[MSMConstants::ENTER_MU_LUNG]);
    break;
  case MSMConstants::MULUNG_IN_PROGESS:
    bController->Delay(10000);
    break;
  }

  for (PxXY response : responses)
  {
    bController->LeftClick(response);
    bController->Delay(1000);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessQuest
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessQuest(FunctionSet* func, MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // If the state has response on, click the coordinate.
  if (true == InterfacePxXY[state].Response)
  {
    bController->LeftClick(InterfacePxXY[state]);
    return;
  }

  // Check to see if we are logged out, update current state if so.
  if (true == IsLoggedOut())
  {
    CurrentState = MSMManager::LOGGING_IN;
    state = MSMConstants::START_MENU;
    return;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessManualActions
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessManualActions(FunctionSet* func, 
                                      MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  switch (func->function)
  {
  case FunctionIdEnum::MOVE:
    bController->LeftDown(ActionPxXY[func->params[0]]);
    
    if (1 < func->params.size())
    {
      bController->Delay(func->params[1]);
    }
      
    bController->LeftUp(ActionPxXY[func->params[0]]);
    break;
  case FunctionIdEnum::SKILL:
    bController->LeftClick(ActionPxXY[MSMConstants::ACTION_SKILL0 + func->params[0]]);
    break;
  case FunctionIdEnum::QUICKSLOT:
    bController->LeftClick(ActionPxXY[MSMConstants::ACTION_QUICK0 + func->params[0]]);
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessSnipeAll
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessSnipeAll(FunctionSet* func, MSMConstants::InterfaceEnum& state)
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->SendStatus(state, func->syntax);

  // First check to see if an item is available!
  PxXY pixels = bController->GetPX(InterfacePxXY[MSMConstants::SELECT_TS_ITEM]);
  if (pixels == InterfacePxXY[MSMConstants::SELECT_TS_ITEM])
  {
    // Navigate until the item dialog pops up
    bController->LClickUntilChange
      (&InterfacePxXY[MSMConstants::SELECT_TS_ITEM],
       &InterfacePxXY[MSMConstants::SELECT_TS_ITEM],
       1000);

    // If the Buy button is visible, click it
    bController->LClickUntilChange
      (&InterfacePxXY[MSMConstants::SELECT_BUY],
       &InterfacePxXY[MSMConstants::SELECT_BUY],
       1000);
  }
  else
  {
    // No item is available, depending on the state, navigate to the next page.
    switch (state)
    {
    case MSMConstants::SELECT_CATEGORY_0:
    case MSMConstants::SELECT_CATEGORY_1:
    case MSMConstants::SELECT_CATEGORY_2:
    case MSMConstants::SELECT_CATEGORY_3:
    case MSMConstants::SELECT_CATEGORY_4:
    case MSMConstants::SELECT_CATEGORY_5:
      if (func->lastState == state)
      {
        // If the state hasn't changed, close the dialog..
        bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
      }
      else
      {
        // Move to next state
        bController->LeftClick(InterfacePxXY[state + 1]);
      }
      break;
    case MSMConstants::SELECT_CATEGORY_6:
      // If we reach the end of armor, close the trade station
      pixels = bController->GetPX(InterfacePxXY[MSMConstants::SELECT_ARMOR]);
      if (pixels == InterfacePxXY[MSMConstants::SELECT_ARMOR])
      {
        bController->LeftClick(InterfacePxXY[MSMConstants::DIALOG_OPEN]);
      }
      else
      {
        bController->LeftClick(InterfacePxXY[state + 1]);
      }
      break;
    case MSMConstants::SELECT_CATEGORY_7:
      // Attempt to scroll the weapon section. If the pixel did not change,
      // switch to weapon, otherwise select the first section.
      bController->Drag(InterfacePxXY[state], 
                        InterfaceActionPxXY[MSMConstants::SCROLL_TRADE_STATION]);
      bController->Delay(2000);

      pixels = bController->GetPX(InterfacePxXY[state]);
      if (pixels == InterfacePxXY[state])
      {
        bController->LeftClick(InterfacePxXY[MSMConstants::SELECT_ARMOR]);
      }
      break;
    default:
      bController->LeftClick(InterfacePxXY[MSMConstants::SELECT_CATEGORY_0]);
    }

    // Update last state to check for error
    func->lastState = state;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: IsLoggedInBusy
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::IsLoggedInBusy() const
{
  PxXY pixels = reinterpret_cast<BotController*>(Controller)->GetPX
                (ValidatePxXY[MSMConstants::BUSY]);
  if (pixels == ValidatePxXY[MSMConstants::BUSY])
  {
    return false;
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: IsLoggedOut
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::IsLoggedOut() const
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  for (unsigned int i = MSMConstants::START_MENU_AD;
       i < MSMConstants::START_MENU;
       ++i)
  {
    PxXY pixels = bController->GetPX(InterfacePxXY[i]);
    if (pixels == InterfacePxXY[i])
    {
      if (static_cast<MSMConstants::InterfaceEnum>(i) == MSMConstants::ATTENDANCE_DIALOG)
      {
        pixels = bController->GetPX(ValidatePxXY[MSMConstants::ATTENDANCE_CHECK]);
        if (pixels == ValidatePxXY[MSMConstants::ATTENDANCE_CHECK])
        {
          return true;
        }
      }
      else
      {
        // We think we are logged out, verify this is truly the case.
        pixels = bController->GetPX(ValidatePxXY[MSMConstants::MENU_CHECK]);
        if ((pixels != ValidatePxXY[MSMConstants::MENU_CHECK]) &&
            (pixels != PxXY(0, 0, 0, true))) // Loading screen check
        {
          pixels = bController->GetPX(InterfacePxXY[MSMConstants::DUNGEON_DIALOG]);
          if (pixels != InterfacePxXY[MSMConstants::DUNGEON_DIALOG])
          {
            return true;
          }
        }
      }
    }
  }

  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessAds
//
////////////////////////////////////////////////////////////////////////////////////////
bool MSMManager::ProcessAds(MSMConstants::InterfaceEnum& state) const
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);

  bool detected = false;
  for (unsigned int i = MSMConstants::INGAME_AD; 
       i <= MSMConstants::ELITE_BATTLE_AD1; 
       ++i)
  {
    PxXY pixels = bController->GetPX(InterfacePxXY[i]);
    if (pixels == InterfacePxXY[i])
    {
      state = static_cast<MSMConstants::InterfaceEnum>(i);
      bController->LeftClick(InterfacePxXY[i]);
      detected = true;
      break;
    }
  }

  return detected;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: ProcessCaptcha
//
////////////////////////////////////////////////////////////////////////////////////////
void MSMManager::ProcessCaptcha() const
{
  BotController* bController = reinterpret_cast<BotController*>(Controller);
  bController->RefreshPixels();

  // Look for all Captcha images
  PxXY pixels;
  bool captchaFound = true;
  for (unsigned int i = MSMConstants::CAPTCHA_M; i <= MSMConstants::CAPTCHA_E; ++i)
  {
    pixels = bController->GetPX(ValidatePxXY[i], false);
    if (pixels != ValidatePxXY[i])
    {
      captchaFound = false;
      break;
    }
  }

  if (true == captchaFound)
  {
    bController->SendStatus("[Warning] Captcha detected");

    // Select each captch symbol
    for (unsigned int i = MSMConstants::CAPTCHA_M_UPPER; 
         i <= MSMConstants::CAPTCHA_E_LOWER; 
         ++i)
    {
      pixels = bController->GetPX(InterfacePxXY[i], false);
      if (pixels == InterfacePxXY[i])
      {
        bController->LeftClick(InterfacePxXY[i]);
        bController->Delay(1000);
      }
    }

    // Submit
    if (true == bController->LClickUntilChange(
                  &InterfacePxXY[MSMConstants::CAPTCHA_SUBMIT],
                  &InterfacePxXY[MSMConstants::CAPTCHA_SUBMIT],
                  2000))
    {
      bController->SendStatus("[Notice] Captcha submitted");
    } 
  }
}