rm -rf a.out 
cc main.c -lm -fsanitize=address -fsanitize=null -g3 -D DEBUG=0 -o comp
./comp file.hr

# Argument provided, process the specified file
file_count=$(ls 1.tests/*.hr 2>/dev/null | wc -l)

next_file_number=$((file_count + 1))
new_filename="$(printf '%03d' "$next_file_number")"

new_hr_file="1.tests/${new_filename}.hr"
new_s_file="1.tests/cmp/${new_filename}.s"
echo "create $new_hr_file"
echo "create $new_s_file"

cp "file.hr" "$new_hr_file"
cp "file.s" "$new_s_file"

# Compare the generated file.s with cmp/new_filename.s
if cmp -s "file.s" "$new_s_file"; then
    echo -e "${GREEN}SUCCESS${NC} for $new_hr_file"
else
    echo -e "${RED}FAILED${NC} for $new_hr_file"
fi
