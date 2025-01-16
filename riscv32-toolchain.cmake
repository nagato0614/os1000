# ========== riscv32-toolchain.cmake ==========

# クロスコンパイルすることを明示
set(CMAKE_SYSTEM_NAME Generic)
# または Linux などと指定しても可。
# set(CMAKE_SYSTEM_NAME Linux)

# CPU アーキテクチャ(ターゲット)を指定
set(CMAKE_SYSTEM_PROCESSOR riscv32)

# C コンパイラを Clang (riscv32向け) に指定
# Homebrew llvm を使っている場合の例
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")

# macOS 固有のフラグを無効化するために、これらを空にする
set(CMAKE_OSX_ARCHITECTURES "")
set(CMAKE_OSX_DEPLOYMENT_TARGET "")
set(CMAKE_OSX_SYSROOT "")

# C_FLAGS に必要なクロスコンパイル用オプションを追加
set(CMAKE_C_FLAGS "-std=c17 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib")
set(CMAKE_CXX_FLAGS "-std=c++17 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib")
