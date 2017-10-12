;; Object ede-generic-config
;; EDE Generic Project Configuration
(ede-generic-config "ede-generic-config"
  :file "EDEConfig.el"
  :build-command "rm -rf build; mkdir build/; cd build/ && cmake .. && make"
  :debug-command "gdb ./build/calc"
  :run-command "./build/calc"
  :c-include-path '("." "/usr/include/qt/QtWidgets")
  :c-preprocessor-files '("common.hpp"))
