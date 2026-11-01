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
            packages =
              with pkgs;
              [
                ac-library
                clang-tools
                gcc
                just
                python312
                pyright
                ruff
                time
                uv
              ]
              ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
                pkgs.wl-clipboard
                pkgs.xclip
              ];

            shellHook = ''
              # サブディレクトリから nix develop しても、リポジトリのルートを基準にする。
              project_root=""
              if command -v git >/dev/null 2>&1; then
                project_root="$(git rev-parse --show-toplevel 2>/dev/null || true)"
                if [ -n "$project_root" ] && [ ! -f "$project_root/flake.nix" ]; then
                  project_root=""
                fi
              fi
              if [ -z "$project_root" ]; then
                dir="$PWD"
                while true; do
                  if [ -f "$dir/flake.nix" ] && [ -f "$dir/scripts/sync-dev-files" ]; then
                    project_root="$dir"
                    break
                  fi
                  if [ "$dir" = "/" ]; then
                    break
                  fi
                  dir="$(dirname "$dir")"
                done
              fi
              if [ -z "$project_root" ]; then
                echo "リポジトリのルートが見つかりません。リポジトリの中で nix develop を実行してください。" >&2
                exit 1
              fi

              export CP_IN_DEV_SHELL=1
              export CP_PROJECT_ROOT="$project_root"
              export AC_LIBRARY_INCLUDE="${pkgs.ac-library}/include"
              export CPLUS_INCLUDE_PATH="$project_root:$AC_LIBRARY_INCLUDE''${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"
              export UV_PROJECT_ENVIRONMENT="$project_root/.venv"
              export UV_PYTHON_DOWNLOADS=never
              export UV_PYTHON="${pkgs.python312}/bin/python"
              export PYTHONPATH="$project_root/library/python''${PYTHONPATH:+:$PYTHONPATH}"

              uv sync --frozen --project "$project_root"
              export PATH="$UV_PROJECT_ENVIRONMENT/bin:$project_root/scripts:$PATH"
              "$project_root/scripts/sync-dev-files"
              . "$project_root/scripts/shell-functions.sh"

              echo "competitive-programming-study"
              echo "  root:   $project_root"
              echo "  C++:    g++ / clangd / AtCoder Library"
              echo "  Python: python / uv / pyright / ruff / oj"
              echo "  移動:   abc 400 / abc 400 e / contest arc190"
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
                export CP_IN_DEV_SHELL=1
                export CP_PROJECT_ROOT=${./.}
                export AC_LIBRARY_INCLUDE=${pkgs.ac-library}/include
                bash "$CP_PROJECT_ROOT/scripts/check"
                touch "$out"
              '';
        }
      );
    };
}
