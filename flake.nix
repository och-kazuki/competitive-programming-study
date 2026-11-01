{
  description = "競技プログラミング用の C++ / Python 開発環境";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      forAllSystems =
        f:
        nixpkgs.lib.genAttrs systems (
          system:
          f {
            pkgs = import nixpkgs { inherit system; };
          }
        );
    in
    {
      devShells = forAllSystems (
        { pkgs }:
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              ac-library
              clang-tools
              gcc
              just
              python312
              pyright
              ruff
              time
              uv
            ];

            shellHook = ''
              export CP_PROJECT_ROOT="$PWD"
              export AC_LIBRARY_INCLUDE="${pkgs.ac-library}/include"
              export CPLUS_INCLUDE_PATH="$PWD:$AC_LIBRARY_INCLUDE''${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"
              export UV_PROJECT_ENVIRONMENT="$PWD/.venv"
              export UV_PYTHON_DOWNLOADS=never
              export UV_PYTHON="${pkgs.python312}/bin/python"
              export PYTHONPATH="$PWD/library/python''${PYTHONPATH:+:$PYTHONPATH}"

              uv sync --frozen
              export PATH="$UV_PROJECT_ENVIRONMENT/bin:$PWD/scripts:$PATH"
              "$PWD/scripts/sync-dev-files"

              echo "competitive-programming-study"
              echo "  C++:    g++ / clangd / AtCoder Library"
              echo "  Python: python / uv / pyright / ruff / oj"
              echo "  実行:   runcpp / runpy / just --list"
            '';
          };
        }
      );

      checks = forAllSystems (
        { pkgs }:
        {
          devShell = self.devShells.${pkgs.system}.default;
          check =
            pkgs.runCommand "competitive-programming-study-check"
              {
                nativeBuildInputs = [
                  pkgs.bash
                  pkgs.coreutils
                  pkgs.gcc
                  pkgs.python312
                  pkgs.ruff
                  pkgs.ac-library
                ];
              }
              ''
                export CP_PROJECT_ROOT=${./.}
                export AC_LIBRARY_INCLUDE=${pkgs.ac-library}/include
                bash "${./scripts/check}"
                touch "$out"
              '';
        }
      );
    };
}
