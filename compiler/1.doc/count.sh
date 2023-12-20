find ../ -type f \( -name '*.c' -o -name '*.s' \) -print0 | xargs -0 wc -l

