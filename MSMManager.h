#pragma once
#include "FunctionDefines.h"
#include "MSMConstants.h"
#include <mutex>
#include <string>

class MSMManager
{
public:
  enum MSMState
  {
    INITIAL,
    LOGGING_IN,
    LOGGED_IN,
    LOGGING_OUT,
    CHANGING_CHANNEL,
    GUILD,
    ACHIEVEMENTS,
    MAIL,
    ELITE_DUNGEON,
    MU_LUNG,
    DAILY_DUNGEON,
    CHAOS_DAILY_DUNGEON,
    QUEST,
    SNIPE_ALL,
    MAX_MSM_STATE
  };

  static std::vector<std::string> SplitSyntax(std::string text, const char* delim);
  static void StringReplace(std::string& text, const char* delim, const char* ch);
  static std::string BuildScript(std::vector<std::string> syntax, 
                                 std::vector<FunctionSet>& script);
  static bool ReadScriptFile(std::string path, std::vector<std::string>& syntax);
  void CalibrateLogin();
  void CalibrateQuest();
  void GeneratePixelData(unsigned int w, unsigned int h);
  bool CorrectPixelData(std::string path);
  static unsigned int MapParameter(std::string token);
  bool ProcessMSMFunction(FunctionSet* function, MSMConstants::InterfaceEnum& state);
  void ResetCurrentState();
  MSMState GetCurrentState() const;
  const char* GetCurrentStateString() const;
  MSMManager(void* controller, unsigned int w, unsigned int h);
  ~MSMManager();

private:
  const std::string MSMStateStrings[MAX_MSM_STATE] =
  {
    "Initial State",
    "Logging In",
    "Logged In",
    "Logging Out",
    "Changing Channel",
    "Guild Attendance",
    "Achievements",
    "Mail",
    "Elite Dungeon",
    "Mu Lung",
    "Daily Dungeon",
    "Chaos Daily Dungeon",
    "Quest",
    "Snipe"
  };

  // MSMManager State processing
  bool ProcessInitialState(FunctionSet*& func, 
                           MSMConstants::InterfaceEnum& state);
  bool ProcessLoggingInState(FunctionSet*& func, 
                             MSMConstants::InterfaceEnum& state);
  bool ProcessLoggedInState(FunctionSet*& func, 
                            MSMConstants::InterfaceEnum& state);
  bool ProcessLoggingOutState(FunctionSet*& func,
                              MSMConstants::InterfaceEnum& state);
  bool ProcessChangingChannelState(FunctionSet*& func, 
                                   MSMConstants::InterfaceEnum& state);
  bool ProcessGuildState(FunctionSet*& func,
                         MSMConstants::InterfaceEnum& state);
  bool ProcessAchievementsState(FunctionSet*& func,
                                MSMConstants::InterfaceEnum& state);
  bool ProcessMailState(FunctionSet*& func,
                        MSMConstants::InterfaceEnum& state);
  bool ProcessEliteDungeonState(FunctionSet*& func,
                                MSMConstants::InterfaceEnum& state);
  bool ProcessDailyDungeonState(FunctionSet*& func,
                                MSMConstants::InterfaceEnum& state);
  bool ProcessChaosDailyDungeonState(FunctionSet*& func,
                                     MSMConstants::InterfaceEnum& state);
  bool ProcessMuLungState(FunctionSet*& func,
                          MSMConstants::InterfaceEnum& state);
  bool ProcessQuestState(FunctionSet*& func,
                         MSMConstants::InterfaceEnum& state);
  bool ProcessSnipeAllState(FunctionSet*& func,
                            MSMConstants::InterfaceEnum& state);

  // SubState processing
  void ProcessLogin(MSMConstants::InterfaceEnum& state);
  void ProcessEliteDungeon(FunctionSet* func,  MSMConstants::InterfaceEnum& state);
  void ProcessDailyDungeon(FunctionSet* func, MSMConstants::InterfaceEnum& state);
  void ProcessChaosDailyDungeon(FunctionSet* func, MSMConstants::InterfaceEnum& state);
  void ProcessMuLung(FunctionSet* func, MSMConstants::InterfaceEnum& state);
  void ProcessQuest(FunctionSet* func, MSMConstants::InterfaceEnum& state);
  void ProcessManualActions(FunctionSet* func, MSMConstants::InterfaceEnum& state);
  void ProcessSnipeAll(FunctionSet* func, MSMConstants::InterfaceEnum& state);
  bool IsLoggedInBusy() const;
  bool IsLoggedOut() const;
  bool ProcessAds(MSMConstants::InterfaceEnum& state) const;
  void ProcessCaptcha() const;

  // Members
  void* Controller;
  MSMState CurrentState;
  PxXY* ActionPxXY;
  PxXY* InterfacePxXY;
  PxXY* InterfaceActionPxXY;
  PxXY* ValidatePxXY;
  static std::mutex Lock;
  unsigned int CurrentCharSlot;
};

