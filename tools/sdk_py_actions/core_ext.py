# SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0
import fnmatch
import glob
import json
import locale
import os
import re
import shutil
import subprocess
import sys
from typing import Any
from typing import Dict
from typing import List
from typing import Optional
from urllib.error import URLError
from urllib.request import Request
from urllib.request import urlopen
from webbrowser import open_new_tab

import click
from click.core import Context
from sdk_py_actions.constants import GENERATORS
from sdk_py_actions.constants import PREVIEW_TARGETS
from sdk_py_actions.constants import SUPPORTED_TARGETS
from sdk_py_actions.constants import URL_TO_DOC
from sdk_py_actions.errors import FatalError
from sdk_py_actions.global_options import global_options
from sdk_py_actions.tools import ensure_build_directory
from sdk_py_actions.tools import generate_hints
from sdk_py_actions.tools import sdk_version
from sdk_py_actions.tools import merge_action_lists
from sdk_py_actions.tools import print_warning
from sdk_py_actions.tools import PropertyDict
from sdk_py_actions.tools import run_target
from sdk_py_actions.tools import TargetChoice
from sdk_py_actions.tools import yellow_print


def action_extensions(base_actions: Dict, project_path: str) -> Any:

    def menuconfig(target_name: str, ctx: Context, args: PropertyDict, board: Optional[str], board_search_path: Optional[str]) -> None:
        """
        Menuconfig target is build_target extended with the style argument for setting the value for the environment
        variable.
        """
        menuconfig_path = os.path.join(os.environ['SIFLI_SDK_PATH'], 'tools', 'kconfig', 'menuconfig.py')
        board = ['--board', board] if board else []
        board_search_path = ['--board_search_path', board_search_path] if board_search_path else []
        subprocess.run([sys.executable, menuconfig_path] + board + board_search_path)

    def verbose_callback(ctx: Context, param: List, value: str) -> Optional[str]:
        if not value or ctx.resilient_parsing:
            return None

        for line in ctx.command.verbose_output:
            print(line)

        return value

    def validate_root_options(ctx: Context, args: PropertyDict, tasks: List) -> None:
        args.project_dir = os.path.realpath(args.project_dir)
        if args.build_dir is not None and args.project_dir == os.path.realpath(args.build_dir):
            raise FatalError(
                'Setting the build directory to the project directory is not supported. Suggest dropping '
                "--build-dir option, the default is a 'build' subdirectory inside the project directory.")
        if args.build_dir is None:
            args.build_dir = os.path.join(args.project_dir, 'build')
        args.build_dir = os.path.realpath(args.build_dir)

    def sdk_version_callback(ctx: Context, param: str, value: str) -> None:
        if not value or ctx.resilient_parsing:
            return

        version = sdk_version()

        if not version:
            raise FatalError('SiFli-SDK version cannot be determined')

        print('SiFli-SDK %s' % version)
        sys.exit(0)

    def help_and_exit(action: str, ctx: Context, param: List, json_option: bool, add_options: bool) -> None:
        if json_option:
            output_dict = {}
            output_dict['target'] = get_target(param.project_dir)  # type: ignore
            output_dict['actions'] = []
            actions = ctx.to_info_dict().get('command').get('commands')
            for a in actions:
                action_info = {}
                action_info['name'] = a
                action_info['description'] = actions[a].get('help')
                if add_options:
                    action_info['options'] = actions[a].get('params')
                output_dict['actions'].append(action_info)
            print(json.dumps(output_dict, sort_keys=True, indent=4))
        else:
            print(ctx.get_help())
        ctx.exit()

    root_options = {
        'global_options': [
            {
                'names': ['--version'],
                'help': 'Show SiFli-SDK version and exit.',
                'is_flag': True,
                'expose_value': False,
                'callback': sdk_version_callback,
            },
            {
                'names': ['-C', '--project-dir'],
                'scope': 'shared',
                'help': 'Project directory.',
                'type': click.Path(),
                'default': os.getcwd(),
            },
            {
                'names': ['-B', '--build-dir'],
                'help': 'Build directory.',
                'type': click.Path(),
                'default': None,
            },
            {
                'names': ['-v', '--verbose'],
                'help': 'Verbose build output.',
                'is_flag': True,
                'is_eager': True,
                'default': False,
                'callback': verbose_callback,
            },
            {
                'names': ['--no-hints'],
                'help': 'Disable hints on how to resolve errors and logging.',
                'is_flag': True,
                'default': False
            }
        ],
        'global_action_callbacks': [validate_root_options],
    }

    build_actions = {
        'actions': {
            'menuconfig': {
                'callback': menuconfig,
                'help': 'Run "menuconfig" project configuration tool.',
                'options': global_options + [
                    {
                        'names': ['--board'],
                        'help': (
                            'board name'),
                        'envvar': 'MENUCONFIG_BOARD',
                        'default': None,
                    },
                    {
                        'names': ['--board_search_path'],
                        'help': (
                            'board search path'),
                        'envvar': 'MENUCONFIG_BOARD_SEARCH_PATH',
                        'default': None,
                    },
                ],
            },
        }
    }

    help_action = {
        'actions': {
            'help': {
                'callback': help_and_exit,
                'help': 'Show help message and exit.',
                'hidden': True,
                'options': [
                    {
                        'names': ['--json', 'json_option'],
                        'is_flag': True,
                        'help': 'Print out actions in machine-readable format for selected target.'
                    },
                    {
                        'names': ['--add-options'],
                        'is_flag': True,
                        'help': 'Add options about actions to machine-readable format.'
                    }
                ],
            }
        }
    }

    return merge_action_lists(root_options, build_actions, help_action)
