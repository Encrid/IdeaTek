# Copyright (C) 2026 Enkrid
#
# Under the GNU Affero General Public License, version 3 only: see license.
import os
import subprocess
import sys


def show(argv, env):
    done = subprocess.run(argv, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    said = done.stdout.decode('utf-8', 'replace').splitlines()
    for line in said[:14]:
        print('  %s' % line)
    return said


def main():
    if os.environ.get('IDEATEK_DEBUG_STOP'):
        sys.exit(1)
    for name in ('VK_DRIVER_FILES', 'VK_ICD_FILENAMES', 'VULKAN_LOADER'):
        print('%s = %r' % (name, os.environ.get(name)))
    print('env names: %s' % sorted(name for name in os.environ if name.startswith('VK_')))
    folder = os.path.dirname(os.environ['VK_DRIVER_FILES'])
    info = os.path.join(folder, 'vulkaninfo.exe')
    print('vulkaninfo at %s: %s' % (info, os.path.isfile(info)))
    if os.path.isfile(info):
        print('run vulkaninfo:')
        show([info, '--summary'], dict(os.environ))
    probe = os.path.abspath(sys.argv[sys.argv.index('--probe') + 1])
    without = {name: value for name, value in os.environ.items() if name != 'VULKAN_LOADER'}
    print('run the probe on the system loader:')
    said = show([probe], without)
    print('run the probe as-is:')
    show([probe], dict(os.environ))
    for root in ('HKEY_CURRENT_USER', 'HKEY_LOCAL_MACHINE'):
        print('registry %s:' % root)
        try:
            import winreg
            key = winreg.OpenKey(getattr(winreg, root), r'Software\Khronos\Vulkan')
            print('  %s' % winreg.QueryInfoKey(key))
        except OSError as error:
            print('  %s' % error)
    settings = os.path.join(os.environ.get('LOCALAPPDATA', ''), 'vulkan', 'settings', 'vk_loader_settings.json')
    print('settings file %s: %s' % (settings, os.path.isfile(settings)))
    print('debug done')


if __name__ == '__main__':
    main()
