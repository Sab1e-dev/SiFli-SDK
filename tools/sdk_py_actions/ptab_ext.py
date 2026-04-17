# SPDX-FileCopyrightText: 2025-2026 SiFli Technologies Co., Ltd
# SPDX-License-Identifier: Apache-2.0

from __future__ import annotations

"""
PTAB v3 extension for sdk.py.

Provides a command to export the effective project PTAB after applying
chip/board-level overlay files.
"""

import os
import sys
import tomllib
from typing import Dict, Optional, Tuple

import click
import yaml

from sdk_py_actions.cli.context import SdkContext
from sdk_py_actions.cli.registry import CommandRegistry
from sdk_py_actions.errors import FatalError

EXTENSION_ID = "ptab"
EXTENSION_VERSION = "2.1.0"
EXTENSION_API_VERSION = 2
MIN_SDK_VERSION = None


def _read_project_config(project_dir: str) -> Dict[str, Optional[str]]:
    config_path = os.path.join(project_dir, '.project.toml')
    config = {
        'board': None,
        'board_search_path': None,
    }
    if not os.path.exists(config_path):
        return config

    try:
        with open(config_path, 'rb') as f:
            data = tomllib.load(f)
    except Exception as e:
        raise FatalError(f'Failed to read .project.toml: {e}')

    config['board'] = data.get('board')
    config['board_search_path'] = data.get('board_search_path')
    return config


def _resolve_board_options(
    project_dir: str,
    board: Optional[str],
    board_search_path: Optional[str],
) -> Tuple[str, Optional[str]]:
    if not board or board_search_path is None:
        config = _read_project_config(project_dir)
        if not board:
            board = config.get('board')
        if board_search_path is None:
            board_search_path = config.get('board_search_path')

    if not board:
        raise FatalError('Board name is required. Use "--board" or run "sdk.py set-target" first.')

    if board_search_path:
        board_search_path = os.path.abspath(board_search_path)

    return board, board_search_path


def _load_ptab_module():
    build_tools = os.path.join(os.environ['SIFLI_SDK_PATH'], 'tools', 'build')
    if build_tools not in sys.path:
        sys.path.insert(0, build_tools)
    import ptab as ptab_module
    return ptab_module


def ptab_export_callback(
    sdk_ctx: SdkContext,
    board: Optional[str] = None,
    board_search_path: Optional[str] = None,
    output_path: Optional[str] = None,
    strict: bool = False,
) -> None:
    project_dir = os.path.realpath(sdk_ctx.project_dir)
    board, board_search_path = _resolve_board_options(project_dir, board, board_search_path)

    ptab_module = _load_ptab_module()
    board = ptab_module.normalize_board_name(board)
    chip = ptab_module.detect_chip_dir_from_board(
        board,
        board_search_path=board_search_path,
        sdk_root=os.environ['SIFLI_SDK_PATH'],
    )
    if not chip:
        raise FatalError(f'Unable to detect CHIP from board rtconfig.py: {board}')

    board_path, _ = ptab_module.get_board_paths(
        board,
        board_search_path=board_search_path,
        sdk_root=os.environ['SIFLI_SDK_PATH'],
    )

    prepared = ptab_module.prepare_project_ptab(
        project_dir,
        board,
        chip,
        board_path=board_path,
        emit_summary=False,
        validate=True,
        strict_validation=strict,
    )

    ptab_obj = prepared.get('ptab_obj')
    if not ptab_obj or not ptab_obj.is_v3():
        raise FatalError('ptab-export only supports ptab v3 projects.')

    effective_data = prepared.get('merged_data')
    if effective_data is None:
        ptab_path = prepared.get('path')
        if not ptab_path:
            raise FatalError('Unable to determine PTAB path for export.')
        with open(ptab_path, encoding='utf-8-sig') as f:
            effective_data = yaml.safe_load(f)

    output_yaml = ptab_module.dump_ptab_yaml(effective_data)

    if output_path:
        output_path = os.path.abspath(output_path)
        ptab_module.write_effective_ptab_yaml(output_path, effective_data)
        prepared.get('report', {})['effective_path'] = output_path
        click.echo(f'Wrote effective PTAB YAML to {output_path}', err=True)
    else:
        prepared.get('report', {})['effective_path'] = '<stdout>'
        click.echo(output_yaml, nl=False)

    ptab_module.print_overlay_summary(prepared.get('report', {}), dedupe=False)

    issues = prepared.get('report', {}).get('validation', [])
    error_count = sum(1 for issue in issues if getattr(issue, 'severity', '') == 'error')
    warning_count = sum(1 for issue in issues if getattr(issue, 'severity', '') == 'warning')
    click.echo(
        f'Validated PTAB v3: {error_count} error(s), {warning_count} warning(s)',
        err=True,
    )


def register(registry: CommandRegistry) -> None:
    registry.command(
        path='ptab-export',
        callback=ptab_export_callback,
        help='Export the effective project ptab v3 YAML after applying overlay files.',
        options=[
            {
                'names': ['--board'],
                'help': 'Board name. Falls back to .project.toml when omitted.',
                'default': None,
            },
            {
                'names': ['--board-search-path', '--board_search_path'],
                'help': 'Additional board search path.',
                'default': None,
            },
            {
                'names': ['--output', '-o', 'output_path'],
                'help': 'Write the effective PTAB YAML to a file. Defaults to stdout.',
                'type': click.Path(dir_okay=False, resolve_path=False),
                'default': None,
            },
            {
                'names': ['--strict', '-s'],
                'help': 'Treat validation warnings as errors.',
                'is_flag': True,
                'default': False,
            },
        ],
    )
