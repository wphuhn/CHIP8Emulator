#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <exception>
#include <iostream>

#include "libretro.h"

using namespace std;

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;

#define PIXEL_COLOR 0xFFFF

// Creation of a singleton for libretro purposes, but allowing for a
// backend that's TDD friendly
static Chip8Machine& get_instance() {
    static Chip8Machine instance;
    return instance;
}

RETRO_API void retro_init(void) {
    Chip8Machine *instance = &get_instance();
    chip8machine_init(*instance);
}

RETRO_API void retro_deinit(void) {
    Chip8Machine *instance = &get_instance();
    chip8machine_deinit(*instance);
}

RETRO_API unsigned retro_api_version(void) { return RETRO_API_VERSION; }

RETRO_API void retro_get_system_info(struct retro_system_info *info) {
    memset(info, 0, sizeof(*info));
    info->library_name = "chip8-libretro";
    info->library_version = "0.0.1";
    info->valid_extensions = "";
    info->need_fullpath = false;
    info->block_extract = false;
}

RETRO_API void retro_get_system_av_info(struct retro_system_av_info *info) {
    Chip8Machine *instance = &get_instance();
    chip8machine_get_system_av_info(info, *instance);
}

RETRO_API void retro_set_environment(retro_environment_t environment) {}
RETRO_API void retro_set_video_refresh(retro_video_refresh_t videoRefresh) {
    video_cb = videoRefresh;
}
RETRO_API void retro_set_audio_sample(retro_audio_sample_t audioSample){
    audio_cb = audioSample;
}
RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t audioSampleBatch) {}
RETRO_API void retro_set_input_poll(retro_input_poll_t inputPoll) {}
RETRO_API void retro_set_input_state(retro_input_state_t inputState) {}
RETRO_API void retro_set_controller_port_device(unsigned port, unsigned device) {}

void random_noise(retro_audio_sample_t audio_cb) {
    for (int j = 0; j < 735; j++) {
        audio_cb(rand() % 32768, rand() % 32768);
    }
}

void square_wave(retro_audio_sample_t audio_cb) {
    // 420 Hz Triangle wave (7 cycles per frame, 60 frames per second
    for (int j = 0; j < 7; j++) {
        // 735 iterations per frame to reach 44.100 kHz sampling @ 60 fps
        for (int i = 0; i < 52; i++) audio_cb(32768, 32768);
        for (int i = 0; i < 53; i++) audio_cb(0, 0);
    }
}

void triangle_wave(retro_audio_sample_t audio_cb) {
    // 420 Hz Triangle wave (7 cycles per frame, 60 frames per second
    for (int j = 0; j < 7; j++) {
        // 735 iterations per frame to reach 44.100 kHz sampling @ 60 fps
        for (int i = 0; i < 52; i++) audio_cb(i * 630, i * 630);
        for (int i = 0; i < 53; i++) audio_cb(32768 - i * 630, 32768 - i * 630);
    }
}

void sine_wave(retro_audio_sample_t audio_cb) {
    // 420 Hz Triangle wave (7 cycles per frame, 60 frames per second
    unsigned short amp;
    for (int j = 0; j < 7; j++) {
        // 735 iterations per frame to reach 44.100 kHz sampling @ 60 fps
        for (int i = 0; i < 105; i++) {
            amp = (unsigned short) 32768.0 * (1.0 + sin(2.0 * M_PI * i / 105.0)) / 2.0;
            audio_cb(amp, amp);
            std::cout << amp << std::endl;
        }
    }
}

RETRO_API void retro_reset(void) {
    Chip8Machine *instance = &get_instance();
    chip8machine_reset(*instance);
}

RETRO_API void retro_run(void) {
    Chip8Machine *instance = &get_instance();
    chip8machine_run(*instance);
}

RETRO_API size_t retro_serialize_size(void) {return 1;}
RETRO_API bool retro_serialize(void *data, size_t size)
{
    if (size < retro_serialize_size()) return false;
    return true;
}
RETRO_API bool retro_unserialize(const void* data, size_t size)
{
    if (size < retro_serialize_size()) return false;
    return true;
}

RETRO_API void retro_cheat_reset(void) {}
RETRO_API void retro_cheat_set(unsigned index, bool enabled, const char *code) {}

RETRO_API bool retro_load_game(const struct retro_game_info *game) {
    Chip8Machine *instance = &get_instance();
    return chip8machine_load_game(game, *instance);
}
RETRO_API bool retro_load_game_special(unsigned game_type, const struct retro_game_info* game_info, size_t num_info) {return true;}
RETRO_API void retro_unload_game(void) {}

RETRO_API unsigned retro_get_region(void) {return 0;}

RETRO_API void* retro_get_memory_data(unsigned id) {
    void* data;
    return data;
}
RETRO_API size_t retro_get_memory_size(unsigned int id) {
    // TODO:  I have no idea why this dummy invocation is needed...
    Chip8Machine dummy_instance = get_instance();

    Chip8Machine *instance = &get_instance();
    return chip8machine_get_memory_size(id, *instance);
}
RETRO_API void chip8machine_init(Chip8Machine &my_machine) {
    my_machine.reset();
}

RETRO_API void chip8machine_deinit(Chip8Machine &my_machine) {
    my_machine.reset();
}

RETRO_API void chip8machine_get_system_av_info(struct retro_system_av_info *info, const Chip8Machine &my_machine) {
    memset(info, 0, sizeof(*info));
    int height = my_machine.display_height;
    int width = my_machine.display_width;
    int x_scale = my_machine.x_scale;
    int y_scale = my_machine.y_scale;

    info->geometry.aspect_ratio = -1.0; // Use default
    info->geometry.base_height = y_scale * height;
    info->geometry.base_width = x_scale * width;
    info->geometry.max_height = y_scale * height;
    info->geometry.max_width = x_scale * width;
    info->timing.fps = 60.0;
    info->timing.sample_rate = 44100;
}

RETRO_API void chip8machine_reset(Chip8Machine &my_machine) {
    my_machine.reset();
}

RETRO_API void chip8machine_run(Chip8Machine &my_machine, bool run_silent) {
    // TODO:  Strong suspicion this will break the moment users try to size screen
    int width = my_machine.x_scale * my_machine.display_width;
    int height = my_machine.y_scale * my_machine.display_height;

    // TODO:  For now, each frame is one instruction
    my_machine.advance();

    unsigned short framebuffer[height * width];

    for (int y_machine = 0; y_machine < my_machine.display_height; y_machine++) {
        for (int x_machine = 0; x_machine < my_machine.display_width; x_machine++) {
            unsigned short pixel = my_machine.get_pixel(x_machine, y_machine);
            if (pixel > 0) {
                pixel = PIXEL_COLOR;
            }
            for (int y_sub = 0; y_sub < my_machine.y_scale; y_sub++) {
                for (int x_sub = 0; x_sub < my_machine.x_scale; x_sub++) {
                    unsigned short x = my_machine.y_scale * x_machine + x_sub;
                    unsigned short y = my_machine.y_scale * y_machine + y_sub;
                    framebuffer[y * width + x] = pixel;

                }
            }
        }
    }

    if (not run_silent) {
        video_cb(framebuffer, width, height, sizeof(unsigned short) * width);
 //       random_noise(audio_cb);
    }
}

RETRO_API bool chip8machine_load_game(const struct retro_game_info *game, Chip8Machine &my_machine) {
    if (game->size == 0) return false;
    if (game->data == nullptr) return false;
    std::vector<unsigned char> rom = Memory::convert_bitstream_to_vector(game->data, game->size);
    my_machine.load_rom(rom);
    return true;
}

RETRO_API size_t chip8machine_get_memory_size(unsigned int id, const Chip8Machine &my_machine) {
    return my_machine.memory_size;
}
