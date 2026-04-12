#!/usr/bin/env python3
"""Generate a new Vroom module scaffold without copying ModuleTemplate files.

Usage:
  python cmake/create_module.py Editor
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path


def _to_macro_name(module_name: str) -> str:
    # Convert CamelCase / mixed names into uppercase C-style macro segments.
    # Example: "RenderObjects" -> "RENDEROBJECTS", "My_Module" -> "MYMODULE".
    return re.sub(r"[^A-Za-z0-9]", "", module_name).upper()


def _cmake_content(module_name: str, macro_name: str) -> str:
    return f"""# ----- Module definition -----

project({module_name})
add_module({module_name})

# ----- Dependencies -----

module_link_modules({module_name} Core)
# module_link_library({module_name} PUBLIC some_library)

# ----- Compile definitions -----

module_compile_definition(
  {module_name}
  PRIVATE VRM_{macro_name}_EXPORTS=1
)

# ----- Compile options -----
"""


def _api_header_content(macro_name: str) -> str:
    return f"""#pragma once

#if defined(__GNUC__)
  #if defined(VRM_{macro_name}_EXPORTS)
    #define VRM_{macro_name}_API __attribute__ ((visibility (\"default\")))
  #else
    #define VRM_{macro_name}_API
  #endif
#elif defined(_MSC_VER)
  #if defined(VRM_{macro_name}_EXPORTS)
    #define VRM_{macro_name}_API __declspec(dllexport)
  #else
    #define VRM_{macro_name}_API
  #endif
#else
  #define VRM_{macro_name}_API
#endif
"""


def _validate_name(module_name: str) -> None:
    if not re.fullmatch(r"[A-Za-z][A-Za-z0-9_]*", module_name):
        raise ValueError(
            "Invalid module name. Use CMake-safe identifiers like 'Editor' or 'MyModule_2'."
        )


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Create a Vroom module scaffold from generated content."
    )
    parser.add_argument("module_name", help="Module name, e.g. Editor")
    parser.add_argument(
        "--vroom-root",
        type=Path,
        default=Path(__file__).resolve().parents[1] / "Vroom",
        help="Path to Vroom/ (auto-detected by default).",
    )
    args = parser.parse_args()

    module_name = args.module_name
    _validate_name(module_name)

    vroom_root = args.vroom_root.resolve()
    module_dir = vroom_root / module_name
    if not vroom_root.exists():
        print(f"Error: Vroom root not found: {vroom_root}")
        return 1

    if module_dir.exists():
        print(f"Error: Module already exists: {module_dir}")
        return 1

    macro_name = _to_macro_name(module_name)

    (module_dir / "private" / module_name).mkdir(parents=True, exist_ok=False)
    (module_dir / "protected" / module_name).mkdir(parents=True, exist_ok=False)
    (module_dir / "public" / module_name).mkdir(parents=True, exist_ok=False)
    (module_dir / "src").mkdir(parents=True, exist_ok=False)

    (module_dir / "CMakeLists.txt").write_text(
        _cmake_content(module_name, macro_name), encoding="utf-8"
    )
    (module_dir / "public" / module_name / "Api.h").write_text(
        _api_header_content(macro_name), encoding="utf-8"
    )

    cmake_root_file = vroom_root / "CMakeLists.txt"
    registration_line = f'add_subdirectory("{module_name}")'

    cmake_text = cmake_root_file.read_text(encoding="utf-8")
    if registration_line in cmake_text:
        print(f"Registration already present in {cmake_root_file}")
    else:
        marker = '# VRM-SUBDIRS-MARKERS'
        if marker in cmake_text:
            cmake_text = cmake_text.replace(
                marker,
                f'{registration_line}\n{marker}',
                1,
            )
        else:
            cmake_text += f"\n{registration_line}\n"
        cmake_root_file.write_text(cmake_text, encoding="utf-8")
        print(f"Registered module in {cmake_root_file}")

    print(f"Created module: {module_name}")
    print(f"Path: {module_dir}")
    print("Generated files:")
    print(f"  - {module_dir / 'CMakeLists.txt'}")
    print(f"  - {module_dir / 'public' / module_name / 'Api.h'}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
