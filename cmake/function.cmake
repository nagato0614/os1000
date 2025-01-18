# 指定したユーザープログラムをビルドしてカーネルにリンクする
# 引数: target (ユーザープログラム名)
# 例: add_user_executable(shell)
# この場合、user_scripts/shell/shell.c がビルドされる
function(add_user_executable target)
    file(GLOB_RECURSE TARGET_SOURCES user_scripts/${target}/*.c)

    add_executable(${target}.elf
            ${TARGET_SOURCES}
            ${USER_SOURCES}
            ${STDLIB_SOURCES}
    )

    target_include_directories(${target}.elf PRIVATE
            user
    )

    # リンカスクリプト (user.ld) がある場合は明示的に指定
    # -Wl,-Map=user.map でマップファイルも生成
    target_link_options(${target}.elf PRIVATE
            "-T${CMAKE_SOURCE_DIR}/ld/user.ld"
            "-Wl,-Map=user.map"
    )

    # ======== target.elf から target.bin を生成するカスタムコマンド ========
    # shell.elf を shell.bin に変換
    add_custom_command(
            OUTPUT ${target}.bin
            COMMAND ${OBJCOPY} --set-section-flags .bss=alloc,contents -O binary ${target}.elf ${target}.bin
            DEPENDS ${target}.elf
            COMMENT "Converting ${target}.elf to ${target}.bin"
            VERBATIM
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    # shell.bin を shell.bin.o に変換
    add_custom_command(
            OUTPUT ${target}.bin.o
            COMMAND ${OBJCOPY} -I binary -O elf32-littleriscv ${target}.bin ${target}.bin.o
            DEPENDS shell.bin
            COMMENT "Converting ${target}.bin to ${target}.bin.o"
            VERBATIM
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    add_custom_target(${target}_bin_o
            DEPENDS ${target}.bin.o
    )

    add_dependencies(kernel.elf ${target}_bin_o)
    target_sources(kernel.elf PRIVATE ${CMAKE_BINARY_DIR}/${target}.bin.o)

endfunction(add_user_executable)