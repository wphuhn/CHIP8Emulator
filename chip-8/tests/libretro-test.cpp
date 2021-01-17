#include "libretro.h"

#include "gtest/gtest.h"

TEST (RetroInit, Exists) {
    retro_init();
}

TEST (RetroDeinit, Exists) {
    retro_deinit();
}

TEST (RetroApiVersion, ReturnsPublicAPIVersion) {
    unsigned version;
    version = retro_api_version();
    EXPECT_EQ (version, RETRO_API_VERSION);
}

TEST (RetroGetSystemInfo, SetsProperVariables) {
    retro_system_info* info = (retro_system_info*) malloc(sizeof(retro_system_info));
    retro_get_system_info(info);
    EXPECT_STREQ(info->library_name, "lib-retro");
    EXPECT_STREQ(info->library_version, "1.0.0");
    EXPECT_STREQ(info->valid_extensions, "");
    EXPECT_EQ(info->need_fullpath,  false);
    EXPECT_EQ(info->block_extract, false);
}

TEST (RetroGetSystemAvInfo, SetsProperVariables) {
    retro_system_av_info* info = (retro_system_av_info*) malloc(sizeof(retro_system_av_info));
    retro_get_system_av_info(info);
    EXPECT_FLOAT_EQ(info->geometry.aspect_ratio, 1.0);
    EXPECT_EQ(info->geometry.base_height, 400);
    EXPECT_EQ(info->geometry.base_width, 400);
    EXPECT_EQ(info->geometry.max_height, 400);
    EXPECT_EQ(info->geometry.max_width, 400);
    EXPECT_FLOAT_EQ(info->timing.fps, 60.0);
    EXPECT_FLOAT_EQ(info->timing.sample_rate, 44100.0);
}

TEST (RetroSetEnvironment, Exists) {
   retro_environment_t environment;
   retro_set_environment(environment);
}

TEST (RetroSetVideoRefresh, Exists) {
    retro_video_refresh_t videoRefresh;
    retro_set_video_refresh(videoRefresh);
}

TEST (RetroSetAudioSample, Exists) {
    retro_audio_sample_t audioSample;
    retro_set_audio_sample(audioSample);
}

TEST (RetroSetAudioSampleBatch, Exists) {
    retro_audio_sample_batch_t audioSampleBatch;
    retro_set_audio_sample_batch(audioSampleBatch);
}

TEST (RetroSetInputPoll, Exists) {
    retro_input_poll_t inputPoll;
    retro_set_input_poll(inputPoll);
}

TEST (RetroSetInputState, Exists) {
    retro_input_state_t inputState;
    retro_set_input_state(inputState);
}

TEST (RetroSetControllerPortDevice, Exists) {
    unsigned port, device;
    retro_set_controller_port_device(port, device);
}

TEST (RetroReset, Exists) {
    retro_reset();
}

// TODO:  Figure out how to test...
/*TEST (RetroRun, Exists) {
    retro_run();
}*/

TEST (RetroSerializeSize, ReturnsOne) {
    size_t actual, expected = 1;
    actual = retro_serialize_size();
    EXPECT_EQ (expected, actual);
}

TEST (RetroSerialize, ReturnsFalseWhenSizeLessThanOne) {
    void* data;
    size_t size = 0;
    bool actual, expected = false;
    actual = retro_serialize(data, size);
    EXPECT_EQ (expected, actual);
}

TEST (RetroSerialize, ReturnsTrueWhenSizeIsOne) {
    void* data;
    size_t size = 1;
    bool actual, expected = true;
    actual = retro_serialize(data, size);
    EXPECT_EQ (expected, actual);
}

TEST (RetroUnserialize, ReturnsFalseWhenSizeLessThanOne) {
    void* data;
    size_t size = 0;
    bool actual, expected = false;
    actual = retro_unserialize(data, size);
    EXPECT_EQ (expected, actual);
}

TEST (RetroUnserialize, ReturnsTrueWhenSizeIsOne) {
    void* data;
    size_t size = 1;
    bool actual, expected = true;
    actual = retro_unserialize(data, size);
    EXPECT_EQ (expected, actual);
}

TEST (RetroCheatReset, Exists) {
    retro_cheat_reset();
}

TEST (RetroCheatSet, Exists) {
    unsigned index;
    bool enabled;
    char* code;
    retro_cheat_set(index, enabled, code);
}

TEST (RetroLoadGame, ReturnsTrue) {
    retro_game_info *game;
    bool actual, expected = true;
    actual = retro_load_game(game);
    EXPECT_EQ (expected, actual);
}

TEST (RetroLoadGameSpecial, ReturnTrue) {
    unsigned game_type;
    retro_game_info *info;
    size_t num_info;
    bool actual, expected = true;
    actual = retro_load_game_special(game_type, info, num_info);
    EXPECT_EQ (expected, actual);
}

TEST (RetroUnloadGame, Exists) {
    retro_unload_game();
}

TEST (RetroGetRegion, ReturnsZero) {
    unsigned actual, expected = 0;
    actual = retro_get_region();
    EXPECT_EQ (expected, actual);
}

TEST (RetroGetMemoryData, Exists) {
    unsigned id;
    void* data;
    data = retro_get_memory_data(id);
}

TEST (RetoGetMemorySize, ReturnsZero) {
    unsigned id;
    size_t actual, expected = 0;
    actual = retro_get_memory_size(id);
    EXPECT_EQ (expected, actual);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

