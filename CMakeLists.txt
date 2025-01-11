cmake_minimum_required(VERSION 3.28)
project(os1000 C)

set(CMAKE_C_STANDARD 17)
set(OBJCOPY /opt/homebrew/opt/llvm/bin/llvm-objcopy)

# main.c と common.c をコンパイルし、kernel.elf を出力
add_executable(kernel.elf
        kernel.c
        common.c
)

# ユーザー空間で動かすプログラム
add_executable(shell.elf
        shell.c
        common.c
        user.c
)

# リンカスクリプト (kernel.ld) がある場合は明示的に指定
# -Wl,-Map=kernel.map でマップファイルも生成
target_link_options(kernel.elf PRIVATE
        "-T${CMAKE_SOURCE_DIR}/kernel.ld"
        "-Wl,-Map=kernel.map"
)

# リンカスクリプト (user.ld) がある場合は明示的に指定
# -Wl,-Map=user.map でマップファイルも生成
target_link_options(shell.elf PRIVATE
        "-T${CMAKE_SOURCE_DIR}/user.ld"
        "-Wl,-Map=user.map"
)

# ======== shell.elf から shell.bin を生成するカスタムコマンド ========

# shell.elf を shell.bin に変換
add_custom_command(
        OUTPUT shell.bin
        COMMAND ${OBJCOPY} --set-section-flags .bss=alloc,contents -O binary ${CMAKE_BINARY_DIR}/shell.elf ${CMAKE_BINARY_DIR}/shell.bin
        DEPENDS shell.elf
        COMMENT "Converting shell.elf to shell.bin"
        VERBATIM
)

# shell.bin を shell.bin.o に変換
add_custom_command(
        OUTPUT shell.bin.o
        COMMAND ${OBJCOPY} -I binary -O elf32-littleriscv ${CMAKE_BINARY_DIR}/shell.bin ${CMAKE_BINARY_DIR}/shell.bin.o
        DEPENDS shell.bin
        COMMENT "Converting shell.bin to shell.bin.o"
        VERBATIM
)

# ======== shell.bin.o を生成するカスタムターゲット ========
add_custom_target(shell_bin_o
        DEPENDS shell.bin.o
)

# ======== kernel.elf に shell.bin.o をリンクする ========
# kernel.elf に shell.bin.o を追加するための設定
add_dependencies(kernel.elf shell_bin_o)
target_sources(kernel.elf PRIVATE ${CMAKE_BINARY_DIR}/shell.bin.o)


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
