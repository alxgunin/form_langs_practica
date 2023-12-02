# CMake generated Testfile for 
# Source directory: /home/alxgunin/form_langs_practica
# Build directory: /home/alxgunin/form_langs_practica/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(TestPart1 "{PROJECT_SOURCE_DIR}/bin/all_tests")
set_tests_properties(TestPart1 PROPERTIES  _BACKTRACE_TRIPLES "/home/alxgunin/form_langs_practica/CMakeLists.txt;22;add_test;/home/alxgunin/form_langs_practica/CMakeLists.txt;0;")
subdirs("tests")
