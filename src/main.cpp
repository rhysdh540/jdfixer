#include "main.hpp"
#include "ModConfig.hpp"

#include "GlobalNamespace/BeatmapObjectSpawnMovementData.hpp"

using namespace GlobalNamespace;

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
// Stores the ID and version of our mod, and is sent to
// the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
// other config tools such as config-utils don't use this config, so it can be
// removed if those are in use
Configuration &getConfig() {
  static Configuration config(modInfo);
  return config;
}

// Called at the early stages of game loading
MOD_EXTERN_FUNC void setup(CModInfo *info) noexcept {
  *info = modInfo.to_c();

  getModConfig().Init(modInfo);

  // File logging
  Paper::Logger::RegisterFileContextId(PaperLogger.tag);

  PaperLogger.info("Completed setup!");
}

MAKE_HOOK_MATCH(BeatmapObjectSpawnMovementData_Init, &BeatmapObjectSpawnMovementData::Init, void,
                BeatmapObjectSpawnMovementData *self, int32_t noteLinesCount, float_t startNoteJumpMovementSpeed, float_t startBpm,
                __BeatmapObjectSpawnMovementData__NoteJumpValueType noteJumpValueType, float_t noteJumpValue,
                IJumpOffsetYProvider* jumpOffsetYProvider, UnityEngine::Vector3 rightVec, UnityEngine::Vector3 forwardVec) {
    noteJumpValueType = BeatmapObjectSpawnMovementData::NoteJumpValueType::JumpDuration;
    noteJumpValue = getModConfig().JumpDuration.GetValue();
    BeatmapObjectSpawnMovementData_Init(self, noteLinesCount, startNoteJumpMovementSpeed, startBpm, noteJumpValueType, noteJumpValue, jumpOffsetYProvider, rightVec, forwardVec);
}

// Called later on in the game loading - a good time to install function hooks
MOD_EXTERN_FUNC void late_load() noexcept {
  il2cpp_functions::Init();

  PaperLogger.info("Installing hooks...");
  INSTALL_HOOK(PaperLogger, BeatmapObjectSpawnMovementData_Init);
  PaperLogger.info("Installed all hooks!");
}