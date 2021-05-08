#include "libretro.h"

#include "gtest/gtest.h"

TEST (RetroInit, InitializesChip8InDefaultState) {
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    int pc = my_machine.get_pc();
    EXPECT_EQ( pc, 0x200 );
}

TEST (RetroInit, ReinitializesIfAlreadyInitialized) {
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    my_machine.set_pc(0x201);
    chip8machine_init(my_machine);
    int pc = my_machine.get_pc();
    EXPECT_EQ( pc, 0x200 );
}

TEST (RetroDeinit, ResetsChip8ToDefaultState) {
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    my_machine.set_pc(0x201);
    chip8machine_deinit(my_machine);
    int pc = my_machine.get_pc();
    EXPECT_EQ( pc, 0x200 );
}

TEST (RetroApiVersion, ReturnsPublicAPIVersion) {
    unsigned version;
    version = retro_api_version();
    EXPECT_EQ (version, RETRO_API_VERSION);
}

TEST (RetroGetSystemInfo, SetsProperVariables) {
    retro_system_info* info = (retro_system_info*) malloc(sizeof(retro_system_info));
    retro_get_system_info(info);
    EXPECT_STREQ(info->library_name, "chip8-libretro");
    EXPECT_STREQ(info->library_version, "0.0.1");
    EXPECT_STREQ(info->valid_extensions, "");
    EXPECT_EQ(info->need_fullpath,  false);
    EXPECT_EQ(info->block_extract, false);
}

TEST (RetroGetSystemAvInfo, SetsProperVariables) {
    retro_system_av_info* info = (retro_system_av_info*) malloc(sizeof(retro_system_av_info));

    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    chip8machine_get_system_av_info(info, my_machine);

    int height = my_machine.display_height;
    int width = my_machine.display_width;

    EXPECT_FLOAT_EQ(info->geometry.aspect_ratio, 1.0);
    EXPECT_EQ(info->geometry.base_height, height);
    EXPECT_EQ(info->geometry.base_width, width);
    EXPECT_EQ(info->geometry.max_height, height);
    EXPECT_EQ(info->geometry.max_width, width);
    EXPECT_FLOAT_EQ(info->timing.fps, 60.0);
    EXPECT_FLOAT_EQ(info->timing.sample_rate, 44100.0);
}

// TODO:  Ignoring environment tests for now
TEST (RetroSetEnvironment, Exists) {
   retro_environment_t environment;
   retro_set_environment(environment);
}

// TODO:  Ignoring video tests for now
TEST (RetroSetVideoRefresh, Exists) {
    retro_video_refresh_t videoRefresh;
    retro_set_video_refresh(videoRefresh);
}

// TODO:  Ignoring audio tests for now
TEST (RetroSetAudioSample, Exists) {
    retro_audio_sample_t audioSample;
    retro_set_audio_sample(audioSample);
}

// TODO:  Ignoring audio tests for now
TEST (RetroSetAudioSampleBatch, Exists) {
    retro_audio_sample_batch_t audioSampleBatch;
    retro_set_audio_sample_batch(audioSampleBatch);
}

// TODO:  Ignoring controller tests for now
TEST (RetroSetInputPoll, Exists) {
    retro_input_poll_t inputPoll;
    retro_set_input_poll(inputPoll);
}

// TODO:  Ignoring controller tests for now
TEST (RetroSetInputState, Exists) {
    retro_input_state_t inputState;
    retro_set_input_state(inputState);
}

// TODO:  Ignoring controller tests for now
TEST (RetroSetControllerPortDevice, Exists) {
    unsigned port, device;
    retro_set_controller_port_device(port, device);
}

TEST (RetroReset, ResetsStateOfEmulator) {
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    my_machine.set_pc(0x201);
    chip8machine_reset(my_machine);
    int pc = my_machine.get_pc();
    EXPECT_EQ( pc, 0x200 );
}

// TODO:  Figure out how to test...
//TEST (RetroRun, Exists) {
//    retro_run();
//}

// TODO:  Ignoring serialization tests for now
TEST (RetroSerializeSize, ReturnsOne) {
    size_t actual, expected = 1;
    actual = retro_serialize_size();
    EXPECT_EQ (expected, actual);
}

// TODO:  Ignoring serialization tests for now
TEST (RetroSerialize, ReturnsFalseWhenSizeLessThanOne) {
    void* data;
    size_t size = 0;
    bool actual, expected = false;
    actual = retro_serialize(data, size);
    EXPECT_EQ (expected, actual);
}

// TODO:  Ignoring serialization tests for now
TEST (RetroSerialize, ReturnsTrueWhenSizeIsOne) {
    void* data;
    size_t size = 1;
    bool actual, expected = true;
    actual = retro_serialize(data, size);
    EXPECT_EQ (expected, actual);
}

// TODO:  Ignoring serialization tests for now
TEST (RetroUnserialize, ReturnsFalseWhenSizeLessThanOne) {
    void* data;
    size_t size = 0;
    bool actual, expected = false;
    actual = retro_unserialize(data, size);
    EXPECT_EQ (expected, actual);
}

// TODO:  Ignoring serialization tests for now
TEST (RetroUnserialize, ReturnsTrueWhenSizeIsOne) {
    void* data;
    size_t size = 1;
    bool actual, expected = true;
    actual = retro_unserialize(data, size);
    EXPECT_EQ (expected, actual);
}

// TODO:  Ignoring cheat tests for now
TEST (RetroCheatReset, Exists) {
    retro_cheat_reset();
}

// TODO:  Ignoring cheat tests for now
TEST (RetroCheatSet, Exists) {
    unsigned index;
    bool enabled;
    char* code;
    retro_cheat_set(index, enabled, code);
}

TEST (RetroLoadGame, ReturnsTrueWhenInfoValidAndInitCalled) {
    retro_game_info *game = new retro_game_info;
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    game->size = 4;
    game->data = (void *) new unsigned char[4] {0xDE, 0xAD, 0xBE, 0xEF};
    bool result = chip8machine_load_game(game, my_machine);
    EXPECT_EQ (result, true);
}

TEST (RetroLoadGame, LoadsROMIntoMemoryWhenCallSuccessful) {
    retro_game_info *game = new retro_game_info;
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    game->size = 4;
    game->data = (void *) new unsigned char[4] {0xDE, 0xAD, 0xBE, 0xEF};
    chip8machine_load_game(game, my_machine);
    // Only grab the active portion of RAM
    std::vector<unsigned char> ram = my_machine.get_ram(false);
    for (int i = 0; i < game->size; i++) {
        EXPECT_EQ (((unsigned char *)game->data)[i], ram[i]);
    }
}

TEST (RetroLoadGame, ReturnsFalseWhenSizeZero) {
    retro_game_info *game = new retro_game_info;
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    game->size = 0;
    game->data = (void *) new unsigned char[4] {0xDE, 0xAD, 0xBE, 0xEF};
    bool result = chip8machine_load_game(game, my_machine);
    EXPECT_EQ (result, false);
}

TEST (RetroLoadGame, ReturnsFalseWhenDataNull) {
    retro_game_info *game = new retro_game_info;
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    game->size = 1;
    game->data = nullptr;
    bool result = chip8machine_load_game(game, my_machine);
    EXPECT_EQ (result, false);
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

// TODO:  Ignoring region tests for now
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

TEST (RetoGetMemorySize, ReturnsCorrectSize) {
    unsigned dummy;
    Chip8Machine my_machine;
    chip8machine_init(my_machine);
    size_t actual, expected = 0x1000;
    actual = chip8machine_get_memory_size(dummy, my_machine);
    EXPECT_EQ (expected, actual);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
