# Excluding runtime/int from all tests, as LibRetro makes use of short

# Excluding runtime/references from all tests, since Google themselves took it off the style guide,
# and what few online comments I could find about this check laid out a convincing argument that this
# is contrary to the spirit of modern C++ (forcing you to use pointers when you don't have to.)

# Excluding whitespace/line_length from the tests, because my experience is that restricting line
# length in tests makes them less legible, not more
cpplint --recursive --exclude ../include/libretro.h --exclude ../tests/ --filter=-runtime/int,-runtime/references ..
cpplint --recursive --filter=-whitespace/line_length,-runtime/int,-runtime/references ../tests/
