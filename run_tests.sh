make

ctest

lcov --directory . --capture --output-file coverage.info

lcov --remove coverage.info '/usr/include/*' '*/_deps/*' '*/src/*' --output-file coverage.info.cleaned

genhtml coverage.info.cleaned --output-directory out

lcov --list coverage.info.cleaned