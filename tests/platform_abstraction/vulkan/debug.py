# Copyright (C) 2026 Enkrid
#
# Under the GNU Affero General Public License, version 3 only: see license.
import json
import os
import subprocess
import sys


def main():
    if os.environ.get('IDEATEK_DEBUG_STOP'):
        sys.exit(1)
    for name in ('VK_DRIVER_FILES', 'VK_ICD_FILENAMES', 'VULKAN_LOADER'):
        print('%s = %r' % (name, os.environ.get(name)))
    files = os.environ.get('VK_DRIVER_FILES', '')
    folder = os.path.dirname(files)
    print('folder %s holds %s' % (folder, sorted(os.listdir(folder)) if os.path.isdir(folder) else 'nothing'))
    if os.path.isfile(files):
        print('json %s' % open(files, encoding='utf-8').read().replace('\n', ' '))
    probe = sys.argv[sys.argv.index('--probe') + 1]
    clean = files.strip()
    runs = (('as-is', dict(os.environ, VK_LOADER_DEBUG='driver,error')),
            ('clean', dict(os.environ, VK_DRIVER_FILES=clean, VK_LOADER_DEBUG='driver,error')),
            ('add', dict(os.environ, VK_ADD_DRIVER_FILES=clean, VK_LOADER_DEBUG='driver,error')))
    for how, env in runs:
        done = subprocess.run([probe], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        said = done.stdout.decode('utf-8', 'replace').splitlines()
        print('run %s gave %s' % (how, said[0] if said else 'nothing'))
        for line in said:
            if 'no instance' in line or 'gpus' in line or 'Found ICD' in line or 'locations' in line:
                print('  %s' % line)
    print('debug done')


if __name__ == '__main__':
    main()
