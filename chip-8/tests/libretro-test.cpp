#include "../include/libretro.h"

#include "gtest/gtest.h"

#include "chip8machinetester.hpp"
#include "test-constants.hpp"

class RetroFixture : public ::testing::Test {
 protected:
    RetroFixture() {
        tester.set_machine(&my_machine);
        chip8machine_init(my_machine);
    }

    Chip8Machine my_machine;
    Chip8MachineTester tester;
};

TEST_F(RetroFixture, RetroInitInitializesChip8InDefaultState) {
    ADDR_TYPE pc = tester.get_pc();
    EXPECT_EQ(pc, TEST_ROM_START_ADDRESS);
}

TEST_F(RetroFixture, RetroInitReinitializesIfAlreadyInitialized) {
    ADDR_TYPE start_pc = 0x201;
    assert(start_pc != TEST_ROM_START_ADDRESS);
    tester.set_pc(start_pc);
    chip8machine_init(my_machine);
    ADDR_TYPE pc = tester.get_pc();
    EXPECT_EQ(pc, TEST_ROM_START_ADDRESS);
}

TEST_F(RetroFixture, RetroDeinitResetsChip8ToDefaultState) {
    ADDR_TYPE start_pc = 0x201;
    assert(start_pc != TEST_ROM_START_ADDRESS);
    tester.set_pc(start_pc);
    chip8machine_deinit(my_machine);
    ADDR_TYPE pc = tester.get_pc();
    EXPECT_EQ(pc, TEST_ROM_START_ADDRESS);
}

TEST(RetroApiVersion, ReturnsPublicAPIVersion) {
    unsigned version;
    version = retro_api_version();
    EXPECT_EQ(version, RETRO_API_VERSION);
}

TEST(RetroGetSystemInfo, SetsProperVariables) {
    auto info = (retro_system_info*) malloc(sizeof(retro_system_info));
    retro_get_system_info(info);
    EXPECT_STREQ(info->library_name, "chip8-libretro");
    EXPECT_STREQ(info->library_version, "0.0.1");
    EXPECT_STREQ(info->valid_extensions, "");
    EXPECT_EQ(info->need_fullpath,  false);
    EXPECT_EQ(info->block_extract, false);
}

TEST_F(RetroFixture, RetroGetSystemAvInfoSetsProperVariables) {
    auto info = (retro_system_av_info*) malloc(sizeof(retro_system_av_info));

    chip8machine_get_system_av_info(info, my_machine);

    int height = my_machine.display_height;
    int width = my_machine.display_width;
    int x_scale = Upscaler::x_scale;
    int y_scale = Upscaler::y_scale;

    EXPECT_FLOAT_EQ(info->geometry.aspect_ratio, -1.0);
    EXPECT_EQ(info->geometry.base_height, y_scale * height);
    EXPECT_EQ(info->geometry.base_width, x_scale * width);
    EXPECT_EQ(info->geometry.max_height, y_scale * height);
    EXPECT_EQ(info->geometry.max_width, x_scale * width);
    EXPECT_FLOAT_EQ(info->timing.fps, 60.0);
    EXPECT_FLOAT_EQ(info->timing.sample_rate, 44100.0);
}

// We don't need this subroutine for anything, but the RetroArch API
// requires it, so test only that it exists and can be called
TEST(RetroSetEnvironment, ExistsAndDoesntCrash) {
    retro_environment_t environment;
    retro_set_environment(environment);
}

// This is a boilerplate subroutine that sets the callback, we don't
// need any special modification for it, so test only that it exists
// and can be called
TEST(RetroSetVideoRefresh, ExistsAndDoesntCrash) {
    retro_video_refresh_t videoRefresh;
    retro_set_video_refresh(videoRefresh);
}

// TODO(WPH):  Ignoring audio tests for now
TEST(RetroSetAudioSample, Exists) {
    retro_audio_sample_t audioSample;
    retro_set_audio_sample(audioSample);
}

// TODO(WPH):  Ignoring audio tests for now
TEST(RetroSetAudioSampleBatch, Exists) {
    retro_audio_sample_batch_t audioSampleBatch;
    retro_set_audio_sample_batch(audioSampleBatch);
}

// TODO(WPH):  Ignoring controller tests for now
TEST(RetroSetInputPoll, Exists) {
    retro_input_poll_t inputPoll;
    retro_set_input_poll(inputPoll);
}

// TODO(WPH):  Ignoring controller tests for now
TEST(RetroSetInputState, Exists) {
    retro_input_state_t inputState;
    retro_set_input_state(inputState);
}

// TODO(WPH):  Ignoring controller tests for now
TEST(RetroSetControllerPortDevice, Exists) {
    unsigned port, device;
    retro_set_controller_port_device(port, device);
}

TEST_F(RetroFixture, RetroResetResetsStateOfEmulator) {
    ADDR_TYPE start_pc = 0x201;
    assert(start_pc != TEST_ROM_START_ADDRESS);
    tester.set_pc(start_pc);
    chip8machine_reset(my_machine);
    ADDR_TYPE pc = tester.get_pc();
    EXPECT_EQ(pc, TEST_ROM_START_ADDRESS);
}

// TODO(WPH):  Ignoring serialization tests for now
TEST(RetroSerializeSize, ReturnsOne) {
    size_t actual, expected = 1;
    actual = retro_serialize_size();
    EXPECT_EQ(expected, actual);
}

// TODO(WPH):  Ignoring serialization tests for now
TEST(RetroSerialize, ReturnsFalseWhenSizeLessThanOne) {
    void* data;
    size_t size = 0;
    bool actual, expected = false;
    actual = retro_serialize(data, size);
    EXPECT_EQ(expected, actual);
}

// TODO(WPH):  Ignoring serialization tests for now
TEST(RetroSerialize, ReturnsTrueWhenSizeIsOne) {
    void* data;
    size_t size = 1;
    bool actual, expected = true;
    actual = retro_serialize(data, size);
    EXPECT_EQ(expected, actual);
}

// TODO(WPH):  Ignoring serialization tests for now
TEST(RetroUnserialize, ReturnsFalseWhenSizeLessThanOne) {
    void* data;
    size_t size = 0;
    bool actual, expected = false;
    actual = retro_unserialize(data, size);
    EXPECT_EQ(expected, actual);
}

// TODO(WPH):  Ignoring serialization tests for now
TEST(RetroUnserialize, ReturnsTrueWhenSizeIsOne) {
    void* data;
    size_t size = 1;
    bool actual, expected = true;
    actual = retro_unserialize(data, size);
    EXPECT_EQ(expected, actual);
}

// TODO(WPH):  Ignoring cheat tests for now
TEST(RetroCheatReset, Exists) {
    retro_cheat_reset();
}

// TODO(WPH):  Ignoring cheat tests for now
TEST(RetroCheatSet, Exists) {
    unsigned index;
    bool enabled;
    char* code;
    retro_cheat_set(index, enabled, code);
}

TEST_F(RetroFixture, RetroLoadGameReturnsTrueWhenInfoValidAndInitCalled) {
    auto game = new retro_game_info;
    game->size = 4;
    game->data = (void *) new unsigned char[4] {0xDE, 0xAD, 0xBE, 0xEF};
    bool result = chip8machine_load_game(game, my_machine);
    EXPECT_EQ(result, true);
}

TEST_F(RetroFixture, RetroLoadGameLoadsROMIntoMemoryWhenCallSuccessful) {
    auto game = new retro_game_info;
    game->size = 4;
    game->data = (void *) new unsigned char[4] {0xDE, 0xAD, 0xBE, 0xEF};
    chip8machine_load_game(game, my_machine);
    // Only grab the active portion of RAM
    std::vector<unsigned char> ram = tester.get_ram(false);
    for (int i = 0; i < game->size; i++) {
        EXPECT_EQ(((unsigned char *)game->data)[i], ram[i]);
    }
}

TEST_F(RetroFixture, RetroLoadGameReturnsFalseWhenSizeZero) {
    auto game = new retro_game_info;
    game->size = 0;
    game->data = (void *) new unsigned char[4] {0xDE, 0xAD, 0xBE, 0xEF};
    bool result = chip8machine_load_game(game, my_machine);
    EXPECT_EQ(result, false);
}

TEST_F(RetroFixture, RetroLoadGameReturnsFalseWhenDataNull) {
    auto game = new retro_game_info;
    game->size = 1;
    game->data = nullptr;
    bool result = chip8machine_load_game(game, my_machine);
    EXPECT_EQ(result, false);
}

TEST(RetroLoadGameSpecial, ReturnTrue) {
    unsigned game_type;
    retro_game_info *info;
    size_t num_info;
    bool actual, expected = true;
    actual = retro_load_game_special(game_type, info, num_info);
    EXPECT_EQ(expected, actual);
}

TEST(RetroUnloadGame, Exists) {
    retro_unload_game();
}

// TODO(WPH):  Ignoring region tests for now
TEST(RetroGetRegion, ReturnsZero) {
    unsigned actual, expected = 0;
    actual = retro_get_region();
    EXPECT_EQ(expected, actual);
}

TEST_F(RetroFixture, RetroGetMemoryDataReturnsPointerToMachineMemory) {
    auto game = new retro_game_info;
    unsigned id;
    game->size = 4;
    game->data = (void *) new unsigned char[4] {0xDE, 0xAD, 0xBE, 0xEF};
    chip8machine_load_game(game, my_machine);
    auto data = (unsigned char *) chip8machine_get_memory_data(id, my_machine);
    for (int i = 0; i < game->size; i++) {
        EXPECT_EQ(((unsigned char *)game->data)[i], data[i + TEST_ROM_START_ADDRESS]);
    }
}

TEST_F(RetroFixture, RetroGetMemorySizeReturnsCorrectSize) {
    unsigned dummy;
    // size_t actual, expected = 0x1000;
    size_t actual, expected = 0;
    actual = chip8machine_get_memory_size(dummy, my_machine);
    EXPECT_EQ(expected, actual);
}

TEST_F(RetroFixture, RetroRunAdvancesOneInstruction) {
    auto game = new retro_game_info;
    game->size = 4;
    game->data = (void *) new unsigned char[4] {0xDE, 0xAD, 0xBE, 0xEF};
    chip8machine_load_game(game, my_machine);
    chip8machine_run(my_machine, true);
    ADDR_TYPE pc = tester.get_pc();
    EXPECT_EQ(pc, 0x202);
    EXPECT_EQ(tester.get_ram()[pc], 0xBE);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
