line1='alias run="bash _run.sh"'
line2='alias tests="bash _tests.sh"'
line3='alias copy="bash _copy.sh"'

if ! grep -qF "$line1" ~/.bashrc || ! grep -qF "$line2" ~/.bashrc || ! grep -qF "$line3" ~/.bashrc; then
    echo "" >> ~/.bashrc
    echo "$line1" >> ~/.bashrc
    echo "$line2" >> ~/.bashrc
    echo "$line3" >> ~/.bashrc
    echo "" >> ~/.bashrc
    . ~/.bashrc
fi

tail -f > /dev/null