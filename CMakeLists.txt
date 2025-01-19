cmake_minimum_required(VERSION 3.28)
project(os1000 C)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/riscv32-toolchain.cmake")

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(OBJCOPY /opt/homebrew/opt/llvm/bin/llvm-objcopy)

include(cmake/function.cmake)

file(GLOB_RECURSE KERNEL_SOURCES kernel/*.c)
file(GLOB_RECURSE USER_SOURCES user/*.c)
file(GLOB_RECURSE STDLIB_SOURCES stdlib/*.c)


# main.c と common.c をコンパイルし、kernel.elf を出力
add_executable(kernel.elf
        ${KERNEL_SOURCES}
        ${STDLIB_SOURCES}
)

# kernel.elf をビルドするときだけ __KERNEL__ を定義
target_compile_definitions(kernel.elf PRIVATE
        __KERNEL__
)

target_include_directories(kernel.elf PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib
)

# 指定したユーザープログラムをビルドしてカーネルにリンクする
add_user_executable(shell)


# リンカスクリプト (kernel.ld) がある場合は明示的に指定
# -Wl,-Map=kernel.map でマップファイルも生成
target_link_options(kernel.elf PRIVATE
        "-T${CMAKE_SOURCE_DIR}/ld/kernel.ld"
        "-Wl,-Map=kernel.map"
)

# disk ディレクトリを丸ごとビルドディレクトリにコピー
file(COPY disk DESTINATION ${CMAKE_BINARY_DIR})

# disk ディレクトリを tar アーカイブ化するカスタムコマンド
add_custom_command(
        OUTPUT disk.tar
        COMMAND tar cf ../disk.tar --format=ustar hello.txt meow.txt
        DEPENDS shell.bin
        COMMENT "Creating disk.tar"
        VERBATIM
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/disk
)

# disk.tar を生成するカスタムターゲット
add_custom_target(disk_tar
        DEPENDS disk.tar
)

add_dependencies(kernel.elf disk_tar)

# lorem.txt をビルドディレクトリにコピー
configure_file(lorem.txt ${CMAKE_BINARY_DIR}/lorem.txt COPYONLY)


# QEMU 実行を run ターゲットとして定義
add_custom_target(run
        COMMAND qemu-system-riscv32
        -machine virt
        -bios default
        -nographic
        -serial mon:stdio
        --no-reboot
        -kernel kernel.elf
        -drive id=drive0,file=disk.tar,format=raw,if=none
        -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0
        DEPENDS kernel.elf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running QEMU with kernel.elf..."
)