#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import errno
import logging
import os
import subprocess
import sys
from pathlib import Path

from build import build, PROJECT_ROOT


def parse_options():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--domain', dest='domain', action='store',
                                  default=None, help='Domain file in PDDL')
    parser.add_argument('-i', '--instance', dest='instance',
                        action='store', default=None,
                        help='Instance file in PDDL', required=True)
    parser.add_argument('--build', dest='build', action='store_true',
                        help='Build planner before search.')
    parser.add_argument('--debug', dest='debug', action='store_true',
                        help='Run planner in debug mode.')
    parser.add_argument('-s', '--search', dest='search', action='store',
                        default=None, help='Search algorithm', choices=("naive", "gbfs"),
                        required=True)
    parser.add_argument('-e', '--heuristic', dest='heuristic', action='store',
                        default=None, choices=("blind", "goalcount", "landmark"),
                        help='Heuristic to guide the search (ignore in case of blind search)',
                        required=True)
    parser.add_argument('-g', '--generator', dest='generator', action='store',
                        default=None, help='Successor generator method',
                        choices=('yannakakis', 'join', 'random_join', 'ordered_join', 'inverse_ordered_join', 'full_reducer'),
                        required=True)
    parser.add_argument('-l', '--landmark', dest='landmark', action='store',
                        default=None, help='Landmark file',
                        required=False)
    parser.add_argument('--state', action='store', help='Successor generator method',
                        default="sparse", choices=("sparse", "extensional"))
    parser.add_argument('--seed', action='store', help='Random seed.',
                        default=1)
    parser.add_argument('--translator-output-file', dest='translator_file',
                        default='output.lifted',
                        help='Output file of the translator')

    args = parser.parse_args()
    if args.domain is None:
        args.domain = find_domain_filename(args.instance)
        if args.domain is None:
            raise RuntimeError(f'Could not find domain filename matching instance file "{args.instance}"')

    return args


def find_domain_filename(task_filename):
    """
    Find domain filename for the given task using automatic naming rules.
    """
    dirname, basename = os.path.split(task_filename)

    domain_basenames = [
        "domain.pddl",
        basename[:3] + "-domain.pddl",
        "domain_" + basename,
        "domain-" + basename,
    ]

    for domain_basename in domain_basenames:
        domain_filename = os.path.join(dirname, domain_basename)
        if os.path.exists(domain_filename):
            return domain_filename


def validate(domain_name, instance_name, planfile):
    plan = Path(planfile)
    if not plan.is_file():
        logging.info(f'No plan file to validate could be found at "{planfile}"')
        return

    try:
        validate_inputs = ["validate", domain_name, instance_name, planfile]
        _ = subprocess.call(' '.join(validate_inputs), shell=True)
    except OSError as err:
        if err.errno == errno.ENOENT:
            logging.error("Error: 'validate' binary not found. Is it on the PATH?")
            return
        else:
            logging.error(f"Error executing 'validate': {err}")


def main():
    options = parse_options()

    build_dir = os.path.join(PROJECT_ROOT, 'builds', 'debug' if options.debug else 'release')

    if options.build:
        build(options.debug)

    # Create build path
    if not os.path.exists(build_dir):
        raise OSError("Planner not built!")

    os.chdir(PROJECT_ROOT)

    # Invoke the Python preprocessor
    subprocess.call([os.path.join(build_dir, 'translator', 'translate.py'),
                     options.domain, options.instance, '--output-file', options.translator_file])

    # Invoke the C++ search component
    cmd = []
    #if options.debug:
    #	cmd += ['gdbserver', ':1234']
    cmd += [os.path.join(build_dir, 'search', 'search'),
           '-f', options.translator_file,
           '-s', options.search,
           '-e', options.heuristic,
           '-g', options.generator,
           '-r', options.state,
           '-l', options.landmark,
           '--seed', str(options.seed)]

    print(f'Executing "{" ".join(cmd)}"')
    code = subprocess.call(cmd)

    # If we found a plan, try to validate it
    if code == 0:
        validate(options.domain, options.instance, os.path.join(PROJECT_ROOT, 'sas_plan'))

    return code


if __name__ == '__main__':
    sys.exit(main())
