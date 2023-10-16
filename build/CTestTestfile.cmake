# CMake generated Testfile for 
# Source directory: /Users/gad/work/mipt/grade2/form_langs_practica
# Build directory: /Users/gad/work/mipt/grade2/form_langs_practica/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[TestPart1]=] "{Testing_SOURCE_DIR}/bin/all_tests")
set_tests_properties([=[TestPart1]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/gad/work/mipt/grade2/form_langs_practica/CMakeLists.txt;23;add_test;/Users/gad/work/mipt/grade2/form_langs_practica/CMakeLists.txt;0;")
subdirs("tests")
