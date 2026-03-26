# SPDX-FileCopyrightText: 2025 SiFli Technologies Co., Ltd
# SPDX-License-Identifier: Apache-2.0

"""Interactive/new-config board generator for sdk.py."""

from typing import Any, Dict, Optional

import click
from click.core import Context

from sdk_py_actions.global_options import global_options
from sdk_py_actions.new_board import run_new_board
from sdk_py_actions.tools import PropertyDict


def action_extensions(base_actions: Dict, project_path: str) -> Any:
    def new_board_callback(
        target_name: str,
        ctx: Context,
        args: PropertyDict,
        config_path: Optional[str],
        output_dir: Optional[str],
    ) -> None:
        run_new_board(
            config_path=config_path,
            output_root=output_dir or args.project_dir,
        )

    return {
        'actions': {
            'new-board': {
                'callback': new_board_callback,
                'help': 'Create a new board from interactive prompts or a JSON config file.',
                'options': global_options + [
                    {
                        'names': ['--config', '-c', 'config_path'],
                        'help': 'Path to a JSON config file for non-interactive board creation.',
                        'type': click.Path(exists=True, dir_okay=False, resolve_path=False),
                        'default': None,
                    },
                    {
                        'names': ['--output-dir', '-o', 'output_dir'],
                        'help': 'Directory where the board/base directories will be created. Defaults to the current terminal directory.',
                        'type': click.Path(exists=False, file_okay=False, dir_okay=True, resolve_path=False),
                        'default': None,
                    },
                ],
            },
        },
    }
