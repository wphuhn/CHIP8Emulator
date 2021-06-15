#include "../include/libretro.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
static const short MAX_AUDIO_VALUE = 32767;
static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;

// Creation of a singleton for libretro purposes, but allowing for a
// backend that's TDD friendly
static Emulator::Chip8Machine &get_instance() {
  static Emulator::Chip8Machine instance;
  return instance;
}

RETRO_API void retro_init(void) {
  Emulator::Chip8Machine *instance = &get_instance();
  chip8machine_init(*instance);
}

RETRO_API void retro_deinit(void) {
  Emulator::Chip8Machine *instance = &get_instance();
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
  Emulator::Chip8Machine *instance = &get_instance();
  chip8machine_get_system_av_info(info, *instance);
}

RETRO_API void retro_set_environment(retro_environment_t environment) {}
RETRO_API void retro_set_video_refresh(retro_video_refresh_t videoRefresh) {
  video_cb = videoRefresh;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
RETRO_API void retro_set_audio_sample(retro_audio_sample_t audioSample) {
  audio_cb = audioSample;
}
#pragma clang diagnostic pop

RETRO_API void retro_set_audio_sample_batch(
    retro_audio_sample_batch_t audioSampleBatch) {}
RETRO_API void retro_set_input_poll(retro_input_poll_t inputPoll) {}
RETRO_API void retro_set_input_state(retro_input_state_t inputState) {}
RETRO_API void retro_set_controller_port_device(
    unsigned port, unsigned device) {}

void random_noise(retro_audio_sample_t my_audio_cb) {
  // Shameless copy-paste from Google
  // If this subroutine actually gets used, generator creation should be
  // moved out of this subroutine
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<short> distribution(0, MAX_AUDIO_VALUE);

  // 735 iterations to reach 44.100 kHz sampling @ 60 fps
  const int n_iterations = 735;
  for (int j = 0; j < n_iterations; j++) {
    my_audio_cb(distribution(gen), distribution(gen));
  }
}

void square_wave(retro_audio_sample_t my_audio_cb) {
  // 420 Hz Triangle wave (7 cycles per frame, 60 frames per second)
  const int n_cycles_per_frame = 7;
  // 105 iterations per cycle to reach 44.100 kHz sampling @ 60 fps
  // @ 7 cycles per frame
  const int n_iterations_per_cycle = 105;
  // Corresponds to duty cycle of 50%
  const int n_high_per_cycle = 52;

  int n_low_per_cycle = n_iterations_per_cycle - n_high_per_cycle;
  for (int j = 0; j < n_cycles_per_frame; j++) {
    for (int i = 0; i < n_high_per_cycle; i++)
      my_audio_cb(
          MAX_AUDIO_VALUE, MAX_AUDIO_VALUE);
    for (int i = 0; i < n_low_per_cycle; i++) my_audio_cb(0, 0);
  }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
void triangle_wave(retro_audio_sample_t my_audio_cb) {
  // 420 Hz Triangle wave (7 cycles per frame, 60 frames per second)
  const int n_cycles_per_frame = 7;
  // 105 iterations per cycle to reach 44.100 kHz sampling @ 60 fps
  // @ 7 cycles per frame
  const int n_iterations_per_cycle = 105;
  // Corresponds to duty cycle of 50%
  const int n_rising_per_cycle = 52;
  // For a max value of 32767, the closest
  const short rising_increment = 630;
  const short falling_increment = 628;

  short n_falling_per_cycle = n_iterations_per_cycle - n_rising_per_cycle;
  for (int j = 0; j < n_cycles_per_frame; j++) {
    // 735 iterations per frame to reach 44.100 kHz sampling @ 60 fps
    for (int i = 0; i < n_rising_per_cycle; i++)
      my_audio_cb(
          i * rising_increment, i * rising_increment);
    for (int i = 0; i < n_falling_per_cycle; i++)
      my_audio_cb(
          MAX_AUDIO_VALUE - i * falling_increment,
          MAX_AUDIO_VALUE - i * falling_increment);
  }
}
#pragma clang diagnostic pop

void sine_wave(retro_audio_sample_t my_audio_cb) {
  // 420 Hz Triangle wave (7 cycles per frame, 60 frames per second)
  const int n_cycles_per_frame = 7;
  // 105 iterations per cycle to reach 44.100 kHz sampling @ 60 fps
  // @ 7 cycles per frame
  const int n_iterations_per_cycle = 105;

  short signal;
  double phase;
  for (int j = 0; j < n_cycles_per_frame; j++) {
    for (int i = 0; i < n_iterations_per_cycle; i++) {
      phase = 2.0 * M_PI * i / n_iterations_per_cycle;
      signal = static_cast<short>(
          (1.0 * MAX_AUDIO_VALUE) * sin(phase) / 2.0);
      my_audio_cb(signal, signal);
      std::cout << signal << std::endl;
    }
  }
}

RETRO_API void retro_reset(void) {
  Emulator::Chip8Machine *instance = &get_instance();
  chip8machine_reset(*instance);
}

RETRO_API void retro_run(void) {
  Emulator::Chip8Machine *instance = &get_instance();
  chip8machine_run(*instance);
}

RETRO_API size_t retro_serialize_size(void) { return 1; }
RETRO_API bool retro_serialize(void *data, size_t size) {
  if (size < retro_serialize_size()) return false;
  return true;
}
RETRO_API bool retro_unserialize(const void *data, size_t size) {
  if (size < retro_serialize_size()) return false;
  return true;
}

RETRO_API void retro_cheat_reset(void) {}
RETRO_API void retro_cheat_set(unsigned index, bool enabled, const char *code) {
}

RETRO_API bool retro_load_game(const struct retro_game_info *game) {
  Emulator::Chip8Machine *instance = &get_instance();
  return chip8machine_load_game(game, *instance);
}
RETRO_API bool retro_load_game_special(unsigned game_type,
                                       const struct retro_game_info *game_info,
                                       size_t num_info) {
  return true;
}
RETRO_API void retro_unload_game(void) {}

RETRO_API unsigned retro_get_region(void) { return 0; }

RETRO_API void *retro_get_memory_data(unsigned id) {
  Emulator::Chip8Machine *instance = &get_instance();
  return chip8machine_get_memory_data(id, *instance);
}

RETRO_API size_t retro_get_memory_size(unsigned int id) {
  Emulator::Chip8Machine *instance = &get_instance();
  return chip8machine_get_memory_size(id, *instance);
}

namespace Emulator {

RETRO_API void chip8machine_init(Chip8Machine &my_machine) {
  my_machine.reset();
}

RETRO_API void chip8machine_deinit(Chip8Machine &my_machine) {
  my_machine.reset();
}

RETRO_API void chip8machine_get_system_av_info(
    struct retro_system_av_info *info,
    const Chip8Machine &my_machine) {
  memset(info, 0, sizeof(*info));
  int height = my_machine.display_height;
  int width = my_machine.display_width;
  int x_scale = Upscaler::x_scale;
  int y_scale = Upscaler::y_scale;

  info->geometry.aspect_ratio = -1.0;  // Use default
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
  // TODO(WPH):  Strong suspicion this will break the moment users try
  //  to resize screen
  int width = Upscaler::x_scale * my_machine.display_width;
  int height = Upscaler::y_scale * my_machine.display_height;

  // TODO(WPH):  For now, each frame is one instruction
  my_machine.advance();

  unsigned short frame_buffer[height * width];

  Upscaler::upscale(frame_buffer, width, my_machine);

  if (!run_silent) {
    video_cb(frame_buffer, width, height, sizeof(unsigned short) * width);
    // sine_wave(audio_cb);
  }
}

RETRO_API bool chip8machine_load_game(const struct retro_game_info *game,
                                      Chip8Machine &my_machine) {
  if (game->size == 0) return false;
  if (game->data == nullptr) return false;
  std::vector<unsigned char> rom =
      Memory::convert_bitstream_to_vector(game->data, game->size);
  my_machine.load_rom(rom);
  return true;
}

RETRO_API void *chip8machine_get_memory_data(unsigned id,
                                             const Chip8Machine &my_machine) {
  return my_machine.get_pointer_to_ram_start();
}

RETRO_API size_t chip8machine_get_memory_size(unsigned int id,
                                              const Chip8Machine &my_machine) {
  return 0;
  // return my_machine.memory_size;
}

}  // namespace Emulator

// CLion has a long-standing bug (3 years old...) about this being incorrectly
// identified as having no associated push statement, so remove it only for the
// CLion IDE
#ifndef __CLION_IDE_
#pragma clang diagnostic pop
#endif
