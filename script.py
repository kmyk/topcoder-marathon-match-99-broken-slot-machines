# Python Version: 3.x
# -*- coding: utf-8 -*-
import colorama
import json
import subprocess
import sys
import matplotlib.pyplot as plt

def command_record(args):
    data = []
    for seed in range(1, args.n + 1):
        proc = subprocess.run([ './Tester', '-json', '-seed', str(seed) ], stdout=subprocess.PIPE)
        print(proc.stdout.decode(), file=sys.stderr)
        lines = proc.stdout.splitlines()
        data += [ json.loads(lines[-1].decode()) ]
    json.dump(data, sys.stdout)

red   = lambda s: colorama.Fore.RED   + s + colorama.Style.RESET_ALL
green = lambda s: colorama.Fore.GREEN + s + colorama.Style.RESET_ALL
blue  = lambda s: colorama.Fore.BLUE  + s + colorama.Style.RESET_ALL

def command_report(args):
    with open(args.a) as fh:
        a = json.load(fh)
    if args.diff is None:
        b = None
    else:
        with open(args.diff) as fh:
            b = json.load(fh)
        assert len(a) <= len(b)
    indices = list(range(len(a)))
    indices.sort(key=lambda i: a[i][args.key])
    score_a = 0
    score_b = 0
    width = 12
    print(''.join([
        'seed'.rjust(width),
        'a'.rjust(width),
        'diff'.rjust(width) if b is not None else '',
        'score'.rjust(width),
        'theor.'.rjust(width),
        'coins'.rjust(width),
        'maxTime'.rjust(width),
        'noteTime'.rjust(width),
        'numMachines'.rjust(width),
    ]))
    for i in indices:
        seed = a[i]['seed']
        coins = a[i]['coins']
        maxTime = a[i]['maxTime']
        noteTime = a[i]['noteTime']
        numMachines = a[i]['numMachines']
        val_a = a[i]['result']
        theor = a[i]['theoretical']
        score_a += val_a / theor
        if b is None:
            style_a = lambda s: s
        else:
            assert a[i]['seed'] == b[i]['seed']
            val_b = b[i]['result']
            score_b += val_b / theor
            if val_a > val_b:
                style_a = green
                style_b = red
            elif val_a < val_b:
                style_a = red
                style_b = green
            else:
                style_a = lambda s: s
                style_b = lambda s: s
        if val_a / theor >= 1.0:
            style_score = green
        elif val_a / theor >= 0.7:
            style_score = lambda s: s
        else:
            style_score = red
        s = ''.join([
            str(seed).rjust(width),
            style_a(str(val_a).rjust(width)),
            style_b(str(val_b).rjust(width)) if b is not None else '',
            style_score(('%.3f' % (val_a / theor)).rjust(width)),
            ('%.3f' % theor).rjust(width),
            str(coins).rjust(width),
            str(maxTime).rjust(width),
            str(noteTime).rjust(width),
            str(numMachines).rjust(width),
        ])
        print(s)
        if args.plot:
            plt.plot(a[i][args.key], val_a, 'ro')
            plt.plot(b[i][args.key], val_b, 'bo')
    scale = 100 / len(indices)
    print(score_a * scale, score_b * scale if b is not None else '')
    if args.plot:
        plt.show()

def main():
    import argparse
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command')
    parser_info = subparsers.add_parser('record')
    parser_info.add_argument('-n', type=int, default=300)
    parser_diff = subparsers.add_parser('report')
    parser_diff.add_argument('a')
    parser_diff.add_argument('--diff')
    parser_diff.add_argument('--key', default='seed')
    parser_diff.add_argument('--plot', action='store_true')
    args = parser.parse_args()

    if args.command == 'record':
        command_record(args)
    elif args.command == 'report':
        command_report(args)
    else:
        parser.print_help()

if __name__ == '__main__':
    main()
