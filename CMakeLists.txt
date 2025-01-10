cmake_minimum_required(VERSION 3.28)
project(os1000 C)

set(CMAKE_C_STANDARD 17)

# main.c と common.c をコンパイルし、kernel.elf を出力
add_executable(kernel.elf
        main.c
        common.c
)

# リンカスクリプト (kernel.ld) がある場合は明示的に指定
# -Wl,-Map=kernel.map でマップファイルも生成
target_link_options(kernel.elf PRIVATE
        "-T${CMAKE_SOURCE_DIR}/kernel.ld"
        "-Wl,-Map=kernel.map"
)

# QEMU 実行を run ターゲットとして定義
add_custom_target(run
        COMMAND qemu-system-riscv32
        -machine virt
        -bios default
        -nographic
        -serial mon:stdio
        --no-reboot
        -kernel kernel.elf
        DEPENDS kernel.elf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running QEMU with kernel.elf..."
)
