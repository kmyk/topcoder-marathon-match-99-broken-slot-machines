# Python Version: 3.x
# -*- coding: utf-8 -*-
from colorama import Fore, Back, Style
import json
import subprocess
import sys
import matplotlib.pyplot as plt

def command_get(args):
    data = []
    for seed in range(1, args.n + 1):
        proc = subprocess.run([ 'sh', '-c', 'cd tester ; java Tester -exec ../a.out -json -seed ' + str(seed) ], stdout=subprocess.PIPE)
        print(proc.stdout.decode(), file=sys.stderr)
        lines = proc.stdout.splitlines()
        data += [ json.loads(lines[-1].decode()) ]
    json.dump(data, sys.stdout)

def command_diff(args):
    with open(args.a) as fh:
        a = json.load(fh)
    with open(args.b) as fh:
        b = json.load(fh)
    assert len(a) == len(b)
    indices = list(range(len(a)))
    indices.sort(key=lambda i: a[i][args.key])
    score_a = 0
    score_b = 0
    width = 12
    print(''.join([
        'seed'.rjust(width),
        'a'.rjust(width),
        'b'.rjust(width),
        'coins'.rjust(width),
        'maxTime'.rjust(width),
        'noteTime'.rjust(width),
        'numMachines'.rjust(width),
    ]))
    for i in indices:
        assert a[i]['seed'] == b[i]['seed']
        seed = a[i]['seed']
        coins = a[i]['coins']
        maxTime = a[i]['maxTime']
        noteTime = a[i]['noteTime']
        numMachines = a[i]['numMachines']
        val_a = a[i]['result']
        val_b = b[i]['result']
        score_a += val_a / max(1, val_a, val_b)
        score_b += val_b / max(1, val_a, val_b)
        if val_a > val_b:
            style_a = Fore.GREEN
            style_b = Fore.RED
        elif val_a < val_b:
            style_a = Fore.RED
            style_b = Fore.GREEN
        else:
            style_a = ''
            style_b = ''
        s = ''.join([
            str(seed).rjust(width),
            style_a, str(val_a).rjust(width), Style.RESET_ALL,
            style_b, str(val_b).rjust(width), Style.RESET_ALL,
            str(coins).rjust(width),
            str(maxTime).rjust(width),
            str(noteTime).rjust(width),
            str(numMachines).rjust(width),
        ])
        print(s)
        if args.plot:
            plt.plot(a[i][args.key], val_a, 'ro')
            plt.plot(b[i][args.key], val_b, 'bo')
    print(score_a, score_b)
    if args.plot:
        plt.show()

def main():
    import argparse
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command')
    parser_info = subparsers.add_parser('get')
    parser_info.add_argument('-n', type=int, default=300)
    parser_diff = subparsers.add_parser('diff')
    parser_diff.add_argument('a')
    parser_diff.add_argument('b')
    parser_diff.add_argument('--key', default='seed')
    parser_diff.add_argument('--plot', action='store_true')
    args = parser.parse_args()

    if args.command == 'get':
        command_get(args)
    elif args.command == 'diff':
        command_diff(args)
    else:
        parser.print_help()

if __name__ == '__main__':
    main()
